#pragma once

#include <memory>
#include <vector>

namespace cuke
{
    namespace internal
    {

        class CucumberFeature;
        class CucumberScenario;
        class CucumberStep;
        class ReporterOptions;

        class CucumberEventListenerIF
        {
        public:
            virtual void executionBegin() = 0;
            virtual void executionEnd() = 0;
            virtual void featureBegin(const CucumberFeature& feature) = 0;
            virtual void featureSkip(const CucumberFeature& feature) = 0;
            virtual void featureEnd(const CucumberFeature& feature) = 0;
            virtual void scenarioBegin(const CucumberScenario& scenario) = 0;
            virtual void scenarioSkip(const CucumberScenario& scenario) = 0;
            virtual void scenarioEnd(const CucumberScenario& scenario) = 0;
            virtual void stepBegin(const CucumberStep& step) = 0;
            virtual void stepSkip(const CucumberStep& step) = 0;
            virtual void stepEnd(const CucumberStep& step) = 0;
        };

        class CucumberEventListener : public CucumberEventListenerIF
        {
        public:
            explicit CucumberEventListener(const ReporterOptions& options);

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

        private:
            void add(std::shared_ptr<CucumberEventListenerIF> listener);

            std::vector<std::shared_ptr<CucumberEventListenerIF>> myListeners;
        };

    }

}