#include "FilterTagOptions.hpp"

#include <algorithm>
#include <string>
#include <vector>

namespace cuke::internal
{
    namespace
    {
        std::vector<std::string> parse_filter_tags(int ac, char** av)
        {
            std::vector<std::string> tags;
            bool foundTagsOption = false;
            for (int i = 1; i < ac; ++i) {
                std::string_view arg(av[i]);
                if (arg == "--tags")
                {
                    foundTagsOption = true;
                    continue;
                }
                else if (arg[0] == '-')
                {
                    foundTagsOption = false;
                    continue;
                }
                else if (foundTagsOption)
                {
                    tags.emplace_back(arg);
                }
            }
            return tags;
        }
    }

    void FilterTagOptions::parse(int ac, char**av)
    {
        std::vector<std::string> filterTags = parse_filter_tags(ac, av);
        for (const auto& tagExpr : filterTags)
        {
            myFilterExpressions.emplace_back(cucumber::tag_expressions::parse(tagExpr));
        }
    }

    bool FilterTagOptions::evaluate(const std::vector<std::string>& tags) const
    {
        return std::all_of(
            myFilterExpressions.begin(),
            myFilterExpressions.end(),
            [&tags](const TagExpression& expr) { return expr->evaluate(tags); }
        );
    }
} // namespace cuke::internal
