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
            uint64_t myStartTime = 0;
            bool myScenarioSkipped = false;

            int myPassedScenarios = 0;
            int myFailedScenarios = 0;
            int myPassedSteps = 0;
            int myFailedSteps = 0;
            int myUndefSteps = 0;
            int mySkippedSteps = 0;
        };

    } // namespace internal

} // namespace cuke