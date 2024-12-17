//--------------------------------------------------
// A test communicator that lots json messages from files
//
// @author: Wild Boar
//
// @date: 2024-10-17
//--------------------------------------------------

#pragma once

#include <iostream>
using namespace std;

#include "httplib.h"
#include <jsoncpp/json/json.h>

#include "RovCommsBase.h"
#include "Status.h"

namespace NVL_App
{
	class HttpRovComms : public RovCommsBase
	{
	private:
		string _ip;
	public:
		HttpRovComms(Repository * repository);
		virtual unique_ptr<Status> GetCurrentStatus() override;
	};
}
