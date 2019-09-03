# configcraft
Configuration file parsing code generator

**configcraft** takes an example configuration file, and it generates a working parsing code with default values taken from the input file.

For example to generate the parsing C++ code for a JSON file simply run:

```
./configcraft -jc config.json
```

This will generate 3 files:

```
Config.hpp
ConfigJSON.hpp
ConfigJSON.cpp
```

If the configuration file content is in a string named `cfg`, you simply add the following to your code:

```
#include "ConfigJSON.hpp"

[...]

ConfigJSON cfg;

cfg.parse(cfg);
```

Your configuration can be retrieved with `cfg.getRoot()`, that returns a type `ROOT` defined in `Config.hpp`.

**Currently only C++ and JSON are supported: contributions are welcome**
