#include <CukeDocument.hpp>
#include <CukeRunner.hpp>
#include <FilterTagOptions.hpp>
#include <ReporterOptions.hpp>

#include <filesystem>
#include <iostream>

using namespace cuke::internal;
namespace fs = std::filesystem;

static void print_help()
{
    std::cout << "Usage: cukecpp --feature <feature_path> [options]\n"
                 "  --help                 Print this message\n"
                 "  --feature <path>       Path to feature file or directory containing feature files\n"
                 "  --tags <tag1> <tag2>   Tags to filter scenarios\n"
                 "  -V, --verbose          Enable verbose output from test run\n"
                 "  --json                 Generate JSON report from test run\n"
                 "  --html                 Generate HTML report from test run\n"
              << std::endl;
}

static void parse_helper_options(int ac, char** av)
{
    for (int i = 1; i < ac; ++i) {
        std::string_view arg(av[i]);
        if (arg == "--help")
        {
            print_help();
            exit(0);
        }
    }
}

static std::vector<std::string> parse_feature_files(int ac, char** av)
{
    std::vector<std::string> featureFiles;
    bool foundFeatureOption = false;
    for (int i = 1; i < ac; ++i) {
        std::string_view arg(av[i]);
        if (arg == "--feature")
        {
            foundFeatureOption = true;
            continue;
        }
        else if (arg[0] == '-')
        {
            foundFeatureOption = false;
            continue;
        }
        else if (foundFeatureOption)
        {
            fs::path featurePath(arg);
            if (!fs::exists(featurePath))
            {
                std::cerr << featurePath << " does not exist." << std::endl;
            }
            else if (fs::is_regular_file(featurePath))
            {
                featureFiles.emplace_back(featurePath.string());
            }
            else if (fs::is_directory(featurePath))
            {
                for(auto& entry : fs::recursive_directory_iterator(featurePath))
                {
                    if (fs::is_regular_file(entry) && (entry.path().extension() == ".feature"))
                    {
                        featureFiles.emplace_back(entry.path().string());
                    }
                }
            }
        }
    }
    return featureFiles;
}

static bool runBdd(const std::vector<std::string>& features, const ReporterOptions& reporterOpts, const FilterTagOptions& tagOpts)
{
    using namespace ::cuke::internal;
    CukeRunner cukeRunner(reporterOpts, tagOpts);
    bool success = true;
    for (auto& feature : features)
    {
        success = cukeRunner.run(feature) && success;
    }
    return success;
}

int main(int argc, char** argv)
{
    // Parse helper options
    parse_helper_options(argc, argv);

    // Collect feature files
    std::vector<std::string> featureFiles = parse_feature_files(argc, argv);
    if (featureFiles.empty())
    {
        std::cout << "No features to run!!" << std::endl;
        print_help();
        exit(0);
    }

    // Collect filter tags
    FilterTagOptions filterTagOptions;
    try
    {
        filterTagOptions.parse(argc, argv);
    }
    catch (std::exception& e)
    {
        std::cerr << "Error parsing tag expressions: " << e.what() << std::endl;
        exit(1);
    }

    // Collect listener options
    ReporterOptions reporterOptions;
    reporterOptions.parse(argc, argv);

    try 
    {
        bool success = runBdd(featureFiles, reporterOptions, filterTagOptions);
        return success ? 0 : 1;
    }
    catch(std::exception& e)
    {
        std::cerr << "Error running BDD tests: " << e.what() << std::endl;
        exit(1);
    }

    return 0;
}
