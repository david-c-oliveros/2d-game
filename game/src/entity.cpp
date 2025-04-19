#include "entity.h"

#include "shader.h"
#include "map.h"



Entity::Entity(uint32_t _ID, glm::vec2 _vWorldPos)
    : m_ID(_ID), vWorldPos(_vWorldPos * Globals::GLM_TILE_SIZE + Globals::GLM_TILE_SIZE * 0.5f)
{
}



Entity::~Entity()
{
}



void Entity::SetPosition(glm::vec2 _vWorldPos)
{
    vWorldPos = _vWorldPos * Globals::GLM_TILE_SIZE + Globals::GLM_TILE_SIZE * 0.5f;
}



void Entity::SetID(uint32_t _ID)
{
    m_ID = _ID;
}
