//--------------------------------------------------
// A communicator for getting information from the ROV
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#pragma once

#include <memory>
#include <iostream>
using namespace std;

#include <NVLib/RandomUtils.h>

#include "Status.h"

namespace NVL_App
{
	class RovCommsBase
	{
	public:
		virtual unique_ptr<Status> GetCurrentStatus() = 0;
	};
}
