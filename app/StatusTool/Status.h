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
		double _posCertainity;
		bool _velocityValid;
		double _FOM;
		string _trackName;
	public:
		Status(double latitude, double longitude, double heading, double depth, double altitude, double temperature, const string& mode, int satelliteCount, double posCertainity, bool velocityValid, double FOM, const string& trackName) :
			_timeStamp(string()), _latitude(latitude), _longitude(longitude), _heading(heading), _depth(depth), _altitude(altitude), _temperature(temperature), _mode(mode), _satelliteCount(satelliteCount), _posCertainity(posCertainity), _velocityValid(velocityValid), _FOM(FOM), _trackName(trackName) {}

		Status(const string& timeStamp, double latitude, double longitude, double heading, double depth, double altitude, double temperature, const string& mode, int satelliteCount, double posCertainity, bool velocityValid, double FOM, const string& trackName) :
			_timeStamp(timeStamp), _latitude(latitude), _longitude(longitude), _heading(heading), _depth(depth), _altitude(altitude), _temperature(temperature), _mode(mode), _satelliteCount(satelliteCount), _posCertainity(posCertainity), _velocityValid(velocityValid), _FOM(FOM), _trackName(trackName) {}

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
			_posCertainity = status->_posCertainity;
			_velocityValid = status->_velocityValid;
			_FOM = status->_FOM;
			_trackName = status->_trackName;
		}

		inline string& GetTimeStamp() { return _timeStamp; }
		inline double& GetLatitude() { return _latitude; }
		inline double& GetLongitude() { return _longitude; }
		inline double& GetHeading() { return _heading; }
		inline double& GetDepth() { return _depth; }
		inline double& GetAltitude() { return _altitude; }
		inline double& GetTemperature() { return _temperature; }
		inline string& GetMode() { return _mode; }
		inline int& GetSatelliteCount() { return _satelliteCount; }
		inline double& GetPosCertainity() { return _posCertainity; }
		inline bool GetVelocityValid() { return _velocityValid; }
		inline double& GetFOM() { return _FOM; }
		inline string& GetTrackName() { return _trackName; }
	};
}
