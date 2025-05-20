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
				 "position_counter, "
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
				 "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));

	statement->setDouble(1, status->GetLatitude());
	statement->setDouble(2, status->GetLongitude());
	statement->setInt(3, status->GetPositionCounter());
	statement->setDouble(4, status->GetHeading());
	statement->setDouble(5, status->GetDepth());
	statement->setDouble(6, status->GetTemperature());
	statement->setInt(7, status->GetDriveMode());
	statement->setInt(8, status->GetSatellites());
	statement->setDouble(9, status->GetHdop());
	statement->setDouble(10, status->GetHaccuracy());
	statement->setDouble(11, status->GetDistance());
	statement->setDouble(12, status->GetFom());
	statement->setBoolean(13, status->GetVelocityValid());
	statement->setString(14, status->GetTrackName());

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
				 "position_counter, "
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
		auto positionCounter = result->getInt(3);
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

		return unique_ptr<Status>(new Status(created, latitude, longitude, positionCounter, heading, depth, temperature, driveMode, satellites, hdop, haccuracy, distance, fom, velocityValid, string()));
	}

	return unique_ptr<Status>(nullptr);
}

/**
 * Retrieve the last position counter for the given track name
 * @param trackName The name of the track that we are retrieving for
 */
int Repository::GetLastPositionCounter(const string& trackName)
{
    try {
        auto query = "SELECT position_counter FROM status WHERE track_name = ? ORDER BY created_at DESC LIMIT 1";
        auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
        statement->setString(1, trackName);
        
        auto result = statement->executeQuery();
        
        if (result->next()) {
            return result->getInt(1);
        }
        
        return -1; // Return -1 if no records exist (ensures first record is always added)
    }
    catch (sql::SQLException &e) {
        return -1;
    }
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
		  << "position_counter, "
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
		auto latitude = result->getDouble(1);
		auto longitude = result->getDouble(2);
		auto positionCounter = result->getInt(3);
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

		auto status = new Status(created, latitude, longitude, positionCounter, heading, depth, temperature, driveMode, satellites, hdop, haccuracy, distance, fom, velocityValid, trackName);
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
