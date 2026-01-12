#pragma once

#include "CucumberJsonReporter.hpp"

namespace cuke
{
    namespace internal
    {

        class CucumberHtmlReporter : public CucumberJsonReporter
        {
        protected:
            void dumpReport() override;
        };

    } // namespace internal

} // namespace cuke
