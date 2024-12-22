//--------------------------------------------------
// Control the setting values
//
// @author: Wild Boar
//
// @date: 2024-10-15
//--------------------------------------------------

#pragma once

#include <unordered_map>
#include <iostream>
using namespace std;

#include "Repository.h"

namespace NVL_App
{
    class Settings
    {
    private:
        Repository* _repo;
        unordered_map<string, string> _fields;
    public:
        Settings(Repository* repo, unordered_map<string, string>& parameters);

        string Render();

        inline Repository* GetRepo() { return _repo; }
        inline unordered_map<string, string>& GetFields() { return _fields; }

    private:
        void RenderSettings(string& content);
        void SubmitForm();
        void ReplacePlaceholder(string& content, const string& placeholder, const string& value);
    };
}