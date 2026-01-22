#include "CukeRunner.hpp"
#include "CukeUtilities.hpp"

#include <nlohmann/json.hpp>

using namespace cuke::internal;
using namespace nlohmann;

CukeRunner::CukeRunner(const ReporterOptions& options, const FilterTagOptions& filterTags) :
    myEventListener{options},
    myFilterTags{filterTags}
{
    myEventListener.executionBegin();
}

CukeRunner::~CukeRunner()
{
    myEventListener.executionEnd();
}

bool CukeRunner::run(std::string_view featureFile)
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
    for (auto&& scenario : feature.scenarios)
    {
        runningOk = runScenario(scenario) && runningOk;
    }
    feature.status = (runningOk ? CucumberExecutionStatus::passed : CucumberExecutionStatus::failed);

    endFeature(feature);
    return runningOk;
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
        scenario.status = (runningOk ? CucumberExecutionStatus::passed : CucumberExecutionStatus::failed);
    }

    endScenario(scenario);
    return runningOk;
}

void CukeRunner::skipScenario(CukeScenario& scenario)
{
    scenario.status = CucumberExecutionStatus::skipped;
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
    if (step.step_defs.empty()) // Handling undefined step definition
    {
        step.status = CucumberExecutionStatus::undefined;
        std::stringstream ss;
        ss << "Undefined step definition. Implement a step definition like:" << "\n";
        ss << snippetStep(step);
        step.error = ss.str();
    }
    else if (step.step_defs.size() > 1) // Handling ambiguous step definitions
    {
        step.status = CucumberExecutionStatus::ambiguous;
        std::stringstream ss;
        ss << "Ambiguous step definition matches found:" << "\n";
        for (const auto& stepInfo : step.step_defs)
        {
            ss << "  " << stepInfo.source << ": \"" << stepInfo.regexp << "\"\n";
        }
        step.error = ss.str();
    }
    else // Good
    {
        std::string error;
        if (invokeStep(step, error))
        {
            step.status = CucumberExecutionStatus::passed;
        }
        else
        {
            step.status = CucumberExecutionStatus::failed;
            step.error = error;
        }
    }

    endStep(step);
    return step.status == CucumberExecutionStatus::passed;
}

void CukeRunner::skipStep(CukeStep& step)
{
    step.status = CucumberExecutionStatus::skipped;
    myEventListener.stepSkip(step);
}

void CukeRunner::endStep(CukeStep& step)
{
    step.end_time = now();
    myEventListener.stepEnd(step);
}

bool CukeRunner::invokeStep(CukeStep& step, std::string& error) const
{
    auto stepInfo = step.step_defs.at(0);
    bool success = true;
    if (CucumberArgumentType::DocString == step.arg_type)
    {
        success = myCukeServer.invoke(stepInfo.id, step.doc_string_arg.value(), error);
    }
    else if (CucumberArgumentType::DataTable == step.arg_type)
    {
        success = myCukeServer.invoke(stepInfo.id, step.data_table_arg.value_or(CucumberTableArg()), error);
    }
    else // if (NoArgument == step.arg_type)
    {
        std::vector<std::string> args;
        for (const auto& [key, value] : stepInfo.args)
        {
            args.emplace_back(value);
        }
        success = myCukeServer.invoke(stepInfo.id, args, error);
    }
    return success;
}

std::string CukeRunner::snippetStep(const CukeStep& step) const
{
    auto multiLineArg = [&]()
    {
        switch (step.arg_type)
        {
            case CucumberArgumentType::DataTable:
                return std::string{"DataTable"};
            case CucumberArgumentType::DocString:
                return std::string{"DocString"};
            default:
                return std::string();
        }
    };
    return myCukeServer.snippetText(step.action, step.text, multiLineArg());
}

std::vector<CukeStepInfo> CukeRunner::stepMatch(std::string_view stepText) const
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
            stepInfo.args.emplace_back(arg["pos"].get<unsigned int>(), arg["val"].get<std::string>());
        }
        steps.emplace_back(stepInfo);
    }

    return steps;
}
