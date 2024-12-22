#include "Settings.h"
#include <fstream>
#include <sstream>
using namespace NVL_App;

Settings::Settings(Repository * repo, unordered_map<string, string>& parameters) : _repo(repo), _fields(parameters) {}

string Settings::Render()
{
    std::ifstream file("templates/settings.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Replace placeholders with actual values
    auto trackName = _repo->GetField(Repository::Field::CURRENT_TRACK);
    auto status = _repo->GetField(Repository::Field::LOGGER_STATE);
    auto stoppedSelected = status == "STOPPED" ? "selected" : string();
    auto startedSelected = status == "STARTED" ? "selected" : string();
    auto rate = _repo->GetField(Repository::Field::RATE);

    content.replace(content.find("{{trackName}}"), 12, trackName);
    content.replace(content.find("{{stoppedSelected}}"), 19, stoppedSelected);
    content.replace(content.find("{{startedSelected}}"), 19, startedSelected);
    content.replace(content.find("{{rate}}"), 7, rate);

    return content;
}