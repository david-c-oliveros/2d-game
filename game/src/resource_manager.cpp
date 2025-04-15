#include "resource_manager.h"


//std::map<std::string, GLTexture> RM::mTextures;
std::map<std::string, GLShader> RM::mShaders;


GLShader RM::LoadShader(const std::string vShaderFile,
                        const std::string fShaderFile,
                        const std::string gShaderFile,
                        const std::string sName)
{
    mShaders[sName] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return mShaders[sName];
}


//GLTexture RM::LoadTexture(const std::string sFile, bool bAlpha, const std::string sName)
//{
//    return mTextures[sName] = loadTexturesFromFile(sFile, bAlpha);
//}


GLShader RM::GetShader(const std::string sName)
{
    return mShaders[sName];
}


//GLTexture RM::GetTexture(const std::string sName)
//{
//    return mTextures[sName];
//}


void RM::Clear()
{
    for (auto it : mShaders)
    {
        glDeleteProgram(it.second.nID);
    }

//    for (auto it : mTextures)
//    {
//        glDeleteTextures(1, &it.second.nID);
//    }
}


//GLTexture RM::loadTextureFromFile(const std::string sFile, bool bAlpha)
//{
//    GLTexture cTexture;
//
//    if (bAlpha)
//    {
//        cTexture.nInternal_Format = GL_RGBA;
//        cTexture.nImage_Format = GL_RGBA;
//    }
//
//    int32_t nWidth = 0;
//    int32_t nHeight = 0;
//    int32_t nrChannels = 0;
//    uint32_t* pData = stbi_load(cFile, &nWidth, &nHeight, &nrChannels, 0);
//
//    if (NULL == pData)
//        std::cout << "- WARNING::File " << sFile << " not found" << std::endl;
//
//    cTexture.Generate(pData, nWidth, nHeight);
//
//    stb_image_free(pData);
//
//    return cTexture;
//}


GLShader RM::loadShaderFromFile(const std::string vShaderFile,
                                const std::string fShaderFile,
                                const std::string gShaderFile)
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
