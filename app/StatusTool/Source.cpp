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
#include "Settings.h"
#include "QueryPage.h"

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

    // Status page
    CROW_ROUTE(app, "/")([]()
    {
        auto page = crow::mustache::load_text("status.html");
        return page;
    });

    // Settings page
    CROW_ROUTE(app, "/settings")([&IP_DB](const crow::request& request)
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");

        auto parameters = unordered_map<string, string>();
        if (request.url_params.get("submit") != nullptr) 
        {
            parameters["submit"] = "submit";
            parameters["status"] = request.url_params.get("status");
            parameters["interval"] = request.url_params.get("interval");
        }

        auto page = NVL_App::Settings(&repo, parameters);

        return page.Render();
    });

    // Query page
    CROW_ROUTE(app, "/query")([&IP_DB](const crow::request& request)
    {
        auto parameters = unordered_map<string, string>();
        if (request.url_params.get("submit") != nullptr) 
        {
            parameters["submit"] = "submit";
            parameters["start"] = request.url_params.get("start");
            parameters["end"] = request.url_params.get("end");
        }

        auto page = NVL_App::QueryPage(parameters);

        return page.Render();
    });

    // GPX data
    CROW_ROUTE(app, "/gpx")([&IP_DB](const crow::request& request)
    {
        auto repo = NVL_App::Repository(IP_DB, "BlueROV");        
        auto statuses = vector<NVL_App::Status *>();
	    repo.GetStatuses(request.url_params.get("start"), request.url_params.get("end"), statuses);
	    auto maker = NVL_App::GPXMaker(statuses);
        return maker.RenderXML();
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

        auto response = stringstream();

        response << "<div class=\"p-4\">";

        response << "<p class=\"alert alert-info\">Latest update</p>";

        response << "<div class=\"container-fluid border\">";

        response << "<div class=\"row\" style=\"background-color:white;\">";
        response << "<div class=\"col-sm-4\"><b>Created</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetTimeStamp() << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:lightgrey;\">";
        response << "<div class=\"col-sm-4\"><b>Latitude</b></div>";
        response << "<div class=\"col_sm_8\">" << fixed << setprecision(8) << status->GetLatitude() << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:white;\">";
        response << "<div class=\"col-sm-4\"><b>Longitude</b></div>";
        response << "<div class=\"col_sm_8\">" << fixed << setprecision(8) << status->GetLongitude() << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:lightgrey;\">";
        response << "<div class=\"col-sm-4\"><b>Heading</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetHeading() << " degrees</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:white;\">";
        response << "<div class=\"col-sm-4\"><b>Depth</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetDepth() << " meters</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:lightgrey;\">";
        response << "<div class=\"col-sm-4\"><b>Altitude</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetAltitude() << " meters</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:white;\">";
        response << "<div class=\"col-sm-4\"><b>Temperature</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetTemperature() << " degrees</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:lightgrey;\">";
        response << "<div class=\"col-sm-4\"><b>Mode</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetMode() << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:white;\">";
        response << "<div class=\"col-sm-4\"><b>Satellite Count</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetSatelliteCount() << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:lightgrey;\">";
        response << "<div class=\"col-sm-4\"><b>Pose Certainty</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetPosCertainity() << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:white;\">";
        response << "<div class=\"col-sm-4\"><b>Valid Velocity</b></div>";
        response << "<div class=\"col_sm_8\">" << (status->GetVelocityValid() ? "true" : "false") << "</div>";
        response << "</div>";

        response << "<div class=\"row\" style=\"background-color:lightgrey;\">";
        response << "<div class=\"col-sm-4\"><b>FOM</b></div>";
        response << "<div class=\"col_sm_8\">" << status->GetFOM() << "</div>";
        response << "</div>";

        response << "</div></div>";

        return response.str();
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
