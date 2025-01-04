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
	auto query = "INSERT INTO status (latitude, longitude, heading, rov_depth, dvl_altitude, temperature, drive_mode, gps_sat_count, gps_hdop, gps_haccuracy, dvl_velocity_valid, track_name) "
				 "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));

	statement->setDouble(1, status->GetLatitude());
	statement->setDouble(2, status->GetLongitude());
	statement->setDouble(3, status->GetHeading());
	statement->setDouble(4, status->GetDepth());
	statement->setDouble(5, status->GetAltitude());
	statement->setDouble(6, status->GetTemperature());
	statement->setString(7, status->GetMode());
	statement->setInt(8, status->GetSatelliteCount());
	statement->setDouble(9, status->GetHdop());
	statement->setDouble(10, status->GetHaccuracy());
	statement->setBoolean(11, status->GetVelocityValid());
	statement->setString(12, status->GetTrackName());

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
	auto query = "SELECT id, latitude, longitude, heading, rov_depth, dvl_altitude, temperature, drive_mode, gps_sat_count, gps_hdop, gps_haccuracy, dvl_velocity_valid, created_at "
				 "FROM status ORDER BY created_at DESC LIMIT 1;";

	auto statement = unique_ptr<sql::Statement>(_connection->createStatement());
	auto result = statement->executeQuery(query);

	if (result->next()) 
	{
		auto latitude = result->getDouble(2);
		auto longitude = result->getDouble(3);
		auto heading = result->getDouble(4);
		auto depth = result->getDouble(5);
		auto altitude = result->getDouble(6);
		auto temperature = result->getDouble(7);
		auto mode = string(result->getString(8).c_str());
		auto satCount = result->getInt(9);
		auto hdop = result->getDouble(10);
		auto haccuracy = result->getDouble(11);
		auto velocityValid = result->getBoolean(12);
		auto created = string(result->getString(13));

		return unique_ptr<Status>(new Status(created, latitude, longitude, heading, depth, altitude, temperature, mode, satCount, hdop, haccuracy, velocityValid, string()));
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
	auto query = "SELECT id, latitude, longitude, heading, rov_depth, dvl_altitude, temperature, drive_mode, gps_sat_count, gps_hdop, gps_haccuracy, dvl_velocity_valid, created_at, "
	             "TIME_TO_SEC(ABS(TIMEDIFF(?,created_at))) as time_diff "
				 "FROM status ORDER BY time_diff LIMIT 1;";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
	statement->setString(1, time);

	auto result = statement->executeQuery();

	if (result->next()) 
	{
		auto latitude = result->getDouble(2);
		auto longitude = result->getDouble(3);
		auto heading = result->getDouble(4);
		auto depth = result->getDouble(5);
		auto altitude = result->getDouble(6);
		auto temperature = result->getDouble(7);
		auto mode = string(result->getString(8).c_str());
		auto satCount = result->getInt(9);
		auto hdop = result->getDouble(10);
		auto haccuracy = result->getDouble(11);
		auto velocityValid = result->getBoolean(12);
		auto created = string(result->getString(13));
		auto timeDiff = result->getFloat(14);

		cout << "Time difference: " << timeDiff << endl;

		return unique_ptr<Status>(new Status(created, latitude, longitude, heading, depth, altitude, temperature, mode, satCount, hdop, haccuracy, velocityValid, string()));
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

/**
 * Retrieve the statuses between a given range
 * @param start The start date that we are getting
 * @param end The end date that we are getting
 * @param output The output set of statuses
 */
void Repository::GetStatuses(const string& start, const string& end, vector<Status *>& output) 
{
	auto query = stringstream();
	query << "SELECT id, latitude, longitude, heading, rov_depth, dvl_altitude, temperature, drive_mode, gps_sat_count, gps_hdop, gps_haccuracy, dvl_velocity_valid, created_at ";
	query << "FROM status WHERE created_at BETWEEN ? AND ? ORDER BY created_at DESC";

	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query.str()));
	statement->setString(1, start);
	statement->setString(2, end);
	auto result = statement->executeQuery();

	while (result->next()) 
	{
		auto latitude = result->getDouble(2);
		auto longitude = result->getDouble(3);
		auto heading = result->getDouble(4);
		auto depth = result->getDouble(5);
		auto altitude = result->getDouble(6);
		auto temperature = result->getDouble(7);
		auto mode = string(result->getString(8).c_str());
		auto satCount = result->getInt(9);
		auto hdop = result->getDouble(10);
		auto haccuracy = result->getDouble(11);
		auto velocityValid = result->getBoolean(12);
		auto created = string(result->getString(13));

		auto status = new Status(created, latitude, longitude, heading, depth, altitude, temperature, mode, satCount, hdop, haccuracy, velocityValid, string());
		output.push_back(status);
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
	query << "SELECT id, latitude, longitude, heading, rov_depth, dvl_altitude, temperature, drive_mode, gps_sat_count, gps_hdop, gps_haccuracy, dvl_velocity_valid, created_at ";
	query << "FROM status WHERE track_name=\'" << trackName << "\' ORDER BY created_at DESC";

	auto statement = unique_ptr<sql::Statement>(_connection->createStatement());
	auto result = statement->executeQuery(query.str());

	while (result->next()) 
	{
		auto latitude = result->getDouble(2);
		auto longitude = result->getDouble(3);
		auto heading = result->getDouble(4);
		auto depth = result->getDouble(5);
		auto altitude = result->getDouble(6);
		auto temperature = result->getDouble(7);
		auto mode = string(result->getString(8).c_str());
		auto satCount = result->getInt(9);
		auto hdop = result->getDouble(10);
		auto haccuracy = result->getDouble(11);
		auto velocityValid = result->getBoolean(12);
		auto created = string(result->getString(13));

		auto status = new Status(created, latitude, longitude, heading, depth, altitude, temperature, mode, satCount, hdop, haccuracy, velocityValid, string());
		output.push_back(status);
	}
}


/**
 * Retrieve all tracks
 */
vector<string> Repository::GetTracks()
{
	vector<string> tracks;
	auto query = stringstream();
	query << "SELECT track_name FROM status GROUP BY track_name ORDER BY MAX(created_at) DESC";

	auto statement = unique_ptr<sql::Statement>(_connection->createStatement());
	auto result = statement->executeQuery(query.str());

	while (result->next()) {
		tracks.push_back(result->getString(1).c_str());
	}

	return tracks;
}


size_t Repository::GetTrackDataSize(const string& trackName)
{
    auto query = stringstream();
	query << "SELECT * FROM status WHERE track_name = ?";
	auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query.str()));
	statement->setString(1, trackName);

	auto result = statement->executeQuery();
	size_t totalSize = 0;

	while (result->next()) {
		auto metaData = result->getMetaData();
		int columnCount = metaData->getColumnCount();

		for (int i = 1; i <= columnCount; ++i) {
			int columnType = metaData->getColumnType(i);
			switch (columnType) {
				case sql::DataType::DOUBLE:
					totalSize += sizeof(result->getDouble(i));
					break;
				case sql::DataType::INTEGER:
					totalSize += sizeof(result->getInt(i));
					break;
				case sql::DataType::VARCHAR:
				case sql::DataType::CHAR:
					totalSize += result->getString(i).length() * sizeof(char);
					break;
				case sql::DataType::BOOLEAN:
					totalSize += sizeof(result->getBoolean(i));
					break;
				case sql::DataType::SMALLINT:
				case 4: // Explicitly handle type ID 4 for SMALLINT
					totalSize += sizeof(result->getInt(i)); // Assuming getInt is correct for SMALLINT
					break;
				case sql::DataType::TIMESTAMP:
					totalSize += sizeof(result->getString(i));
					break;
				default:
					cout << "Unhandled data type: " << metaData->getColumnTypeName(i) << " (Type ID: " << columnType << ")" << endl;
			}
		}
	}
    return totalSize;
}
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

