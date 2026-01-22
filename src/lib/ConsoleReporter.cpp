#include "ConsoleReporter.hpp"

#include "CukeDocument.hpp"
#include "CukeUtilities.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace cuke::internal;

namespace
{
    inline const std::string RST = "\033[0m";
    inline const std::string RED = "\033[31m";
    inline const std::string GRN = "\033[32m";
    inline const std::string YEL = "\033[33m";
    inline const std::string CYN = "\033[36m";
    inline const std::string GRY = "\033[90m";
    inline const std::string BOLD = "\033[1m";
}

void ConsoleReporter::executionBegin()
{
    myStartTime = now();
}

void ConsoleReporter::executionEnd()
{
    uint64_t endTime = now();

    int totalScenarios = myPassedScenarios + mySkippedScenarios + myFailedScenarios;
    std::cout << totalScenarios << " scenarios";

    std::vector<int> scenarioCounts = {myFailedScenarios, mySkippedScenarios, myPassedScenarios};
    std::vector<std::string> scenarioDescriptions = {"failed", "skipped", "passed"};
    std::vector<std::string> scenarioColors = {RED, CYN, GRN};

    std::cout << " (";
    for (size_t i = 0, acc = 0; i < scenarioCounts.size(); i++)
    {
        if (scenarioCounts[i] > 0)
        {
            if (acc > 0) std::cout << ", ";
            std::cout << scenarioColors[i] << scenarioCounts[i] << " " << scenarioDescriptions[i] << RST;
        } 
        acc += scenarioCounts[i];
    }
    std::cout << ")" << std::endl; 

    int totalSteps = myPassedSteps + myFailedSteps + myUndefSteps + mySkippedSteps;
    std::cout << totalSteps << " steps";

    std::vector<int> stepCounts = {myFailedSteps, mySkippedSteps, myUndefSteps, myPassedSteps};
    std::vector<std::string> stepDescriptions = {"failed", "skipped", "undefined", "passed"};
    std::vector<std::string> stepColors = {RED, CYN, YEL, GRN};

    std::cout << " (";
    for (size_t i = 0, acc = 0; i < stepCounts.size(); i++)
    {
        if (stepCounts[i] > 0)
        {
            if (acc > 0) std::cout << ", ";
            std::cout << stepColors[i] << stepCounts[i] << " " << stepDescriptions[i] << RST;
        } 
        acc += stepCounts[i];
    }
    std::cout << ")" << std::endl; 

    auto timeCostInMSec = endTime - myStartTime;
    auto min = static_cast<int>(timeCostInMSec / 60000);
    auto sec = static_cast<double>(timeCostInMSec % 60000) / 1000;
    std::cout << "Finished in " << min << "m" << std::setprecision(3) << sec << "s" << std::endl;
}

void ConsoleReporter::featureBegin(const CukeFeature& feature)
{    
    std::cout << BOLD << "Feature: " << feature.name << RST << std::endl;
}

void ConsoleReporter::featureEnd(const CukeFeature& feature)
{
    // Do nothing
}

void ConsoleReporter::scenarioBegin(const CukeScenario& scenario)
{
    std::cout << Indent(2) << BOLD << "Scenario: " << scenario.name << RST << std::endl;
}

void ConsoleReporter::scenarioEnd(const CukeScenario& scenario)
{
    if (CucumberExecutionStatus::passed == scenario.status) myPassedScenarios++;
    else if (CucumberExecutionStatus::failed == scenario.status) myFailedScenarios++;
    else if (CucumberExecutionStatus::skipped == scenario.status) mySkippedScenarios++;
}

void ConsoleReporter::scenarioSkip(const CukeScenario& scenario)
{
    // Do nothing
}

void ConsoleReporter::stepBegin(const CukeStep& step)
{
    // Do nothing
}

void ConsoleReporter::stepEnd(const CukeStep& step)
{
    if (CucumberExecutionStatus::undefined == step.status)
    {
        myUndefSteps++;
        std::cout << Indent(4) << YEL << step.action << " " << step.text << RST;
        std::cout << GRY << " # Step definition undefined! Implement it using the snippet below?" << RST << std::endl;
        std::cout << std::endl << YEL << step.error.value() << RST << std::endl;
    }
    else if (CucumberExecutionStatus::passed == step.status)
    {
        myPassedSteps++;
        std::cout << Indent(4) << GRN << step.action << " " << step.text << RST;
        std::cout << GRY << " # " << step.step_defs.at(0).source << RST << std::endl;
    }
    else if (CucumberExecutionStatus::failed == step.status)
    {
        myFailedSteps++;
        std::cout << Indent(4) << RED << step.action << " " << step.text << RST;
        std::cout << GRY << " # " << step.step_defs.at(0).source << RST << std::endl;
        std::cout << std::endl << RED << step.error.value() << RST << std::endl;
    }
    else if (CucumberExecutionStatus::ambiguous == step.status)
    {
        std::cout << Indent(4) << RED << step.action << " " << step.text << RST;
        std::cout << GRY << " # Multiple ambiguous matches" << RST << std::endl;
        std::cout << std::endl << RED << step.error.value() << RST << std::endl;
    }
    else if (CucumberExecutionStatus::skipped == step.status)
    {
        mySkippedSteps++;
        std::cout << Indent(4) << CYN << step.action << " " << step.text << RST << std::endl;
    }
}

void ConsoleReporter::stepSkip(const CukeStep& step)
{
    // Do nothing
}
std::ostream& Indent::output(std::ostream& os) const
{
    for (int i = 0; i < myValue; i++)
    {
        os << " ";
    }
    return os;
}
