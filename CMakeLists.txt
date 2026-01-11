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

###
#
# Main project check
#
###
set(CUKE_CPP_MAIN_PROJECT OFF)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CUKE_CPP_MAIN_PROJECT ON)
endif()

###
#
# Options
#
###
if(${CUKE_CPP_MAIN_PROJECT})
    set(CUKE_CPP_BUILD_TESTS_INIT ON)
    set(CUKE_CPP_EXPORT_COMPILE_INIT ON)
else()
    set(CUKE_CPP_BUILD_TESTS_INIT OFF)
    set(CUKE_CPP_EXPORT_COMPILE_INIT OFF)
endif()

option(
    CUKE_CPP_BUILD_TESTS
    "Build the unit tests."
    ${CUKE_CPP_BUILD_TESTS_INIT}
)

option(
    CUKE_CPP_EXPORT_COMPILE
    "Export compile commands."
    ${CUKE_CPP_EXPORT_COMPILE_INIT}
)

#set(CUKE_CPP_FETCH_DEPS_INIT ON)
option(
    CUKE_CPP_FETCH_DEPS
    "Fetch dependencies via FetchContent."
    ${CUKE_CPP_FETCH_DEPS_INIT}
)

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

    # Fetch GTest (required by cucumber_cpp)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    # Prevent GTest from overriding compiler/linker options
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    # Set GTest variables for find_package compatibility
    set(GTEST_FOUND TRUE)
    set(GTEST_INCLUDE_DIR ${googletest_SOURCE_DIR}/googletest/include)
    set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
    set(GTEST_LIBRARY gtest)
    set(GTEST_LIBRARIES gtest)
    set(GTEST_MAIN_LIBRARY gtest_main)
    set(GTEST_MAIN_LIBRARIES gtest_main)
    set(GTEST_BOTH_LIBRARIES gtest gtest_main)

    # Fetch Asio (header-only library required by cucumber_cpp)
    FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-30-2
    )
    FetchContent_GetProperties(asio)
    if(NOT asio_POPULATED)
        FetchContent_Populate(asio)
        # Asio is header-only, just set the include directory
        set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)
    endif()

    # Fetch TCLAP (header-only command line parser required by cucumber_cpp)
    FetchContent_Declare(
        tclap
        GIT_REPOSITORY https://github.com/mirror/tclap.git
        GIT_TAG v1.2.5
    )
    FetchContent_GetProperties(tclap)
    if(NOT tclap_POPULATED)
        FetchContent_Populate(tclap)
        # TCLAP is header-only, just set the include directory
        set(TCLAP_INCLUDE_DIR ${tclap_SOURCE_DIR}/include)
    endif()

    # Fetch Boost libraries required by the project
    set(BOOST_INCLUDE_LIBRARIES filesystem;program_options;system;assert;any;core;config)
    FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.86.0
    )
    FetchContent_MakeAvailable(boost)

    # Configure cucumber_cpp options before fetching
    set(CUKE_ENABLE_GTEST ON CACHE BOOL "Enable Google Test framework" FORCE)
    set(CUKE_ENABLE_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(CUKE_TESTS_UNIT OFF CACHE BOOL "Enable unit tests" FORCE)
    
    FetchContent_Declare(
        cucumber_cpp
        GIT_REPOSITORY https://github.com/cucumber/cucumber-cpp.git
        GIT_TAG main
        OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(cucumber_cpp)

    

# FetchContent_GetProperties(cucumber_cpp)
    # if(NOT cucumber_cpp_POPULATED)
    #    FetchContent_Populate(cucumber_cpp)

    #     # The content is now downloaded to ${cucumber_cpp_SOURCE_DIR}
    #     # You can now use the files directly (e.g., for header-only libs)
    #     # Do NOT call FetchContent_MakeAvailable or add_subdirectory
        
    #     # Add cucumber_cpp include directories to the project
    #     include_directories(${cucumber_cpp_SOURCE_DIR}/include)
    # endif()

endif()

find_package(cucumber_gherkin CONFIG REQUIRED)
find_package(cucumber_cpp CONFIG REQUIRED)
message(STATUS "cucumber_cpp source directory is: ${cucumber_cpp_SOURCE_DIR}")
message(STATUS "cucumber_cpp binary directory is: ${cucumber_cpp_BINARY_DIR}")

###
#
# Targets
#
###
add_subdirectory(src/lib)
add_subdirectory(src/examples)

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
