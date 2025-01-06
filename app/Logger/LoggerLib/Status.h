//--------------------------------------------------
// An entity containing the status that we dealing with
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#pragma once

#include <iostream>
#include <string>
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
		double _temperature;
		int _driveMode;
		int _satellites;
		double _hdop;
		double _haccuracy;
		double _dvl_distance;
		double _dvl_fom;
		bool _dvl_velocity_valid;
		string _trackName;
		
	public:
		Status(
			double latitude,
			double longitude,
			double heading,
			double depth,
			double temperature,
			int driveMode,
			int satellites,
			double hdop,
			double haccuracy,
			double dvl_distance,
			double dvl_fom,
			bool dvl_velocity_valid,
			const string& trackName) :
			_timeStamp(string()), 
			_latitude(latitude), 
			_longitude(longitude), 
			_heading(heading), 
			_depth(depth), 
			_temperature(temperature), 
			_driveMode(driveMode), 
			_satellites(satellites), 
			_hdop(hdop), 
			_haccuracy(haccuracy), 
			_dvl_distance(dvl_distance),
			_dvl_fom(dvl_fom),
			_dvl_velocity_valid(dvl_velocity_valid),
			_trackName(trackName) 
		{}

		Status(
			const string& timeStamp,
			double latitude,
			double longitude,
			double heading,
			double depth,
			double temperature,
			int driveMode,
			int satellites,
			double hdop,
			double haccuracy,
			double dvl_distance,
			double dvl_fom,
			bool dvl_velocity_valid,
			const string& trackName) :
			_timeStamp(timeStamp), 
			_latitude(latitude), 
			_longitude(longitude), 
			_heading(heading), 
			_depth(depth), 
			_temperature(temperature), 
			_driveMode(driveMode), 
			_satellites(satellites), 
			_hdop(hdop), 
			_haccuracy(haccuracy), 
			_dvl_distance(dvl_distance),
			_dvl_fom(dvl_fom),
			_dvl_velocity_valid(dvl_velocity_valid),
			_trackName(trackName) 
		{}

		inline const string& GetTimeStamp() const { return _timeStamp; }
		inline double GetLatitude() const { return _latitude; }
		inline double GetLongitude() const { return _longitude; }
		inline double GetHeading() const { return _heading; }
		inline double GetDepth() const { return _depth; }
		inline double GetTemperature() const { return _temperature; }
		inline int GetDriveMode() const { return _driveMode; }
		inline int GetSatellites() const { return _satellites; }
		inline double GetHdop() const { return _hdop; }
		inline double GetHaccuracy() const { return _haccuracy; }
		inline double GetDistance() const { return _dvl_distance; }
		inline double GetFom() const { return _dvl_fom; }
		inline bool GetVelocityValid() const { return _dvl_velocity_valid; }
		inline const string& GetTrackName() const { return _trackName; }
	};
}
