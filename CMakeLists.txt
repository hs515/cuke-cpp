set(VERSION 0.0.1)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")
    file(STRINGS "VERSION" LINES)
    list(GET LINES 0 VERSION)
endif()

# Crude Semver parsing
if("${VERSION}" MATCHES "^([^\\.]+)\\.([^\\.]+)\\.([^\\.]+)$")
    set(VER_MAJOR ${CMAKE_MATCH_1})
    set(VER_MINOR ${CMAKE_MATCH_2})
    set(VER_PATCH ${CMAKE_MATCH_3})
else()
    message(FATAL_ERROR "unable to parse version: ${VERSION}")
endif()

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(cuke_cpp VERSION ${VERSION} LANGUAGES C CXX)

# Cpp standard check
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
elseif(CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "C++17 (above) is required")
endif()

###
#
# Options
#
###
option(CUKE_CPP_BUILD_TESTS	"Build the unit tests." OFF)
option(CUKE_CPP_ENABLE_COVERAGE	"Enable GCov compile and link." OFF)
option(CUKE_CPP_STRICT          "Additional and more strict checks" OFF)
option(CUKE_CPP_FETCH_DEPS	"Fetch dependencies via FetchContent." OFF)
option(CUKE_CPP_EXPORT_COMPILE	"Export compile commands." ON)

if(CUKE_CPP_ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

if(CUKE_CPP_STRICT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

###
# Configuration
#
###
include(GNUInstallDirs)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS)
endif()

if(CUKE_CPP_EXPORT_COMPILE)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

###
# CMake dependencies
#
###
if(CUKE_CPP_FETCH_DEPS)
    message(STATUS "Fetching dependencies ...")

    include(FetchContent)

    # Fetch cucumber-gherkin cpp
    set(CUCUMBER_GHERKIN_FETCH_DEPS ON)
    FetchContent_Declare(
        cucumber_gherkin
        GIT_REPOSITORY https://github.com/cucumber/gherkin.git
        GIT_TAG v37.0.1
        GIT_CONFIG core.sparseCheckout=true
        OVERRIDE_FIND_PACKAGE
        SOURCE_SUBDIR "cpp"
    )
    FetchContent_MakeAvailable(cucumber_gherkin)

    # Configure cucumber_cpp options before fetching
    set(CUKE_ENABLE_GTEST ON CACHE BOOL "Enable Google Test framework" FORCE)
    set(CUKE_ENABLE_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(CUKE_TESTS_UNIT OFF CACHE BOOL "Enable unit tests" FORCE)
    FetchContent_Declare(
        cucumber_cpp
        GIT_REPOSITORY https://github.com/hs515/cucumber-cpp.git
        GIT_TAG main
        OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(cucumber_cpp)

    # Fetch cucumber-tag-expressions cpp
    FetchContent_Declare(
        cucumber_tag_expressions
        GIT_REPOSITORY https://github.com/hs515/tag-expressions.git
        GIT_TAG cpp-tag-expressions
        OVERRIDE_FIND_PACKAGE
        SOURCE_SUBDIR "cpp"
    )
    FetchContent_MakeAvailable(cucumber_tag_expressions)
endif()

find_package(cucumber_gherkin CONFIG REQUIRED)
find_package(cucumber_cpp CONFIG REQUIRED)
find_package(cucumber_tag_expressions CONFIG REQUIRED)

message(STATUS "cucumber_cpp source directory is: ${cucumber_cpp_SOURCE_DIR}")
message(STATUS "cucumber_cpp binary directory is: ${cucumber_cpp_BINARY_DIR}")

###
#
# Targets
#
###
add_subdirectory(src)
add_subdirectory(examples)

# ###
# #
# # Installation support
# #
# ###
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#   "cmake/cucumber_gherkin-config.cmake.in"
#   "${PROJECT_BINARY_DIR}/cucumber_gherkin-config.cmake"
#   INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucumber_gherkin"
#   PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
# )

# write_basic_package_version_file(
#   "${PROJECT_BINARY_DIR}/cucumber_gherkin-config-version.cmake"
#   COMPATIBILITY AnyNewerVersion
# )

# install(
#     TARGETS
#         cucumber_gherkin_lib
#         cucumber_gherkin_bin
#         cucumber_gherkin_generate_tokens_bin
#     EXPORT cucumber_gherkin-targets
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# install(
#     EXPORT cucumber_gherkin-targets
#     FILE cucumber_gherkin-targets.cmake
#     NAMESPACE cucumber::
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucumber_gherkin"
# )

# install(
#     FILES
#         "${PROJECT_BINARY_DIR}/cucumber_gherkin-config.cmake"
#         "${PROJECT_BINARY_DIR}/cucumber_gherkin-config-version.cmake"
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucumber_gherkin"
# )


# install(
#     DIRECTORY "${PROJECT_SOURCE_DIR}/include/gherkin/"
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cucumber
# )
# endif()

# find_package(cucumber_gherkin CONFIG REQUIRED)

# ###
# #
# # Targets
# #
# ###
# add_subdirectory(src/lib/gherkin)
# add_subdirectory(src/bin/gherkin)
# add_subdirectory(src/bin/gherkin-generate-tokens)

# ###
# #
# # Installation support
# #
# ###
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#   "cmake/cucumber_gherkin-config.cmake.in"
#   "${PROJECT_BINARY_DIR}/cucumber_gherkin-config.cmake"
#   INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucumber_gherkin"
#   PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
# )

# write_basic_package_version_file(
#   "${PROJECT_BINARY_DIR}/cucumber_gherkin-config-version.cmake"
#   COMPATIBILITY AnyNewerVersion
# )

# install(
#     TARGETS
#         cucumber_gherkin_lib
#         cucumber_gherkin_bin
#         cucumber_gherkin_generate_tokens_bin
#     EXPORT cucumber_gherkin-targets
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# install(
#     EXPORT cucumber_gherkin-targets
#     FILE cucumber_gherkin-targets.cmake
#     NAMESPACE cucumber::
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucumber_gherkin"
# )

# install(
#     FILES
#         "${PROJECT_BINARY_DIR}/cucumber_gherkin-config.cmake"
#         "${PROJECT_BINARY_DIR}/cucumber_gherkin-config-version.cmake"
#     DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cucumber_gherkin"
# )


# install(
#     DIRECTORY "${PROJECT_SOURCE_DIR}/include/gherkin/"
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cucumber
# )
