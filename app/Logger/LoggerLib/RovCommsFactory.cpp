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
 * @return unique_ptr<RovCommsBase> Returns a unique_ptr<RovCommsBase>
 */
unique_ptr<RovCommsBase> RovCommsFactory::GetCommunicator(const string& name)
{
	if (name == "random") return unique_ptr<RovCommsBase>(new RandomRovComms());
	else if (name == "file") return unique_ptr<RovCommsBase>(new FileRovComms());
	else if (name == "http") return unique_ptr<RovCommsBase>(new HttpRovComms());
	else throw runtime_error("Unknown Communicator type: " + name);
}