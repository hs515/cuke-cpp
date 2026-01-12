#include "CukeServer.hpp"

#include <nlohmann/json.hpp>

using namespace cucumber::internal;
using namespace cuke::internal;
using namespace nlohmann;

std::string CukeServer::handle(const std::string& request) const
{
    try
    {
        return myHandler.handle(request);
    }
    catch(const std::exception& e)
    {
        json response = json::array();
        response.emplace_back("fail");
        json messageObj;
        messageObj["message"] = e.what();
        response.emplace_back(messageObj);
        return response.dump();
    }
}

std::vector<CucumberStepInfo> CukeServer::stepMatch(const std::string& stepText) const
{
    json request = json::array();
    json payload;
    payload["name_to_match"] = stepText;
    request.emplace_back("step_matches");
    request.emplace_back(payload);

    std::vector<CucumberStepInfo> steps; 
    json response = json::parse(handle(request.dump()));
    if ("success" == response.at(0).get<std::string>())
    {
        for (auto&& stepJson : response.at(1))
        {
            auto stepInfo = CucumberStepInfo();
            stepInfo.id = stepJson["id"].get<std::string>();
            stepInfo.regexp = stepJson["regexp"].get<std::string>();
            stepInfo.source = stepJson["source"].get<std::string>();
            for(auto arg : stepJson["args"])
            {
                StepMatchArg pair(arg["pos"].get<unsigned int>(), arg["val"].get<std::string>());
                stepInfo.args.emplace_back(pair);
            }
            steps.emplace_back(stepInfo);
        }
    }
    return steps;
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

bool CukeServer::invoke(const std::string& stepId, const std::vector<std::string>& args, std::string& error) const
{
    json jsonArgs = json::array();
    for (auto&& arg: args) {
        jsonArgs.emplace_back(arg);
    }

    json request = json::array();
    json payload;
    payload["id"] = stepId;
    payload["args"] = jsonArgs;
    request.emplace_back("invoke");
    request.emplace_back(payload);

    json response = json::parse(handle(request.dump()));

    bool success = response[0].get<std::string>() == "success";
    error = (success) ? "" : response[1]["message"].get<std::string>();
    return success;
}

std::string CukeServer::snippetText(const std::string& stepAction, const std::string& stepText, const std::string& multilineArgClass) const
{
    json request = json::array();
    json payload;
    payload["step_keyword"] = stepAction;
    payload["step_name"] = stepText;
    payload["multiline_arg_class"] = multilineArgClass;
    request.emplace_back("snippet_text");
    request.emplace_back(payload);

    json response = json::parse(handle(request.dump()));

    bool success = response[0].get<std::string>() == "success";
    return (success) ? response[1].get<std::string>() : "";
}
