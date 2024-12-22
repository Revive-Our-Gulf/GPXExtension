#include "Settings.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace NVL_App;

Settings::Settings(Repository* repo, unordered_map<string, string>& parameters) : _repo(repo), _fields(parameters) {
    // Call SubmitForm if the submit parameter is present
    if (_fields.find("submit") != _fields.end()) {
        SubmitForm();
    }
}

string Settings::Render()
{
    std::ifstream file("templates/settings.html");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open settings.html" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    RenderSettings(content);

    return content;
}

void Settings::SubmitForm()
{
    // Update the repository fields if parameters are provided
    if (_fields.find("status") != _fields.end()) {
        _repo->SetField(Repository::Field::LOGGER_STATE, _fields["status"]);
    }
    if (_fields.find("interval") != _fields.end()) {
        _repo->SetField(Repository::Field::RATE, _fields["interval"]);
    }
    if (_fields.find("track") != _fields.end()) {
        _repo->SetField(Repository::Field::CURRENT_TRACK, _fields["track"]);
    }
}

void Settings::RenderSettings(string& content)
{
    // Replace placeholders with actual values
    auto trackName = _repo->GetField(Repository::Field::CURRENT_TRACK);
    auto status = _repo->GetField(Repository::Field::LOGGER_STATE);
    auto stoppedSelected = status == "STOPPED" ? "selected" : string();
    auto startedSelected = status == "STARTED" ? "selected" : string();
    auto rate = _repo->GetField(Repository::Field::RATE);

    ReplacePlaceholder(content, "{{trackName}}", trackName);
    ReplacePlaceholder(content, "{{stoppedSelected}}", stoppedSelected);
    ReplacePlaceholder(content, "{{startedSelected}}", startedSelected);
    ReplacePlaceholder(content, "{{rate}}", rate);
}

void Settings::ReplacePlaceholder(string& content, const string& placeholder, const string& value)
{
    size_t pos = 0;
    while ((pos = content.find(placeholder, pos)) != std::string::npos) {
        content.replace(pos, placeholder.length(), value);
        pos += value.length();
    }
}