string Repository::ExecuteQuery(const string& query, const string& trackName)
{
    auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query));
    statement->setString(1, trackName);

    auto result = statement->executeQuery();
    if (result->next()) {
        return result->getString(1).c_str();
    } else {
        throw runtime_error("No entries found for track: " + trackName);
    }
}

string Repository::GetEarliestEntryDate(const string& trackName, const string& field)
{
    auto query = stringstream();
    query << "SELECT DATE(" << field << ") FROM status WHERE track_name = ? ORDER BY " << field << " ASC LIMIT 1";
    return ExecuteQuery(query.str(), trackName);
}

string Repository::GetEarliestEntryTime(const string& trackName, const string& field)
{
    auto query = stringstream();
    query << "SELECT TIME(" << field << ") FROM status WHERE track_name = ? ORDER BY " << field << " ASC LIMIT 1";
    return ExecuteQuery(query.str(), trackName);
}

string Repository::GetLatestEntryDate(const string& trackName, const string& field)
{
    auto query = stringstream();
    query << "SELECT DATE(" << field << ") FROM status WHERE track_name = ? ORDER BY " << field << " DESC LIMIT 1";
    return ExecuteQuery(query.str(), trackName);
}

string Repository::GetLatestEntryTime(const string& trackName, const string& field)
{
    auto query = stringstream();
    query << "SELECT TIME(" << field << ") FROM status WHERE track_name = ? ORDER BY " << field << " DESC LIMIT 1";
    return ExecuteQuery(query.str(), trackName);
}





void Repository::DeleteTrack(const string& trackName)
{
    auto query = stringstream();
    query << "DELETE FROM status WHERE track_name = ?";
    auto statement = unique_ptr<sql::PreparedStatement>(_connection->prepareStatement(query.str()));
    statement->setString(1, trackName);
    statement->executeQuery();
}
