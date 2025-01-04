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

	// GLOBAL_POSITION_INT
	auto global_position_int_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/GLOBAL_POSITION_INT");

	auto latitude = global_position_int_message["lat"].asDouble() / 1e7;
	auto longitude = global_position_int_message["lon"].asDouble() / 1e7;
	auto depth = global_position_int_message["relative_alt"].asDouble() / 1e3;
	auto altitude = global_position_int_message["alt"].asDouble() / 1e3; 

	// VFR_HUD for heading
	auto vfr_hud_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/VFR_HUD");
	auto heading = vfr_hud_message["heading"].asDouble();

	// SCALED_PRESSURE2 for temperature
	auto scaled_pressure2_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/SCALED_PRESSURE2");
	auto temperature = scaled_pressure2_message["temperature"].asDouble() / 100;

	// HEARTBEAT for drive mode
	auto heartbeat_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/HEARTBEAT");
	auto driveMode = heartbeat_message["custom_mode"].asDouble();
	auto driveString = stringstream(); driveString << driveMode;

	// GPS_RAW_INT
	auto gps_raw_int_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/GPS_RAW_INT");
	auto satelliteCount = gps_raw_int_message["satellites_visible"].asInt();
	auto hdop = gps_raw_int_message["eph"].asDouble();
	auto haccuracy = gps_raw_int_message["h_acc"].asDouble();

	// Retrieve the track name
	auto trackName = GetTrackName();

	// Return the result
	return unique_ptr<Status>(new Status(latitude, longitude, heading, depth, altitude, temperature, driveString.str(), satelliteCount, hdop, haccuracy, false, trackName));
}

/**
 * @brief Retrieve a JSON message from the given path
 * @param path The path to the message
 * @return Json::Value The JSON value of the message
 */
Json::Value HttpRovComms::GetMessage(httplib::Client& client, const string& path)
{
	// Fire off the request
	auto detail = client.Get(path);
	auto body = detail->body;

	// Decode the message
	auto reader = Json::Reader(); 
	auto value = Json::Value();
	reader.parse(body, value);

	return value["message"];
}

/**
 * Add the logic to get a value
 */
double HttpRovComms::GetValue(const string& path, const string& name) 
{
	// Setup client
	auto client = httplib::Client(_ip, 80);

	// Fire off the request 
	auto detail = client.Get(path);
	auto body = detail->body;

	// Decode the message
	auto reader = Json::Reader(); auto value = Json::Value();
	reader.parse(body, value);
	auto result = value["message"][name].asDouble();

	// Return the result
	return result;
}
