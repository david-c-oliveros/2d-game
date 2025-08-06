#include "resource_manager.h"

/*********************************************************************/
/*        Returns the character contructed from the json data        */
/*********************************************************************/
template <typename T>
T RM::constructCharacter(nlohmann::json chJson)
{
    return T(GetNewResourceID(),
             chJson.at("name"),
             glm::vec2(chJson.at("position")[0],
                       chJson.at("position")[1]));
}


/******************************************************************************/
/*        Configures the passed in character object from the json data        */
/******************************************************************************/
template <typename T>
void RM::configCharacter(T &ch, nlohmann::json chJson)
{
    ch.AttachAnimatedSprite(chJson.at("sprite_name"),
                            chJson.at("sprite_path"),
                            (glm::ivec2)Globals::GLM_TILE_SIZE,
                            glm::ivec2(chJson.at("sheet_size")[0],
                                       chJson.at("sheet_size")[1]));

    for (auto anim : chJson.at("animations").items())
    {
        glm::ivec2 vStartIndex(anim.value().at("start_index")[0], anim.value().at("start_index")[1]);
        glm::ivec2 vEndIndex(anim.value().at("end_index")[0], anim.value().at("end_index")[1]);

        ch.AddAnimation(anim.key(), vStartIndex, vEndIndex);
        ch.SetAnimationFrequency(anim.key(), anim.value().at("frequency"));
    }

    ch.SetCurrentAnimation(chJson.at("starting_animation"));
}
