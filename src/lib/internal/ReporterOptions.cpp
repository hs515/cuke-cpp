#include "ReporterOptions.hpp"

#include <string_view>

using namespace cuke::internal;

void ReporterOptions::parse(int ac, char**av)
{
    for (int i = 1; i < ac; ++i) {
        std::string_view arg(av[i]);

        if ((arg == "--verbose") || (arg == "-V"))
        {
            consoleReport = true;
        }

        if (arg == "--json")
        {
            jsonReport = true;
        }

        if (arg == "--html")
        {
            htmlReport = true;
        }
    }
}
