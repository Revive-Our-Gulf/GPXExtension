//--------------------------------------------------
// Unit Tests for class RovComms
//
// @author: Wild Boar
//
// @date: 2024-07-25
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
TEST(RovComms_Test, read_test)
{
		// Setup
	auto repository = Repository("127.0.0.1", "BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Create a communicator
	auto comms = RovCommsFactory::GetCommunicator("random", &repository);

	// Get the next status
	auto status = comms->GetCurrentStatus();

	// Check the values
	ASSERT_GE(status->GetHeading(), 80);
	ASSERT_GE(status->GetDepth(), 480);
	ASSERT_GE(status->GetAltitude(), 5);
	ASSERT_GE(status->GetTemperature(), 10);
	ASSERT_EQ(status->GetMode(), "DEPTH HOLD");
	ASSERT_GE(status->GetSatelliteCount(), 2);
	ASSERT_GE(status->GetHdop(), 0.6);
	ASSERT_GE(status->GetHaccuracy(), 0.6);
	ASSERT_TRUE(status->GetVelocityValid());
	
}