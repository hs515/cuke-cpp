#include "CucumberConsoleReporter.hpp"

#include "client/CukeDocument.hpp"
#include "CukeUtilities.hpp"

#include <chrono>
#include <iomanip> 
#include <vector>

using namespace cuke::internal;

#define RST  "\033[0m"
#define RED  "\033[31m"
#define GRN  "\033[32m"
#define YEL  "\033[33m"
#define CYN  "\033[36m"
#define GRY  "\033[90m"
#define BOLD "\033[1m"

void CucumberConsoleReporter::executionBegin()
{
    myStartTime = now();
}

void CucumberConsoleReporter::executionEnd()
{
    uint64_t endTime = now();

    int totalScenarios = myPassedScenarios + mySkippedScenarios + myFailedScenarios;
    std::cout << totalScenarios << " scenarios";

    int scenarioCounts[3] = { myFailedScenarios, mySkippedScenarios, myPassedScenarios };
    std::string scenarioDescriptions[3] = {"failed", "skipped", "passed"};
    std::string scenarioColors[3] = {RED, CYN, GRN};

    std::cout << " (";
    for (int i = 0, acc = 0; i < 3; i++)
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

    int stepCounts[4] = { myFailedSteps, mySkippedSteps, myUndefSteps, myPassedSteps };
    std::string stepDescriptions[4] = {"failed", "skipped", "undefined", "passed"};
    std::string stepColors[4] = {RED, CYN, YEL, GRN};

    std::cout << " (";
    for (int i = 0, acc = 0; i < 4; i++)
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
    int min = static_cast<int>(timeCostInMSec / 60000);
    double sec = static_cast<double>(timeCostInMSec % 60000) / 1000;
    std::cout << "Finished in " << min << "m" << std::setprecision(3) << sec << "s" << std::endl;
}

void CucumberConsoleReporter::featureBegin(const CucumberFeature& feature)
{    
    std::cout << BOLD << "Feature: " << feature.name << RST << std::endl;
}

void CucumberConsoleReporter::featureEnd(const CucumberFeature& feature)
{
   
}

void CucumberConsoleReporter::featureSkip(const CucumberFeature& feature)
{
   
}

void CucumberConsoleReporter::scenarioBegin(const CucumberScenario& scenario)
{
    myScenarioSkipped = false;
    std::cout << Indent(2) << BOLD << "Scenario: " << scenario.name << RST << std::endl;
}

void CucumberConsoleReporter::scenarioEnd(const CucumberScenario& scenario)
{
    if (passed == scenario.status) myPassedScenarios++;
    else if (failed == scenario.status) myFailedScenarios++;
    else if (skipped == scenario.status) mySkippedScenarios++;
}

void CucumberConsoleReporter::scenarioSkip(const CucumberScenario& scenario)
{
    myScenarioSkipped = true;
}

void CucumberConsoleReporter::stepBegin(const CucumberStep& step)
{
    
}

void CucumberConsoleReporter::stepEnd(const CucumberStep& step)
{
    if (undefined == step.status)
    {
        myUndefSteps++;
        std::cout << Indent(4) << YEL << step.action << " " << step.text << RST;
        std::cout << GRY << " # Step definition undefined! Implement it using the snippet below?" << RST << std::endl;
        std::cout << std::endl << YEL << step.error << RST << std::endl;
    }
    else if (passed == step.status)
    {
        myPassedSteps++;
        std::cout << Indent(4) << GRN << step.action << " " << step.text << RST;
        std::cout << GRY << " # " << step.step_defs.at(0).source << RST << std::endl;
    }
    else if (failed == step.status)
    {
        myFailedSteps++;
        std::cout << Indent(4) << RED << step.action << " " << step.text << RST;
        std::cout << GRY << " # " << step.step_defs.at(0).source << RST << std::endl;
        std::cout << std::endl << RED << step.error << RST << std::endl;
    }
    else if (ambiguous == step.status)
    {
        std::cout << Indent(4) << RED << step.action << " " << step.text << RST;
        std::cout << GRY << " # Multiple ambiguous matches" << RST << std::endl;
        std::cout << std::endl << RED << "Ambiguous matches of \"" << step.text << "\":" << std::endl;
        for (auto&& stepInfo : step.step_defs)
        {
            std::cout << Indent(2) << stepInfo.source << ": /" << stepInfo.regexp << "/" << std::endl;
        }
        std::cout << RST << std::endl;
    }
}

void CucumberConsoleReporter::stepSkip(const CucumberStep& step)
{
    if (!myScenarioSkipped)
    {
        mySkippedSteps++;
        std::cout << Indent(4) << CYN << step.action << " " << step.text << RST << std::endl;
    }
}

std::ostream& cuke::internal::operator<<(std::ostream& os, const Indent& indent)
{
    for (int i = 0; i < indent.myValue; i++)
    {
        os << " ";
    }
    return os;
}
