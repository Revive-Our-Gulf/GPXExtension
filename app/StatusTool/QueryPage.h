//--------------------------------------------------
// Query Page for GPX
//
// @author: Wild Boar
//
// @date: 2024-10-18
//--------------------------------------------------

#pragma once

#include <iostream>
#include <unordered_map>
using namespace std;

#include <NVLib/StringUtils.h>

namespace NVL_App
{
	class QueryPage
	{
	private:
		unordered_map<string, string> _fields;
	public:
		QueryPage(unordered_map<string, string>& parameters);

		string Render();

		inline unordered_map<string, string>& GetFields() { return _fields; }

	private:
		void RenderForm(ostream& writer);
		void RenderResponse(ostream& writer);

		void RenderHeader(ostream& writer);
		void RenderFooter(ostream& writer);
	};
}
