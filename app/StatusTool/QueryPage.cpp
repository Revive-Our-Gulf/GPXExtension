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
 * @param parameters The input parameters of the application
 */
QueryPage::QueryPage(unordered_map<string, string>& parameters)  : _fields(parameters)
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

	writer << "<div class=\"p-4\">";

	writer << "<div class=\"alert alert-info\">Select the time range for the GPX</div>";

	// Render Start Time
	auto start = NVLib::StringUtils::GetDateTimeString();
	writer << "<div class=\"form-group\">";
	writer << "<label=for\"status\" class=\"form-label\">Start Time:</label>";
	writer << "<input type=\"datetime-local\"  class=\"form-control\" name=\"start\" id=\"start\" value=\"" << start << "\" step=\"any\" />";
	writer << "</div>";

	// Render End Time
	auto end = NVLib::StringUtils::GetDateTimeString();
	writer << "<div class=\"form-group\">";
	writer << "<label=for\"status\" class=\"form-label\">End Time:</label>";
	writer << "<input type=\"datetime-local\" class=\"form-control\" name=\"end\" id=\"end\" value=\"" << end << "\" step=\"any\" />";
	writer << "</div>";

	// Add a submit button
	writer << "<br><input type=\"submit\" id=\"submit\" name=\"submit\" value=\"Submit\"  class=\"btn btn-primary\">";

	// End the form
	writer << "</div></form>";
}

/**
 * @brief Render the response after an update
 * @param writer The element that we are rendering to
 */
void QueryPage::RenderResponse(ostream& writer)
{
	writer << "<div class=\"p-4\">";

	writer << "<div class=\"alert alert-info\">GPX Response Received</div>";

	writer << "<div class=\"pt-3 pb-4\">";
	writer << "<a href=\"\\gpx?start=" << _fields["start"] << "&end="<< _fields["end"] << "\" download=\"gpx.xml\" class=\"btn btn-info\"/>Download</a>";
	writer << "</div>";

	writer << "<p class=\"text-muted\">GPX preview</p>";
	writer << "<div class=\"bg-light border border-dark pt-2\">";
	writer << "<pre><code class=\"p-2\" id=\"xml\"></code></pre>"; 
	writer << "</div></div>";

	writer << "<script>";

	writer << "function TextReceived() {";
	writer << "var xml = this.responseText;";
	writer << "var box = document.getElementById('xml');";
	writer << "box.innerText = new XmlBeautify().beautify(xml, {";
	writer << "indent: \"   \", useSelfClosingElement: true });";
	writer << "} ";

	writer << "var request = new XMLHttpRequest();";
	writer << "request.addEventListener(\"load\", TextReceived);";
	writer << "request.open(\"GET\", \"\\gpx?start=" << _fields["start"] << "&end="<< _fields["end"] << "\");";
	writer << "request.send();";
	writer << "</script>";
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
  	writer << "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/css/bootstrap.min.css\">";
  	writer << "<script src=\"https://cdn.jsdelivr.net/npm/jquery@3.7.1/dist/jquery.slim.min.js\"></script>";
  	writer << "<script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js\"></script>";
  	writer << "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/js/bootstrap.bundle.min.js\"></script>";
	writer << "<script src=\"https://cdn.jsdelivr.net/npm/xml-beautify@1.2.3/dist/XmlBeautify.js\"></script>";
	writer << "</head>";

	writer << "<body>";

  	writer << "<nav class=\"navbar  navbar-expand-sm bg-dark navbar-dark\">";
    writer << "<div class=\"navbar-header\">";
    writer << "<a class=\"navbar-brand\" href=\"#\">GPX Plugin</a>";
    writer << "</div>";
    writer << "<ul class=\"navbar-nav\">";
	writer << "<li><a class=\"nav-link\" href=\".\">Home</a></li>";
	writer << "<li class=\"nav-item active\"><a class=\"nav-link\" href=\"#\">Query</a></li>";
	writer << "<li><a class=\"nav-link\" href=\"\\settings\">Settings</a></li>";
	writer << "</ul>";     
  	writer << "</nav>";

	writer << "<div class=\"pl-5\">";
}

/**
 * Render the footer stuff
 * @param writer The writer that we are rendering to
 */
void QueryPage::RenderFooter(ostream& writer) 
{
	if (_fields.find("submit") != _fields.end()) writer << "<div class=\"pl-4 pt-4\">Go <a href=\"\\query\">back</a>.</div>";
	writer << "</div></body></html>" << endl;
}
