#include "CucumberJsonReporter.hpp"

#include "client/CukeDocument.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace cuke::internal;
using json = nlohmann::json;

static std::string statusToString(CucumberExecutionStatus status)
{
    switch (status)
    {
        case pending: return "pending";
        case passed: return "passed";
        case failed: return "failed";
        case undefined: return "undefined";
        case ambiguous: return "ambiguous";
        case skipped: return "skipped";
        default: return "Unknown";
    }
}

void CucumberJsonReporter::executionBegin()
{
    myJson["startTime"] = CucumberRunnable::now();
    myJson["features"] = json::array();
}

void CucumberJsonReporter::executionEnd()
{
    myJson["endTime"] = CucumberRunnable::now();
    myJson["duration"] = myJson["endTime"].get<uint64_t>() - myJson["startTime"].get<uint64_t>();
    myJson["passedFeatures"] = myPassedFeatures;
    myJson["failedFeatures"] = myFailedFeatures;
    myJson["skippedFeatures"] = mySkippedFeatures;

    dumpReport();
}

void CucumberJsonReporter::featureBegin(const CucumberFeature& feature)
{
    json featureNode;
    json tagsNode = json::array();

    for (auto&& tag : feature.getTags())
    {
        tagsNode.emplace_back(tag);
    }

    featureNode["featureName"] = feature.getName();
    featureNode["featureFileName"] = feature.getFilename();
    featureNode["featureTags"] = tagsNode;
    featureNode["featureScenarios"] = json::array();

    myFeatureNodePtr = &(myJson["features"].emplace_back(featureNode));

    myFeatureSkippedScenarios = 0;
    myFeaturePassedScenarios = 0;
    myFeatureFailedScenarios = 0;
}

void CucumberJsonReporter::featureEnd(const CucumberFeature& feature)
{
    (*myFeatureNodePtr)["status"] = statusToString(feature.getStatus());
    (*myFeatureNodePtr)["startTime"] = feature.getStartTime();
    (*myFeatureNodePtr)["endTime"] = feature.getEndTime();
    (*myFeatureNodePtr)["duration"] = feature.getEndTime() - feature.getStartTime();
    (*myFeatureNodePtr)["passedScenarios"] = myFeaturePassedScenarios;
    (*myFeatureNodePtr)["failedScenarios"] = myFeatureFailedScenarios;
    (*myFeatureNodePtr)["skippedScenarios"] = myFeatureSkippedScenarios;

    if (feature.getStatus() == passed) myPassedFeatures++; 
    else if (feature.getStatus() == failed) myFailedFeatures++; 
}

void CucumberJsonReporter::featureSkip(const CucumberFeature& feature)
{
    featureBegin(feature);
    mySkippedFeatures++;
    myFeatureSkippedScenarios = feature.getScenarios().size();
    featureEnd(feature);
}

void CucumberJsonReporter::scenarioBegin(const CucumberScenario& scenario)
{
    json scenarioNode;
    json tagsNode = json::array();

    for (auto&& tag : scenario.getTags())
    {
        tagsNode.emplace_back(tag);
    }

    scenarioNode["scenarioName"] = scenario.getName();
    scenarioNode["scenarioTags"] = tagsNode;
    scenarioNode["scenarioSteps"] = json::array();

    json& treeNode = (*myFeatureNodePtr)["featureScenarios"].emplace_back(scenarioNode);
    myScenarioNodePtr = &treeNode;
}

void CucumberJsonReporter::scenarioEnd(const CucumberScenario& scenario)
{
    (*myScenarioNodePtr)["startTime"] = scenario.getStartTime();
    (*myScenarioNodePtr)["endTime"] = scenario.getEndTime();
    (*myScenarioNodePtr)["duration"] = scenario.getEndTime() - scenario.getStartTime();
    (*myScenarioNodePtr)["status"] = statusToString(scenario.getStatus());

    if (scenario.getStatus() == passed) myFeaturePassedScenarios++; 
    else if (scenario.getStatus() == failed) myFeatureFailedScenarios++; 
}

void CucumberJsonReporter::scenarioSkip(const CucumberScenario& scenario)
{
    scenarioBegin(scenario);
    (*myScenarioNodePtr)["duration"] = 0;
    myFeatureSkippedScenarios++;
}

void CucumberJsonReporter::stepBegin(const CucumberStep& step)
{
    json stepNode;
    stepNode["keyword"] = step.getAction();
    stepNode["text"] = step.getText();
    json& treeNode = (*myScenarioNodePtr)["scenarioSteps"].emplace_back(stepNode);
    myStepNodePtr = &treeNode;
}

void CucumberJsonReporter::stepEnd(const CucumberStep& step)
{
    (*myStepNodePtr)["status"] = statusToString(step.getStatus());
    (*myStepNodePtr)["error"] = step.getError();
    (*myStepNodePtr)["startTime"] = step.getStartTime();
    (*myStepNodePtr)["endTime"] = step.getEndTime();
    (*myStepNodePtr)["duration"] = step.getEndTime() - step.getStartTime();
}

void CucumberJsonReporter::stepSkip(const CucumberStep& step)
{
    stepBegin(step);
    stepEnd(step);
}

const json& CucumberJsonReporter::getJson() const
{
    return myJson;
}

void CucumberJsonReporter::dumpReport()
{
    const std::filesystem::path destDir("reports");
    if (!std::filesystem::exists(destDir))
    {
        std::filesystem::create_directories(destDir);
    }

    std::ofstream reportFile("report/cuke_report.json");

    if (!reportFile.is_open())
    {
        std::cerr << "Error: Unable to create JSON report file!" << std::endl;
        return;
    }

    reportFile << myJson.dump(4);
    reportFile.close();
}
