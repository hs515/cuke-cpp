#ifndef CUKE_CPP_INTERNAL_CONSOLEREPORTER_HPP
#define CUKE_CPP_INTERNAL_CONSOLEREPORTER_HPP

#include "EventListener.hpp"

#include <memory>
#include <ostream>

namespace cuke::internal
{
    class CukeFeature;
    class CukeScenario;
    class CukeStep;

    class Indent
    {
    public:
        explicit Indent(int value) : myValue(value) {}
    private:
        int myValue;
        std::ostream& operator<<(std::ostream& os) const;
        friend std::ostream& operator<<(std::ostream& os, const Indent& indent) { return indent.operator<<(os); }
    };

    class ConsoleReporter : public EventListenerIF
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

    private:
        uint64_t myStartTime = 0;

        int myPassedScenarios = 0;
        int myFailedScenarios = 0;
        int mySkippedScenarios = 0;
        int myPassedSteps = 0;
        int myFailedSteps = 0;
        int myUndefSteps = 0;
        int mySkippedSteps = 0;
    };

} // namespace cuke::internal

#endif // CUKE_CPP_INTERNAL_CONSOLEREPORTER_HPP
