#include "CucumberJsonReporter.hpp"

#include "client/CukeDocument.hpp"
#include "CukeUtilities.hpp"

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
    myJson["startTime"] = now();
    myJson["features"] = json::array();
}

void CucumberJsonReporter::executionEnd()
{
    myJson["endTime"] = now();
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

    for (auto&& tag : feature.tags)
    {
        tagsNode.emplace_back(tag);
    }

    featureNode["featureName"] = feature.name;
    featureNode["featureFileName"] = feature.filename;
    featureNode["featureTags"] = tagsNode;
    featureNode["featureScenarios"] = json::array();

    myFeatureNodePtr = &(myJson["features"].emplace_back(featureNode));

    myFeatureSkippedScenarios = 0;
    myFeaturePassedScenarios = 0;
    myFeatureFailedScenarios = 0;
}

void CucumberJsonReporter::featureEnd(const CucumberFeature& feature)
{
    (*myFeatureNodePtr)["status"] = statusToString(feature.status);
    (*myFeatureNodePtr)["startTime"] = feature.start_time;
    (*myFeatureNodePtr)["endTime"] = feature.end_time;
    (*myFeatureNodePtr)["duration"] = feature.end_time - feature.start_time;
    (*myFeatureNodePtr)["passedScenarios"] = myFeaturePassedScenarios;
    (*myFeatureNodePtr)["failedScenarios"] = myFeatureFailedScenarios;
    (*myFeatureNodePtr)["skippedScenarios"] = myFeatureSkippedScenarios;

    if (passed == feature.status) myPassedFeatures++; 
    else if (failed == feature.status) myFailedFeatures++; 
}

void CucumberJsonReporter::featureSkip(const CucumberFeature& feature)
{
    featureBegin(feature);
    mySkippedFeatures++;
    myFeatureSkippedScenarios = feature.scenarios.size();
    featureEnd(feature);
}

void CucumberJsonReporter::scenarioBegin(const CucumberScenario& scenario)
{
    json scenarioNode;
    json tagsNode = json::array();

    for (auto&& tag : scenario.tags)
    {
        tagsNode.emplace_back(tag);
    }

    scenarioNode["scenarioName"] = scenario.name;
    scenarioNode["scenarioTags"] = tagsNode;
    scenarioNode["scenarioSteps"] = json::array();

    json& treeNode = (*myFeatureNodePtr)["featureScenarios"].emplace_back(scenarioNode);
    myScenarioNodePtr = &treeNode;
}

void CucumberJsonReporter::scenarioEnd(const CucumberScenario& scenario)
{
    (*myScenarioNodePtr)["startTime"] = scenario.start_time;
    (*myScenarioNodePtr)["endTime"] = scenario.end_time;
    (*myScenarioNodePtr)["duration"] = scenario.end_time - scenario.start_time;
    (*myScenarioNodePtr)["status"] = statusToString(scenario.status);

    if (passed == scenario.status) myFeaturePassedScenarios++; 
    else if (failed == scenario.status) myFeatureFailedScenarios++; 
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
    stepNode["keyword"] = step.action;
    stepNode["text"] = step.text;
    json& treeNode = (*myScenarioNodePtr)["scenarioSteps"].emplace_back(stepNode);
    myStepNodePtr = &treeNode;
}

void CucumberJsonReporter::stepEnd(const CucumberStep& step)
{
    (*myStepNodePtr)["status"] = statusToString(step.status);
    (*myStepNodePtr)["error"] = step.error;
    (*myStepNodePtr)["startTime"] = step.start_time;
    (*myStepNodePtr)["endTime"] = step.end_time;
    (*myStepNodePtr)["duration"] = step.end_time - step.start_time;
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
