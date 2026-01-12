#include "CucumberEventListener.hpp"

#include "CucumberConsoleReporter.hpp"
#include "CucumberHtmlReporter.hpp"
#include "CucumberJsonReporter.hpp"
#include "ListenerOptions.hpp"

using namespace cuke::internal;

CucumberEventListener::CucumberEventListener(const ListenerOptions& options)
{
    if (options.consoleReport)
    {
        add(std::make_shared<CucumberConsoleReporter>());
    }
    if (options.jsonReport)
    {
        add(std::make_shared<CucumberJsonReporter>());
    }
    if (options.htmlReport)
    {
        add(std::make_shared<CucumberHtmlReporter>());
    }
}

void CucumberEventListener::executionBegin()
{
    for (auto&& listener : myListeners)
    {
        listener->executionBegin();
    }
}

void CucumberEventListener::executionEnd()
{
    for (auto&& listener : myListeners)
    {
        listener->executionEnd();
    }
}

void CucumberEventListener::featureBegin(const CucumberFeature& feature)
{    
    for (auto&& listener : myListeners)
    {
        listener->featureBegin(feature);
    }
}

void CucumberEventListener::featureEnd(const CucumberFeature& feature)
{
    for (auto&& listener : myListeners)
    {
        listener->featureEnd(feature);
    }
}

void CucumberEventListener::featureSkip(const CucumberFeature& feature)
{
    for (auto&& listener : myListeners)
    {
        listener->featureSkip(feature);
    }
}

void CucumberEventListener::scenarioBegin(const CucumberScenario& scenario)
{
    for (auto&& listener : myListeners)
    {
        listener->scenarioBegin(scenario);
    }
}

void CucumberEventListener::scenarioEnd(const CucumberScenario& scenario)
{
    for (auto&& listener : myListeners)
    {
        listener->scenarioEnd(scenario);
    }
}

void CucumberEventListener::scenarioSkip(const CucumberScenario& scenario)
{
    for (auto&& listener : myListeners)
    {
        listener->scenarioSkip(scenario);
    }
}

void CucumberEventListener::stepBegin(const CucumberStep& step)
{
    for (auto&& listener : myListeners)
    {
        listener->stepBegin(step);
    }
}

void CucumberEventListener::stepEnd(const CucumberStep& step)
{
    for (auto&& listener : myListeners)
    {
        listener->stepEnd(step);
    }
}

void CucumberEventListener::stepSkip(const CucumberStep& step)
{
    for (auto&& listener : myListeners)
    {
        listener->stepSkip(step);
    }
}

void CucumberEventListener::add(const std::shared_ptr<CucumberEventListenerIF> listener)
{
    myListeners.push_back(listener);
}
