#include "CukeDocument.hpp"

#include <cucumber/gherkin/app.hpp>
#include <cucumber/gherkin/file.hpp>
#include <cucumber/messages/feature.hpp>
#include <cucumber/messages/gherkin_document.hpp>
#include <cucumber/messages/pickle.hpp>
#include <cucumber/messages/pickle_step.hpp>

namespace cms = cucumber::messages;

namespace cuke::internal
{
    namespace
    {
        void buildFeature(CukeFeature& feature, const cms::gherkin_document& gherkinDocument);
        void buildFeature(CukeFeature& feature, const cms::feature& msgFeature);
        void buildScenario(CukeScenario& scenario, const cms::pickle& pickle);
        void buildStep(CukeStep& step, const cms::pickle_step& pickleStep);

        void buildFeature(CukeFeature& feature, const cms::gherkin_document& gherkinDocument)
        {
            buildFeature(feature, *gherkinDocument.feature);
        }

        void buildFeature(CukeFeature& feature, const cms::feature& msgFeature)
        {
            feature.name = msgFeature.name;
            feature.description = msgFeature.description;
            for (const auto& tag : msgFeature.tags)
            {
                feature.tags.emplace_back(tag.name);
            }
        }

        void buildScenario(CukeScenario& scenario, const cms::pickle& pickle)
        {
            scenario.name = pickle.name;
            for (const auto& tag : pickle.tags)
            {
                scenario.tags.emplace_back(tag.name);
            }
            for (const auto& pickleStep : pickle.steps)
            {
                CukeStep step;
                buildStep(step, pickleStep);
                scenario.steps.emplace_back(step);
            }
        }

        void buildStep(CukeStep& step, const cms::pickle_step& pickleStep)
        {
            step.text = pickleStep.text;
            switch (pickleStep.type.value_or(cms::pickle_step_type::UNKNOWN))
            {
                case cms::pickle_step_type::CONTEXT:
                    step.action = "Given";
                    break;
                case cms::pickle_step_type::ACTION:
                    step.action = "When";
                    break;
                case cms::pickle_step_type::OUTCOME:
                    step.action = "Then";
                    break;
                default:
                    step.action = "unknown";
                    break;
            }

            if (pickleStep.argument.has_value())
            {
                const auto& argument = pickleStep.argument.value();
                if (argument.data_table.has_value())
                {
                    CucumberTableArg tableArg;
                    const auto& msgTable = argument.data_table.value();
                    for (const auto& msgRow : msgTable.rows)
                    {
                        std::vector<std::string> tableRow;
                        for (const auto& msgCell : msgRow.cells)
                        {
                            tableRow.emplace_back(msgCell.value);
                        }
                        tableArg.emplace_back(tableRow);
                    }
                    step.arg_type = DataTable;
                    step.data_table_arg = tableArg;
                }
                else if (argument.doc_string.has_value())
                {
                    const auto& msgDocString = argument.doc_string.value();
                    step.arg_type = DocString;
                    step.doc_string_arg = msgDocString.content;
                }
            }
            else
            {
                step.arg_type = NoArgument;
            }
        }

    } // namespace

    void CukeDocument::parseFeatureFile(const std::string& filename)
    {
        cucumber::gherkin::app app;
        cucumber::gherkin::app::callbacks cbs{
            .source = [&](const auto& m) {},
            .ast = [&](const auto& m) {
                feature.filename = filename;
                buildFeature(feature, m);
            },
            .pickle = [&](const auto& m) {
                CukeScenario scenario;
                buildScenario(scenario, m);
                feature.scenarios.emplace_back(scenario);
            },
            .error = [&](const auto& m) {}
        };

        app.include_source(true);
        app.include_ast(true);
        app.include_pickles(true);
        app.parse(cucumber::gherkin::file{filename}, cbs);
    }

} // namespace cuke::internal
