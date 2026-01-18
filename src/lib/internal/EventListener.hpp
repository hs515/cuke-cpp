#ifndef CUKE_CPP_INTERNAL_EVENTLISTENER_HPP
#define CUKE_CPP_INTERNAL_EVENTLISTENER_HPP

#include <memory>
#include <vector>

namespace cuke::internal
{

    class CukeFeature;
    class CukeScenario;
    class CukeStep;
    class ReporterOptions;

    class EventListenerIF
    {
    public:
        virtual void executionBegin() = 0;
        virtual void executionEnd() = 0;
        virtual void featureBegin(const CukeFeature& feature) = 0;
        virtual void featureSkip(const CukeFeature& feature) = 0;
        virtual void featureEnd(const CukeFeature& feature) = 0;
        virtual void scenarioBegin(const CukeScenario& scenario) = 0;
        virtual void scenarioSkip(const CukeScenario& scenario) = 0;
        virtual void scenarioEnd(const CukeScenario& scenario) = 0;
        virtual void stepBegin(const CukeStep& step) = 0;
        virtual void stepSkip(const CukeStep& step) = 0;
        virtual void stepEnd(const CukeStep& step) = 0;
    };

    class EventListener : public EventListenerIF
    {
    public:
        explicit EventListener(const ReporterOptions& options);

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

    private:
        void add(std::shared_ptr<EventListenerIF> listener);

        std::vector<std::shared_ptr<EventListenerIF>> myListeners;
    };

} // namespace cuke::internal

#endif // CUKE_CPP_INTERNAL_EVENTLISTENER_HPP
