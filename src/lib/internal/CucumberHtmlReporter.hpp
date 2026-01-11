#pragma once

#include "CucumberEventListener.hpp"

#include <nlohmann/json.hpp>

namespace cuke
{
    namespace internal
    {

        class CucumberFeature;
        class CucumberScenario;
        class CucumberStep;

        using json = nlohmann::json;

        class CucumberHtmlReporter : public CucumberEventListener
        {
        public:
            void executionBegin();
            void executionEnd();
            void featureBegin(const CucumberFeature& feature);
            void featureEnd(const CucumberFeature& feature);
            void featureSkip(const CucumberFeature& feature);
            void scenarioBegin(const CucumberScenario& scenario);
            void scenarioEnd(const CucumberScenario& scenario);
            void scenarioSkip(const CucumberScenario& scenario);
            void stepBegin(const CucumberStep& step);
            void stepEnd(const CucumberStep& step);
            void stepSkip(const CucumberStep& step);

        private:
            void dumpTo(const std::string& path);

            json myJson;

            json* myFeatureNodePtr;
            json* myScenarioNodePtr;
            json* myStepNodePtr;
            int myFeaturePassedScenarios;
            int myFeatureFailedScenarios;
            int myFeatureSkippedScenarios;
        };

    } // namespace internal

} // namespace cuke
