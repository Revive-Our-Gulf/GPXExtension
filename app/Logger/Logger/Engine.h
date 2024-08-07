//--------------------------------------------------
// Defines a basic engine for a vanilla C++ project.
//
// @author: Wild Boar
//
// @date: 2024-07-25
//--------------------------------------------------

#pragma once

#include <chrono>
#include <thread>
#include <iostream>
using namespace std;

#include <NVLib/Logger.h>

#include <LoggerLib/ArgUtils.h>
#include <LoggerLib/RovCommsFactory.h>
#include <LoggerLib/Repository.h>

namespace NVL_App
{
	class Engine
	{
	private:
		NVLib::Parameters * _parameters;
		NVLib::Logger* _logger;
		int _interval;
	public:
		Engine(NVLib::Logger* logger, NVLib::Parameters * parameters);
		~Engine();

		void Run();
	};
}
