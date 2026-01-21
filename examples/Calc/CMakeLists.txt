project(Calc)

if(CUKE_CPP_BUILD_TESTS)
    add_library(Calc STATIC src/Calculator.cpp)
    target_include_directories(Calc INTERFACE src)

    # Apply coverage flags to test executable
    if(CUKE_CPP_ENABLE_COVERAGE)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(Calc PRIVATE -fprofile-arcs -ftest-coverage --coverage -O0 -g)
            target_link_options(Calc PRIVATE -fprofile-arcs -ftest-coverage --coverage)
        endif()
    endif()

    find_package(GTest QUIET)
    if(TARGET GTest::gtest)
        add_executable(GTestCalculatorSteps features/step_definitions/GTestCalculatorSteps.cpp)
        target_link_libraries(GTestCalculatorSteps PRIVATE Calc cucumber::cukecpp GTest::gtest gcov)

        add_executable(FuncArgsCalculatorSteps features/step_definitions/FuncArgsCalculatorSteps.cpp)
        target_link_libraries(FuncArgsCalculatorSteps PRIVATE Calc cucumber::cukecpp GTest::gtest gcov)
    endif()

    if(TARGET Boost::unit_test_framework)
        add_executable(BoostCalculatorSteps features/step_definitions/BoostCalculatorSteps.cpp)
        target_link_libraries(BoostCalculatorSteps PRIVATE Calc cucumber::cukecpp Boost::unit_test_framework gcov)
    endif()

    if(TARGET Qt::Test)
        add_executable(QtTestCalculatorSteps features/step_definitions/QtTestCalculatorSteps.cpp)
        target_link_libraries(QtTestCalculatorSteps Calc Qt::Test cucumber::cukecpp gcov)
    endif()
endif()
