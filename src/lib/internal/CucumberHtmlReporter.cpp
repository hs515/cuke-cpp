#include "CucumberHtmlReporter.hpp"

#include "client/CukeDocument.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace cuke::internal;
using json = nlohmann::json;

static void copyAll(const std::filesystem::path& src, const std::filesystem::path& dest);
static void render(const std::filesystem::path& tmpl, const std::filesystem::path& htmlFile, const json& data);

void CucumberHtmlReporter::executionBegin()
{
    myJson["startTime"] = now();
    myJson["features"] = json::array();
}

void CucumberHtmlReporter::executionEnd()
{
    myJson["endTime"] = now();
    dumpTo("bin/bdd/report");
}

void CucumberHtmlReporter::featureBegin(const CucumberFeature& feature)
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

void CucumberHtmlReporter::featureEnd(const CucumberFeature& feature)
{
    (*myFeatureNodePtr)["status"] = feature.getStatus();
    (*myFeatureNodePtr)["duration"] = feature.getEndTime() - feature.getStartTime();
    (*myFeatureNodePtr)["passedScenarios"] = myFeaturePassedScenarios;
    (*myFeatureNodePtr)["failedScenarios"] = myFeatureFailedScenarios;
    (*myFeatureNodePtr)["skippedScenarios"] = myFeatureSkippedScenarios;
}

void CucumberHtmlReporter::featureSkip(const CucumberFeature& feature)
{
    featureBegin(feature);
    myFeatureSkippedScenarios = feature.getScenarios().size();
    featureEnd(feature);
}

void CucumberHtmlReporter::scenarioBegin(const CucumberScenario& scenario)
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

void CucumberHtmlReporter::scenarioEnd(const CucumberScenario& scenario)
{
    (*myScenarioNodePtr)["startTime"] = scenario.getStartTime();
    (*myScenarioNodePtr)["endTime"] = scenario.getEndTime();

    if (scenario.getStatus() == passed) myFeaturePassedScenarios++; 
    else if (scenario.getStatus() == failed) myFeatureFailedScenarios++; 
}

void CucumberHtmlReporter::scenarioSkip(const CucumberScenario& scenario)
{
    scenarioBegin(scenario);
    (*myScenarioNodePtr)["duration"] = 0;
    myFeatureSkippedScenarios++;
}

void CucumberHtmlReporter::stepBegin(const CucumberStep& step)
{
    json stepNode;
    stepNode["keyword"] = step.getAction();
    stepNode["text"] = step.getText();
    json& treeNode = (*myScenarioNodePtr)["scenarioSteps"].emplace_back(stepNode);
    myStepNodePtr = &treeNode;
}

void CucumberHtmlReporter::stepEnd(const CucumberStep& step)
{
    (*myStepNodePtr)["status"] = step.getStatus();
    (*myStepNodePtr)["error"] = step.getError();
    (*myStepNodePtr)["duration"] = step.getEndTime() - step.getStartTime();
}

void CucumberHtmlReporter::stepSkip(const CucumberStep& step)
{
    stepBegin(step);
    stepEnd(step);
}

void CucumberHtmlReporter::dumpTo(const std::string& path)
{
    // Do not dump if no feature gets run
    if (myJson["features"].size() == 0)
    {
        return;
    }

    const std::filesystem::path srcDir("test/bdd/resources/report");
    const std::filesystem::path destDir(path);

    if (!std::filesystem::exists(destDir))
    {
        std::filesystem::create_directories(destDir);
    }

    // Generate features html
    json& features = myJson["features"];
    for (auto&& feature : features)
    {
        if (feature["featureScenarios"].size() != 0)
        {
            auto featureDest = destDir / (feature["featureFileName"].get<std::string>() + ".html");
            render(srcDir / "feature.html", featureDest, feature);
        }
    }

    // Generate index html
    for (auto&& feature : features)
    {
        // Index page does not use scenario info.
        feature.erase("featureScenarios");
    }
    render(srcDir / "index.html", destDir / "index.html", myJson);

    // Copy assets
    copyAll(srcDir / "assets", destDir);
}

static void copyAll(const std::filesystem::path& src, const std::filesystem::path& dest)
{
    if (std::filesystem::is_directory(src))
    {
        if (!std::filesystem::exists(dest))
        {
            std::filesystem::create_directories(dest);
        }
        for (const std::filesystem::directory_entry& item : std::filesystem::directory_iterator(src)) 
        {
            copyAll(item.path(), dest/item.path().filename());
        }
    } 
    else if (std::filesystem::is_regular_file(src))
    {
        std::filesystem::copy(src, dest, std::filesystem::copy_options::overwrite_existing);
    }
} 

static void render(const std::filesystem::path& tmpl, const std::filesystem::path& html, const json& data)
{
    std::ifstream in(tmpl.string());
    std::ofstream out(html.string());
    std::string line;
    while (std::getline(in, line))
    {
        if (line.find("<!-- data -->") != std::string::npos)
        {
            out << "<script>const data = "
                << data.dump()
                << "</script>" << std::endl;
        }
        else
        {
            out << line << std::endl;
        }
    }
    in.close();
    out.close();
}
