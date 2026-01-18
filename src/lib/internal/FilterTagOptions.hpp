#ifndef CUKE_INTERNAL_FILTERTAGOPTIONS_HPP
#define CUKE_INTERNAL_FILTERTAGOPTIONS_HPP

#include <cucumber/tag-expressions/parser.hpp>

#include <vector>

namespace cuke::internal
{
    using TagExpression = std::unique_ptr<cucumber::tag_expressions::Expression>;

    class FilterTagOptions 
    {
    public:
        /**
         * Parse filter tag expressions from command line arguments
         */
        void parse(int ac, char** av);

        /**
         * Evaluate if the given tags pass the filter expressions
         * @param tags The tags to evaluate
         * @return true if the tags pass the filter expressions, false otherwise
         */
        bool evaluate(const std::vector<std::string>& tags) const;

    private:
        std::vector<TagExpression> myFilterExpressions;
    };
} // namespace cuke::internal

#endif // CUKE_INTERNAL_FILTERTAGOPTIONS_HPP
