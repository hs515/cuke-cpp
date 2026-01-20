#include "HtmlReporter.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace cuke::internal;

static void copyAll(const std::filesystem::path& src, const std::filesystem::path& dest);
static void render(const std::filesystem::path& tmpl, const std::filesystem::path& htmlFile, const json& data);

void HtmlReporter::dumpReport()
{
    const std::filesystem::path srcDir{"templates"};
    const std::filesystem::path destDir{"reports"};

    if (!std::filesystem::exists(destDir))
    {
        std::filesystem::create_directories(destDir);
    }

    // Generate features html
    json result = getJson();
    json& features = result["features"];
    for (auto&& feature : features)
    {
        if (feature["elements"].size() != 0)
        {
            const std::filesystem::path filepath(feature["uri"].get<std::string>());
            const auto filename = filepath.filename().string() + ".html";
            render(srcDir / "feature.html", destDir / filename, feature);
        }
    }

    // Generate index html
    for (auto&& feature : features)
    {
        // Index page does not use scenario info.
        feature.erase("featureScenarios");
    }
    render(srcDir / "index.html", destDir / "index.html", result);

    // Copy assets
    copyAll(srcDir / "assets", destDir);
}

static void copyAll(const std::filesystem::path& src, const std::filesystem::path& dest)
{
    if (std::filesystem::is_directory(src))
    {
        if (!std::filesystem::exists(dest))
        {
            std::filesystem::create_directories(dest);
        }
        for (const std::filesystem::directory_entry& item : std::filesystem::directory_iterator(src)) 
        {
            copyAll(item.path(), dest/item.path().filename());
        }
    } 
    else if (std::filesystem::is_regular_file(src))
    {
        std::filesystem::copy(src, dest, std::filesystem::copy_options::overwrite_existing);
    }
} 

static void render(const std::filesystem::path& tmpl, const std::filesystem::path& html, const json& data)
{
    std::ifstream in(tmpl.string());
    if (!in.is_open())
    {
        std::cerr << "Cannot open template file: " << tmpl.string() << std::endl;
        return;
    }

    std::ofstream out(html.string());
    if (!out.is_open())
    {
        std::cerr << "Cannot create html file: " << html.string() << std::endl;
        in.close();
        return;
    }

    std::string line;
    while (std::getline(in, line))
    {
        if (line.find("<!-- data -->") != std::string::npos)
        {
            out << "<script>const data = "
                << data.dump()
                << "</script>\n";
        }
        else
        {
            out << line << "\n";
        }
    }
    in.close();
    out.close();
}
