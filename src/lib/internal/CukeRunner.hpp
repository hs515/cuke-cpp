#pragma once

#include "CucumberEventListener.hpp"
#include "CucumberRunner.hpp"
#include "CucumberStepInfo.hpp"
#include "ListenerOptions.hpp"
#include "client/CukeDocument.hpp"
#include "server/CukeServer.hpp"

namespace cuke
{
    namespace internal
    {
        class CukeRunner : public CucumberRunner
        {
        public:
            CukeRunner(const ListenerOptions& options);
            ~CukeRunner() override;
            void beginFeature(const CucumberFeature& feature) override;
            void skipFeature(const CucumberFeature& feature) override;
            void endFeature(const CucumberFeature& feature) override;
            void beginScenario(const CucumberScenario& scenario) override;
            void skipScenario(const CucumberScenario& scenario) override;
            void endScenario(const CucumberScenario& scenario) override;
            void beginStep(const CucumberStep& step) override;
            void skipStep(const CucumberStep& step) override;
            void endStep(const CucumberStep& step) override;
            bool invokeStep(const CucumberStep& step, std::string& error) override;
            std::string snippetStep(const CucumberStep& step) override;
            std::vector<CucumberStepInfo> stepMatch(const std::string& stepText) override;

        private:
            CukeServer myCukeServer;
            CucumberEventListener myEventListener;
        };

    } // namespace internal

} // namespace cuke
