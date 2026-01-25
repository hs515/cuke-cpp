#include <cucumber-cpp/autodetect.hpp>

#include <Calculator.hpp>

using cucumber::ScenarioScope;

struct CalcCtx {
    Calculator calc;
    double result;
};

GIVEN("^I have entered (\\d+) into the calculator$") {
    REGEX_PARAM(double, n);
    ScenarioScope<CalcCtx> context;

    context->calc.push(n);
}

WHEN("^I press add") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.add();
}

WHEN("^I press divide") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.divide();
}

THEN("^the result should be (.*) on the screen$") {
    REGEX_PARAM(double, expected);
    ScenarioScope<CalcCtx> context;

    EXPECT_EQ(expected, context->result);
}

GIVEN("^I have entered numbers from a table into the calculator$") {
    TABLE_PARAM(rows);
    ScenarioScope<CalcCtx> context;

    for (const auto& row: rows.hashes()) {
        context->calc.push(std::stod(row.at("number")));
    }
}

GIVEN("^I have entered numbers from a multi-line text into the calculator$") {
    REGEX_PARAM(std::string, text);
    ScenarioScope<CalcCtx> context;

    std::stringstream ss(text);
    std::string num;
    while (ss >> num) {
        context->calc.push(std::stod(num));
    }
}

GIVEN("I have entered an integer number {double} into the calculator") {
    REGEX_PARAM(double, n);
    ScenarioScope<CalcCtx> context;

    context->calc.push(n);
}

GIVEN("^I have entered an integer number (\\d+) into the calculator$") {
    REGEX_PARAM(double, n);
    ScenarioScope<CalcCtx> context;

    context->calc.push(n);
}
