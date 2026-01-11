#include "CukeBackend.hpp"

#include "CucumberConsoleReporter.hpp"
#include "CucumberHtmlReporter.hpp"

using namespace cuke::internal;

CukeBackend::CukeBackend(const ListenerOptions& options)
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

CukeBackend::~CukeBackend()
{
    myEventListener.executionEnd();
}

void CukeBackend::beginFeature(const CucumberFeature& feature)
{
    myEventListener.featureBegin(feature);
}

void CukeBackend::skipFeature(const CucumberFeature& feature)
{
    myEventListener.featureSkip(feature);
}
void CukeBackend::endFeature(const CucumberFeature& feature)
{
    myEventListener.featureEnd(feature);
}

void CukeBackend::beginScenario(const CucumberScenario& scenario)
{
    myCukeServer.beginScenario(scenario.getTags());
    myEventListener.scenarioBegin(scenario);
}

void CukeBackend::skipScenario(const CucumberScenario& scenario)
{
    myEventListener.scenarioSkip(scenario);
}

void CukeBackend::endScenario(const CucumberScenario& scenario)
{
    myCukeServer.endScenario(scenario.getTags());
    myEventListener.scenarioEnd(scenario);
}

void CukeBackend::beginStep(const CucumberStep& step)
{
    myEventListener.stepBegin(step);
}

void CukeBackend::skipStep(const CucumberStep& step)
{
    myEventListener.stepSkip(step);
}

void CukeBackend::endStep(const CucumberStep& step)
{
    myEventListener.stepEnd(step);
}

bool CukeBackend::invokeStep(const CucumberStep& step, std::string& error)
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

std::vector<CucumberStepInfo> CukeBackend::stepMatch(const std::string& stepText)
{
    return myCukeServer.stepMatch(stepText);
}