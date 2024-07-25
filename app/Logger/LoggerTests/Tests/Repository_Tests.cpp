//--------------------------------------------------
// Unit Tests for class Repository
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#include <gtest/gtest.h>

#include <LoggerLib/RovComms.h>
#include <LoggerLib/Repository.h>
using namespace NVL_App;

//--------------------------------------------------
// Test Methods
//--------------------------------------------------

/**
 * @brief Perform the status update
 */
TEST(Repository_Test, status_update)
{
	// Setup
	auto repository = Repository("BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Create insert statuses
	auto insert_1 = Status(1, 1, 1, 1, "TEST", 1, 1, true, 1);
	auto insert_2 = Status(2, 2, 2, 2, "TEST", 2, 2, true, 2);
	auto insert_3 = Status(3, 3, 3, 3, "TEST", 3, 3, true, 3);

	// Add elements and get the times back
	repository.AddStatus(&insert_1); auto added_1 = repository.GetLastStatus();
	sleep(1);
	repository.AddStatus(&insert_2); auto added_2 = repository.GetLastStatus();
	sleep(1);
	repository.AddStatus(&insert_3); auto added_3 = repository.GetLastStatus();

	// Now try and get the "middle" status
	auto match = repository.GetClosestStatus(added_2->GetTimeStamp());

	// Confirm
	ASSERT_EQ(match->GetHeading(), 2);
}

/**
 * @brief Perform the status update
 */
TEST(Repository_Test, confirm_retrieval)
{
	// Setup
	auto repository = Repository("BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Create a new status
	auto comms = NVL_App::RovComms();
	auto status_1 = comms.GetCurrentStatus();

	// Add the status to the repository
	repository.AddStatus(status_1.get());

	// Retrieve the status
	auto status_2 = repository.GetLastStatus();

	// Confirm
	ASSERT_NEAR(status_1->GetHeading(), status_2->GetHeading(), 1e-4);
	ASSERT_NEAR(status_1->GetDepth(), status_2->GetDepth(), 1e-4);
	ASSERT_NEAR(status_1->GetAltitude(), status_2->GetAltitude(), 1e-4);
	ASSERT_NEAR(status_1->GetTemperature(), status_2->GetTemperature(), 1e-4);
	ASSERT_EQ(status_1->GetMode(), status_2->GetMode());
	ASSERT_EQ(status_1->GetSatelliteCount(), status_2->GetSatelliteCount());
	ASSERT_NEAR(status_1->GetPosCertainity(), status_2->GetPosCertainity(), 1e-4);
	ASSERT_NEAR(status_1->GetVelocityValid(), status_2->GetVelocityValid(), 1e-4);
	ASSERT_NEAR(status_1->GetFOM(), status_2->GetFOM(), 1e-4);
}
