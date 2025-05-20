// filepath: /home/sam/repos/GPXExtension/app/StatusTool/Source.cpp
//--------------------------------------------------
// Startup code module
//
// @date: 2024-07-26
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>

#include <crow.h>
#include "GPXMaker.h"
#include "Repository.h"
#include "Settings.h"
#include "QueryPage.h"
#include "Home.h"

#include <unordered_map>
#include <string>
#include <sstream>


// Function to load the navbar HTML from a file
std::string loadNavbarHtml(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to replace a placeholder in a string
std::string replacePlaceholder(const std::string& content, const std::string& placeholder, const std::string& replacement) {
    std::string result = content;
    size_t pos = result.find(placeholder);
    while (pos != std::string::npos) {
        result.replace(pos, placeholder.length(), replacement);
        pos = result.find(placeholder, pos + replacement.length());
    }
    return result;
}

std::unordered_map<std::string, std::string> extractFormData(const std::string& body) {
    std::unordered_map<std::string, std::string> formData;
    std::string delimiter = "Content-Disposition: form-data; name=\"";
    size_t pos = 0;
    while ((pos = body.find(delimiter, pos)) != std::string::npos) {
        pos += delimiter.length();
        size_t endPos = body.find("\"", pos);
        std::string name = body.substr(pos, endPos - pos);
        pos = body.find("\r\n\r\n", endPos) + 4;
        endPos = body.find("\r\n--", pos);
        std::string value = body.substr(pos, endPos - pos);
        formData[name] = value;
        pos = endPos;
    }
    return formData;
}


//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run();

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 */
void Run() 
{
    crow::SimpleApp app;

    // Load configuration parameters
    auto reader = cv::FileStorage("config.xml", cv::FileStorage::FORMAT_XML | cv::FileStorage::READ);
    auto IP_DB = string(); reader["db"] >> IP_DB;
    reader.release();

    // Load the navbar HTML
    std::string navbarHtml = loadNavbarHtml("templates/navbar.html");

    auto repo = NVL_App::Repository(IP_DB, "BlueROV");
    auto parameters = unordered_map<string, string>();
    auto home = NVL_App::Home(&repo, parameters);

    // Home Page
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([&IP_DB, &navbarHtml](const crow::request& req)
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");
        auto parameters = unordered_map<string, string>();

        if (req.method == crow::HTTPMethod::POST)
        {
            // Handle form submission
            auto formData = extractFormData(req.body);
            for (const auto& pair : formData) {
                cout << pair.first << ": " << pair.second << endl;
            }
            repo.SetField(NVL_App::Repository::Field::LOGGER_STATE, formData["status"]);
            repo.SetField(NVL_App::Repository::Field::CURRENT_TRACK, formData["track"]);
        }

        auto page = NVL_App::Home(&repo, parameters);

        std::string content = page.Render();
        content = replacePlaceholder(content, "{{navbar}}", navbarHtml);

        return crow::response(content);
    });


    // Settings page
    CROW_ROUTE(app, "/settings")([&IP_DB, &navbarHtml](const crow::request& req)
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");
        auto parameters = unordered_map<string, string>();
        
        if (req.url_params.get("submit") != nullptr) 
        {
            parameters["submit"] = "submit";
            parameters["interval"] = req.url_params.get("interval");
        }

        auto page = NVL_App::Settings(&repo, parameters);

        std::string content = page.Render();
        content = replacePlaceholder(content, "{{navbar}}", navbarHtml);

        return crow::response(content);
    });

    // GPX data
    CROW_ROUTE(app, "/gpx")([&IP_DB](const crow::request& request)
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");        
        auto statuses = vector<NVL_App::Status *>();
        auto trackName = request.url_params.get("track");
        repo.GetStatuses(trackName, statuses);
        auto maker = NVL_App::GPXMaker(statuses);
        return maker.RenderXML(trackName);
    });

    CROW_ROUTE(app, "/tracks")([&home](const crow::request& req) 
    {
        std::string tracksHtml;
        home.RenderTracks(tracksHtml);
        return crow::response(tracksHtml);
    });

    // CSS
    CROW_ROUTE(app, "/styles.css")([]()
    {
        auto stylesheet = crow::mustache::load_text("styles.css");
        return stylesheet;
    });

    // Logic to register the service
    CROW_ROUTE(app, "/register_service")([]()
    {
        auto stylesheet = crow::mustache::load_text("register_service");
        return stylesheet;
    });

    // Get a status update
    CROW_ROUTE(app, "/current")([&IP_DB]()
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");
        auto status = repo.GetLastStatus();
        auto trackField = NVL_App::Repository::Field::CURRENT_TRACK;
        
        crow::json::wvalue jsonResponse;

        if (status != nullptr) {
            jsonResponse["created"] = status->GetTimeStamp();
            jsonResponse["positionCounter"] = status->GetPositionCounter();
            jsonResponse["latitude"] = status->GetLatitude();
            jsonResponse["longitude"] = status->GetLongitude();
            jsonResponse["heading"] = status->GetHeading();
            jsonResponse["depth"] = status->GetDepth();
            jsonResponse["temperature"] = status->GetTemperature();
            jsonResponse["driveMode"] = status->GetDriveMode();
            jsonResponse["satellites"] = status->GetSatellites();
            jsonResponse["hdop"] = status->GetHdop();
            jsonResponse["haccuracy"] = status->GetHaccuracy();
            jsonResponse["distance"] = status->GetDistance();
            jsonResponse["fom"] = status->GetFom();
            jsonResponse["validVelocity"] = status->GetVelocityValid();
        } else {
            jsonResponse["error"] = "No status available";
        }

        return jsonResponse;
    });

    CROW_ROUTE(app, "/delete_track")([&IP_DB](const crow::request& request) 
    {
        auto track = request.url_params.get("track");
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");
        if (track != nullptr) {
            repo.DeleteTrack(track);
        }
        return crow::response(200);
    });

    CROW_ROUTE(app, "/saveTrack").methods("POST"_method)([&repo](const crow::request& req) {
        auto formData = extractFormData(req.body);
        if (formData.find("track") != formData.end()) {
            repo.SetField(NVL_App::Repository::Field::CURRENT_TRACK, formData["track"]);
        }
        return crow::response(200);
    });

    CROW_ROUTE(app, "/startStop").methods("POST"_method)([&repo](const crow::request& req) {
        auto formData = extractFormData(req.body);
        auto currentStatus = repo.GetField(NVL_App::Repository::Field::LOGGER_STATE);
        auto newStatus = currentStatus == "STOPPED" ? "STARTED" : "STOPPED";

        if (newStatus == "STARTED") {
            auto currentTrackName = repo.GetField(NVL_App::Repository::Field::CURRENT_TRACK);
            
            if (formData.find("track") != formData.end()) {
                currentTrackName = formData["track"];
            }
            
            string baseName = currentTrackName;
            size_t underscorePos = baseName.find_last_of('_');
            if (underscorePos != string::npos) {
                string suffixPart = baseName.substr(underscorePos + 1);
                bool isNumericSuffix = !suffixPart.empty() && 
                    std::all_of(suffixPart.begin(), suffixPart.end(), ::isdigit);
                
                if (isNumericSuffix) {
                    baseName = baseName.substr(0, underscorePos);
                }
            }
            
            auto existingTracks = repo.GetTracks();
            string uniqueTrackName = baseName;
            int suffix = 1;
            
            while (std::find(existingTracks.begin(), existingTracks.end(), uniqueTrackName) != existingTracks.end()) {
                uniqueTrackName = baseName + "_" + std::to_string(suffix);
                suffix++;
            }
            
            repo.SetField(NVL_App::Repository::Field::CURRENT_TRACK, uniqueTrackName);
        }
            
        repo.SetField(NVL_App::Repository::Field::LOGGER_STATE, newStatus);
        return crow::response(200);
    });
    
    //set the port, set the app to run on multiple threads, and run the app
    app.port(5428).multithreaded().run();
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @return SUCCESS and FAILURE
 */
int main(int, char **) 
{
    try
    {
        Run();
    }
    catch (runtime_error exception)
    {
        cerr << "Error: " << exception.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        cerr << "Error: " << exception << endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}