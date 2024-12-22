#include "Home.h"
#include <crow.h>
#include <fstream>
#include <sstream>
using namespace NVL_App;

Home::Home(Repository* repo, unordered_map<string, string>& parameters) : _repo(repo), _fields(parameters) {}

string Home::Render()
{
    std::ifstream file("templates/home.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Fetch tracks from the database
    auto tracks = _repo->GetTracks();
    std::stringstream tracksHtml;
    for (const auto& track : tracks) {
        auto gpxFile = stringstream(); gpxFile << track << ".gpx";
        std::cout << "GPX File: " << gpxFile.str() << std::endl;
        tracksHtml << "<tr><td>" << track << "</td>";
        tracksHtml << "<td><a href=\"\\gpx?track=" << track << "\" download=\"" << gpxFile.str() << "\" class=\"btn btn-info\"/>Download</a></td></tr>";
    }

    // Replace placeholder with tracks HTML
    size_t pos = content.find("{{tracks}}");
    if (pos != string::npos) {
        content.replace(pos, 9, tracksHtml.str());
    }

    return content;
}