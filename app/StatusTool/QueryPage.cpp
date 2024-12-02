//--------------------------------------------------
// Implementation of class QueryPage
//
// @author: Wild Boar
//
// @date: 2024-10-18
//--------------------------------------------------

#include "QueryPage.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructors and Terminators
//--------------------------------------------------

/**
 * @brief Custom Constructor
 * @param repo A link to the repository
 * @param parameters The input parameters of the application
 */
QueryPage::QueryPage(Repository * repo, unordered_map<string, string>& parameters)  : _repo(repo), _fields(parameters)
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
string QueryPage::Render()
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
 * @param writer The element that we are rendering to
 */
void QueryPage::RenderForm(ostream& writer)
{
	// Defines a form
	writer << "<form action=\"\\query\">";

	// Render Start Time
	auto start = NVLib::StringUtils::GetDateTimeString();
	writer << "<label=for\"status\">Start Time:</label><br>";
	writer << "<input type=\"datetime-local\" name=\"start\" id=\"start\" value=\"" << start << "\" step=\"any\" /><br>";

	// Space
	writer << "<br>";

	// Render End Time
	auto end = NVLib::StringUtils::GetDateTimeString();
	writer << "<label=for\"status\">End Time:</label><br>";
	writer << "<input type=\"datetime-local\" name=\"end\" id=\"end\"  value=\"" << end << "\" step=\"any\" /><br>";

	// Add a submit button
	writer << "<br><input type=\"submit\" id=\"submit\" name=\"submit\" value=\"Submit\"><br>";

	// End the form
	writer << "</form>";
}

/**
 * @brief Render the response after an update
 * @param writer The element that we are rendering to
 */
void QueryPage::RenderResponse(ostream& writer)
{
	auto statuses = vector<Status *>();
	_repo->GetStatuses(_fields["start"], _fields["end"], statuses);

	auto maker = NVL_App::GPXMaker(statuses);

	writer << "<pre lang=\"xml\">" << maker.RenderXML() << "</pre>"; 

	//for (auto status : statuses) delete status;
}

//--------------------------------------------------
// Render Response
//--------------------------------------------------

/**
 * Render the header stuff
 * @param writer The writer that we are rendering to
 */
void QueryPage::RenderHeader(ostream& writer) 
{
	writer << "<head>";
  	writer << "<meta charset=\"utf-8\">";
  	writer << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  	writer << "<title>GPX Logger</title>";
  	writer << "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css\">";
	writer << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js\"></script>";
	writer << "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js\"></script>";
	writer << "</head>";
	writer << "<body>";
  	writer << "<h1>Settings</h1>";

	writer << "<body>";
  	writer << "<nav class=\"navbar navbar-default\">";
    writer << "<div class=\"container-fluid\">";
    writer << "<div class=\"navbar-header\">";
    writer << "<a class=\"navbar-brand\" href=\"#\">GPX Plugin</a>";
    writer << "</div>";
    writer << "<ul class=\"nav navbar-nav\">";
	writer << "<li class=\"active\"><a href=\"#\">Home</a></li>";
	writer << "<li><a href=\"\\status\">Status</a></li>";
	writer << "<li><a href=\"\\query\">Query</a></li>";
	writer << "<li><a href=\"\\settings\">Settings</a></li>";
	writer << "</ul>";     
    writer << "</div>";
  	writer << "</nav>";
}

/**
 * Render the footer stuff
 * @param writer The writer that we are rendering to
 */
void QueryPage::RenderFooter(ostream& writer) 
{
	writer << "<p>Go <a href=\"\\\">back</a>.</p>";
	writer << "</body></html>" << endl;
}
