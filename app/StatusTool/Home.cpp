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

        // Fetch earliest and latest entry dates and times
        auto entryDate = _repo->GetEarliestEntryDate(track, "created_at");
        auto earliestEntryTime = _repo->GetEarliestEntryTime(track, "created_at");
        auto latestEntryTime = _repo->GetLatestEntryTime(track, "created_at");

        // Calculate the length of time between the earliest and latest entry times
        std::tm earliestTm = {};
        std::tm latestTm = {};
        std::istringstream earliestStream(earliestEntryTime);
        std::istringstream latestStream(latestEntryTime);
        earliestStream >> std::get_time(&earliestTm, "%H:%M:%S");
        latestStream >> std::get_time(&latestTm, "%H:%M:%S");

        std::time_t earliestTime = std::mktime(&earliestTm);
        std::time_t latestTime = std::mktime(&latestTm);
        double secondsDiff = std::difftime(latestTime, earliestTime);

        int hours = static_cast<int>(secondsDiff) / 3600;
        int minutes = (static_cast<int>(secondsDiff) % 3600) / 60;
        int seconds = static_cast<int>(secondsDiff) % 60;

        std::stringstream durationStream;
        durationStream << std::setw(2) << std::setfill('0') << hours << ":"
                   << std::setw(2) << std::setfill('0') << minutes << ":"
                   << std::setw(2) << std::setfill('0') << seconds;
        std::string duration = durationStream.str();
        
        std::cout << "Duration: " << duration << std::endl;
        std::cout << "Entry Date: " << entryDate << std::endl;
        std::cout << "Earliest Entry Time: " << earliestEntryTime << std::endl;

        tracksHtml << "<tr>";
        tracksHtml << "<td>" << track << "</td>";
        tracksHtml << "<td>" << entryDate << "</td>";
        tracksHtml << "<td>" << earliestEntryTime << "</td>";
        tracksHtml << "<td>" << duration << "</td>";
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