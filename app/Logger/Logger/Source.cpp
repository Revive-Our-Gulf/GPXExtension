//--------------------------------------------------
// Startup code module
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#include "Engine.h"

//--------------------------------------------------
// Add a signal handler
//--------------------------------------------------

/**
 * Add signal handler logic
 * @param signum The signal code
 */
void SignalHandler( int signum ) 
{
   cout << "Interrupt signal (" << signum << ") received" << endl;
   exit(signum);  
}

//--------------------------------------------------
// Execution entry point
//--------------------------------------------------

/**
 * Main Method
 * @param argc The count of the incomming arguments
 * @param argv The number of incomming arguments
 */
int main(int argc, char ** argv) 
{
    auto logger = NVLib::Logger(2);
    logger.StartApplication();

    logger.Log(1, "Registering a signal handler");
    signal(SIGINT, SignalHandler);  

    try
    {
        auto parameters = NVL_App::ArgUtils::Load("Logger", argc, argv);
        NVL_App::Engine(&logger, parameters).Run();
    }
    catch (runtime_error exception)
    {
        logger.Log(1, "Error: %s", exception.what());
        exit(EXIT_FAILURE);
    }
    catch (string exception)
    {
        logger.Log(1, "Error: %s", exception.c_str());
        exit(EXIT_FAILURE);
    }

    logger.StopApplication();

    return EXIT_SUCCESS;
}
