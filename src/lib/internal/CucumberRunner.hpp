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

        class CucumberRunner
        {
        public:
            virtual ~CucumberRunner() = default;
            virtual void beginFeature(const CucumberFeature& feature) = 0;
            virtual void skipFeature(const CucumberFeature& feature) = 0;
            virtual void endFeature(const CucumberFeature& feature) = 0;
            virtual void beginScenario(const CucumberScenario& scenario) = 0;
            virtual void skipScenario(const CucumberScenario& scenario) = 0;
            virtual void endScenario(const CucumberScenario& scenario) = 0;
            virtual void beginStep(const CucumberStep& step) = 0;
            virtual void skipStep(const CucumberStep& step) = 0;
            virtual void endStep(const CucumberStep& step) = 0;
            virtual bool invokeStep(const CucumberStep& step, std::string& error) = 0;
            virtual std::string snippetStep(const CucumberStep& step) = 0;
            virtual std::vector<CucumberStepInfo> stepMatch(const std::string& stepText) = 0;
        };

    } // namespace internal

} // namespace cuke
