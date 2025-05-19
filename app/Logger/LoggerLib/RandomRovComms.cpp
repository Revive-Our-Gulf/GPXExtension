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
	auto positionCounter = NVLib::RandomUtils::GetInteger(0, 100000);
	auto heading = 180.0 + NVLib::RandomUtils::GetInteger(-1800,1800) / 10;
	auto depth = -10.0 + NVLib::RandomUtils::GetInteger(-100, 100) / 10;
	auto temperature = 15.0 + NVLib::RandomUtils::GetInteger(-50,50) / 10;
	auto driveMode = 1;
	auto satellites = 10 + NVLib::RandomUtils::GetInteger(-8,8);
	auto hdop = 0.8 + (NVLib::RandomUtils::GetInteger(-200,200) / 1000.0);
	auto haccuracy =  0.8 + (NVLib::RandomUtils::GetInteger(-200,200) / 1000.0);
	auto distance = 10.0 + NVLib::RandomUtils::GetInteger(-100, 100) / 10;
	auto fom = 0.8 + (NVLib::RandomUtils::GetInteger(-200,200) / 1000.0);
	auto velocityValid = true;

	auto trackName = GetTrackName();

	return unique_ptr<Status>(new Status(latitude, longitude, positionCounter, heading, depth, temperature, driveMode, satellites, hdop, haccuracy, distance, fom, velocityValid, trackName));
}