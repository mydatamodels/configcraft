from conans import ConanFile, CMake, tools


class ConfigCraftConan(ConanFile):
    name = "configcraft"
    version = "0.0.0"
    license = "<Put the package license here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of configcraft here>"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False],
               "unit_tests": [True, False]}
    default_options = {"shared": False,
                       "unit_tests": False}
    generators = "cmake"
    exports = "*"
    requires = "CLI11/1.8.0@configcraft/stable", \
               "nlohmann_json/3.6.1@configcraft/stable", \
               "inja/2.1.0@configcraft/stable"

    def build(self):
        pass

    def package(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def requirements(self):
        if self.options.unit_tests:
            self.requires("gtest/1.8.1@bincrafters/stable")
