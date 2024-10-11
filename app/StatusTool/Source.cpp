//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2024-07-26
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>

#include <crow.h>

#include "GPXMaker.h"
#include "Repository.h"

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

    // Index page
    CROW_ROUTE(app, "/")([]()
    {
        auto page = crow::mustache::load_text("menu.html");
        return page;
    });

   // Status page
    CROW_ROUTE(app, "/status")([]()
    {
        auto page = crow::mustache::load_text("status.html");
        return page;
    });


    // CSS
    CROW_ROUTE(app, "/styles.css")([]()
    {
        auto stylesheet = crow::mustache::load_text("styles.css");
        return stylesheet;
    });

   // Get a status update
    CROW_ROUTE(app, "/current")([&IP_DB]()
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");
        auto status = repo.GetLastStatus();

        auto response = stringstream();
        response << "<table>";

        response << "<tr>";
        response << "<td><b>Created<b></td><td>" << status->GetTimeStamp() << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Latitude<b></td><td>" << status->GetLatitude() << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Longitude<b></td><td>" << status->GetLongitude() << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Heading<b></td><td>" << status->GetHeading() << " degrees</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Depth<b></td><td>" << status->GetDepth() << " meters</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Altitude<b></td><td>" << status->GetAltitude() << " meters</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Temperature<b></td><td>" << status->GetTemperature() << " degrees</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Mode<b></td><td>" << status->GetMode() << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Satellite Count<b></td><td>" << status->GetSatelliteCount() << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Pose Certainty<b></td><td>" << status->GetPosCertainity() << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>Valid Velocity<b></td><td>" << (status->GetVelocityValid() ? "true" : "false") << "</td>";
        response << "</tr>";

        response << "<tr>";
        response << "<td><b>FOM<b></td><td>" << status->GetFOM() << "</td>";
        response << "</tr>";

        response << "</table>";

        return response.str();
    });

    // Generate the GPX file
    CROW_ROUTE(app, "/gpx")([&IP_DB]()
    {
       auto repo = NVL_App::Repository(IP_DB, "BlueROV");
       auto statusList = vector<NVL_App::Status *>(); repo.GetStatuses(10, statusList);
       auto gpx = NVL_App::GPXMaker(statusList);
       return gpx.RenderXML();
    });

    //set the port, set the app to run on multiple threads, and run the app
    app.port(9000).multithreaded().run();
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
