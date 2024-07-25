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
Repository::Repository(const string& database) : _database(database)
{
	// Build connection string
	auto connStr = stringstream(); connStr << "jdbc:mariadb://localhost:3306/" << database;

	// Setup connection
	auto driver = sql::mariadb::get_driver_instance();
	auto url = sql::SQLString(connStr.str()); 
	auto properties = sql::Properties( { {"user", "trevor"}, {"password", "808Rkief$"} } );

	// Connect
	_connection = driver->connect(url, properties);
}

/**
 * The main repository
 */
Repository::~Repository() 
{
	delete _connection;
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
	auto query = "INSERT INTO status (heading, depth, altitude, temperature, mode, sat_count, pos_certainty, velocity_valid, fom) "
				 "VALUES (?,?,?,?,?,?,?,?,?)";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));

	statement->setFloat(1, status->GetHeading());
	statement->setFloat(2, status->GetDepth());
	statement->setFloat(3, status->GetAltitude());
	statement->setFloat(4, status->GetTemperature());
	statement->setString(5, status->GetMode());
	statement->setInt(6, status->GetSatelliteCount());
	statement->setFloat(7, status->GetPosCertainity());
	statement->setBoolean(8, status->GetVelocityValid());
	statement->setFloat(9, status->GetFOM());

	statement->executeQuery();
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
	auto query = "SELECT id, heading, depth, altitude, temperature, mode, sat_count, pos_certainty, velocity_valid, fom, created_at "
				 "FROM status ORDER BY created_at DESC LIMIT 1;";

	auto statement = unique_ptr<sql::Statement>(_connection->createStatement());
	auto result = statement->executeQuery(query);

	if (result->next()) 
	{
		auto heading = result->getFloat(2);
		auto depth = result->getFloat(3);
		auto altitude = result->getFloat(4);
		auto temperature = result->getFloat(5);
		auto mode = string(result->getString(6).c_str());
		auto satCount = result->getInt(7);
		auto posCertainty = result->getFloat(8);
		auto velocityValid = result->getBoolean(9);
		auto fom = result->getFloat(10);
		auto created = string(result->getString(11));

		return unique_ptr<Status>(new Status(created, heading, depth, altitude, temperature, mode, satCount, posCertainty, velocityValid, fom));
	}

	return unique_ptr<Status>(nullptr);
}

/**
 * @brief Get the closest status
 * @param time The time that we are searching for
 * @return unique_ptr<Status> Returns a unique_ptr<Status>
 */
unique_ptr<Status> Repository::GetClosestStatus(const string& time)
{
	auto query = "SELECT id, heading, depth, altitude, temperature, mode, sat_count, pos_certainty, velocity_valid, fom, created_at, "
	             "TIME_TO_SEC(ABS(TIMEDIFF(?,created_at))) as time_diff "
				 "FROM status ORDER BY time_diff LIMIT 1;";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
	statement->setString(1, time);

	auto result = statement->executeQuery();

	if (result->next()) 
	{
		auto heading = result->getFloat(2);
		auto depth = result->getFloat(3);
		auto altitude = result->getFloat(4);
		auto temperature = result->getFloat(5);
		auto mode = string(result->getString(6).c_str());
		auto satCount = result->getInt(7);
		auto posCertainty = result->getFloat(8);
		auto velocityValid = result->getBoolean(9);
		auto fom = result->getFloat(10);
		auto created = string(result->getString(11));
		auto timeDiff = result->getFloat(12);

		cout << "Time difference: " << timeDiff << endl;

		return unique_ptr<Status>(new Status(created, heading, depth, altitude, temperature, mode, satCount, posCertainty, velocityValid, fom));
	}

	return unique_ptr<Status>(nullptr);
}

//--------------------------------------------------
// Retrieve
//--------------------------------------------------

/**
 * The table that we dealing with
 */
void Repository::ClearTable() 
{
	auto query = "delete from status";
	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
	statement->executeQuery();
}