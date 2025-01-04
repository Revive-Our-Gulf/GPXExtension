//--------------------------------------------------
// An entity containing the status that we dealing with
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

namespace NVL_App
{
	class Status
	{
	private:
		string _timeStamp;
		double _latitude;
		double _longitude;
		double _heading;
		double _depth;
		double _altitude;
		double _temperature;
		string _mode;
		int _satelliteCount;
		double _hdop;
		double _haccuracy;
		bool _velocityValid;
		string _trackName;
	public:
		Status(double latitude, double longitude, double heading, double depth, double altitude, double temperature, const string& mode, int satelliteCount, double hdop, double haccuracy, bool velocityValid, const string& trackName) :
			_timeStamp(string()), _latitude(latitude), _longitude(longitude), _heading(heading), _depth(depth), _altitude(altitude), _temperature(temperature), _mode(mode), _satelliteCount(satelliteCount), _hdop(hdop), _haccuracy(haccuracy), _velocityValid(velocityValid), _trackName(trackName) {}

		Status(const string& timeStamp, double latitude, double longitude, double heading, double depth, double altitude, double temperature, const string& mode, int satelliteCount, double hdop, double haccuracy, bool velocityValid, const string& trackName) :
			_timeStamp(timeStamp), _latitude(latitude), _longitude(longitude), _heading(heading), _depth(depth), _altitude(altitude), _temperature(temperature), _mode(mode), _satelliteCount(satelliteCount), _hdop(hdop), _haccuracy(haccuracy), _velocityValid(velocityValid), _trackName(trackName) {}

		Status(Status * status) 
		{
			_timeStamp = status->_timeStamp;
			_latitude = status->_latitude;
			_longitude = status->_longitude;
			_heading = status->_heading;
			_depth = status->_depth;
			_altitude = status->_altitude;
			_temperature = status->_temperature;
			_mode = status->_mode;
			_satelliteCount = status->_satelliteCount;
			_hdop = status->_hdop;
			_haccuracy = status->_haccuracy;
			_velocityValid = status->_velocityValid;
			_trackName = status->_trackName;
		}

		inline string GetTimeStamp() {
			struct tm tm;
			strptime(_timeStamp.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
			time_t t = mktime(&tm);

			char tzBuffer[6];
			strftime(tzBuffer, sizeof(tzBuffer), "%z", localtime(&t));

			std::string tzString(tzBuffer);
			std::string tzHour = tzString.substr(0, 3);

			char buffer[30];
			strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", localtime(&t));

			std::ostringstream out;
			out << buffer << _timeStamp.substr(19) << tzHour;

			return out.str();
		}

		inline double& GetLatitude() { return _latitude; }
		inline double& GetLongitude() { return _longitude; }
		inline double& GetHeading() { return _heading; }
		inline double& GetDepth() { return _depth; }
		inline double& GetAltitude() { return _altitude; }
		inline double& GetTemperature() { return _temperature; }
		inline string& GetMode() { return _mode; }
		inline int& GetSatelliteCount() { return _satelliteCount; }
		inline double& GetHdop() { return _hdop; }
		inline double& GetHaccuracy() { return _haccuracy; }
		inline bool GetVelocityValid() { return _velocityValid; }
		inline string& GetTrackName() { return _trackName; }
	};
}
