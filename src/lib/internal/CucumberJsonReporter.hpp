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

        class CucumberJsonReporter : public CucumberEventListenerIF
        {
        public:
            void executionBegin() override;
            void executionEnd() override;
            void featureBegin(const CucumberFeature& feature) override;
            void featureEnd(const CucumberFeature& feature) override;
            void featureSkip(const CucumberFeature& feature) override;
            void scenarioBegin(const CucumberScenario& scenario) override;
            void scenarioEnd(const CucumberScenario& scenario) override;
            void scenarioSkip(const CucumberScenario& scenario) override;
            void stepBegin(const CucumberStep& step) override;
            void stepEnd(const CucumberStep& step) override;
            void stepSkip(const CucumberStep& step) override;

        protected:
            virtual const json& getJson() const;
            virtual void dumpReport();

        private:
            json myJson;

            json* myFeatureNodePtr = nullptr;
            json* myScenarioNodePtr = nullptr;
            json* myStepNodePtr = nullptr;
            int myPassedFeatures = 0;
            int myFailedFeatures = 0;
            int mySkippedFeatures = 0;
            int myFeaturePassedScenarios = 0;
            int myFeatureFailedScenarios = 0;
            int myFeatureSkippedScenarios = 0;
        };

    } // namespace internal

} // namespace cuke
