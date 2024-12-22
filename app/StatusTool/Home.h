#pragma once

#include <iostream>
#include <sstream>
using namespace std;

#include "Repository.h"

namespace NVL_App
{
    class Home
    {
    private:
		Repository * _repo;
		unordered_map<string, string> _fields;
    public:
        Home(Repository * repo, unordered_map<string, string>& parameters);

        inline Repository * GetRepo() { return _repo; }
		inline unordered_map<string, string>& GetFields() { return _fields; }

        string Render();
    
    private:
        void ReplacePlaceholder(string& content, const string& placeholder, const string& value);
        
    };

    
}