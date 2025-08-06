#pragma once

#include <iostream>
#include <glm/glm.hpp>


class Shader;
class Map;



class Entity
{
    public:
        Entity() = default;
        Entity(uint32_t _ID, glm::vec2 _vWorldPos);
        ~Entity();

        virtual void Update(Map &cMap) = 0;
        virtual void Draw(const std::string sShader) = 0;
        void SetPosition(glm::vec2 _vWorldPos);
        void SetID(uint32_t _ID);


    public:
        /**********************************************************************************/
        /*        Note: Entity's word position is initialized to the                      */
        /*        center of the given tile position when the entity is constructed        */
        /*        (eg. { 0, 0 } tile position is { 16.0, 16.0 } world position            */
        /**********************************************************************************/
        glm::vec2 vWorldPos;
        uint32_t m_ID;
};
