#include "CukeDocument.hpp"

#include "CukeBuilder.hpp"
#include "internal/CucumberRunner.hpp"

#include <cctype>
#include <chrono>
#include <vector>

using namespace cuke::internal;

// Helper function for case-insensitive string comparison
static bool iequals(const std::string& a, const std::string& b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

void CucumberRunnable::setStatus(CucumberExecutionStatus status)
{
    myStatus = status;
}

CucumberExecutionStatus CucumberRunnable::getStatus() const
{
    return myStatus;
}

void CucumberRunnable::setStartTime()
{
    myStartTime = now();
}

uint64_t CucumberRunnable::getStartTime() const
{
    return myStartTime;
}

void CucumberRunnable::setEndTime()
{
    myEndTime = now();
}

uint64_t CucumberRunnable::getEndTime() const
{
    return myEndTime;
}

uint64_t CucumberRunnable::now() const
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void CucumberStep::setStepDefs(const std::vector<CucumberStepInfo>& stepDefs)
{
    myStepDefs = stepDefs;
}

const std::vector<CucumberStepInfo>& CucumberStep::getStepDefs() const
{
    return myStepDefs;
}

void CucumberStep::setAction(const std::string& action)
{
    myAction = action;
}

std::string CucumberStep::getAction() const
{
    return myAction;
}

void CucumberStep::setText(const std::string& text)
{
    myText = text;
}

std::string CucumberStep::getText() const
{
    return myText;
}

void CucumberStep::setError(const std::string& error)
{
    myError = error;
}

std::string CucumberStep::getError() const
{
    return myError;
}

void CucumberStep::setArgType(const std::string& type)
{
    myArgType = type;
}

std::string CucumberStep::getArgType() const
{
    return myArgType;
}

void CucumberStep::setTableArg(const CucumberTableArg& tableArg)
{
    myTableArg = tableArg;
}

const CucumberTableArg& CucumberStep::getTableArg() const
{
    return myTableArg;
}

void CucumberStep::setDocStringArg(const std::string& docString)
{
    myDocStringArg = docString;
}

std::string CucumberStep::getDocStringArg() const
{
    return myDocStringArg;
}

void CucumberTaggable::setName(const std::string& name)
{
    myName = name;
}

std::string CucumberTaggable::getName() const
{
    return myName;
}

void CucumberTaggable::addTag(const std::string& tag)
{
    myTags.emplace_back(tag);
}

const std::vector<std::string>& CucumberTaggable::getTags() const
{
    return myTags;
}

bool CucumberTaggable::hasTag(const std::string& tagName) const
{
    for (auto& tag : myTags)
    {
        if (iequals(tag, tagName))
        {
            return true;
        }
    }
    return false;
}

bool CucumberTaggable::hasWip() const
{
    return hasTag("@WIP") || hasTag("@IGNORE") || hasTag("@SKIP");
}

bool CucumberTaggable::hasAnyTag(const std::vector<std::string>& tags) const
{
    for (auto& tag : tags)
    {
        if (hasTag(tag))
        {
            return true;
        }
    }
    return false;
}

void CucumberScenario::addStep(const CucumberStep& step)
{
    mySteps.emplace_back(step);
}

const std::vector<CucumberStep>& CucumberScenario::getSteps() const
{
    return mySteps;
}

std::vector<CucumberStep>& CucumberScenario::getSteps()
{
    return mySteps;
}

void CucumberFeature::setDescription(const std::string& description)
{
    myDescription = description;
}

std::string CucumberFeature::getDescription() const
{
    return myDescription;
}

void CucumberFeature::addScenario(const CucumberScenario& scenario)
{
    myScenarios.emplace_back(scenario);
}

const std::vector<CucumberScenario>& CucumberFeature::getScenarios() const
{
    return myScenarios;
}

std::vector<CucumberScenario>& CucumberFeature::getScenarios()
{
    return myScenarios;
}

const std::string& CucumberFeature::getFilename() const
{
    return myFilename;
}


//----- 
CucumberFeature::CucumberFeature(const std::string& filename)
    : myFilename(filename)
{
    CukeBuilder::buildFeature(*this, filename);
}

bool CucumberFeature::run(CucumberRunner& runner, const std::vector<std::string>& filterTags)
{
    setStartTime();
    runner.beginFeature(*this);

    bool runningOk = true;
    if (hasWip())
    {
        skip(runner);
    }
    else
    {
        for (auto&& scenario : getScenarios())
        {
            runningOk = scenario.run(runner, filterTags) && runningOk;
        }
        setStatus(runningOk ? passed : failed);
    }

    setEndTime();
    runner.endFeature(*this);
    return runningOk;
}

void CucumberFeature::skip(CucumberRunner& runner)
{
    setStatus(skipped);
    runner.skipFeature(*this);
    for (auto&& scenario : myScenarios)
    {
        scenario.skip(runner);
    }
}

bool CucumberScenario::run(CucumberRunner& runner, const std::vector<std::string>& filterTags)
{
    setStartTime();
    runner.beginScenario(*this);

    bool runningOk = true;
    if (hasWip())
    {
        skip(runner);
    }
    else if (filterTags.size() > 0 && !hasAnyTag(filterTags))
    {
        skip(runner);
    }
    else
    {
        for (auto&& step : getSteps())
        {
            if (runningOk) 
            {
                runningOk = step.run(runner, filterTags);
            }
            else
            {
                step.skip(runner);
            }
        }
        setStatus(runningOk ? passed : failed);
    }

    setEndTime();
    runner.endScenario(*this);
    return runningOk;
}

void CucumberScenario::skip(CucumberRunner& runner)
{
    setStatus(skipped);
    runner.skipScenario(*this);
    for (auto&& step : mySteps)
    {
        step.skip(runner);
    }
}

bool CucumberStep::run(CucumberRunner& runner, const std::vector<std::string>& filterTags)
{
    setStartTime();
    runner.beginStep(*this);

    auto stepDefs = runner.stepMatch(getText()); // Update step definitions
    setStepDefs(stepDefs);

    if (getStepDefs().size() == 0) // Handling undefined step definition
    {
        setStatus(undefined);
        std::string error = runner.snippetStep(*this);
        setError(error);
    }
    else if (getStepDefs().size() > 1) // Handling ambiguous step definitions
    {
        setStatus(ambiguous);
    }
    else // Good
    {
        std::string error;
        if (runner.invokeStep(*this, error))
        {
            setStatus(passed);
        }
        else
        {
            setStatus(failed);
            setError(error);
        }
    }

    setEndTime();
    runner.endStep(*this);
    return getStatus() == passed;
}

void CucumberStep::skip(CucumberRunner& runner)
{
    setStatus(skipped);
    runner.skipStep(*this);
}
