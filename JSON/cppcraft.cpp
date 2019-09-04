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

#include <inja/inja.hpp>

#include "JSON.hpp"
#include "cpptemplates.hpp"

#include "cppcraft.hpp"

namespace configcraft {
namespace json {
namespace cpp {
std::vector<std::string> renderDataClasses(const nlohmann::json &object) {
  static auto classes = std::vector<std::string>();

  inja::Environment env;
  env.add_callback("sanitize", 1, &sanitize);

  classes.push_back(env.render(dataClassesTemplate, object));

  for (auto &[key, value] : object["root"].items()) {
    if (value.is_object()) {
      nlohmann::json child;
      child["root"] = value.get<nlohmann::json>();
      child["name"] = key;
      renderDataClasses(child);
    }
  }

  return classes;
}

std::string renderDataInclude(const nlohmann::json &object) {
  auto out = std::string(dataIncludeTemplate);

  auto classes = renderDataClasses(object);
  std::reverse(classes.begin(), classes.end());
  out += std::accumulate(classes.begin(), classes.end(), std::string(""));

  return out;
}

std::string renderDefinitions(const nlohmann::json &object) {
  inja::Environment env;
  env.add_callback("sanitize", 1, &sanitize);

  auto out = env.render(definitionsTemplate, object);

  for (auto &[key, value] : object["root"].items()) {
    if (value.is_object()) {
      nlohmann::json child;
      child["root"] = value.get<nlohmann::json>();
      child["name"] = key;
      out += renderDefinitions(child);
    }
  }

  return out;
}

std::string renderParseRoot(const nlohmann::json &object) {
  inja::Environment env;
  env.add_callback("sanitize", 1, &sanitize);
  env.set_expression("$$", "$$");

  auto out = env.render(rootTemplate, object);

  return out;
}

std::string renderInclude(const nlohmann::json &object) {
  auto out = std::string(includeHeadTemplate);

  out += renderParseRoot(object);

  out += std::string(includeBodyTemplate);

  out += renderDefinitions(object);

  out += std::string(includeTailTemplate);

  return out;
}

std::string renderImplementations(const nlohmann::json &object) {
  inja::Environment env;
  env.add_callback("sanitize", 1, &sanitize);
  env.set_expression("$$", "$$");

  auto out = env.render(implementationBodyTemplate, object);

  for (auto &[key, value] : object["root"].items()) {
    if (value.is_object()) {
      nlohmann::json child;
      child["root"] = value.get<nlohmann::json>();
      child["name"] = key;
      out += renderImplementations(child);
    }
  }

  return out;
}

std::string renderImplementation(const nlohmann::json &object) {
  auto out = std::string(implementationHeadTemplate);

  out += renderImplementations(object);

  return out;
}

void craft(const std::string &filename) {
  auto root = read(filename);

  auto config = std::ofstream("Config.hpp");
  config << renderDataInclude(root) << "\n";

  auto include = std::ofstream("ConfigJSON.hpp");
  include << renderInclude(root) << "\n";

  auto implementation = std::ofstream("ConfigJSON.cpp");
  implementation << renderImplementation(root) << "\n";
}

} // namespace cpp
} // namespace json
} // namespace configcraft
