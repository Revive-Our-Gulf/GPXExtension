//--------------------------------------------------
// Implementation of class RovCommsFactory
//
// @author: Wild Boar
//
// @date: 2024-08-07
//--------------------------------------------------

#include "RovCommsFactory.h"
using namespace NVL_App;

//--------------------------------------------------
// Select
//--------------------------------------------------

/**
 * @brief Retrieve a communicator for the system
 * @param name The name of the communicator we are requesting
 * @param repository A handle to the repository that is being used
 * @return unique_ptr<RovCommsBase> Returns a unique_ptr<RovCommsBase>
 */
unique_ptr<RovCommsBase> RovCommsFactory::GetCommunicator(const string& name, Repository * repository)
{
	if (name == "random") return unique_ptr<RovCommsBase>(new RandomRovComms(repository));
	else if (name == "file") return unique_ptr<RovCommsBase>(new FileRovComms(repository));
	else if (name == "http") return unique_ptr<RovCommsBase>(new HttpRovComms(repository));
	else throw runtime_error("Unknown Communicator type: " + name);
}