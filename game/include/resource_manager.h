#pragma once

#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <stb_image.h>

#include "texture.h"
#include "shader.h"


class RM
{
    public:
        const static GLShader& LoadShader(const std::string vShaderFile,
                                          const std::string fShaderFile,
                                          const std::string gShaderFile,
                                          const std::string sName);

        const static GLTexture& LoadTexture(const std::string sFile, bool bAlpha, const std::string sName);

        const static GLShader& GetShader(const std::string sName);
        static GLTexture GetTexture(const std::string sName);

        static void Clear();


    public:
        static std::map<std::string, GLShader> mShaders;
        static std::map<std::string, GLTexture> mTextures;


    private:
        static GLShader loadShaderFromFile(const std::string vShaderFile,
                                           const std::string fShaderFile,
                                           const std::string gShaderFile = "");

        static GLTexture loadTextureFromFile(const std::string sFile, bool bAlpha);
};
