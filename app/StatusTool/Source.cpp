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

    // Home Page
    CROW_ROUTE(app, "/")([&IP_DB, &navbarHtml](const crow::request& request)
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");
        auto parameters = unordered_map<string, string>();

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
            parameters["track"] = req.url_params.get("track");
            parameters["status"] = req.url_params.get("status");
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
        //repo.GetStatuses(request.url_params.get("start"), request.url_params.get("end"), statuses);
        auto trackName = request.url_params.get("track");
        repo.GetStatuses(trackName, statuses);
        auto maker = NVL_App::GPXMaker(statuses);
        return maker.RenderXML(trackName);
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
        jsonResponse["track"] = repo.GetField(trackField);
        jsonResponse["created"] = status->GetTimeStamp();
        jsonResponse["latitude"] = status->GetLatitude();
        jsonResponse["longitude"] = status->GetLongitude();
        jsonResponse["heading"] = status->GetHeading();
        jsonResponse["depth"] = status->GetDepth();
        jsonResponse["altitude"] = status->GetAltitude();
        jsonResponse["temperature"] = status->GetTemperature();
        jsonResponse["mode"] = status->GetMode();
        jsonResponse["satelliteCount"] = status->GetSatelliteCount();
        jsonResponse["poseCertainty"] = status->GetPosCertainity();
        jsonResponse["validVelocity"] = status->GetVelocityValid();
        jsonResponse["fom"] = status->GetFOM();

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