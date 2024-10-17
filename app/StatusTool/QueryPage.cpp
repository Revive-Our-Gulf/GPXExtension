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
	writer << "<label=for\"status\">Start Time:</label><br>";
	writer << "<input type=\"datetime-local\" name=\"start\" id=\"start\" /><br>";

	// Space
	writer << "<br>";

	// Render End Time
	writer << "<label=for\"status\">End Time:</label><br>";
	writer << "<input type=\"datetime-local\" name=\"end\" id=\"end\" /><br>";

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
	auto status = _repo->GetClosestStatus(_fields["start"]);
	auto statuses = vector<Status *> { new Status(status.get()) };

	auto maker = NVL_App::GPXMaker(statuses);

	writer << "<textarea rows=\"20\" cols=\"100\">" << maker.RenderXML() << "</textarea>"; 
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
  	writer << "<link rel=\"stylesheet\" href=\"styles.css\">";
	writer << "</head>";
	writer << "<body>";
  	writer << "<h1>Settings</h1>";
}

/**
 * Render the footer stuff
 * @param writer The writer that we are rendering to
 */
void QueryPage::RenderFooter(ostream& writer) 
{
	writer << "<p>Go <a href=\"\\\" target=\"_blank\">back</a>.</p>";
	writer << "</body></html>" << endl;
}
