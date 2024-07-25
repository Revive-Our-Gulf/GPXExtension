//--------------------------------------------------
// Implementation of class Repository
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#include "Repository.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Initializer Constructor
 * @param database Initialize variable <database>
 */
Repository::Repository(const string& database)
{
	_database = database;
}

/**
 * The main repository
 */
Repository::~Repository() 
{
	// TODO: Add teardown logick
}

//--------------------------------------------------
// Update
//--------------------------------------------------

/**
 * @brief Add a new status to the system
 * @param status The status that we are dealing with
 */
void Repository::AddStatus(Status * status)
{
	throw runtime_error("Not implemented");
}

//--------------------------------------------------
// Retrieve
//--------------------------------------------------

/**
 * @brief Get the last status from the system
 * @return unique_ptr<Status> Returns a unique_ptr<Status>
 */
unique_ptr<Status> Repository::GetLastStatus()
{
	throw runtime_error("Not implemented");
}

/**
 * @brief Get the closest status
 * @param time The time that we are searching for
 * @return unique_ptr<Status> Returns a unique_ptr<Status>
 */
unique_ptr<Status> Repository::GetClosestStatus(const string& time)
{
	throw runtime_error("Not implemented");
}
