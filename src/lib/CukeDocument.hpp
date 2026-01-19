#ifndef CUKE_LIB_CLIENT_CUKEDOCUMENT_HPP
#define CUKE_LIB_CLIENT_CUKEDOCUMENT_HPP

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace cuke::internal
{
    using CucumberTableArg = std::vector<std::vector<std::string>>;
    using StepMatchArg = std::pair<int, std::string>;

    /// @brief Execution status of Cucumber runnables
    enum CucumberExecutionStatus
    {
        pending, 
        passed, 
        failed,
        undefined,
        ambiguous,
        skipped
    };

    /// @brief Argument type of Cucumber steps
    enum CucumberArgumentType
    {
        DataTable,
        DocString,
        NoArgument
    };

    struct CukeStepInfo 
    {
        std::string id;       // Step ID
        std::string source;   // Step text
        std::string regexp;   // Step definition regular expression
        std::vector<StepMatchArg> args; // Matching arguments (position : int, text : string)
    };

    /// @brief Features, Scenarios and Steps are CucumberRunnables
    struct CucumberRunnable
    {
        CucumberExecutionStatus status = pending;
        uint64_t start_time = 0;
        uint64_t end_time = 0;
    };

    /// @brief Cucumber step
    struct CukeStep : public CucumberRunnable
    {
        std::string action;
        std::string text;
        std::optional<std::string> error;
        CucumberArgumentType arg_type = NoArgument;
        std::optional<CucumberTableArg> data_table_arg;
        std::optional<std::string> doc_string_arg;
        std::vector<CukeStepInfo> step_defs;
    };

    /// @brief Cucumber runnables that can have tags
    struct CucumberTaggable : public CucumberRunnable
    {
        std::string name;
        std::vector<std::string> tags;
    };

    /// @brief Cucumber Scenario
    struct CukeScenario : public CucumberTaggable
    {
        std::vector<CukeStep> steps;
    };

    /// @brief Cucumber Feature
    struct CukeFeature : public CucumberTaggable
    {
        std::string filename;
        std::string description;
        std::vector<CukeScenario> scenarios;
    };

    struct CukeDocument
    {
        CukeFeature feature;
        void parseFeatureFile(const std::string& filename);
    };
} // namespace cuke::internal

#endif // CUKE_LIB_CLIENT_CUKEDOCUMENT_HPP
