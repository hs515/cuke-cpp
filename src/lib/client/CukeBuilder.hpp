#pragma once

#include <string>
#include <cucumber/messages/gherkin_document.hpp>
#include <cucumber/messages/feature.hpp>
#include <cucumber/messages/pickle.hpp>
#include <cucumber/messages/pickle_step.hpp>

namespace cuke 
{
    namespace internal
    {
        namespace cms = cucumber::messages;
        class CucumberFeature;
        class CucumberScenario;
        class CucumberStep;

        class CukeBuilder
        {
        public:
            static void buildFeature(CucumberFeature& feature, const std::string& filename);
        private:
            static void buildFeature(CucumberFeature& feature, const cms::gherkin_document& gherkinDocument);
            static void buildFeature(CucumberFeature& feature, const cms::feature& msgFeature);
            static void buildScenario(CucumberScenario& scenario, const cms::pickle& pickle);
            static void buildStep(CucumberStep& step, const cms::pickle_step& pickleStep);
        };

    } // namespace internal

} // namespace cuke