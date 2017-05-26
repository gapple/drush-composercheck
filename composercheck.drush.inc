<?php

/**
 * @file
 * Prevent execution of certain commands when project uses composer.
 */

use Drush\Log\LogLevel;

/**
 * Implements drush_hook_COMMAND_validate().
 *
 * Prevent update for composer-managed dependencies.
 */
function drush_composercheck_pm_updatecode_validate() {

  // Find the closest composer.json to the Drupal root directory.
  $drupalRoot = drush_get_context('DRUSH_DRUPAL_ROOT');
  for ($pathRel = 0; $pathRel <= 1; $pathRel++) {
    $composerJsonPath = $drupalRoot . str_repeat('/..', $pathRel) . '/composer.json';
    drush_log(dt('Checking @path', ['@path' => $composerJsonPath]), LogLevel::DEBUG);
    if (file_exists($composerJsonPath)) {
      $composerJson = json_decode(file_get_contents($composerJsonPath), TRUE);
      break;
    }
  }

  if (empty($composerJson)) {
    drush_log(dt('composer.json not found in project'), LogLevel::DEBUG);
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
