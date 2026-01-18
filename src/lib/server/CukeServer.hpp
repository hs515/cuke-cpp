#ifndef CUKE_LIB_SERVER_CUKESERVER_HPP
#define CUKE_LIB_SERVER_CUKESERVER_HPP

#include <string>
#include <utility>
#include <vector>

namespace cuke::internal
{
    /// @brief CucumberCpp runner handler over Wire Protocol
    class CukeServer
    {
    public:
        /// @brief Find step definitions by step text
        /// @param stepText 
        /// @return a json string that contains a vector of matched step definitions info
        std::string stepMatch(const std::string& stepText) const;

        /// @brief Fire begin-scenario hooks
        /// @param tags a vector of tags associated with a scenario
        void beginScenario(const std::vector<std::string>& tags) const;

        /// @brief Fire end-scenario hooks
        /// @param tags 
        void endScenario(const std::vector<std::string>& tags) const;

        /// @brief Invoke step definition
        /// @param stepId 
        /// @param arg invoke argument
        /// @param error set to the error message if invoke failed
        /// @return true if invoke successfully, otherwise false
        bool invoke(const std::string& stepId, const std::string& arg, std::string& error) const;

        /// @brief Invoke step definition
        /// @param stepId 
        /// @param args invoke argument
        /// @param error set to the error message if invoke failed
        /// @return true if invoke successfully, otherwise false
        bool invoke(const std::string& stepId, const std::vector<std::string>& args, std::string& error) const;

        /// @brief Fetch snippet text
        /// @param stepAction can be Given, When or Then
        /// @param stepText 
        /// @param multilineArgClass 
        /// @return snippet text
        std::string snippetText(const std::string& stepAction, const std::string& stepText, const std::string& multilineArgClass) const;

    private:
        std::string handle(const std::string& request) const;
    };
} // namespace cuke::internal

#endif // CUKE_LIB_SERVER_CUKESERVER_HPP
