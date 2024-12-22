#include "Home.h"
#include <crow.h>
#include <fstream>
#include <sstream>
using namespace NVL_App;

string Home::Render()
{
    std::ifstream file("templates/home.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}