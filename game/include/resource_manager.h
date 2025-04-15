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

        static GLShader LoadShader(const std::string vShaderFile,
                                   const std::string fShaderFile,
                                   const std::string gShaderFile,
                                   const std::string sName);

//        GLTexture RM::LoadTexture(const std::string sFile, bool bAlpha, const std::string sName)

        static GLShader GetShader(const std::string sName);
//        GLTexture GetTexture(const std::string sName);
        static void Clear();


    private:
        static GLShader loadShaderFromFile(const std::string vShaderFile,
                                           const std::string fShaderFile,
                                           const std::string gShaderFile = "");

//        GLTexture loadTextureFromFile(const std::string sFile, bool bAlpha);
};
