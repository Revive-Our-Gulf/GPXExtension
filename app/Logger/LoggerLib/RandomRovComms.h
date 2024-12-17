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

#include "RovCommsBase.h"

namespace NVL_App
{
	class RandomRovComms : public RovCommsBase
	{
	public:
		RandomRovComms(Repository * repository);
		virtual unique_ptr<Status> GetCurrentStatus() override;
	};
}
