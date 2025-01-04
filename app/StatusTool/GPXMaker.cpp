//--------------------------------------------------
// Implementation of class GPXMaker
//
// @author: Wild Boar
//
// @date: 2024-07-26
//--------------------------------------------------

#include "GPXMaker.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Initializer Constructor
 * @param statuses Initialize variable <statuses>
 */
GPXMaker::GPXMaker(vector<Status *>& statuses)
{
	_statuses = statuses;
}

/**
 * @brief Main Terminator
 */
GPXMaker::~GPXMaker()
{
	for (auto& status : _statuses) delete status;
}

//--------------------------------------------------
// Render
//--------------------------------------------------

/**
 * @brief Render the GPX xml
 * @param trackName The name of the track that is being rendered
 * @return string Returns a string
 */
string GPXMaker::RenderXML(const string& trackName)
{
	auto writer = stringstream();

	writer << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

	if (_statuses.size() == 0) return "<error>No data found!</error>";

	RenderStartTag(writer);
	RenderMeta(writer);

	writer << "<trk>";
	RenderJourney(writer, trackName);

	writer << "<trkseg>";
	for (auto status : _statuses) RenderStep(writer, status);
	writer << "</trkseg>";
	
	writer << "</trk>";

	writer << "</gpx>";

	return writer.str();
}

//--------------------------------------------------
// Helpers
//--------------------------------------------------

/**
 * Add the functionality to render the start tag
 * @param writer The writer that we are using
 */
void GPXMaker::RenderStartTag(ostream& writer) 
{
	writer << "<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" ";
	writer << "xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" ";
	writer << "xmlns:bluerov2=\"http://bad-elf.com/xmlschemas/GpxExtensionsV1\" ";
	writer << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
	writer << "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www8.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://bad-elf.com/xmlschemas http://bad-elf.com/xmlschemas/GpxExtensionsV1.xsd\" ";
	writer << "version=\"1.0\" creator=\"Revive Our Gulf\">";
}

/**
 * Render associated meta data
 * @param writer The writer that we are using
 */
void GPXMaker::RenderMeta(ostream& writer) 
{
	auto timeString = NVLib::StringUtils::GetDateTimeString();

	writer << "<metadata>";
    writer << "<link href=\"http://www.garmin.com\">";
	writer << "<text>Revive Our Gulf - BlueROV2 GPX plugin</text> ";
    writer << "</link>";
    writer << "<time>" << timeString << "</time>";
    writer << "<extensions>";
	writer << "<bluerov2:gpxDataSource>ROG GPX plugin for BlueOS</bluerov2:gpxDataSource>";
    writer << "<bluerov2:blueOSVersion>1.0</bluerov2:blueOSVersion>";
    writer << "<bluerov2:pluginVersion>1.0</bluerov2:pluginVersion>";
    writer << "</extensions>";
   	writer << "</metadata>";
}

/**
 * Render the journey details
 * @param writer The writer that we are rendering
 */
void GPXMaker::RenderJourney(ostream& writer, const string& trackName) 
{
	writer << "<name>" << trackName << "</name>";
	writer << "<extensions>";
    writer << "<gpxx:TrackExtension>";
	writer << "<gpxx:DisplayColor>Cyan</gpxx:DisplayColor>";
    writer << "</gpxx:TrackExtension>";
    writer << "</extensions>";
}

/**
 * Render a step in the journey
 * @param writer The writer that we are using to render the images
 * @param status The status that we are using as a step
 */
void GPXMaker::RenderStep(ostream& writer, Status * status) 
{
	writer << "<trkpt lat=\"" << fixed << setprecision(8) << status->GetLatitude() << "\" lon=\"" << status->GetLongitude() << defaultfloat << "\">";
	writer << "<ele>" << status->GetAltitude() << "</ele>";
	writer << "<time>" << status->GetTimeStamp() << "</time>";
	writer << "<extensions>";
	writer << "<bluerov2:heading>" << status->GetHeading() << "</bluerov2:heading>";
	writer << "<bluerov2:depth>" << status->GetDepth() << "</bluerov2:depth>";
	writer << "<bluerov2:altitude>" << status->GetAltitude() << "</bluerov2:altitude>";
	writer << "<bluerov2:driveMode>" << status->GetMode() << "</bluerov2:driveMode>";
	writer << "<bluerov2:waterTemp>" << status->GetTemperature() << "</bluerov2:waterTemp>";
	writer << "<bluerov2:gpsSatellites>" << status->GetSatelliteCount() << "</bluerov2:gpsSatellites>";
	writer << "<bluerov2:gpsHDOP>" << status->GetHdop() << "</bluerov2:gpsHDOP>";
	writer << "<bluerov2:gpsHAccuracy>" << status->GetHaccuracy() << "</bluerov2:gpsHAccuracy>";
	writer << "</extensions>";
	writer << "</trkpt>";
}