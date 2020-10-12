/*
 * Authors: M. Kristien
 * Organisation: HYPED
 * Date: 3. April 2019
 * Description:
 *
 *    Copyright 2018 HYPED
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "utils/config.hpp"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>  // redundant includes to make linter stop complaining
#include <vector>
#include "utils/interface_factory.hpp"
#include "utils/logger.hpp"
#include "utils/system.hpp"

namespace hyped {
namespace utils {

#define BUFFER_SIZE                                                            \
  250  // max length of a line in the config file in characters

typedef void (Config::*Parser)(char *line);
struct ModuleEntry {
  Submodule label;
  char name[20];  // no module name should exceed 20 characters
  Parser parse;
};

/**
 * Update this table to register new config mapping. Each row corresponds
 * to a config file line parser. If a matching line is found, the line is
 * forwarded to the parser to update/load any corresponding configuration data.
 *
 * Column 0: Submodule enum entry
 * Column 1: A readable name as it appears in the configuration file
 * Column 2: An address of a Config:: member function that performs the line
 * parsing.
 */
#define MAP_ENTRY(module) {k##module, #module, &Config::parse##module},

ModuleEntry module_map[] = {MODULE_LIST(MAP_ENTRY)};

// if there is no creator configured to an interface, we use this one to prevent
// calling a null pointer function
template <class T>
T *createDefault() {
  printf(
      "ERROOOR: no creator for %s found, creating NULL object\n",
      interfaceName<T>());
  return nullptr;
}

void Config::parseNoModule(char *line) {
}

void Config::parseStateMachine(char *line) {
}

void Config::parseThermometer(char *line) {
}

void Config::parseHeating(char *line) {
}

void Config::parseUserInterface(char *line) {
}

constexpr char config_dir_name[]    = "configurations/";
constexpr auto config_dir_name_size = sizeof(config_dir_name);
void Config::readFile(char *config_file) {
  static_assert(
      config_dir_name_size < BUFFER_SIZE,
      "configuration directory name is too long");
  char file_name[BUFFER_SIZE];
  std::snprintf(file_name, config_dir_name_size, config_dir_name);
  std::snprintf(
      file_name + config_dir_name_size - 1,  // account for dir_name
      sizeof(file_name) -
          config_dir_name_size,  // calculate remaining buffer space
      "%s",
      config_file);  // provide string value to be appended
  // load config file, parse it into data structure
  FILE *file = fopen(file_name, "r");
  if (!file) {
    log_.ERR("CONFIG", "no configuration file %s found, exiting", file_name);
    return;
  }

  log_.INFO("CONFIG", "loading configuration file %s", file_name);

  // allocate line buffer, read and parse file line by line
  char line[BUFFER_SIZE];
  ModuleEntry *current_module = &module_map[0];

  while (fgets(line, sizeof(line), file) != NULL) {
    // remove new line character
    for (char &value : line) {
      if (value == '\n')
        value = '\0';
    }

    // '>' character marks change for submodule
    // '$' character marks an include of a different config file, step into
    // parsing that file all other lines should be forwarded to the module
    // parses, e.g ParseNavigation()
    switch (line[0]) {
      case '#':   // comment
      case '\0':  // empty line
        continue;
      case '>': {
        ModuleEntry *prev_module = current_module;
        for (ModuleEntry &entry : module_map) {
          if (strncmp(entry.name, line + 2, BUFFER_SIZE) == 0) {
            current_module = &entry;
            break;
          }
        }

        if (prev_module == current_module) {
          log_.ERR(
              "CONFIG",
              "module name \"%s\" not found, keeping to module \"%s\"",
              line + 1,
              current_module->name);
        } else {
          log_.DBG("CONFIG", "changing module to \"%s\"", current_module->name);
        }

        break;
      }
      case '$': {
        // check if config_file already in config_files_
        char *new_config_file = line + 2;
        bool duplicate        = false;
        for (char *file : config_files_) {
          if (std::strcmp(file, new_config_file) == 0) {
            duplicate = true;
            break;
          }
        }
        if (duplicate) {
          log_.ERR(
              "CONFIG",
              "circular config include of %s from %s",
              new_config_file,
              config_file);
          break;
        }

        config_files_.push_back(new_config_file);
        log_.DBG(
            "CONFIG",
            "Stepping into %s, reseting module to \"%s\"",
            new_config_file,
            current_module->name);
        current_module = &module_map[0];
        readFile(new_config_file);
        current_module = &module_map[0];
        log_.DBG(
            "CONFIG",
            "Returning into %s, reseting module to \"%s\"",
            config_file,
            current_module->name);
        config_files_.pop_back();
        break;
      }
      default: {
        // dispatch to line parser
        (this->*(current_module->parse))(line);
      }
    }
  }

  fclose(file);
}

Config::Config(char *config_file) : log_(System::getLogger()) {
#define INIT_CREATOR(module, interface)                                        \
  interfaceFactory.get##interface##Instance = createDefault<module::interface>;
  INTERFACE_LIST(INIT_CREATOR)

  config_files_.push_back(config_file);
  readFile(config_file);
  config_files_.pop_back();
}

}  // namespace utils
}  // namespace hyped