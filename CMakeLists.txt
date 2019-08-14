cmake_minimum_required(VERSION 3.12)
project(configcraft)
include(modules.cmake)

# Initialize Conan
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_executable(configcraft)
target_compile_features(configcraft PUBLIC cxx_std_17)
target_include_directories(configcraft PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

foreach(MODULE IN LISTS MODULES)
    include(${MODULE}/${MODULE}.cmake)
endforeach()
