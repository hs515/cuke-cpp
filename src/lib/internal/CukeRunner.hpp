#ifndef CUKE_INTERNAL_CUKERUNNER_HPP
#define CUKE_INTERNAL_CUKERUNNER_HPP

#include "CucumberEventListener.hpp"
#include "FilterTagOptions.hpp"
#include "ReporterOptions.hpp"
#include "client/CukeDocument.hpp"
#include "server/CukeServer.hpp"

#include <string>

namespace cuke::internal
{
    class CukeRunner
    {
    public:
        CukeRunner(const ReporterOptions& options, const FilterTagOptions& filterTags);
        ~CukeRunner();

        bool run(const std::string& featureFile);

    private:
        void beginFeature(CucumberFeature& feature);
        bool runFeature(CucumberFeature& feature);
        void skipFeature(CucumberFeature& feature);
        void endFeature(CucumberFeature& feature);
        void beginScenario(CucumberScenario& scenario);
        bool runScenario(CucumberScenario& scenario);
        void skipScenario(CucumberScenario& scenario);
        void endScenario(CucumberScenario& scenario);
        void beginStep(CucumberStep& step);
        bool runStep(CucumberStep& step);
        void skipStep(CucumberStep& step);
        void endStep(CucumberStep& step);
        bool invokeStep(CucumberStep& step, std::string& error);
        std::string snippetStep(CucumberStep& step);
        std::vector<CukeStepInfo> stepMatch(const std::string& stepText);

        CukeServer myCukeServer;
        CucumberEventListener myEventListener;
        const FilterTagOptions& myFilterTags;
    };
} // namespace cuke::internal

#endif // CUKE_INTERNAL_CUKERUNNER_HPP
