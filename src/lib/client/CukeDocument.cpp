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

// void CucumberRunner::run(const std::string& file, const std::vector<std::string>& filterTags, bool& ok)
// {
//     Gherkin::GherkinParser parser;
//     Gherkin::GherkinDoc doc;
//     Gherkin::GherkinErrors errors;

//     if (!parser.parse(file, doc, errors))
//     {
//         ok = false;
//         std::cerr << "Error parsing " << file << std::endl;
//         for (auto error : errors)
//         {
//             std::wcerr << error->error_text << std::endl;
//         }
//         return;
//     }

//     const Feature* featurePtr = doc.document->feature;
       
//     if (!featurePtr)
//     {
//         // Skip if file contains no feature
//         return;
//     }
    
//     auto feature = std::make_shared<CucumberFeature>(featurePtr);

//     feature->filename = doc.filename;

//     if (featurePtr->tags)
//     {
//         Tag* tag = featurePtr->tags->tags;
//         for (int i = 0; i < featurePtr->tags->tag_count; i++) 
//         {
//             feature->addTag(wcharToString(tag->name));
//             tag++;
//         }
//     }
    
//     for (auto&& pickle : doc.pickles)
//     {
//         auto scenario = std::make_shared<CucumberScenario>(pickle);
//         if (pickle->tags)
//         {
//             PickleTag* tag = pickle->tags->tags;
//             for (int i = 0; i < pickle->tags->tag_count; i++) 
//             {
//                 scenario->addTag(wcharToString(tag->name));
//                 tag++;
//             }
//         }

//         PickleStep* step = pickle->steps->steps;
//         for (int i = 0; i < pickle->steps->step_count; i++) 
//         {
//             auto cucumberStep = std::make_shared<CucumberStep>(step);
//             auto text = cucumberStep->getText();
//             auto stepDefs = myCucumberRunner.stepMatch(text);
//             cucumberStep->setStepDefs(stepDefs);
//             scenario->getSteps().emplace_back(cucumberStep);
//             step++;
//         }

//         feature->scenarios.emplace_back(scenario);
//     }

//     if (feature->hasWip())
//     {
//         skipFeature(feature);
//     }
//     else
//     {
//         ok = runFeature(feature, filterTags) && ok;
//     }
// }

// bool CucumberRunner::runFeature(std::shared_ptr<CucumberFeature> feature, const std::vector<std::string>& filterTags)
// {
//     // Apply filter tags
//     std::vector<std::shared_ptr<CucumberScenario>> scenarios;
//     for (auto&& scenario : feature->scenarios)
//     {
//         if (filterTags.empty() || scenario->hasAnyTag(filterTags))
//         {
//             scenarios.emplace_back(scenario);
//         }
//     }

//     // Exit if no scenario gets run
//     if (scenarios.empty())
//     {
//         return true;
//     }

//     feature->startTime = CucumberEventListenerIF::now();
//     myListener.featureBegin(*feature);

//     bool runningOk = true;
//     for (auto& scenario : scenarios)
//     {
//         if (scenario->hasWip())
//         {
//             skipScenario(scenario);
//         }
//         else
//         {
//             runningOk = runScenario(scenario) && runningOk;
//         }
//     }
    
//     feature->setStatus(runningOk ? passed : failed);
//     feature->endTime = CucumberEventListenerIF::now();
//     myListener.featureEnd(*feature);
//     return runningOk;
// }

// void CucumberRunner::skipFeature(std::shared_ptr<CucumberFeature> feature)
// {
//     feature->setStatus(skipped);
//     myListener.featureSkip(*feature);
//     for (auto&& scenario : feature->scenarios)
//     {
//         skipScenario(scenario);
//     }
// }

// bool CucumberRunner::runScenario(std::shared_ptr<CucumberScenario> scenario)
// {
//     scenario->startTime = CucumberEventListenerIF::now();
//     myListener.scenarioBegin(*scenario);

//     myCucumberRunner.beginScenario(scenario->getTags());
//     bool runningOk = true;
//     for (auto&& step : scenario->getSteps())
//     {
//         if (runningOk) 
//         {
//             runningOk = runStep(step);
//         }
//         else
//         {
//             skipStep(step);
//         }
//     }
//     myCucumberRunner.endScenario(scenario->getTags());

//     scenario->setStatus(runningOk ? passed : failed);
//     scenario->endTime = CucumberEventListenerIF::now();
//     myListener.scenarioEnd(*scenario);
//     return runningOk;
// }

// void CucumberRunner::skipScenario(std::shared_ptr<CucumberScenario> scenario)
// {
//     scenario->setStatus(skipped);
//     myListener.scenarioSkip(*scenario);
//     for (auto&& step : scenario->getSteps())
//     {
//         skipStep(step);
//     }
// }

// bool CucumberRunner::runStep(std::shared_ptr<CucumberStep> step)
// {
//     step->startTime = CucumberEventListenerIF::now();
//     myListener.stepBegin(*step);

//     if (step->getStepDefs().size() == 0) // Handling undefined step definition
//     {
//         step->setStatus(undefined);
//     }
//     else if (step->getStepDefs().size() > 1)  // Handling ambiguous step definitions
//     {
//         step->setStatus(ambiguous);
//     }
//     else  // Good
//     {
//         auto stepInfo = step->getStepDefs().at(0);
//         std::vector<std::string> args;
//         for (auto&& argPair : stepInfo->args)
//         {
//             args.emplace_back(argPair.second);
//         }

//         std::string error;
//         if (myCucumberRunner.invoke(stepInfo->id, args, error))
//         {
//             step->setStatus(passed);
//         }
//         else
//         {
//             step->setStatus(failed);
//             step->setError(error);
//         }
//     }

//     step->endTime = CucumberEventListenerIF::now();
//     myListener.stepEnd(*step);
//     return step->getStatus() == passed;
// }

// void CucumberRunner::skipStep(std::shared_ptr<CucumberStep> step)
// {
//     step->setStatus(skipped);
//     myListener.stepSkip(*step);
// }
