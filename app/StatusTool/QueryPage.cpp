#include "QueryPage.h"
#include <fstream>
#include <sstream>
using namespace NVL_App;

// Constructor definition
QueryPage::QueryPage(unordered_map<string, string>& parameters) : _fields(parameters) {}

string QueryPage::Render()
{
    std::ifstream file("templates/query.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Replace placeholders with actual values
    size_t pos = content.find("{{track}}");
    if (pos != string::npos) {
        content.replace(pos, 8, _fields["track"]);
    }

    return content;
}