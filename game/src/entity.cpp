#include "entity.h"



Entity::Entity(uint32_t _ID, glm::vec2 _vWorldPos)
    : m_ID(_ID), vWorldPos(_vWorldPos)
{
}



Entity::~Entity()
{
}
