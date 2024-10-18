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
 */
HttpRovComms::HttpRovComms()
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

	// Fire off the request
	auto result = client.Get("/mavlink2rest/mavlink/vehicles/1/components/1/messages/GLOBAL_POSITION_INT");
	auto body = result->body;

	// Decode the message
	auto readerGPS = Json::Reader(); auto valueGPS = Json::Value();
	readerGPS.parse(body, valueGPS);
	auto latitude = valueGPS["message"]["lat"].asDouble() / 1e7;
	auto longitude = valueGPS["message"]["lon"].asDouble() / 1e7;
	auto header = valueGPS["message"]["hdg"].asDouble() / 1e2;
	auto depth = valueGPS["message"]["relative_alt"].asDouble() / 1e3;
	auto altitude = valueGPS["message"]["alt"].asDouble() / 1e3; 

	// Return the result
	return unique_ptr<Status>(new Status(latitude, longitude, header, depth, altitude, 0, string(), 0, 0, false, 0));
}
