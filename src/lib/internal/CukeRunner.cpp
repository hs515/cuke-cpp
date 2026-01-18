#include "CukeRunner.hpp"
#include "CukeUtilities.hpp"

#include <nlohmann/json.hpp>

using namespace cuke::internal;
using namespace nlohmann;

CukeRunner::CukeRunner(const ReporterOptions& options, const FilterTagOptions& filterTags) :
    myEventListener(options),
    myFilterTags(filterTags)
{
    myEventListener.executionBegin();
}

CukeRunner::~CukeRunner()
{
    myEventListener.executionEnd();
}

bool CukeRunner::run(const std::string& featureFile)
{
    CukeDocument cukeDocument;
    cukeDocument.parseFeatureFile(featureFile);
    return runFeature(cukeDocument.feature);
}

void CukeRunner::beginFeature(CukeFeature& feature)
{
    feature.start_time = now();
    myEventListener.featureBegin(feature);
}

bool CukeRunner::runFeature(CukeFeature& feature)
{
    beginFeature(feature);

    bool runningOk = true;
    if (!myFilterTags.evaluate(feature.tags))
    {
        skipFeature(feature);
    }
    else
    {
        for (auto&& scenario : feature.scenarios)
        {
            runningOk = runScenario(scenario) && runningOk;
        }
        feature.status = (runningOk ? passed : failed);
    }

    endFeature(feature);
    return runningOk;
}

void CukeRunner::skipFeature(CukeFeature& feature)
{
    feature.status = skipped;
    for (auto&& scenario : feature.scenarios)
    {
        beginScenario(scenario);
        skipScenario(scenario);
        endScenario(scenario);
    }
    myEventListener.featureSkip(feature);
}

void CukeRunner::endFeature(CukeFeature& feature)
{
    feature.end_time = now();
    myEventListener.featureEnd(feature);
}

void CukeRunner::beginScenario(CukeScenario& scenario)
{
    scenario.start_time = now();
    myCukeServer.beginScenario(scenario.tags);
    myEventListener.scenarioBegin(scenario);
}

bool CukeRunner::runScenario(CukeScenario& scenario)
{
    beginScenario(scenario);

    bool runningOk = true;
    if (!myFilterTags.evaluate(scenario.tags))
    {
        skipScenario(scenario);
    }
    else
    {
        for (auto&& step : scenario.steps)
        {
            if (runningOk) 
            {
                runningOk = runStep(step);
            }
            else
            {
                beginStep(step);
                skipStep(step);
                endStep(step);
            }
        }
        scenario.status = (runningOk ? passed : failed);
    }

    endScenario(scenario);
    return runningOk;
}

void CukeRunner::skipScenario(CukeScenario& scenario)
{
    scenario.status = skipped;
    for (auto&& step : scenario.steps)
    {
        beginStep(step);
        skipStep(step);
        endStep(step);
    }
    myEventListener.scenarioSkip(scenario);
}

void CukeRunner::endScenario(CukeScenario& scenario)
{
    scenario.end_time = now();
    myCukeServer.endScenario(scenario.tags);
    myEventListener.scenarioEnd(scenario);
}

void CukeRunner::beginStep(CukeStep& step)
{
    step.start_time = now();
    myEventListener.stepBegin(step);
}

bool CukeRunner::runStep(CukeStep& step)
{
    beginStep(step);

    step.step_defs = stepMatch(step.text); // Update step definitions
    if (step.step_defs.size() == 0) // Handling undefined step definition
    {
        step.status = undefined;
        step.error = snippetStep(step);
    }
    else if (step.step_defs.size() > 1) // Handling ambiguous step definitions
    {
        step.status = ambiguous;
    }
    else // Good
    {
        std::string error;
        if (invokeStep(step, error))
        {
            step.status = passed;
        }
        else
        {
            step.status = failed;
            step.error = error;
        }
    }

    endStep(step);
    return step.status == passed;
}

void CukeRunner::skipStep(CukeStep& step)
{
    step.status = skipped;
    myEventListener.stepSkip(step);
}

void CukeRunner::endStep(CukeStep& step)
{
    step.end_time = now();
    myEventListener.stepEnd(step);
}

bool CukeRunner::invokeStep(CukeStep& step, std::string& error)
{
    auto stepInfo = step.step_defs.at(0);
    bool success = true;
    if ("DocString" == step.arg_type)
    {
        success = myCukeServer.invoke(stepInfo.id, step.doc_string_arg, error);
    }
    else // if ("DataTable" == step.arg_type)
    {
        std::vector<std::string> args;
        for (auto&& argPair : stepInfo.args)
        {
            args.emplace_back(argPair.second);
        }
        success = myCukeServer.invoke(stepInfo.id, args, error);
    }
    return success;
}

std::string CukeRunner::snippetStep(CukeStep& step)
{
    return myCukeServer.snippetText(step.action, step.text, step.arg_type);
}

std::vector<CukeStepInfo> CukeRunner::stepMatch(const std::string& stepText)
{
    json response = json::parse(myCukeServer.stepMatch(stepText));

    std::vector<CukeStepInfo> steps;
    for (auto&& stepJson : response)
    {
        auto stepInfo = CukeStepInfo();
        stepInfo.id = stepJson["id"].get<std::string>();
        stepInfo.regexp = stepJson["regexp"].get<std::string>();
        stepInfo.source = stepJson["source"].get<std::string>();
        for(auto arg : stepJson["args"])
        {
            StepMatchArg pair(arg["pos"].get<unsigned int>(), arg["val"].get<std::string>());
            stepInfo.args.emplace_back(pair);
        }
        steps.emplace_back(stepInfo);
    }

    return steps;
}
