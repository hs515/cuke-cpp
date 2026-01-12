#pragma once

#include "internal/CucumberStepInfo.hpp"

#include <memory>
#include <string>
#include <vector>

namespace cuke 
{
    namespace internal
    {
        class CucumberRunner;

        using CucumberTableArg = std::vector<std::vector<std::string>>;
        using CucumberStepInfo = cuke::internal::CucumberStepInfo;

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

        /// @brief Features, Scenarios and Steps are CucumberRunnables
        class CucumberRunnable
        {
        public:
            void setStatus(CucumberExecutionStatus status);
            CucumberExecutionStatus getStatus() const;
            void setStartTime();
            uint64_t getStartTime() const;
            void setEndTime();
            uint64_t getEndTime() const;

            virtual bool run(CucumberRunner& runner, const std::vector<std::string>& filterTags) = 0;
            virtual void skip(CucumberRunner& runner) = 0;

        private:
            uint64_t now() const;
            CucumberExecutionStatus myStatus = pending;
            uint64_t myStartTime = 0;
            uint64_t myEndTime = 0;
        };

        /// @brief Cucumber step
        class CucumberStep : public CucumberRunnable
        {
        public:
            void setStepDefs(const std::vector<CucumberStepInfo>& stepDefs);
            const std::vector<CucumberStepInfo>& getStepDefs() const;
            void setAction(const std::string& action);
            std::string getAction() const;
            void setText(const std::string& text);
            std::string getText() const;
            void setError(const std::string& error);
            std::string getError() const;
            void setArgType(const std::string& type);
            std::string getArgType() const;

            void setTableArg(const CucumberTableArg& tableArg);
            const CucumberTableArg& getTableArg() const;
            void setDocStringArg(const std::string& docString);
            std::string getDocStringArg() const;

            bool run(CucumberRunner& runner, const std::vector<std::string>& filterTags) override;
            void skip(CucumberRunner& runner) override;

        private:
            std::vector<CucumberStepInfo> myStepDefs;
            std::string myAction;
            std::string myText;
            std::string myError;
            std::string myArgType;
            CucumberTableArg myTableArg;
            std::string myDocStringArg;
        };

        /// @brief Cucumber runnables that can have tags
        class CucumberTaggable : public CucumberRunnable
        {
        public:
            void setName(const std::string& name);
            std::string getName() const;
            void addTag(const std::string& tag);
            bool hasTag(const std::string& tag) const;
            bool hasWip() const;
            bool hasAnyTag(const std::vector<std::string>& tags) const;
            const std::vector<std::string>& getTags() const;
        private:
            std::string myName;
            std::vector<std::string> myTags;
        };

        /// @brief Cucumber Scenario
        class CucumberScenario : public CucumberTaggable
        {
        public:
            void addStep(const CucumberStep& step);
            const std::vector<CucumberStep>& getSteps() const;
            std::vector<CucumberStep>& getSteps();

            bool run(CucumberRunner& runner, const std::vector<std::string>& filterTags) override;
            void skip(CucumberRunner& runner) override;

        private:
            std::vector<CucumberStep> mySteps;
        };

        /// @brief Cucumber Feature
        class CucumberFeature : public CucumberTaggable
        {
        public:
            CucumberFeature(const std::string& filename);
            void setDescription(const std::string& description);
            std::string getDescription() const;

            void addScenario(const CucumberScenario& scenario);
            const std::vector<CucumberScenario>& getScenarios() const;
            std::vector<CucumberScenario>& getScenarios();

            bool run(CucumberRunner& runner, const std::vector<std::string>& filterTags) override;
            void skip(CucumberRunner& runner) override;
            const std::string& getFilename() const;

        private:
            std::string myFilename;
            std::string myDescription;
            std::vector<CucumberScenario> myScenarios;
        };

    } // namespace internal

} // namespace cuke