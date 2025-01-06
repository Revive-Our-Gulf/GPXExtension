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
	auto repository = Repository("127.0.0.1", "BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Setup
	auto communicator = RovCommsFactory::GetCommunicator("file", &repository);

	// Execute
	auto status = communicator->GetCurrentStatus();

	// Confirm
	ASSERT_EQ(status->GetHeading(), 18594);
	ASSERT_EQ(status->GetDepth(), -5);
	ASSERT_EQ(status->GetLatitude(), 6);
	ASSERT_EQ(status->GetLongitude(), 7);
	ASSERT_EQ(status->GetTemperature(), 3763);
}