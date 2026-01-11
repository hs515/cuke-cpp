add_library(cukecpp_lib)
add_library(cucumber::cukecpp ALIAS cukecpp_lib)

add_library(cukecpp_lib_nomain)
add_library(cucumber::cukecpp_nomain ALIAS cukecpp_lib_nomain)

set(CUKE_CPP_LIB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE CUKE_CPP_LIB_SOURCES "${CUKE_CPP_LIB_SRC_DIR}/*.[ch]pp")
list(APPEND CUKE_CPP_LIB_ALL_SOURCES ${CUKE_CPP_LIB_SOURCES})

set(CUKE_CPP_LIB_SOURCES
    client/CukeBuilder.cpp
    client/CukeBuilder.hpp
    client/CukeDocument.cpp
    client/CukeDocument.hpp
    internal/CucumberBackend.hpp
    internal/CucumberConsoleReporter.cpp
    internal/CucumberConsoleReporter.hpp
    internal/CucumberEventListener.cpp
    internal/CucumberEventListener.hpp
    internal/CucumberHtmlReporter.cpp
    internal/CucumberHtmlReporter.hpp
    internal/CucumberStepInfo.hpp
    internal/CukeBackend.cpp
    internal/CukeBackend.hpp
    internal/ListenerOptions.hpp
    server/CukeServer.cpp
    server/CukeServer.hpp
)

target_sources(
    cukecpp_lib
    PRIVATE
        ${CUKE_CPP_LIB_SOURCES}
        internal/main.cpp
)

target_sources(
    cukecpp_lib_nomain
    PRIVATE
        ${CUKE_CPP_LIB_SOURCES}
)

message(STATUS "cucumber cpp build directory is: ${cucumber_cpp_BINARY_DIR}")
target_include_directories(
    cukecpp_lib
    PUBLIC
        $<BUILD_INTERFACE:${CUKE_CPP_LIB_INC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cucumber>
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${cucumber_cpp_SOURCE_DIR}/include
        ${cucumber_cpp_BINARY_DIR}/src # for generated export files, e.g., CukeCppExport.hpp
)

target_include_directories(
    cukecpp_lib_nomain
    PUBLIC
        $<BUILD_INTERFACE:${CUKE_CPP_LIB_INC_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cucumber>
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${cucumber_cpp_SOURCE_DIR}/include
        ${cucumber_cpp_BINARY_DIR}/src # for generated export files, e.g., CukeCppExport.hpp
)

target_link_libraries(
    cukecpp_lib
    PUBLIC
        cucumber::gherkin
        cucumber-cpp-internal
)

target_link_libraries(
    cukecpp_lib_nomain
    PUBLIC
        cucumber::gherkin
        cucumber-cpp-internal
)

set_target_properties(
    cukecpp_lib
    PROPERTIES
        CXX_STANDARD 17 
        VERSION ${VERSION}
        SOVERSION ${VER_MAJOR}
        EXPORT_NAME cukecpp
        OUTPUT_NAME cukecpp
)

set_target_properties(
    cukecpp_lib_nomain
    PROPERTIES
        CXX_STANDARD 17 
        VERSION ${VERSION}
        SOVERSION ${VER_MAJOR}
        EXPORT_NAME cukecpp_nomain
        OUTPUT_NAME cukecpp_nomain
)
