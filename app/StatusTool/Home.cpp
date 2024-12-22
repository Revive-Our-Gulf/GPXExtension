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
        tracksHtml << "<tr>";
        tracksHtml << "<td>" << track << "</td>";
        tracksHtml << "<td><a href=\"\\gpx?track=" << track << "\" download=\"" << gpxFile.str() << "\" class=\"btn btn-info\">Download</a></td>";
        tracksHtml << "<td><button class=\"btn btn-danger\" onclick=\"deleteTrack('" << track << "')\">Delete</button></td>";
        tracksHtml << "</tr>";
    }

    ReplacePlaceholder(content, "{{tracks}}", tracksHtml.str());

    return content;
}

void Home::ReplacePlaceholder(string& content, const string& placeholder, const string& value)
{
    size_t pos = 0;
    while ((pos = content.find(placeholder, pos)) != std::string::npos) {
        content.replace(pos, placeholder.length(), value);
        pos += value.length();
    }
}