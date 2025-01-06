//--------------------------------------------------
// The repository for updating the storage
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#pragma once

#include <sstream>
#include <iostream>
using namespace std;

#include <mariadb/conncpp.hpp>
#include "Status.h"

namespace NVL_App
{
	class Repository
	{
	public:
		enum Field { LOGGER_STATE, RATE, CURRENT_TRACK };
	private:
		sql::Connection * _connection;
		string _database;
	public:
		Repository(const string& ipAddress, const string& database);
		~Repository();

		void AddStatus(Status * status);
		unique_ptr<Status> GetLastStatus();
		void ClearTable();

		void GetStatuses(const string& trackName, vector<Status *>& output);

		void SetField(Field field, const string& value);
		string GetField(Field field);

		inline sql::Connection * GetConnection() { return _connection; }
		inline string& GetDatabase() { return _database; }
	private:
		string GetFieldName(Field field);
	};
}
