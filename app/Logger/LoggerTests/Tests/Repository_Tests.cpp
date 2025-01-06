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