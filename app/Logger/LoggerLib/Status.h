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

		inline string& GetTimeStamp() { return _timeStamp; }
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
