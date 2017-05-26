<?php

/**
 * @file
 * Prevent execution of certain commands when project uses composer.
 */

use Drush\Log\LogLevel;

/**
 * Implements hook_drush_help_alter().
 */
function composercheck_drush_help_alter(&$command) {
  if ($command['command'] == 'pm-download') {
    $command['options']['pm-force'] = 'Allow downloading modules if composer is managing dependencies';
  }
}

/**
 * Load the project's composer.json.
 *
 * @return array|false
 *   The composer.json data, or FALSE if not found.
 */
function _composercheck_get_composer_json() {
  // Find the closest composer.json to the Drupal root directory.
  $drupalRoot = drush_get_context('DRUSH_DRUPAL_ROOT');
  for ($pathRel = 0; $pathRel <= 1; $pathRel++) {
    $composerJsonPath = $drupalRoot . str_repeat('/..', $pathRel) . '/composer.json';
    drush_log(dt('Checking for @path', ['@path' => $composerJsonPath]), LogLevel::DEBUG);
    if (file_exists($composerJsonPath)) {
      drush_log(dt('Found @path', ['@path' => $composerJsonPath]), LogLevel::DEBUG);
      return json_decode(file_get_contents($composerJsonPath), TRUE);
    }
  }

  drush_log(dt('composer.json not found in project'), LogLevel::DEBUG);
  return FALSE;
}

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * Prevent download of modules if dependencies managed by composer.
 */
function drush_composercheck_pm_download_validate() {

  if (drush_get_option('pm-force')) {
    return;
  }

  $composerJson = _composercheck_get_composer_json();
  if (!empty($composerJson)) {
    drush_set_error('COMPOSER_MANAGED', dt('This project uses composer to manage dependencies. To download a module use `composer require`. To override this restriction use the --pm-force option.'));
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * Prevent code update for composer-managed dependencies.
 */
function drush_composercheck_pm_updatecode_validate() {

  $composerJson = _composercheck_get_composer_json();
  if (empty($composerJson)) {
    return;
  }

  $projects = pm_parse_arguments(func_get_args(), FALSE);
  if (empty($projects)) {
    drush_set_error('COMPOSER_MANAGED', dt('This project uses composer to manage dependencies.  To update a module not managed by composer, specify it as an argument.'));
    return;
  }

  $requireTypes = array_filter(
    ['require', 'require-dev'],
    function ($key) use ($composerJson) {
      return array_key_exists($key, $composerJson);
    }
  );

  foreach ($projects as $project) {
    foreach ($requireTypes as $requireType) {
      if (array_key_exists('drupal/' . $project, $composerJson[$requireType])) {
        drush_set_error('COMPOSER_MANAGED', dt('@project is managed by composer.  Use `composer upgrade drupal/@project` to update.', ['@project' => $project]));
        break;
      }
    }
  }
}
