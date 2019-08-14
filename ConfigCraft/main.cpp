/*
Copyright 2019 MyDataModels

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <CLI/CLI.hpp>

#include "JSON/cppcraft.hpp"

int main(int argc, char *argv[]) {
  CLI::App app("Config Craft");

  std::string file;
  app.add_option("config", file, "Configuration file name")
      ->check(CLI::ExistingFile)
      ->required();

  bool json = false;
  auto format = app.add_option_group("format", "Formatting type for output");
  format->add_flag("-j,--json", json, "Specify the output in JSON format");
  // require one of the options to be selected
  format->require_option(1);

  bool cpp = false;
  auto language = app.add_option_group("language", "Language for output");
  language->add_flag("-c,--cpp", cpp, "Specify the output in C++");
  // require one of the options to be selected
  language->require_option(1);

  // parse command args
  CLI11_PARSE(app, argc, argv);

  if (json && cpp) {
      configcraft::json::cpp::craft(file);
  }

  return 0;
}
