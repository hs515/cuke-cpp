#include "ListenerOptions.hpp"

using namespace cuke::internal;

void ListenerOptions::parse(int ac, char**av)
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
