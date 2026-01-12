#pragma once

#include "CucumberStepInfo.hpp"

#include <string>
#include <vector>

namespace cuke
{
    namespace internal
    {
        class CucumberFeature;
        class CucumberScenario;
        class CucumberStep;

        class CucumberBackend
        {
        public:
            virtual void beginExecution() const = 0;
            virtual void endExecution() const = 0;
            virtual void beginFeature(const CucumberFeature& feature) const = 0;
            virtual void skipFeature(const CucumberFeature& feature) const = 0;
            virtual void endFeature(const CucumberFeature& feature) const = 0;
            virtual void beginScenario(const CucumberScenario& scenario) const = 0;
            virtual void skipScenario(const CucumberScenario& scenario) const = 0;
            virtual void endScenario(const CucumberScenario& scenario) const = 0;
            virtual void beginStep(const CucumberStep& step) const = 0;
            virtual void skipStep(const CucumberStep& step) const = 0;
            virtual void endStep(const CucumberStep& step) const = 0;
            virtual void invokeStep(const CucumberStep& step, std::string& error) const = 0;
            virtual std::vector<CucumberStepInfo> stepMatch(const std::string& stepText) const = 0;
        };

    } // namespace internal

} // namespace cuke
