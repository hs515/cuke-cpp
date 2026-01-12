#include "CukeBuilder.hpp"
#include "CukeDocument.hpp"

#include <cucumber/gherkin/app.hpp>
#include <cucumber/gherkin/file.hpp>
#include <iostream>

using namespace cuke::internal;
namespace cms = cucumber::messages;

void CukeBuilder::buildFeature(CucumberFeature& feature, const std::string& filename) 
{
    cucumber::gherkin::app app;
    cucumber::gherkin::app::callbacks cbs{
        .source = [&](const auto& m) {},
        .ast = [&](const auto& m) {
            buildFeature(feature, m);
        },
        .pickle = [&](const auto& m) {
            CucumberScenario scenario;
            buildScenario(scenario, m);
            feature.addScenario(scenario);
        },
        .error = [&](const auto& m) {}
    };

    app.include_source(true);
    app.include_ast(true);
    app.include_pickles(true);
    app.parse(cucumber::gherkin::file{filename}, cbs);
}

void CukeBuilder::buildFeature(CucumberFeature& feature, const cms::gherkin_document& gherkinDocument)
{
    buildFeature(feature, *gherkinDocument.feature);
}

void CukeBuilder::buildFeature(CucumberFeature& feature, const cms::feature& msgFeature)
{
    feature.setName(msgFeature.name);
    feature.setDescription(msgFeature.description);
    for (const auto& tag : msgFeature.tags)
    {
        feature.addTag(tag.name);
    }
}

void CukeBuilder::buildScenario(CucumberScenario& scenario, const cms::pickle& pickle)
{
    scenario.setName(pickle.name);
    for (const auto& tag : pickle.tags)
    {
        scenario.addTag(tag.name);
    }
    for (const auto& pickleStep : pickle.steps)
    {
        CucumberStep step;
        buildStep(step, pickleStep);
        scenario.addStep(step);
    }
}

void CukeBuilder::buildStep(CucumberStep& step, const cms::pickle_step& pickleStep)
{
    step.setText(pickleStep.text);
    switch (pickleStep.type.value_or(cms::pickle_step_type::UNKNOWN))
    {
        case cms::pickle_step_type::CONTEXT:
            step.setAction("Given");
            break;
        case cms::pickle_step_type::ACTION:
            step.setAction("When");
            break;
        case cms::pickle_step_type::OUTCOME:
            step.setAction("Then");
            break;
        default:
            step.setAction("unknown");
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
            step.setArgType("Cucumber::Term::Table");
            step.setTableArg(tableArg);
        }
        else if (argument.doc_string.has_value())
        {
            const auto& msgDocString = argument.doc_string.value();
            step.setArgType("Cucumber::Core::Ast::DocString");
            step.setDocStringArg(msgDocString.content);
        }
    }
    else
    {
        step.setArgType("");
    }
}
