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
 * @param ipAddress The IP address that we are working with
 * @param database Initialize variable <database>
 */
Repository::Repository(const string& ipAddress, const string& database) : _database(database)
{
	// Build connection string

	auto connStr = stringstream(); connStr << "jdbc:mariadb://" << ipAddress << ":3306/" << database;

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
	auto query = "INSERT INTO status "
				 "("
				 "latitude, "
				 "longitude, "
				 "heading, "
				 "depth, "
				 "temperature, "
				 "drive_mode, "
				 "gps_satellites, "
				 "gps_hdop, "
				 "gps_haccuracy, "
				 "dvl_distance, "
				 "dvl_fom, "
				 "dvl_velocity_valid, "
				 "track_name "
				 ") "
				 "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));

	statement->setDouble(1, status->GetLatitude());
	statement->setDouble(2, status->GetLongitude());
	statement->setDouble(3, status->GetHeading());
	statement->setDouble(4, status->GetDepth());
	statement->setDouble(5, status->GetTemperature());
	statement->setInt(6, status->GetDriveMode());
	statement->setInt(7, status->GetSatellites());
	statement->setDouble(8, status->GetHdop());
	statement->setDouble(9, status->GetHaccuracy());
	statement->setDouble(10, status->GetDistance());
	statement->setDouble(11, status->GetFom());
	statement->setBoolean(12, status->GetVelocityValid());
	statement->setString(13, status->GetTrackName());

	cout << status->GetTemperature() << endl;

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
	auto query = "SELECT "
				 "latitude, "
				 "longitude, "
				 "heading, "
				 "depth, "
				 "temperature, "
				 "drive_mode, "
				 "gps_satellites, "
				 "gps_hdop, "
				 "gps_haccuracy, "
				 "dvl_distance, "
				 "dvl_fom, "
				 "dvl_velocity_valid, "
				 "track_name, "
				 "created_at "
				 "FROM status ORDER BY created_at DESC LIMIT 1;";

	auto statement = unique_ptr<sql::Statement>(_connection->createStatement());
	auto result = statement->executeQuery(query);

	if (result->next()) 
	{
		auto latitude = result->getDouble(1);
		auto longitude = result->getDouble(2);
		auto heading = result->getDouble(3);
		auto depth = result->getDouble(4);
		auto temperature = result->getDouble(5);
		auto driveMode = result->getInt(6);
		auto satellites = result->getInt(7);
		auto hdop = result->getDouble(8);
		auto haccuracy = result->getDouble(9);
		auto distance = result->getDouble(10);
		auto fom = result->getDouble(11);
		auto velocityValid = result->getBoolean(12);
		auto created = string(result->getString(13));

		return unique_ptr<Status>(new Status(created, latitude, longitude, heading, depth, temperature, driveMode, satellites, hdop, haccuracy, distance, fom, velocityValid, string()));
	}

	return unique_ptr<Status>(nullptr);
}


/**
 * Retrieve the statuses by track name
 * @param trackName The name of the track that we are retrieving for
 * @param output The list of statuses that we are outputting from the system
 */
void Repository::GetStatuses(const string& trackName, vector<Status *>& output) 
{
	auto query = stringstream();
	query << "SELECT "
		  << "latitude, "
		  << "longitude, "
		  << "heading, "
		  << "depth, "
		  << "temperature, "
		  << "drive_mode, "
		  << "gps_satellites, "
		  << "gps_hdop, "
		  << "gps_haccuracy, "
		  << "dvl_distance, "
		  << "dvl_fom, "
		  << "dvl_velocity_valid, "
		  << "created_at "
		  << "FROM status WHERE track_name=\'" << trackName << "\' ORDER BY created_at ASC";

	auto statement = unique_ptr<sql::Statement>(_connection->createStatement());
	auto result = statement->executeQuery(query.str());

	while (result->next()) 
	{
		auto latitude = result->getDouble(2);
		auto longitude = result->getDouble(3);
		auto heading = result->getDouble(4);
		auto depth = result->getDouble(5);
		auto temperature = result->getDouble(6);
		auto driveMode = result->getInt(7);
		auto satellites = result->getInt(8);
		auto hdop = result->getDouble(9);
		auto haccuracy = result->getDouble(10);
		auto distance = result->getDouble(11);
		auto fom = result->getDouble(12);
		auto velocityValid = result->getBoolean(13);
		auto created = string(result->getString(14));

		auto status = new Status(created, latitude, longitude, heading, depth, temperature, driveMode, satellites, hdop, haccuracy, distance, fom, velocityValid, trackName);
		output.push_back(status);
	}
}

//--------------------------------------------------
// Clear Table
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

//--------------------------------------------------
// Settings
//--------------------------------------------------

/**
 * Add the functionality to set the associated field
 * @param field The field that we are setting
 * @param value The value that we are setting the field
 */
void Repository::SetField(Field field, const string& value) 
{
	// Get the field name
	auto fieldName = GetFieldName(field);

	// Build the query
	auto query = "UPDATE settings SET field_value = ? WHERE field_name = ?;";
	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
	statement->setString(1, value);
	statement->setString(2, fieldName);

	// Execute the query
	statement->executeQuery();
}

/**
 * Get the associated field value
 * @param field The field that we are getting the value for
 */
string Repository::GetField(Field field) 
{
	// Get the field name
	auto fieldName = GetFieldName(field);

	// Build the query
	auto query = "SELECT field_value FROM settings WHERE field_name = ?";
	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
	statement->setString(1, fieldName);

	// Execute and get the response
	auto result = statement->executeQuery();

	if (result->next()) return result->getString(1).c_str();
	else throw runtime_error("Unable to find field: " + fieldName);
}

/**
 * Add the functionality to retrieve the associated field name
 * @param field The field that we want the value for
 */
string Repository::GetFieldName(Field field) 
{
	switch (field)
	{
		case Field::LOGGER_STATE: return "LOGGER_STATE";
		case Field::RATE: return "RATE";
		case Field::CURRENT_TRACK: return "CURRENT_TRACK";
	}

	throw runtime_error("Unknown field type");
}
