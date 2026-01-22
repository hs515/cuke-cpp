#ifndef CUKE_INTERNAL_CUKERUNNER_HPP
#define CUKE_INTERNAL_CUKERUNNER_HPP

#include "CukeDocument.hpp"
#include "CukeServer.hpp"
#include "EventListener.hpp"
#include "FilterTagOptions.hpp"
#include "ReporterOptions.hpp"

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
        bool run(std::string_view featureFile);

    private:
        CukeRunner(const CukeRunner&) = delete;
        CukeRunner& operator=(const CukeRunner&) = delete;

        void beginFeature(CukeFeature& feature);
        bool runFeature(CukeFeature& feature);
        void endFeature(CukeFeature& feature);
        void beginScenario(CukeScenario& scenario);
        bool runScenario(CukeScenario& scenario);
        void skipScenario(CukeScenario& scenario);
        void endScenario(CukeScenario& scenario);
        void beginStep(CukeStep& step);
        bool runStep(CukeStep& step);
        void skipStep(CukeStep& step);
        void endStep(CukeStep& step);
        bool invokeStep(CukeStep& step, std::string& error) const;
        std::string snippetStep(const CukeStep& step) const;
        std::vector<CukeStepInfo> stepMatch(std::string_view stepText) const;

        CukeServer myCukeServer = CukeServer();
        EventListener myEventListener;
        const FilterTagOptions& myFilterTags;
    };
} // namespace cuke::internal

#endif // CUKE_INTERNAL_CUKERUNNER_HPP
