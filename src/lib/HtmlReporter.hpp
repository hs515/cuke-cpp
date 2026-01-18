#ifndef CUKE_CPP_INTERNAL_HTMLREPORTER_HPP
#define CUKE_CPP_INTERNAL_HTMLREPORTER_HPP

#include "JsonReporter.hpp"

namespace cuke::internal
{
    class HtmlReporter : public JsonReporter
    {
    protected:
        void dumpReport() override;
    };
} // namespace cuke

#endif // CUKE_CPP_INTERNAL_HTMLREPORTER_HPP
