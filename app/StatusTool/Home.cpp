// filepath: /home/sam/repos/GPXExtension/app/StatusTool/Home.cpp
#include "Home.h"
#include <crow.h>
#include <fstream>
#include <sstream>
using namespace NVL_App;

Home::Home(Repository* repo, unordered_map<string, string>& parameters) : _repo(repo), _fields(parameters) {
    if (_fields.find("submit") != _fields.end()) {
        SubmitForm();
    }
}

string Home::Render()
{
    std::ifstream file("templates/home.html");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open settings.html" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    RenderSettings(content);

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

        auto duration = CalculateDuration(earliestEntryTime, latestEntryTime);

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

void Home::RenderSettings(string& content)
{
    // Fetch track name and status
    auto trackName = _repo->GetField(Repository::Field::CURRENT_TRACK);
    auto status = _repo->GetField(Repository::Field::LOGGER_STATE);

    std::cout << "Status: " << status << std::endl;

    status = status == "STOPPED" ? "STARTED" : "STOPPED";

    std::cout << "New Status: " << status << std::endl;

    // Replace placeholders in the content
    ReplacePlaceholder(content, "{{trackName}}", trackName);
    ReplacePlaceholder(content, "{{status}}", status == "STOPPED" ? "Start" : "Stop");
    _repo->SetField(Repository::Field::LOGGER_STATE, status);
}

void Home::SubmitForm()
{
    // Update the repository fields if parameters are provided
    if (_fields.find("status") != _fields.end()) {
        _repo->SetField(Repository::Field::LOGGER_STATE, _fields["status"]);
    }
    if (_fields.find("track") != _fields.end()) {
        _repo->SetField(Repository::Field::CURRENT_TRACK, _fields["track"]);
    }
}

void Home::ReplacePlaceholder(string& content, const string& placeholder, const string& value)
{
    size_t pos = 0;
    while ((pos = content.find(placeholder, pos)) != std::string::npos) {
        content.replace(pos, placeholder.length(), value);
        pos += value.length();
    }
}

std::string Home::CalculateDuration(const std::string& earliestEntryTime, const std::string& latestEntryTime)
{
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
    return durationStream.str();
}