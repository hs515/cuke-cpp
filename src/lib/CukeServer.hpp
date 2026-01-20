#ifndef CUKE_LIB_SERVER_CUKESERVER_HPP
#define CUKE_LIB_SERVER_CUKESERVER_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

namespace cuke::internal
{
    /**
     * @brief CucumberCpp runner handler over Wire Protocol
     */
    class CukeServer
    {
    public:
        /**
         * @brief Find step definitions by step text
         * @param stepText 
         * @return a json string that contains a vector of matched step definitions info
         */
        std::string stepMatch(std::string_view stepText) const;

        /**
         * @brief Fire begin-scenario hooks
         * @param tags a vector of tags associated with a scenario
         */
        void beginScenario(const std::vector<std::string>& tags) const;

        /**
         * @brief Fire end-scenario hooks
         * @param tags a vector of tags associated with a scenario
         */
        void endScenario(const std::vector<std::string>& tags) const;

        /**
         * @brief Invoke step definition
         * @param stepId 
         * @param docArg invoke doc string argument
         * @param error set to the error message if invoke failed
         * @return true if invoke successfully, otherwise false
         */
         bool invoke(std::string_view stepId, std::string_view docArg, std::string& error) const;

        /**
         * @brief Invoke step definition
         * @param stepId 
         * @param tableArg invoke data table argument
         * @param error set to the error message if invoke failed
         * @return true if invoke successfully, otherwise false
         */
        bool invoke(std::string_view stepId, const std::vector<std::vector<std::string>>& tableArg, std::string& error) const;

        /**
         * @brief Invoke step definition
         * @param stepId 
         * @param args invoke regex arguments
         * @param error set to the error message if invoke failed
         * @return true if invoke successfully, otherwise false
         */
        bool invoke(std::string_view stepId, const std::vector<std::string>& args, std::string& error) const;

        /**
         * @brief Fetch snippet text
         * @param stepAction can be Given, When or Then
         * @param stepText 
         * @param multilineArgClass 
         * @return snippet text
         */
         std::string snippetText(std::string_view stepAction, std::string_view stepText, std::string_view multilineArgClass) const;

    private:
        /**
         * @brief Handle a request string and return the response string
         * @param request request string
         * @return response string
         */
         std::string handle(std::string_view request) const;

        /**
         * @brief Invoke step definition
         * @param stepId 
         * @param jsonArgs invoke arguments in json format
         * @param error set to the error message if invoke failed
         * @return true if invoke successfully, otherwise false
         */
        bool invokeJson(std::string_view stepId, const nlohmann::json& jsonArgs, std::string& error) const;
    };
} // namespace cuke::internal

#endif // CUKE_LIB_SERVER_CUKESERVER_HPP
