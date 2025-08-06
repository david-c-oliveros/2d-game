#pragma once

#include <map>

#include "util.h"
#include "button.h"



class Scene
{
    public:
        Scene(std::string &sSceneFile);
        ~Scene();

        void AddLabel(std::string sName, std::string sLabelText);
        void AddButton(std::string sName, std::string sLabelText);


    public:
        std::string sName;
        std::map<std::string, std::unique_ptr<Label>> mLabels;
        std::map<std::string, std::unique_ptr<Button>> mButtons;
};
