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

option(CUKE_ENABLE_BOOST_TEST   "Enable Boost.Test framework" OFF)
option(CUKE_ENABLE_GTEST        "Enable Google Test framework" OFF)
option(CUKE_ENABLE_QT_5         "Enable Qt5 framework" OFF)
option(CUKE_ENABLE_QT_6         "Enable Qt6 framework" OFF)

#
# Check that at least one test framework is enabled
#

set(CUKE_TEST_FRAMEWORKS
    CUKE_ENABLE_BOOST_TEST
    CUKE_ENABLE_GTEST
    CUKE_ENABLE_QT_5
    CUKE_ENABLE_QT_6
)

set(TEST_FRAMEWORK_FOUND FALSE)
foreach(test_framework ${CUKE_TEST_FRAMEWORKS})
    if(${test_framework})
        set(TEST_FRAMEWORK_FOUND TRUE)
    endif()
endforeach()

if(NOT TEST_FRAMEWORK_FOUND)
    message(WARNING "No test framework enabled. At least one should be enabled. Options are: ${CUKE_TEST_FRAMEWORKS}.")
endif()

#
# Boost
#

set(Boost_USE_STATIC_RUNTIME OFF)
if(CUKE_ENABLE_BOOST_TEST)
    # "An external test runner utility is required to link with dynamic library" (Boost User's Guide)
    set(CMAKE_CXX_FLAGS "-DBOOST_TEST_DYN_LINK ${CMAKE_CXX_FLAGS}")
    find_package(Boost 1.70 COMPONENTS unit_test_framework REQUIRED)
endif()

#
# GTest
#

if(CUKE_ENABLE_GTEST)
    find_package(GTest 1.11.0 REQUIRED)
endif()

#
# Qt
#

if(CUKE_ENABLE_QT_5 AND CUKE_ENABLE_QT_6)
    message(FATAL_ERROR "Qt version 5 and 6 enabled, please select at most one")
endif()


if(CUKE_ENABLE_QT_5)
    find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Test
    )

    message(STATUS "Found Qt version: ${Qt5Core_VERSION_STRING}")
    if(Qt5Core_VERSION_STRING VERSION_LESS 5.15.0)
        add_library(Qt::Core    INTERFACE IMPORTED)
        add_library(Qt::Gui     INTERFACE IMPORTED)
        add_library(Qt::Widgets INTERFACE IMPORTED)
        add_library(Qt::Test    INTERFACE IMPORTED)
        set_target_properties(Qt::Core    PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Core   )
        set_target_properties(Qt::Gui     PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Gui    )
        set_target_properties(Qt::Widgets PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Widgets)
        set_target_properties(Qt::Test    PROPERTIES INTERFACE_LINK_LIBRARIES Qt5::Test   )
    endif()
endif()

if(CUKE_ENABLE_QT_6)
    find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Test
    )

    message(STATUS "Found Qt version: ${Qt6Core_VERSION}")
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
