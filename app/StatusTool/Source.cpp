//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2024-07-26
//--------------------------------------------------

#include <iostream>
using namespace std;

#include <crow.h>

//--------------------------------------------------
// Function Prototypes
//--------------------------------------------------
void Run();

//--------------------------------------------------
// Execution Logic
//--------------------------------------------------

/**
 * Main entry point into the application
 */
void Run() 
{
     crow::SimpleApp app;
    
    // Index page
    CROW_ROUTE(app, "/")([]()
    {
        auto page = crow::mustache::load_text("main.html");
        return page;
    });

    // CSS
    CROW_ROUTE(app, "/styles.css")([]()
    {
        auto stylesheet = crow::mustache::load_text("styles.css");
        return stylesheet;
    });

    //set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
}

//--------------------------------------------------
// Entry Point
//--------------------------------------------------

/**
 * Main Method
 * @return SUCCESS and FAILURE
 */
int main(int, char **) 
{
    try
    {
        Run();
    }
    catch (runtime_error exception)
    {
        cerr << "Error: " << exception.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        cerr << "Error: " << exception << endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
