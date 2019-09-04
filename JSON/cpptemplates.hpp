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

#include <inja/environment.hpp>

auto sanitize(inja::Arguments &args) {
  std::string str = args.at(0)->get<std::string>();
  str.erase(std::remove_if(str.begin(), str.end(),
                           [](auto c) { return !std::isalnum(c) && c != '_'; }),
            str.end());
  return str;
}

auto upperFirst(inja::Arguments &args) {
  std::string str = args.at(0)->get<std::string>();

  str[0] = static_cast<char>(std::toupper(str[0]));

  return str;
}

auto lowerFirst(inja::Arguments &args) {
  std::string str = args.at(0)->get<std::string>();

  str[0] = static_cast<char>(std::tolower(str[0]));

  return str;
}

constexpr auto dataClassesTemplate =
    R"(struct {{ upperFirst(sanitize(name)) }} {
## for key, value in root
##   if isObject(value)
  {{ upperFirst(sanitize(key)) }} {{ lowerFirst(sanitize(key)) }};
##   else if isArray(value)
##     if isBoolean(value.0)
  std::vector<bool> {{ lowerFirst(sanitize(key)) }};
##     else if isFloat(value.0)
  std::vector<double> {{ lowerFirst(sanitize(key)) }};
##     else if isInteger(value.0)
  std::vector<int> {{ lowerFirst(sanitize(key)) }};
##     else if isString(value.0)
  std::vector<std::string> {{ lowerFirst(sanitize(key)) }};
##     else if isObject(value.0)
  std::vector<nlohmann::json> {{ lowerFirst(sanitize(key)) }};
##     endif
##   else if isBoolean(value)
  bool {{ lowerFirst(sanitize(key)) }};
##   else if isFloat(value)
  double {{ lowerFirst(sanitize(key)) }};
##   else if isInteger(value)
  int {{ lowerFirst(sanitize(key)) }};
##   else if isString(value)
  std::string {{ lowerFirst(sanitize(key)) }};
##   endif
## endfor
};

)";

constexpr auto dataIncludeTemplate =
    R"(#pragma once

#include <string>
#include <vector>

)";

constexpr auto definitionsTemplate =
    R"(## for key, value in root
##   if isObject(value)
  void fromJson(const nlohmann::json &j, {{ upperFirst(sanitize(key)) }} &{{ lowerFirst(sanitize(key)) }});

##   endif
## endfor)";

constexpr auto rootTemplate =
    R"(
##   for childKey, childValue in root
##     if isObject(childValue)
    fromJson(j.value("$$ lowerFirst(sanitize(childKey)) $$", nlohmann::json{{"$$ lowerFirst(sanitize(childKey)) $$", {}}}), root.$$ lowerFirst(sanitize(childKey)) $$);
##     else if isArray(childValue)
    root.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$",
##       if isBoolean(childValue.0)
    std::vector<bool>{
##       else if isFloat(childValue.0)
    std::vector<double>{
##       else if isInteger(childValue.0)
    std::vector<int>{
##       else if isString(childValue.0)
    std::vector<std::string>{
##       else if isObject(childValue.0)
    std::vector<nlohmann::json>{
##       endif
##       for element in childValue
##         if isString(childValue.0) or isObject(childValue.0)
##           if not loop.is_last
      R"~($$ element $$)~",
##           else
      R"~("$$ element $$)~"});
##           endif
##         else
##           if not loop.is_last
      $$ element $$,
##           else
      $$ element $$});
##           endif
##         endif
##       endfor
##     else if isBoolean(childValue)
    root.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$", $$ childValue $$);
##     else if isNumber(childValue)
    root.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$", $$ childValue $$);
##     else if isString(childValue)
    root.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$", std::string(R"~($$ childValue $$)~"));
##     endif
##   endfor
)";

constexpr auto includeHeadTemplate =
    R"(#pragma once

#include <nlohmann/json.hpp>

#include "Config.hpp"

class ConfigJSON {
public:
  template <typename T>
  void parse(T &&json) {
    auto j = nlohmann::json::parse(std::forward<T>(json));
)";

constexpr auto includeBodyTemplate =
    R"(  }

  const Root &getRoot() const { return root; }

private:
  Root root;

)";

constexpr auto includeTailTemplate =
    R"(};
)";

constexpr auto implementationBodyTemplate =
    R"(## for key, value in root
##   if isObject(value)
  void ConfigJSON::fromJson(const nlohmann::json &j, $$ upperFirst(sanitize(key)) $$ &$$ lowerFirst(sanitize(key)) $$) {
##     for childKey, childValue in value
##       if isObject(childValue)
    fromJson(j.value("$$ lowerFirst(sanitize(childKey)) $$", nlohmann::json{{"$$ lowerFirst(sanitize(childKey)) $$", {}}}), $$ lowerFirst(sanitize(key)) $$.$$ lowerFirst(sanitize(childKey)) $$);
##       else if isArray(childValue)
    $$ lowerFirst(sanitize(key)) $$.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$",
##         if isBoolean(childValue.0)
    std::vector<bool>{
##         else if isFloat(childValue.0)
    std::vector<double>{
##         else if isInteger(childValue.0)
    std::vector<int>{
##         else if isString(childValue.0)
    std::vector<std::string>{
##         else if isObject(childValue.0)
    std::vector<nlohmann::json>{
##         endif
##         for element in childValue
##           if isString(childValue.0) or isObject(childValue.0)
##             if not loop.is_last
      R"~($$ element $$)~",
##           else
      R"~("$$ element $$)~"});
##             endif
##           else
##             if not loop.is_last
      $$ element $$,
##             else
      $$ element $$});
##             endif
##           endif
##         endfor
##       else if isBoolean(childValue)
    $$ lowerFirst(sanitize(key)) $$.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$", $$ childValue $$);
##       else if isNumber(childValue)
    $$ lowerFirst(sanitize(key)) $$.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$", $$ childValue $$);
##       else if isString(childValue)
    $$ lowerFirst(sanitize(key)) $$.$$ lowerFirst(sanitize(childKey)) $$ = j.value("$$ lowerFirst(sanitize(childKey)) $$", std::string(R"~($$ childValue $$)~"));
##       endif
##     endfor
  }

##   endif
## endfor
)";

constexpr auto implementationHeadTemplate =
    R"(#include "ConfigJSON.hpp"

)";
