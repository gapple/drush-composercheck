Drush Composer Check
===============

Prevent execution of certain drush commands when project uses composer for dependency management

## Installation

### Global
#### Using Composer
1. Require `composer/installers`
    ```bash
    composer global require composer/installers
    ```

2. Configure the installer path for drush plugins in `~/.composer/composer.json`
    ```json
    {
      "extra": {
        "installer-paths": {
          "../.drush/plugins/{$name}": ["type:drupal-drush"]
        }
      }
    }
    ```

3. Require `gapple/drush-composercheck`
    ```bash
    composer global require gapple/drush-composercheck
    ```

#### Manual Installation
1. [Download the release package](https://github.com/gapple/drush-composercheck/releases)
2. Unzip the package to `~/.drush/`


### Project
#### Using Composer
1. Require `composer/installers` in your project
    ```bash
    composer require composer/installers
    ```

2. Configure the installer path for drush plugins in your project's `composer.json`
    ```json
    {
      "extra": {
        "installer-paths": {
          "drush/contrib/{$name}": ["type:drupal-drush"]
        }
      }
    }
    ```

3. Require `gapple/drush-composercheck` in your project
    ```bash
    composer require gapple/drush-composercheck
    ```

#### Manual Installation
1. [Download the release package](https://github.com/gapple/drush-composercheck/releases)
2. Unzip the package to the `drush` folder within your project
