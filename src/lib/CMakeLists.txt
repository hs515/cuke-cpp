add_library(cukecpp_lib)
add_library(cucumber::cukecpp ALIAS cukecpp_lib)

add_library(cukecpp_lib_nomain)
add_library(cucumber::cukecpp_nomain ALIAS cukecpp_lib_nomain)

set(CUKE_CPP_LIB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB_RECURSE CUKE_CPP_LIB_SOURCES "${CUKE_CPP_LIB_SRC_DIR}/*.[ch]pp")
list(APPEND CUKE_CPP_LIB_ALL_SOURCES ${CUKE_CPP_LIB_SOURCES})

set(CUKE_CPP_LIB_SOURCES
    client/CukeDocument.cpp
    client/CukeDocument.hpp
    internal/CucumberConsoleReporter.cpp
    internal/CucumberConsoleReporter.hpp
    internal/CucumberEventListener.cpp
    internal/CucumberEventListener.hpp
    internal/CucumberHtmlReporter.cpp
    internal/CucumberHtmlReporter.hpp
    internal/CucumberJsonReporter.cpp
    internal/CucumberJsonReporter.hpp
    internal/CukeRunner.cpp
    internal/CukeRunner.hpp
    internal/CukeUtilities.cpp
    internal/CukeUtilities.hpp
    internal/FilterTagOptions.cpp
    internal/FilterTagOptions.hpp
    internal/ReporterOptions.cpp
    internal/ReporterOptions.hpp
    server/CukeServer.cpp
    server/CukeServer.hpp
)

target_sources(
    cukecpp_lib_nomain
    PRIVATE
        ${CUKE_CPP_LIB_SOURCES}
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
    cukecpp_lib_nomain
    PUBLIC
        cucumber::gherkin
        cucumber-cpp-internal
        cucumber::tag-expressions
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

target_sources(
    cukecpp_lib
    PRIVATE
        internal/main.cpp
)

target_link_libraries(
    cukecpp_lib
    PUBLIC
        cucumber::cukecpp_nomain
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
