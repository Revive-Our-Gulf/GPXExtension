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

    _logger->Log(1, "Getting Parameters");
    _ip = ArgUtils::GetString(parameters, "database");
    _factoryType = ArgUtils::GetString(parameters, "factory");

    _logger->Log(1, "Input [database]: %s", _ip.c_str());
    _logger->Log(1, "Input [factory]: %s", _factoryType.c_str());
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
    auto repository = Repository(_ip, "BlueROV");

    _logger->Log(1, "Setting up ROV communicator");
    auto communicator = RovCommsFactory::GetCommunicator(_factoryType, &repository);

    _logger->Log(1, "Starting control");
    while(true) 
    {
        auto status = repository.GetField(Repository::Field::LOGGER_STATE);
        auto interval = NVLib::StringUtils::String2Int(repository.GetField(Repository::Field::RATE));
    
        if (status != "STOPPED") 
        {
            auto timeString = NVLib::StringUtils::GetDateTimeString();
            _logger->Log(1, "Adding status entry: %s", timeString.c_str());
            auto status = communicator->GetCurrentStatus();
            auto position_counter = status->GetPositionCounter();
            auto trackName = repository.GetField(Repository::Field::CURRENT_TRACK);

            auto lastPositionCounter = repository.GetLastPositionCounter(trackName);
            if (position_counter > lastPositionCounter) 
            {
                repository.AddStatus(status.get());
            }            
        }

        this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
}
