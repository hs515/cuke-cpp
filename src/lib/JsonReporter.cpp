#include "JsonReporter.hpp"

#include "CukeDocument.hpp"
#include "CukeUtilities.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace cuke::internal;

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

void JsonReporter::executionBegin()
{
    myJson["start_time"] = now();
    myJson["end_time"] = now();
    myJson["duration"] = 0;
    myJson["passed"] = 0;
    myJson["failed"] = 0;
    myJson["skipped"] = 0;
    myJson["features"] = json::array();
}

void JsonReporter::executionEnd()
{
    myJson["end_time"] = now();
    myJson["duration"] = myJson["end_time"].get<uint64_t>() - myJson["start_time"].get<uint64_t>();
    myJson["passed"] = myPassedFeatures;
    myJson["failed"] = myFailedFeatures;
    myJson["skipped"] = mySkippedFeatures;

    dumpReport();
}

void JsonReporter::featureBegin(const CukeFeature& feature)
{
    json featureNode;
    json tagsNode = json::array();

    for (auto&& tag : feature.tags)
    {
        tagsNode.emplace_back(tag);
    }

    const std::filesystem::path filepath(feature.filename);
    featureNode["uri"] = feature.filename;
    featureNode["keyword"] = "Feature";
    featureNode["name"] = feature.name;
    featureNode["description"] = feature.description;
    featureNode["tags"] = tagsNode;
    featureNode["status"] = statusToString(pending);
    featureNode["start_time"] = now();
    featureNode["end_time"] = now();
    featureNode["duration"] = 0;
    featureNode["passed"] = 0;
    featureNode["failed"] = 0;
    featureNode["skipped"] = 0;
    featureNode["elements"] = json::array();

    myFeatureNodePtr = &(myJson["features"].emplace_back(featureNode));

    myFeatureSkippedScenarios = 0;
    myFeaturePassedScenarios = 0;
    myFeatureFailedScenarios = 0;
}

void JsonReporter::featureEnd(const CukeFeature& feature)
{
    (*myFeatureNodePtr)["status"] = statusToString(feature.status);
    (*myFeatureNodePtr)["start_time"] = feature.start_time;
    (*myFeatureNodePtr)["end_time"] = feature.end_time;
    (*myFeatureNodePtr)["duration"] = feature.end_time - feature.start_time;
    (*myFeatureNodePtr)["passed"] = myFeaturePassedScenarios;
    (*myFeatureNodePtr)["failed"] = myFeatureFailedScenarios;
    (*myFeatureNodePtr)["skipped"] = myFeatureSkippedScenarios;

    if (passed == feature.status) myPassedFeatures++; 
    else if (failed == feature.status) myFailedFeatures++; 
}

void JsonReporter::featureSkip(const CukeFeature& feature)
{
    mySkippedFeatures++;
    myFeatureSkippedScenarios = feature.scenarios.size();
}

void JsonReporter::scenarioBegin(const CukeScenario& scenario)
{
    json scenarioNode;
    json tagsNode = json::array();

    for (auto&& tag : scenario.tags)
    {
        tagsNode.emplace_back(tag);
    }

    scenarioNode["keyword"] = "Scenario";
    scenarioNode["name"] = scenario.name;
    scenarioNode["description"] = "";
    scenarioNode["tags"] = tagsNode;
    scenarioNode["status"] = statusToString(pending);
    scenarioNode["start_time"] = now();
    scenarioNode["end_time"] = now();
    scenarioNode["duration"] = 0;
    scenarioNode["steps"] = json::array();

    json& treeNode = (*myFeatureNodePtr)["elements"].emplace_back(scenarioNode);
    myScenarioNodePtr = &treeNode;
}

void JsonReporter::scenarioEnd(const CukeScenario& scenario)
{
    (*myScenarioNodePtr)["status"] = statusToString(scenario.status);
    (*myScenarioNodePtr)["start_time"] = scenario.start_time;
    (*myScenarioNodePtr)["end_time"] = scenario.end_time;
    (*myScenarioNodePtr)["duration"] = scenario.end_time - scenario.start_time;

    if (passed == scenario.status) myFeaturePassedScenarios++; 
    else if (failed == scenario.status) myFeatureFailedScenarios++;
}

void JsonReporter::scenarioSkip(const CukeScenario& scenario)
{
    myFeatureSkippedScenarios++;
}

void JsonReporter::stepBegin(const CukeStep& step)
{
    json stepNode;
    stepNode["keyword"] = step.action;
    stepNode["name"] = step.text;
    stepNode["status"] = statusToString(pending);
    stepNode["start_time"] = now();
    stepNode["end_time"] = now();
    stepNode["duration"] = 0;
    json& treeNode = (*myScenarioNodePtr)["steps"].emplace_back(stepNode);
    myStepNodePtr = &treeNode;
}

void JsonReporter::stepEnd(const CukeStep& step)
{
    (*myStepNodePtr)["status"] = statusToString(step.status);
    (*myStepNodePtr)["start_time"] = step.start_time;
    (*myStepNodePtr)["end_time"] = step.end_time;
    (*myStepNodePtr)["duration"] = step.end_time - step.start_time;
    if (step.error.has_value()) (*myStepNodePtr)["error_message"] = step.error.value();
}

void JsonReporter::stepSkip(const CukeStep& step)
{

}

const json& JsonReporter::getJson() const
{
    return myJson;
}

void JsonReporter::dumpReport()
{
    const std::filesystem::path destDir{"reports"};
    if (!std::filesystem::exists(destDir))
    {
        std::filesystem::create_directories(destDir);
    }

    std::ofstream reportFile(destDir / "cuke_report.json");

    if (!reportFile.is_open())
    {
        std::cerr << "Error: Unable to create JSON report file!" << std::endl;
        return;
    }

    reportFile << myJson.dump(4);
    reportFile.close();
}
