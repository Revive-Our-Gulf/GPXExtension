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
#include "Repository.h"

namespace NVL_App
{
	class RovCommsBase
	{
	private: 
		Repository * _repository;
	public:
		RovCommsBase(Repository * repository) : _repository(repository) {}
		virtual unique_ptr<Status> GetCurrentStatus() = 0;
	protected:
		Repository * GetRepository() { return _repository; }
		string GetTrackName() { return _repository->GetField(Repository::Field::CURRENT_TRACK); }
	};
}
