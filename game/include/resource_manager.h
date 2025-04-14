#pragma once

#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "shader.h"


class RM
{
    public:
        static std::map<std::string, GLShader> mShaders;

        static GLShader LoadShader(std::string vShaderFile,
                                   std::string fShaderFile,
                                   std::string gShaderFile,
                                   std::string sName);

        static GLShader GetShader(std::string sName);
        static void Clear();


    private:
        static GLShader loadShaderFromFile(std::string vShaderFile,
                                           std::string fShaderFile,
                                           std::string gShaderFile = "");
};
