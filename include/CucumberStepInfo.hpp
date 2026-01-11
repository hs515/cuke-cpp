#pragma once

#include <string>
#include <utility>
#include <vector>

namespace cuke
{
    namespace internal
    {

        using StepMatchArg = std::pair<int, std::string>;

        /// @brief Step definition matching info
        struct CucumberStepInfo 
        {
            std::string id;       // Step ID
            std::string source;   // Step text
            std::string regexp;   // Step definition regular expression
            std::vector<StepMatchArg> args; // Matching arguments (position : int, text : string)
        };

    } // namespace internal

} // namespace cuke
