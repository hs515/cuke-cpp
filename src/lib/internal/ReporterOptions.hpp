#ifndef CUKE_INTERNAL_REPORTEROPTIONS_HPP
#define CUKE_INTERNAL_REPORTEROPTIONS_HPP

namespace cuke::internal
{
    struct ReporterOptions 
    {
        bool consoleReport = false;
        bool jsonReport = false;
        bool htmlReport = false;

        /**
         * Parse reporter options from command line arguments
         */
        void parse(int ac, char** av);
    };
} // namespace cuke::internal

#endif // CUKE_INTERNAL_REPORTEROPTIONS_HPP
