//--------------------------------------------------
// Implementation of class Settings
//
// @author: Wild Boar
//
// @date: 2024-10-15
//--------------------------------------------------

#include "Settings.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param repo A link to the repository
 * @param parameters The input parameters of the application
 */
Settings::Settings(Repository * repo, unordered_map<string, string>& parameters) : _repo(repo), _fields(parameters)
{
	// Extra implementation can go here
}

//--------------------------------------------------
// Render
//--------------------------------------------------

/**
 * @brief The main render logic
 * @return string Returns a string
 */
string Settings::Render()
{
	auto result = stringstream();

	RenderHeader(result);

	if (_fields.find("submit") == _fields.end()) RenderForm(result);
	else RenderResponse(result);

	RenderFooter(result);

	return result.str();
}

//--------------------------------------------------
// Render Form
//--------------------------------------------------

/**
 * @brief Render the form for the application
 * @param writer The form that we are rendering
 */
void Settings::RenderForm(ostream& writer)
{
	// Defines a form
	writer << "<form action=\"\\settings\">";

	// Render the Status
	auto status = _repo->GetField(Repository::Field::LOGGER_STATE);
	auto stoppedSelected = status == "STOPPED" ? "selected" : string();
	auto startedSelected = status == "STARTED" ? "selected" : string();
	writer << "<label=for\"status\">Set Status:</label><br>";
	writer << "<select name=\"status\" id=\"status\">";
	writer << "<option value=\"STOPPED\" " << stoppedSelected << ">STOPPED</option>";
	writer << "<option value=\"STARTED\" " << startedSelected << ">STARTED</option>";
	writer << "</select><br>";

	// Space
	writer << "<br>";

	// Render the interval timer
	auto rate = _repo->GetField(Repository::Field::RATE);
	writer << "<label=for\"interval\">Polling Interval:</label><br>";
	writer << "<input type=\"number\" id=\"interval\" name=\"interval\" value=\"" << rate << "\" min=\"500\" max=\"60000\"><br>";

	// Add a submit button
	writer << "<br><input type=\"submit\" id=\"submit\" name=\"submit\" value=\"Submit\"><br>";

	// End the form
	writer << "</form>";
}

//--------------------------------------------------
// Render Response
//--------------------------------------------------

/**
 * @brief Render the response after an update
 * @param writer The element that we are rendering to
 */
void Settings::RenderResponse(ostream& writer)
{
	try 
	{
		_repo->SetField(Repository::Field::LOGGER_STATE, _fields["status"]);
		_repo->SetField(Repository::Field::RATE, _fields["interval"]);
		writer << "<p><b>Update performed successfully<b></p>";
	}
	catch(...) 
	{
		writer << "<p><b>ERROR: Update failed!<b></p>";
	}
}

//--------------------------------------------------
// Render Response
//--------------------------------------------------

/**
 * Render the header stuff
 * @param writer The writer that we are rendering to
 */
void Settings::RenderHeader(ostream& writer) 
{
	writer << "<head>";
  	writer << "<meta charset=\"utf-8\">";
  	writer << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  	writer << "<title>GPX Logger</title>";
  	writer << "<link rel=\"stylesheet\" href=\"styles.css\">";
	writer << "</head>";
	writer << "<body>";
  	writer << "<h1>Settings</h1>";
}

/**
 * Render the footer stuff
 * @param writer The writer that we are rendering to
 */
void Settings::RenderFooter(ostream& writer) 
{
	writer << "<p>Go <a href=\"\\\">back</a>.</p>";
	writer << "</body></html>" << endl;
}
