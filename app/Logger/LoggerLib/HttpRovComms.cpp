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
	auto global_position_int = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/GLOBAL_POSITION_INT");

	auto position_counter = global_position_int["status"]["time"]["counter"].asInt();

	auto global_position_int_message = global_position_int["message"];
	auto latitude = global_position_int_message["lat"].asDouble() / 1e7;
	auto longitude = global_position_int_message["lon"].asDouble() / 1e7;
	auto depth = global_position_int_message["relative_alt"].asDouble() / 1e3;
	auto heading = global_position_int_message["hdg"].asDouble() / 1e2;

	// SCALED_PRESSURE2 for temperature
	auto scaled_pressure2_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/SCALED_PRESSURE2")["message"];
	auto temperature = scaled_pressure2_message["temperature"].asDouble() / 100;

	// HEARTBEAT for drive mode
	auto heartbeat_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/HEARTBEAT")["message"];
	auto driveMode = heartbeat_message["custom_mode"].asDouble();

	// GPS_RAW_INT
	auto gps_raw_int_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/GPS_RAW_INT")["message"];
	auto satellites = gps_raw_int_message["satellites_visible"].asInt();
	auto hdop = gps_raw_int_message["eph"].asDouble() / 1e2;
	auto haccuracy = gps_raw_int_message["h_acc"].asDouble();

	// RANGEFINDER
	auto rangefinder_message = GetMessage(client, "/mavlink2rest/mavlink/vehicles/1/components/1/messages/RANGEFINDER")["message"];
	auto distance = rangefinder_message["distance"].asDouble();

	// Retrieve the track name
	auto trackName = GetTrackName();

    // Create and return the Status object
    return std::make_unique<Status>(latitude, longitude, position_counter, heading, depth, temperature, driveMode, satellites, hdop, haccuracy, distance, 0.0, true, trackName);
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

	return value;
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
