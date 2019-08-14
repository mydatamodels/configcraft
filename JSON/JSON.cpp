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

#include <fstream>

#include "JSON.hpp"

namespace configcraft {
namespace json {
nlohmann::json read(const std::string &filename) {
  auto infile = std::ifstream(filename);
  auto txt = std::string(std::istreambuf_iterator<char>(infile),
                         std::istreambuf_iterator<char>());

  nlohmann::json root;
  root["root"] = nlohmann::json::parse(txt);
  root["name"] = "root";

  return root;
}
} // namespace json
} // namespace configcraft
