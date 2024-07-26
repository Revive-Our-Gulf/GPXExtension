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
 * @return string Returns a string
 */
string GPXMaker::RenderXML()
{
	auto writer = stringstream();

	writer << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	writer << "<gpx />";
	writer << "</xml>";

	return writer.str();
}
