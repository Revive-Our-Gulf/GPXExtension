//--------------------------------------------------
// Implementation of class FileRovComms
//
// @author: Wild Boar
//
// @date: 2024-08-07
//--------------------------------------------------

#include "FileRovComms.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Default Constructor
 * @param repository A handle to the database
 */
FileRovComms::FileRovComms(Repository * repository) : RovCommsBase(repository)
{
	// Extra implementation
}

//--------------------------------------------------
// Read
//--------------------------------------------------

/**
 * @brief Retrieve the current state from the ROV
 * @return unique_ptr<Status> Returns a unique_ptr<Status>
 */
unique_ptr<Status> FileRovComms::GetCurrentStatus()
{
	// Connect fields from GPS query (latitude, longitude, heading, depth, altitude)
	auto jsonGPS = ReadMessage("GPS");
	auto readerGPS = Json::Reader(); auto valueGPS = Json::Value();
	readerGPS.parse(jsonGPS, valueGPS);
	auto latitude = valueGPS["message"]["lat"].asDouble();
	auto longitude = valueGPS["message"]["lon"].asDouble();
	auto positionCounter = valueGPS["status"]["counter"].asInt();
	auto header = valueGPS["message"]["hdg"].asDouble();
	auto depth = valueGPS["message"]["relative_alt"].asDouble();
	auto altitude = valueGPS["message"]["alt"].asDouble(); 

	// Connect fields from IMU query (temperature)
	auto jsonIMU = ReadMessage("IMU");
	auto readerIMU = Json::Reader(); auto valueIMU = Json::Value();
	readerIMU.parse(jsonIMU, valueIMU);
	auto temperature = valueIMU["message"]["temperature"].asDouble(); 

	// Connect fields from GPS_RAW (satellite Count)
	auto jsonGPS_RAW = ReadMessage("GPS_RAW");
	auto readerGPS_RAW = Json::Reader(); auto valueGPS_RAW = Json::Value();
	readerGPS_RAW.parse(jsonGPS_RAW, valueGPS_RAW);
	auto satellite_count = valueGPS_RAW["message"]["satellites_visible"].asDouble();

	// Retrieve the current track name
	auto trackName = GetTrackName();

	// Return the result
	return unique_ptr<Status>(new Status(latitude, longitude, positionCounter, header, depth, temperature, 1, satellite_count, 0, 0, 0, 0, false, trackName));
}

//--------------------------------------------------
// ReadMessage
//--------------------------------------------------

/**
 * Read a message
 * @param messageName The message that we are dealing with
 * @return The message that we are dealing with
 */
string FileRovComms::ReadMessage(const string& messageName) 
{
	auto folder = "../Logger/messages"; auto fileName = stringstream(); fileName << messageName << ".json";
	auto path = NVLib::FileUtils::PathCombine(folder, fileName.str());
	return NVLib::FileUtils::ReadFile(path);
}