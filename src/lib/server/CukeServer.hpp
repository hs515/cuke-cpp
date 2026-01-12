#pragma once

#include "internal/CucumberStepInfo.hpp"

#include <cucumber-cpp/internal/connectors/wire/WireProtocol.hpp>
#include <cucumber-cpp/internal/CukeEngineImpl.hpp>

#include <string>
#include <utility>
#include <vector>

namespace cuke
{
    namespace internal
    {

        using CukeEngineImpl = cucumber::internal::CukeEngineImpl;
        using JsonSpiritWireMessageCodec = cucumber::internal::JsonWireMessageCodec;
        using WireProtocolHandler = cucumber::internal::WireProtocolHandler;
        using CucumberStepInfo = cuke::internal::CucumberStepInfo;

        /// @brief CucumberCpp runner handler over Wire Protocol
        class CukeServer
        {
        public:
            /// @brief Find step definitions by step text
            /// @param stepText 
            /// @return a vector of step definitions info
            std::vector<CucumberStepInfo> stepMatch(const std::string& stepText) const;

            /// @brief Fire begin-scenario hooks
            /// @param tags a vector of tags associated with a scenario
            void beginScenario(const std::vector<std::string>& tags) const;

            /// @brief Fire end-scenario hooks
            /// @param tags 
            void endScenario(const std::vector<std::string>& tags) const;

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
            CukeEngineImpl myCukeEngine;
            JsonSpiritWireMessageCodec myWireCodec;
            const WireProtocolHandler myHandler = WireProtocolHandler(myWireCodec, myCukeEngine);
        };

    } // namespace internal

} // namespace cuke
