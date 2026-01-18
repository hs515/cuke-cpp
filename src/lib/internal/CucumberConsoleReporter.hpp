#pragma once

#include "CucumberEventListener.hpp"

#include <memory>
#include <iostream>
#include <string>

namespace cuke 
{
    namespace internal
    {

        class CucumberFeature;
        class CucumberScenario;
        class CucumberStep;

        class Indent
        {
        public:
            Indent(int value) : myValue(value) {}
        private:
            int myValue;
            friend std::ostream& operator<<(std::ostream& os, const Indent& indent);
        };

        std::ostream& operator<<(std::ostream& os, const Indent& indent);

        class CucumberConsoleReporter : public CucumberEventListenerIF
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

        private:
            uint64_t myStartTime = 0;
            bool myScenarioSkipped = false;

            int myPassedScenarios = 0;
            int myFailedScenarios = 0;
            int mySkippedScenarios = 0;
            int myPassedSteps = 0;
            int myFailedSteps = 0;
            int myUndefSteps = 0;
            int mySkippedSteps = 0;
        };

    } // namespace internal

} // namespace cuke