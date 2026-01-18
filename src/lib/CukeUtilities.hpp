#ifndef CUKE_INTERNAL_UTILITIES_HPP
#define CUKE_INTERNAL_UTILITIES_HPP

#include <cucumber/tag-expressions/parser.hpp>

#include <vector>

namespace cuke::internal
{
    /**
     * Get current time in milliseconds since epoch
     * @return current time in milliseconds since epoch
     */
    uint64_t now();
} // namespace cuke::internal

#endif // CUKE_INTERNAL_UTILITIES_HPP
