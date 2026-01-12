#pragma once

#include <string>
#include <utility>
#include <vector>

namespace cuke
{
    namespace internal
    {

        struct ListenerOptions 
        {
            bool consoleReport = false;
            bool jsonReport = false;
            bool htmlReport = false;

            void parse(int ac, char** av);
        };

    } // namespace internal

} // namespace cuke
