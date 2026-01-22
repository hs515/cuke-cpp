#include "CukeServer.hpp"

#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>
#include <nlohmann/json.hpp>

using namespace cucumber::internal;
using namespace nlohmann;

namespace cuke::internal
{
    namespace
    {
        WireProtocolHandler getWireProtocolHandler()
        {
            static CukeEngineImpl engine_;
            static JsonWireMessageCodec codec_;
            static auto instance = WireProtocolHandler(codec_, engine_);
            return instance;
        }
    } // namespace

    std::string CukeServer::handle(std::string_view request) const
    {
        return getWireProtocolHandler().handle(std::string{request});
    }

    std::string CukeServer::stepMatch(std::string_view stepText) const
    {
        json request = json::array();
        json payload;
        payload["name_to_match"] = std::string{stepText};
        request.emplace_back("step_matches");
        request.emplace_back(payload);

        if (json response = json::parse(handle(request.dump())); "success" == response.at(0).get<std::string>())
        {
            return response.at(1).dump();
        }
        return "";
    }

    void CukeServer::beginScenario(const std::vector<std::string>& tags) const
    {
        json jsonTags = json::array();
        for (auto&& tag: tags) {
            jsonTags.emplace_back(tag);
        }

        json request = json::array();
        json payload;
        payload["tags"] = jsonTags;
        request.emplace_back("begin_scenario");
        request.emplace_back(payload);

        handle(request.dump());
    }

    void CukeServer::endScenario(const std::vector<std::string>& tags) const
    {
        json jsonTags = json::array();
        for (auto&& tag: tags) {
            jsonTags.emplace_back(tag);
        }

        json request = json::array();
        json payload;
        payload["tags"] = jsonTags;
        request.emplace_back("end_scenario");
        request.emplace_back(payload);

        handle(request.dump());
    }

    bool CukeServer::invokeJson(std::string_view stepId, const json& jsonArgs, std::string& error) const
    {
        json request = json::array();
        json payload;
        payload["id"] = std::string{stepId};
        payload["args"] = jsonArgs;
        request.emplace_back("invoke");
        request.emplace_back(payload);

        json response = json::parse(handle(request.dump()));

        bool success = response[0].get<std::string>() == "success";
        error = success ? "" : response[1]["message"].get<std::string>();
        return success;
    }

    bool CukeServer::invoke(std::string_view stepId, std::string_view docArg, std::string& error) const
    {
        json jsonArgs = docArg;
        return invokeJson(stepId, jsonArgs, error);
    }

    bool CukeServer::invoke(std::string_view stepId, const std::vector<std::vector<std::string>>& tableArg, std::string& error) const
    {
        json jsonArgs = json::array();
        json jsonTable = json::array();
        for (auto&& row: tableArg) {
            jsonTable.emplace_back(row);
        }
        jsonArgs.emplace_back(jsonTable);

        return invokeJson(stepId, jsonArgs, error);
    }

    bool CukeServer::invoke(std::string_view stepId, const std::vector<std::string>& args, std::string& error) const
    {
        json jsonArgs = json::array();
        for (auto&& arg: args) {
            jsonArgs.emplace_back(arg);
        }

        return invokeJson(stepId, jsonArgs, error);
    }

    std::string CukeServer::snippetText(std::string_view stepAction, std::string_view stepText, std::string_view multilineArgClass) const
    {
        json request = json::array();
        json payload;
        payload["step_keyword"] = std::string{stepAction};
        payload["step_name"] = std::string{stepText};
        payload["multiline_arg_class"] = std::string{multilineArgClass};
        request.emplace_back("snippet_text");
        request.emplace_back(payload);

        json response = json::parse(handle(request.dump()));

        bool success = response[0].get<std::string>() == "success";
        return success ? response[1].get<std::string>() : "";
    }

} // namespace cuke::internal
