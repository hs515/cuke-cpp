#ifndef CUKE_CPP_INTERNAL_JSONREPORTER_HPP
#define CUKE_CPP_INTERNAL_JSONREPORTER_HPP

#include "EventListener.hpp"

#include <nlohmann/json.hpp>

namespace cuke::internal
{
    class CukeFeature;
    class CukeScenario;
    class CukeStep;

    using json = nlohmann::json;

    class JsonReporter : public EventListenerIF
    {
    public:
        void executionBegin() override;
        void executionEnd() override;
        void featureBegin(const CukeFeature& feature) override;
        void featureEnd(const CukeFeature& feature) override;
        void featureSkip(const CukeFeature& feature) override;
        void scenarioBegin(const CukeScenario& scenario) override;
        void scenarioEnd(const CukeScenario& scenario) override;
        void scenarioSkip(const CukeScenario& scenario) override;
        void stepBegin(const CukeStep& step) override;
        void stepEnd(const CukeStep& step) override;
        void stepSkip(const CukeStep& step) override;

    protected:
        virtual const json& getJson() const;
        virtual void dumpReport();

    private:
        json myJson;

        json* myFeatureNodePtr = nullptr;
        json* myScenarioNodePtr = nullptr;
        json* myStepNodePtr = nullptr;
        int myPassedFeatures = 0;
        int myFailedFeatures = 0;
        int mySkippedFeatures = 0;
        int myFeaturePassedScenarios = 0;
        int myFeatureFailedScenarios = 0;
        int myFeatureSkippedScenarios = 0;
    };

} // namespace cuke::internal

#endif // CUKE_CPP_INTERNAL_JSONREPORTER_HPP
