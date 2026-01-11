#include "CucumberEventListener.hpp"

#include <chrono>

using namespace cuke::internal;

uint64_t CucumberEventListenerIF::now()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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
