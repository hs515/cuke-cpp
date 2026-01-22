#include "EventListener.hpp"

#include "ConsoleReporter.hpp"
#include "HtmlReporter.hpp"
#include "JsonReporter.hpp"
#include "ReporterOptions.hpp"

using namespace cuke::internal;

EventListener::EventListener(const ReporterOptions& options)
{
    if (options.consoleReport)
    {
        add(std::make_shared<ConsoleReporter>());
    }
    if (options.jsonReport)
    {
        add(std::make_shared<JsonReporter>());
    }
    if (options.htmlReport)
    {
        add(std::make_shared<HtmlReporter>());
    }
}

void EventListener::executionBegin()
{
    for (const auto& listener : myListeners)
    {
        listener->executionBegin();
    }
}

void EventListener::executionEnd()
{
    for (const auto& listener : myListeners)
    {
        listener->executionEnd();
    }
}

void EventListener::featureBegin(const CukeFeature& feature)
{    
    for (const auto& listener : myListeners)
    {
        listener->featureBegin(feature);
    }
}

void EventListener::featureEnd(const CukeFeature& feature)
{
    for (const auto& listener : myListeners)
    {
        listener->featureEnd(feature);
    }
}

void EventListener::scenarioBegin(const CukeScenario& scenario)
{
    for (const auto& listener : myListeners)
    {
        listener->scenarioBegin(scenario);
    }
}

void EventListener::scenarioEnd(const CukeScenario& scenario)
{
    for (const auto& listener : myListeners)
    {
        listener->scenarioEnd(scenario);
    }
}

void EventListener::scenarioSkip(const CukeScenario& scenario)
{
    for (const auto& listener : myListeners)
    {
        listener->scenarioSkip(scenario);
    }
}

void EventListener::stepBegin(const CukeStep& step)
{
    for (const auto& listener : myListeners)
    {
        listener->stepBegin(step);
    }
}

void EventListener::stepEnd(const CukeStep& step)
{
    for (const auto& listener : myListeners)
    {
        listener->stepEnd(step);
    }
}

void EventListener::stepSkip(const CukeStep& step)
{
    for (const auto& listener : myListeners)
    {
        listener->stepSkip(step);
    }
}

void EventListener::add(const std::shared_ptr<EventListenerIF> listener)
{
    myListeners.push_back(listener);
}
