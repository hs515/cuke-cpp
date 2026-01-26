add_library(cukecpp_lib)
add_library(cucumber::cukecpp ALIAS cukecpp_lib)

add_library(cukecpp_lib_nomain)
add_library(cucumber::cukecpp_nomain ALIAS cukecpp_lib_nomain)

set(CUKE_CPP_LIB_SOURCES
    lib/ConsoleReporter.cpp
    lib/ConsoleReporter.hpp
    lib/CukeDocument.cpp
    lib/CukeDocument.hpp
    lib/CukeRunner.cpp
    lib/CukeRunner.hpp
    lib/CukeServer.cpp
    lib/CukeServer.hpp
    lib/CukeUtilities.cpp
    lib/CukeUtilities.hpp
    lib/EventListener.cpp
    lib/EventListener.hpp
    lib/FilterTagOptions.cpp
    lib/FilterTagOptions.hpp
    lib/HtmlReporter.cpp
    lib/HtmlReporter.hpp
    lib/JsonReporter.cpp
    lib/JsonReporter.hpp
    lib/ReporterOptions.cpp
    lib/ReporterOptions.hpp
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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<BUILD_INTERFACE:${cucumber_cpp_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${cucumber_cpp_SOURCE_DIR}/src> # for generated export files, e.g., CukeExport.hpp
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cucumber>
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
        lib/main.cpp
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

git_get_version(CUKE_VERSION)
message(STATUS "Version: ${CUKE_VERSION}")
target_compile_definitions(cukecpp_lib PRIVATE
    CUKE_VERSION="${CUKE_VERSION}"
)

#include(GNUInstallDirs)
#install(
#    FILES
#        "$<BUILD_INTERFACE:${cucumber_cpp_SOURCE_DIR}/src>/cucumber-cpp/internal/CukeExport.hpp"
#    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cucumber-cpp/internal"
#)

install(
    TARGETS
    	cukecpp_lib
        cukecpp_lib_nomain
        cucumber-cpp-internal
    EXPORT   CukeCpp
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT      CukeCpp
    NAMESPACE   CukeCpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
    FILE        CukeCppConfig.cmake
)

