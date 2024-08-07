//--------------------------------------------------
// A test communicator that lots json messages from files
//
// @author: Wild Boar
//
// @date: 2024-08-07
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include <jsoncpp/json/json.h>

#include <NVLib/FileUtils.h>

#include "RovCommsBase.h"

namespace NVL_App
{
	class FileRovComms : public RovCommsBase
	{
	public:
		FileRovComms();
		virtual unique_ptr<Status> GetCurrentStatus() override;
	private:
		string ReadMessage(const string& messageName);
	};
}