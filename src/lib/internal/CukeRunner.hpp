#ifndef CUKE_INTERNAL_CUKERUNNER_HPP
#define CUKE_INTERNAL_CUKERUNNER_HPP

#include "EventListener.hpp"
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
        /**
         * Constructor
         * @param options Reporter options
         * @param filterTags Filter tag options
         */
        CukeRunner(const ReporterOptions& options, const FilterTagOptions& filterTags);

        /**
         * Destructor
         */
        ~CukeRunner();

        /**
         * Run a feature file
         * @param featureFile The feature file path
         * @return true if all scenarios passed, false otherwise
         */
        bool run(const std::string& featureFile);

    private:
        void beginFeature(CukeFeature& feature);
        bool runFeature(CukeFeature& feature);
        void skipFeature(CukeFeature& feature);
        void endFeature(CukeFeature& feature);
        void beginScenario(CukeScenario& scenario);
        bool runScenario(CukeScenario& scenario);
        void skipScenario(CukeScenario& scenario);
        void endScenario(CukeScenario& scenario);
        void beginStep(CukeStep& step);
        bool runStep(CukeStep& step);
        void skipStep(CukeStep& step);
        void endStep(CukeStep& step);
        bool invokeStep(CukeStep& step, std::string& error);
        std::string snippetStep(CukeStep& step);
        std::vector<CukeStepInfo> stepMatch(const std::string& stepText);

        CukeServer myCukeServer;
        EventListener myEventListener;
        const FilterTagOptions& myFilterTags;
    };
} // namespace cuke::internal

#endif // CUKE_INTERNAL_CUKERUNNER_HPP
