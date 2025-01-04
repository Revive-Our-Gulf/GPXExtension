//--------------------------------------------------
// Unit Tests for class Repository
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#include <gtest/gtest.h>

#include <LoggerLib/RovCommsFactory.h>
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
	auto repository = Repository("127.0.0.1", "BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Create insert statuses
	auto insert_1 = Status(1,1,1, 1, 1, 1, "TEST", 1, 1, true, 1, string());
	auto insert_2 = Status(2,2,2, 2, 2, 2, "TEST", 2, 2, true, 2, string());
	auto insert_3 = Status(3,3,3, 3, 3, 3, "TEST", 3, 3, true, 3, string());

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
	auto repository = Repository("127.0.0.1", "BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Create a new status
	auto comms = NVL_App::RovCommsFactory::GetCommunicator("random", &repository);
	auto status_1 = comms->GetCurrentStatus();

	// Add some decimals to the lat and long
	status_1->GetLatitude() = 0.123456789;
	status_1->GetLongitude() = 0.123456789;

	// Add the status to the repository
	repository.AddStatus(status_1.get());

	// Retrieve the status
	auto status_2 = repository.GetLastStatus();

	// Confirm
	ASSERT_NEAR(status_1->GetLatitude(), status_2->GetLatitude(), 1e-9);
	ASSERT_NEAR(status_1->GetLongitude(), status_2->GetLongitude(), 1e-9);
	ASSERT_NEAR(status_1->GetHeading(), status_2->GetHeading(), 1e-4);
	ASSERT_NEAR(status_1->GetDepth(), status_2->GetDepth(), 1e-4);
	ASSERT_NEAR(status_1->GetAltitude(), status_2->GetAltitude(), 1e-4);
	ASSERT_NEAR(status_1->GetTemperature(), status_2->GetTemperature(), 1e-4);
	ASSERT_EQ(status_1->GetMode(), status_2->GetMode());
	ASSERT_EQ(status_1->GetSatelliteCount(), status_2->GetSatelliteCount());
	ASSERT_NEAR(status_1->GetHdop(), status_2->GetHdop(), 1e-4);
	ASSERT_NEAR(status_1->GetHaccuracy(), status_2->GetHaccuracy(), 1e-4);
	ASSERT_NEAR(status_1->GetVelocityValid(), status_2->GetVelocityValid(), 1e-4);

}

/**
 * Add the test by track name
 */
TEST(Repository_Test, state_by_track_name) 
{
	// Setup
	auto repository = Repository("127.0.0.1", "BlueROV_Test");

	// Clear the repo
	repository.ClearTable();

	// Create a new status
	auto comms = NVL_App::RovCommsFactory::GetCommunicator("random", &repository);

	// Add an entry for track 1
	repository.SetField(Repository::Field::CURRENT_TRACK, "track 1");
	auto status_1 = comms->GetCurrentStatus();

	// Add entries for track 2
	repository.SetField(Repository::Field::CURRENT_TRACK, "track 2");
	auto status_2 = comms->GetCurrentStatus();
	auto status_3 = comms->GetCurrentStatus();

	// Add the statuses to the repository
	repository.AddStatus(status_1.get());
	repository.AddStatus(status_2.get());
	repository.AddStatus(status_3.get());

	// Retrieve track 2
	auto retrieved = vector<Status *>(); repository.GetStatuses("track 2", retrieved);

	// Validate
	ASSERT_EQ(retrieved.size(), 2);
	ASSERT_EQ(retrieved[0]->GetLatitude(), status_2->GetLatitude());
	ASSERT_EQ(retrieved[1]->GetLatitude(), status_3->GetLatitude());

	// Free
	for (auto status: retrieved) delete status;
}

/**
 * Add the logic to get and set the field value
 */
TEST(Repository_Test, field_get_and_set)
{
	// Setup
	auto repository = Repository("127.0.0.1", "BlueROV_Test");

	// Set the field to a pre-value
	repository.SetField(Repository::Field::LOGGER_STATE, "STOPPED");
	repository.SetField(Repository::Field::RATE, "1000");

	// Confirm the setting
	ASSERT_EQ(repository.GetField(Repository::Field::LOGGER_STATE), "STOPPED");
	ASSERT_EQ(repository.GetField(Repository::Field::RATE), "1000");

	// Set the field to a pre-value
	repository.SetField(Repository::Field::LOGGER_STATE, "STARTED");
	repository.SetField(Repository::Field::RATE, "500");

	// Confirm the setting
	ASSERT_EQ(repository.GetField(Repository::Field::LOGGER_STATE), "STARTED");
	ASSERT_EQ(repository.GetField(Repository::Field::RATE), "500");
}