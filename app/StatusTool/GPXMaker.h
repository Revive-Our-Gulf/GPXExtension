//--------------------------------------------------
// Converts a set of statuses to a GPX file
//
// @author: Wild Boar
//
// @date: 2024-07-26
//--------------------------------------------------

#pragma once

#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

#include "Status.h"

namespace NVL_App
{
	class GPXMaker
	{
	private:
		vector<Status *> _statuses;
	public:
		GPXMaker(vector<Status *>& statuses);
		~GPXMaker();

		string RenderXML();
	};
}
