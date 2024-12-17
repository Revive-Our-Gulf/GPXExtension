//--------------------------------------------------
// Implementation of class RovComms
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#include "RandomRovComms.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Default Constructor
 * @param A handle to the application repository
 */
RandomRovComms::RandomRovComms(Repository * repository) : RovCommsBase(repository)
{
	NVLib::RandomUtils::TimeSeedRandomNumbers();
}

//--------------------------------------------------
// Read
//--------------------------------------------------

/**
 * @brief Retrieve the current state from the ROV
 * @return unique_ptr<Status> Returns a unique_ptr<Status>
 */
unique_ptr<Status> RandomRovComms::GetCurrentStatus()
{
	auto latitude = NVLib::RandomUtils::GetInteger(-1e8,1e8) / 1e7;
	auto longitude = NVLib::RandomUtils::GetInteger(-1e8,1e8) / 1e7;
	auto heading = 90 + NVLib::RandomUtils::GetInteger(-10,10);
	auto depth = 500 + NVLib::RandomUtils::GetInteger(-20, 20);
	auto altitude = 8 + NVLib::RandomUtils::GetInteger(-3, 3);
	auto temperature = 15 + NVLib::RandomUtils::GetInteger(-5,5);
	auto mode = "DEPTH HOLD";
	auto satCount = 10 + NVLib::RandomUtils::GetInteger(-8,8);
	auto posCert = 0.8 + (NVLib::RandomUtils::GetInteger(-200,200) / 1000.0);
	auto velocity = true;
	auto fom =  0.8 + (NVLib::RandomUtils::GetInteger(-200,200) / 1000.0);

	auto trackName = GetTrackName();

	return unique_ptr<Status>(new Status(latitude, longitude, heading, depth, altitude, temperature, mode, satCount, posCert, velocity, fom, trackName));
}