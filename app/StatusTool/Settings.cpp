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

	writer << "<div class=\"p-4\">";

	// Render the Status
	auto status = _repo->GetField(Repository::Field::LOGGER_STATE);
	auto stoppedSelected = status == "STOPPED" ? "selected" : string();
	auto startedSelected = status == "STARTED" ? "selected" : string();
	writer << "<div class=\"form-group\">";
	writer << "<label=for\"status\" class=\"form-label\">Set Status:</label>";
	writer << "<select name=\"status\" id=\"status\" class=\"form-control\">";
	writer << "<option value=\"STOPPED\" " << stoppedSelected << ">STOPPED</option>";
	writer << "<option value=\"STARTED\" " << startedSelected << ">STARTED</option>";
	writer << "</select>";
	writer << "</div>";

	// Render the interval timer
	auto rate = _repo->GetField(Repository::Field::RATE);
	writer << "<div class=\"form-group\">";
	writer << "<label=for\"interval\" class=\"form-label\">Polling Interval:</label>";
	writer << "<input type=\"number\" class=\"form-control\" id=\"interval\" name=\"interval\" value=\"" << rate << "\" min=\"500\" max=\"60000\">";
	writer << "</div>";

	// Add a submit button
	writer << "<input type=\"submit\" id=\"submit\" name=\"submit\" value=\"Submit\" class=\"btn btn-primary\">";

	writer << "</div>";

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
		writer << "<div class=\"alert alert-success m-4\"><b>Update performed successfully<b></div>";
	}
	catch(...) 
	{
		writer << "<div class=\"alert alert-danger m-4\"><b>ERROR: Update failed!<b></div>";
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
  	writer << "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/css/bootstrap.min.css\">";
  	writer << "<script src=\"https://cdn.jsdelivr.net/npm/jquery@3.7.1/dist/jquery.slim.min.js\"></script>";
  	writer << "<script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js\"></script>";
  	writer << "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/js/bootstrap.bundle.min.js\"></script>";
	writer << "</head>";

	writer << "<body>";

  	writer << "<nav class=\"navbar  navbar-expand-sm bg-dark navbar-dark\">";
    writer << "<div class=\"navbar-header\">";
    writer << "<a class=\"navbar-brand\" href=\"#\">GPX Plugin</a>";
    writer << "</div>";
    writer << "<ul class=\"navbar-nav\">";
	writer << "<li><a class=\"nav-link\" href=\".\">Home</a></li>";
	writer << "<li><a class=\"nav-link\" href=\"\\status\">Status</a></li>";
	writer << "<li><a class=\"nav-link\" href=\"\\query\">Query</a></li>";
	writer << "<li class=\"nav-item active\"><a class=\"nav-link\" href=\"#\">Settings</a></li>";
	writer << "</ul>";     
  	writer << "</nav>";

	writer << "<div class=\"pl-5\">";
}

/**
 * Render the footer stuff
 * @param writer The writer that we are rendering to
 */
void Settings::RenderFooter(ostream& writer) 
{
	// writer << "<div class=\"pt-4\">Go <a href=\"\\\">back</a>.</div>";
	writer << "</div></body></html>" << endl;
}
