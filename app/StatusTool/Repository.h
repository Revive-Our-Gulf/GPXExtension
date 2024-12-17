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
		Repository(const string& ipaddress, const string& database);
		~Repository();

		void AddStatus(Status * status);
		unique_ptr<Status> GetLastStatus();
		unique_ptr<Status> GetClosestStatus(const string& time);
		void GetStatuses(int limit, vector<Status *>& output);
		void GetStatuses(const string& start, const string& end, vector<Status *>& output);
		void ClearTable();

		void SetField(Field field, const string& value);
		string GetField(Field field);

		inline sql::Connection * GetConnection() { return _connection; }
		inline string& GetDatabase() { return _database; }
	private:
		string GetFieldName(Field field);
	};
}
