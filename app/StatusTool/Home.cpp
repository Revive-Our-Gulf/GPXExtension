#include "Home.h"
#include <crow.h>
#include <fstream>
#include <sstream>
#include <sys/statvfs.h>
using namespace NVL_App;

Home::Home(Repository* repo, unordered_map<string, string>& parameters) : _repo(repo), _fields(parameters) {}



std::pair<std::string, double> Home::GetFreeDiskSpace() {
    struct statvfs stat;

    if (statvfs("/", &stat) != 0) {
        // Error handling
        return {"Error", 0.0};
    }

    // Calculate free space and total space in GB
    unsigned long long freeSpace = stat.f_bsize * stat.f_bfree;
    unsigned long long totalSpace = stat.f_bsize * stat.f_blocks;
    double freeSpaceGB = freeSpace / (1024.0 * 1024.0 * 1024.0);
    double totalSpaceGB = totalSpace / (1024.0 * 1024.0 * 1024.0);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << freeSpaceGB << " GB free of " << totalSpaceGB << " GB";
    double usedPercentage = 100 - ((freeSpaceGB / totalSpaceGB) * 100.0);
    usedPercentage = std::round(usedPercentage * 10) / 10.0;
    return {ss.str(), usedPercentage};
}

string Home::Render()
{
    std::ifstream file("templates/home.html");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open home.html" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    std::string tracksHtml;
    RenderTracks(tracksHtml);
    ReplacePlaceholder(content, "{{tracks}}", tracksHtml);

    RenderSettings(content);
    RenderDiskSpace(content);
   

    return content;
}

void Home::RenderDiskSpace(string& content){
    auto [freeDiskSpace, usedPercentage] = GetFreeDiskSpace();
    ReplacePlaceholder(content, "{{freeDiskSpace}}", freeDiskSpace);

    std::stringstream progressBar;
    progressBar << "<div class=\"progress bg-dark\">";
    progressBar << "<div class=\"progress-bar\" role=\"progressbar\" aria-valuenow=\"" << usedPercentage << "\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width: " << usedPercentage << "%;\">";
    progressBar << usedPercentage << "% (Used)</div></div>";
    ReplacePlaceholder(content, "{{progressBar}}", progressBar.str());
}

void Home::RenderTracks(std::string& tracksHtml) {
    // Fetch tracks from the database
    auto tracks = _repo->GetTracks();
    std::stringstream tracksStream;

    if (tracks.empty()) {
        tracksStream << "<tr><td colspan=\"7\">No tracks available</td></tr>";
    } else {
        for (const auto& track : tracks) {
            auto gpxFile = std::stringstream(); gpxFile << track << ".gpx";

            // Fetch earliest and latest entry dates and times
            auto entryDate = _repo->GetEarliestEntryDate(track, "created_at");
            auto earliestEntryTime = _repo->GetEarliestEntryTime(track, "created_at");
            auto latestEntryTime = _repo->GetLatestEntryTime(track, "created_at");

            // Remove decimal points from entry times
            earliestEntryTime = earliestEntryTime.substr(0, earliestEntryTime.find('.'));
            latestEntryTime = latestEntryTime.substr(0, latestEntryTime.find('.'));

            auto duration = CalculateDuration(earliestEntryTime, latestEntryTime);

            // Convert duration to a more user-friendly format
            std::istringstream durationStream(duration);
            int hours, minutes, seconds;
            char colon;
            durationStream >> hours >> colon >> minutes >> colon >> seconds;
            std::stringstream userFriendlyDuration;
            if (hours > 0) {
                userFriendlyDuration << hours << "h ";
            }
            if (minutes > 0 || hours > 0) {
                userFriendlyDuration << minutes << "m ";
            }
            userFriendlyDuration << seconds << "s";

            auto trackSize = _repo->GetTrackDataSize(track);

            std::string friendlyTrackSize;
            std::stringstream sizeStream;
            sizeStream << std::fixed << std::setprecision(1);
            if (trackSize >= 1024 * 1024 * 1024) {
                sizeStream << (trackSize / (1024.0 * 1024.0 * 1024.0)) << " GB";
            } else if (trackSize >= 1024 * 1024) {
                sizeStream << (trackSize / (1024.0 * 1024.0)) << " MB";
            } else if (trackSize >= 1024) {
                sizeStream << (trackSize / 1024.0) << " KB";
            } else {
                sizeStream << trackSize << " B";
            }
            friendlyTrackSize = sizeStream.str();

            tracksStream << "<tr>";
            tracksStream << "<td><button class=\"btn btn-transparent btn-secondary text-dark\" id=\"trackName\" onclick=\"copyToClipboard('trackName')\">" << track << "</button></td>";
            tracksStream << "<td>" << entryDate << "</td>";
            tracksStream << "<td class=\"d-none d-md-table-cell\">" << earliestEntryTime << "</td>";
            tracksStream << "<td class=\"d-none d-md-table-cell\">" << latestEntryTime << "</td>";
            tracksStream << "<td>" << userFriendlyDuration.str() << "</td>";
            tracksStream << "<td>" << friendlyTrackSize << "</td>";
            tracksStream << "<td><button class=\"btn btn-secondary btn-sm btn-transparent btn-grey\" onclick=\"window.open('\\gpx?track=" << track << "', '_blank')\"><i class=\"fa fa-eye\"></i></button></td>";
            tracksStream << "<td><a class=\"btn btn-secondary btn-sm btn-transparent btn-grey\" href=\"/gpx?track=" << track << "\" download=\"" << gpxFile.str() << "\"><i class=\"fa fa-download\"></i></a></td>";
            tracksStream << "<td><button class=\"btn btn-danger btn-sm btn-transparent btn-red\" onclick=\"deleteTrack('" << track << "')\"><i class=\"fa fa-trash\"></i></button></td>";
            tracksStream << "</tr>";
        }
    }
    
    tracksHtml = tracksStream.str();
}

void Home::RenderSettings(string& content)
{
    // Fetch track name and status
    auto trackName = _repo->GetField(Repository::Field::CURRENT_TRACK);
    auto status = _repo->GetField(Repository::Field::LOGGER_STATE);

    // Replace placeholders in the content
    ReplacePlaceholder(content, "{{trackName}}", trackName);
    ReplacePlaceholder(content, "{{status}}", status == "STOPPED" ? "Start" : "Stop");
}

void Home::SubmitForm()
{
    auto currentStatus = _repo->GetField(Repository::Field::LOGGER_STATE);
    auto newStatus = currentStatus == "STOPPED" ? "STARTED" : "STOPPED";
    _repo->SetField(Repository::Field::LOGGER_STATE, newStatus);
    
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