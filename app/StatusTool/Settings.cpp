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
    if (_fields.find("interval") != _fields.end()) {
        _repo->SetField(Repository::Field::RATE, _fields["interval"]);
    }
}

void Settings::RenderSettings(string& content)
{
    auto rate = _repo->GetField(Repository::Field::RATE);
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