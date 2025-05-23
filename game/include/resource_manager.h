#pragma once

#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <stb_image.h>

#include "character.h"
#include "player.h"

#include "util.h"



class Entity;
class Map;
class Npc;
class GLTexture;
class GLShader;



struct EntityCollection
{
    Player cPlayer;
    std::vector<Npc> aNpcs;
};


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
        static EntityCollection LoadEntityData(const std::string &sDataFile);

        static void Clear();


    public:
        static std::map<std::string, GLShader> mShaders;
        static std::map<std::string, GLTexture> mTextures;
        static uint32_t m_nCurrentResourceID;


    private:
        static GLShader loadShaderFromFile(const std::string vShaderFile,
                                           const std::string fShaderFile,
                                           const std::string gShaderFile = "");

        static GLTexture loadTextureFromFile(const std::string sFile, bool bAlpha);
        static uint32_t getNewResourceID();

        template <typename T>
        static T constructCharacter(nlohmann::json chJson);

        template <typename T>
        static void configCharacter(T &ch, nlohmann::json chJson);
};

#include "resource_manager.inl"
