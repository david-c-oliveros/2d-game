#include "resource_manager.h"


std::map<std::string, GLShader> RM::mShaders;


GLShader RM::LoadShader(std::string vShaderFile,
                                     std::string fShaderFile,
                                     std::string gShaderFile,
                                     std::string sName)
{
    mShaders[sName] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return mShaders[sName];
}


GLShader RM::GetShader(std::string sName)
{
    return mShaders[sName];
}


void RM::Clear()
{
    for (auto it : mShaders)
    {
        glDeleteProgram(it.second.nID);
    }
}


GLShader RM::loadShaderFromFile(std::string vShaderFile,
                                             std::string fShaderFile,
                                             std::string gShaderFile)
{
    std::string sVertexCode;
    std::string sFragmentCode;
    std::string sGeometryCode;

    try
    {
        std::ifstream vShaderFileIn(vShaderFile);
        std::ifstream fShaderFileIn(fShaderFile);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFileIn.rdbuf();
        fShaderStream << fShaderFileIn.rdbuf();

        vShaderFileIn.close();
        fShaderFileIn.close();

        sVertexCode = vShaderStream.str();
        sFragmentCode = fShaderStream.str();

        if (gShaderFile != "")
        {
            std::ifstream gShaderFileIn(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFileIn.rdbuf();
            gShaderFileIn.close();
            sGeometryCode = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "- ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vShaderCode = sVertexCode.c_str();
    const char* fShaderCode = sFragmentCode.c_str();
    const char* gShaderCode;

    if (gShaderFile != "")
        gShaderCode = sGeometryCode.c_str();

    GLShader cShader;
    cShader.Compile(vShaderCode, fShaderCode, gShaderFile != "" ? gShaderCode : nullptr);

    return cShader;
}
