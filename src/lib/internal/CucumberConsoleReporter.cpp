#include "CucumberConsoleReporter.hpp"

#include "client/CukeDocument.hpp"

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

    int totalScenarios = myPassedScenarios + myFailedScenarios;
    std::cout << totalScenarios << " scenarios";
    std::cout << " (";
    if (myFailedScenarios > 0) 
    {
        std::cout << RED << myFailedScenarios << " failed" << RST;
    }
    if (myPassedScenarios > 0) 
    {
        if (myFailedScenarios > 0) std::cout << ", ";
        std::cout << GRN << myPassedScenarios << " passed" << RST;
    }
    std::cout << ")" << std::endl; 

    int totalSteps = myPassedSteps + myFailedSteps + myUndefSteps + mySkippedSteps;
    std::cout << totalSteps << " steps";

    int counts[4] = { myFailedSteps, mySkippedSteps, myUndefSteps, myPassedSteps };
    std::string descriptions[4] = {"failed", "skipped", "undefined", "passed"};
    std::string colors[4] = {RED, CYN, YEL, GRN};

    std::cout << " (";
    for (int i = 0, acc = 0; i < 4; i++)
    {
        if (counts[i] > 0)
        {
            if (acc > 0) std::cout << ", ";
            std::cout << colors[i] << counts[i] << " " << descriptions[i] << RST;
        } 
        acc += counts[i];
    }
    std::cout << ")" << std::endl; 

    auto timeCostInMSec = endTime - myStartTime;
    int min = static_cast<int>(timeCostInMSec / 60000);
    double sec = static_cast<double>(timeCostInMSec % 60000) / 1000;
    std::cout << "Finished in " << min << "m" << std::setprecision(3) << sec << "s" << std::endl;
}

void CucumberConsoleReporter::featureBegin(const CucumberFeature& feature)
{    
    std::cout << BOLD << "Feature: " << feature.getName() << RST << std::endl;
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
    std::cout << Indent(2) << BOLD << "Scenario: " << scenario.getName() << RST << std::endl;
}

void CucumberConsoleReporter::scenarioEnd(const CucumberScenario& scenario)
{
    if (scenario.getStatus() == passed) myPassedScenarios++;
    else if (scenario.getStatus() == failed) myFailedScenarios++;
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
    if (step.getStatus() == undefined)
    {
        myUndefSteps++;
        std::cout << Indent(4) << YEL << step.getAction() << " " << step.getText() << RST << std::endl;
    }
    else if (step.getStatus() == passed)
    {
        myPassedSteps++;
        std::cout << Indent(4) << GRN << step.getAction() << " " << step.getText() << RST;
        std::cout << GRY << " # " << step.getStepDefs().at(0).source << RST << std::endl;
    }
    else if (step.getStatus() == failed)
    {
        myFailedSteps++;
        std::cout << Indent(4) << RED << step.getAction() << " " << step.getText() << RST;
        std::cout << GRY << " # " << step.getStepDefs().at(0).source << RST << std::endl;
    }
    else if (step.getStatus() == ambiguous)
    {
        std::cout << Indent(4) << RED << step.getAction() << " " << step.getText() << std::endl;
        std::cout << Indent(6) << "Ambiguous match of \"" << step.getText() << "\":" << std::endl;
        for (auto&& stepInfo : step.getStepDefs())
        {
            std::cout << Indent(8) << stepInfo.source << ": /" << stepInfo.regexp << "/" << std::endl;
        }
        std::cout << RST << std::endl;
    }
}

void CucumberConsoleReporter::stepSkip(const CucumberStep& step)
{
    if (!myScenarioSkipped)
    {
        mySkippedSteps++;
        std::cout << Indent(4) << CYN << step.getAction() << " " << step.getText() << RST << std::endl;
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
