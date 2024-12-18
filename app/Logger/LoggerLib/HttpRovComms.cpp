//--------------------------------------------------
// Implementation of class HttpRovComms
//
// @author: Wild Boar
//
// @date: 2024-10-17
//--------------------------------------------------

#include "HttpRovComms.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Default Constructor
 * @param repository A handle to the database
 */
HttpRovComms::HttpRovComms(Repository * repository) : RovCommsBase(repository)
{
	_ip = "192.168.2.2";
}

//--------------------------------------------------
// Read
//--------------------------------------------------

/**
 * @brief Retrieve the current state from the ROV
 * @return unique_ptr<Status> Returns a unique_ptr<Status>
 */
unique_ptr<Status> HttpRovComms::GetCurrentStatus()
{
	// Setup client
	auto client = httplib::Client(_ip, 80);

	// Fire off the request ddddd
	auto result = client.Get("/mavlink2rest/mavlink/vehicles/1/components/1/messages/GLOBAL_POSITION_INT");
	auto body = result->body;

	// Decode the message
	auto readerGPS = Json::Reader(); auto valueGPS = Json::Value();
	readerGPS.parse(body, valueGPS);
	auto latitude = valueGPS["message"]["lat"].asDouble() / 1e7;
	auto longitude = valueGPS["message"]["lon"].asDouble() / 1e7;
	//auto header = valueGPS["message"]["hdg"].asDouble() / 1e2;
	auto depth = valueGPS["message"]["relative_alt"].asDouble() / 1e3;
	auto altitude = valueGPS["message"]["alt"].asDouble() / 1e3; 

	// Get a better heading
	result = client.Get("/mavlink2rest/mavlink/vehicles/1/components/1/messages/VFR_HUD");
	auto valueHeading = Json::Value();
	readerGPS.parse(result->body, valueHeading) / 1e2;
	auto heading = valueHeading["message"]["heading"].asDouble();

	// Get the temperature
	result = client.Get("/mavlink2rest/mavlink/vehicles/1/components/1/messages/SCALED_PRESSURE2");
	auto valuePressure = Json::Value();
	readerGPS.parse(result->body, valuePressure);
	auto temperature = valuePressure["message"]["temperature"].asDouble() / 1e2;

	// Get the drive mode
	result = client.Get("/mavlink2rest/mavlink/vehicles/1/components/1/messages/HEARTBEAT");
	auto valueHeart = Json::Value();
	readerGPS.parse(result->body, valueHeart);
	auto driveMode = valueHeart["message"]["custom_mode"].asString();

	// Retrieve the track name
	auto trackName = GetTrackName();

	// Return the result
	return unique_ptr<Status>(new Status(latitude, longitude, heading, depth, altitude, temperature, driveMode, 0, 0, false, 0, trackName));
}
