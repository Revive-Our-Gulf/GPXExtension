//--------------------------------------------------
// Implementation code for the Engine
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#include "Engine.h"
using namespace NVL_App;

//--------------------------------------------------
// Constructor and Terminator
//--------------------------------------------------

/**
 * Main Constructor
 * @param logger The logger that we are using for the system
 * @param parameters The input parameters
 */
Engine::Engine(NVLib::Logger* logger, NVLib::Parameters* parameters) 
{
    _logger = logger; _parameters = parameters;

    _interval = ArgUtils::GetInteger(parameters, "interval");
    _logger->Log(1, "Recording at interval: %i", _interval);
}

/**
 * Main Terminator 
 */
Engine::~Engine() 
{
    delete _parameters; 
}

//--------------------------------------------------
// Execution Entry Point
//--------------------------------------------------

/**
 * Entry point function
 */
void Engine::Run()
{
    _logger->Log(1, "Connecting to the database");
    auto repository = Repository("BlueROV");

    _logger->Log(1, "Setting up ROV communicator");
    auto communicator = RovCommsFactory::GetCommunicator("random");

    _logger->Log(1, "Starting control");
    while(true) 
    {
        auto status = communicator->GetCurrentStatus();
        repository.AddStatus(status.get());
        this_thread::sleep_for(std::chrono::milliseconds(_interval));
    }
}
