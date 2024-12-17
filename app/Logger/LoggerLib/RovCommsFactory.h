//--------------------------------------------------
// Defines a factory for getting the correct type of communicator for the ROV
//
// @author: Wild Boar
//
// @date: 2024-08-07
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include "Repository.h"

#include "RovCommsBase.h"
#include "RandomRovComms.h"
#include "FileRovComms.h"
#include "HttpRovComms.h"

namespace NVL_App
{
	class RovCommsFactory
	{
	public:
		static unique_ptr<RovCommsBase> GetCommunicator(const string& name, Repository * repository);
	};
}