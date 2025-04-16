#include "entity.h"



Entity::Entity(uint32_t _ID, glm::vec2 _vWorldPos)
    : m_ID(_ID), vWorldPos(_vWorldPos * Globals::GLM_TILE_SIZE + Globals::GLM_TILE_SIZE * 0.5f)
{
}



Entity::~Entity()
{
}
