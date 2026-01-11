#pragma once

#include "CucumberEventListener.hpp"
#include "CucumberBackend.hpp"
#include "CucumberStepInfo.hpp"
#include "ListenerOptions.hpp"
#include "client/CukeDocument.hpp"
#include "server/CukeServer.hpp"

namespace cuke
{
    namespace internal
    {
        class CukeBackend : public CucumberBackend
        {
        public:
            CukeBackend(const ListenerOptions& options);
            ~CukeBackend() override;
            virtual void beginFeature(const CucumberFeature& feature) override;
            virtual void skipFeature(const CucumberFeature& feature) override;
            virtual void endFeature(const CucumberFeature& feature) override;
            virtual void beginScenario(const CucumberScenario& scenario) override;
            virtual void skipScenario(const CucumberScenario& scenario) override;
            virtual void endScenario(const CucumberScenario& scenario) override;
            virtual void beginStep(const CucumberStep& step) override;
            virtual void skipStep(const CucumberStep& step) override;
            virtual void endStep(const CucumberStep& step) override;
            virtual bool invokeStep(const CucumberStep& step, std::string& error) override;
            virtual std::vector<CucumberStepInfo> stepMatch(const std::string& stepText) override;

        private:
            CukeServer myCukeServer;
            CucumberEventListener myEventListener;
        };

    } // namespace internal

} // namespace cuke
