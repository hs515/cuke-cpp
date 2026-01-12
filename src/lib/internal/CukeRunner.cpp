#include "CukeRunner.hpp"

#include "CucumberConsoleReporter.hpp"
#include "CucumberHtmlReporter.hpp"

using namespace cuke::internal;

CukeRunner::CukeRunner(const ListenerOptions& options)
{
    if (options.consoleReport)
    {
        myEventListener.add(std::make_shared<CucumberConsoleReporter>());
    }
    if (options.htmlReport)
    {
        myEventListener.add(std::make_shared<CucumberHtmlReporter>());
    }

    myEventListener.executionBegin();
}

CukeRunner::~CukeRunner()
{
    myEventListener.executionEnd();
}

void CukeRunner::beginFeature(const CucumberFeature& feature)
{
    myEventListener.featureBegin(feature);
}

void CukeRunner::skipFeature(const CucumberFeature& feature)
{
    myEventListener.featureSkip(feature);
}
void CukeRunner::endFeature(const CucumberFeature& feature)
{
    myEventListener.featureEnd(feature);
}

void CukeRunner::beginScenario(const CucumberScenario& scenario)
{
    myCukeServer.beginScenario(scenario.getTags());
    myEventListener.scenarioBegin(scenario);
}

void CukeRunner::skipScenario(const CucumberScenario& scenario)
{
    myEventListener.scenarioSkip(scenario);
}

void CukeRunner::endScenario(const CucumberScenario& scenario)
{
    myCukeServer.endScenario(scenario.getTags());
    myEventListener.scenarioEnd(scenario);
}

void CukeRunner::beginStep(const CucumberStep& step)
{
    myEventListener.stepBegin(step);
}

void CukeRunner::skipStep(const CucumberStep& step)
{
    myEventListener.stepSkip(step);
}

void CukeRunner::endStep(const CucumberStep& step)
{
    myEventListener.stepEnd(step);
}

bool CukeRunner::invokeStep(const CucumberStep& step, std::string& error)
{
    auto stepInfo = step.getStepDefs().at(0);
    std::vector<std::string> args;
    for (auto&& argPair : stepInfo.args)
    {
        args.emplace_back(argPair.second);
    }
    bool success = myCukeServer.invoke(stepInfo.id, args, error);
    return success;
}

std::string CukeRunner::snippetStep(const CucumberStep& step)
{
    return myCukeServer.snippetText(step.getAction(), step.getText(), step.getArgType());
}

std::vector<CucumberStepInfo> CukeRunner::stepMatch(const std::string& stepText)
{
    return myCukeServer.stepMatch(stepText);
}