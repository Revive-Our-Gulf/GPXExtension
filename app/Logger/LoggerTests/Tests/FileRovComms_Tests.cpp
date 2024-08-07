//--------------------------------------------------
// Unit Tests for class FileRovComms
//
// @author: Wild Boar
//
// @date: 2024-08-07
//--------------------------------------------------

#include <gtest/gtest.h>

#include <LoggerLib/RovCommsFactory.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Confirm that reading that we are dealing with
 */
TEST(FileRovComms_Test, read_test)
{
	// Setup
	auto communicator = RovCommsFactory::GetCommunicator("file");

	// Execute
	auto status = communicator->GetCurrentStatus();

	// Confirm
	ASSERT_EQ(status->GetHeading(), 18594);
	ASSERT_EQ(status->GetDepth(), -5);
	ASSERT_EQ(status->GetAltitude(), 5);
	ASSERT_EQ(status->GetLatitude(), 6);
	ASSERT_EQ(status->GetLongitude(), 7);
	ASSERT_EQ(status->GetSatelliteCount(), 3);
	ASSERT_EQ(status->GetTemperature(), 3763);
}