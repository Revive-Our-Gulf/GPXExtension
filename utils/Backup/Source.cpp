//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2024-11-25
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <NVLib/Logger.h>
#include <NVLib/Path/PathHelper.h>
#include <NVLib/Parameters/Parameters.h>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "ArgReader.h"
#include "Repository.h"

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run(NVLib::Parameters * parameters);

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 * @param parameters The input parameters
 */
void Run(NVLib::Parameters * parameters) 
{
    if (parameters == nullptr) return; auto logger = NVLib::Logger(1);

    logger.StartApplication();

    logger.Log(1, "Loading the database location");
    auto reader = cv::FileStorage("config.xml", cv::FileStorage::FORMAT_XML | cv::FileStorage::READ);
    auto IP_DB = string(); reader["db"] >> IP_DB;
    logger.Log(1, "Loaded database location: %s", IP_DB.c_str());
    reader.release();

    logger.Log(1, "Creating a link to the database");
    auto repo = NVL_App::Repository(IP_DB, "BlueROV");

    logger.Log(1, "Retrieving all the available statuses");
    auto result = vector<NVL_App::Status *>(); repo.GetStatuses(result);
    logger.Log(1, "Number of statuses returned: %i", result.size());

    logger.Log(1, "Writing the output to a CSV file");
    auto writer = ofstream("status.csv"); if (!writer.is_open()) throw runtime_error("Unable to open: status.csv");
    for (auto record : result) { writer << record->ToString() << endl; delete record; }
    writer.close();

    logger.StopApplication();
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incoming arguments
 * @param argv The number of incoming arguments
 * @return SUCCESS and FAILURE
 */
int main(int argc, char ** argv) 
{
    NVLib::Parameters * parameters = nullptr;

    try
    {
        parameters = NVL_Utils::ArgReader::GetParameters(argc, argv);
        Run(parameters);
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

    if (parameters != nullptr) delete parameters;

    return EXIT_SUCCESS;
}
