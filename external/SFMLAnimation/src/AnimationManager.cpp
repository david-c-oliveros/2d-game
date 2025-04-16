#include "AnimationManager.hpp"

std::map<std::string, GLTexture> AnimationManager::m_textures;
std::map<std::string, sf::Vector2i> AnimationManager::m_indices;
std::map<std::string, sf::Vector2i> AnimationManager::m_startingIndices;
std::map<std::string, sf::Vector2i> AnimationManager::m_endingIndices;
std::map<std::string, sf::Vector2i> AnimationManager::m_sheetSizes;
std::map<std::string, sf::Vector2i> AnimationManager::m_spriteSizes;
std::map<std::string, int> AnimationManager::m_frequencies;
std::map<std::string, int> AnimationManager::m_timesUpdated;
std::map<std::string, int> AnimationManager::m_timesBetweenUpdate;
std::map<uint32_t, std::string> AnimationManager::m_animations;



void AnimationManager::forceUpdate(std::string animation, const std::shared_ptr<GLSprite> &sprite)
{
    m_timesBetweenUpdate[animation] = m_frequencies[animation];
    update(animation, sprite);
}


bool AnimationManager::update(std::string animation, const std::shared_ptr<GLSprite> &sprite)
{
    if (m_timesBetweenUpdate[animation] < m_frequencies[animation])
    {
        m_timesBetweenUpdate[animation]++;
        return false;
    }

    if (m_sheetSizes[animation] != sf::Vector2i(0, 0))
    {
        sf::IntRect rect(sf::Vector2i(m_indices[animation].x * m_spriteSizes[animation].x,
        m_indices[animation].y * m_spriteSizes[animation].y),
        m_spriteSizes[animation]);

        /*****************************************************************/
        /*        Modification: Implement Starting/Ending Indices        */
        /*****************************************************************/
        if (m_indices[animation].y < m_sheetSizes[animation].y &&
            m_indices[animation].y < m_endingIndices[animation].y)
        {
            m_indices[animation].y++;
        }
        else
        {
            // Otherwise, we move over one column and go back to the top
            m_indices[animation].y = m_startingIndices[animation].y;
            m_indices[animation].x++;
            // And then reset the sheet if we are past the width of the sheet
            if (m_indices[animation].x >= m_sheetSizes[animation].x ||
                m_indices[animation].x >= m_endingIndices[animation].x)
            {
                m_indices[animation].x = m_startingIndices[animation].x;
            }
        }

        sprite->SetTextureRect(rect);
        m_timesBetweenUpdate[animation] = 0;
    }
    else
    {
        std::cout << "No animation entry found for \"" << animation << "\"!" << std::endl;
    }

    return true;
}


void AnimationManager::addAnimation(std::string animation, GLTexture texture,
    sf::Vector2i sheetSize, sf::Vector2i spriteSize, sf::Vector2i index, int frequency,
    sf::Vector2i startingIndex)
{
    // First, we make a entry to the animations map
    animation = animation;
    // Next, we want to make an entry in the texture map
    m_textures[animation] = texture;
    // Next, we make sheet size entry
    m_sheetSizes[animation].x = sheetSize.x;
    m_sheetSizes[animation].y = sheetSize.y;

    // Next, we make sprite size entry
    m_spriteSizes[animation].x = spriteSize.x;
    m_spriteSizes[animation].y = spriteSize.y;

    // Our index vector
    m_indices[animation].x = index.x;
    m_indices[animation].y = index.y;

    // Our starting index vector
    m_startingIndices[animation].x = startingIndex.x;
    m_startingIndices[animation].y = startingIndex.y;

    // Our ending index vector
    m_endingIndices[animation].x = sheetSize.x;
    m_endingIndices[animation].y = sheetSize.y;

    // Our update rate (frequency)
    m_frequencies[animation] = frequency;
    // We don't need to initialize the times udpated because its default is 0
}


void AnimationManager::deleteAnimation(std::string animation)
{
    // We just erase each entry in every map
    m_textures.erase(animation);
    m_indices.erase(animation);
    m_startingIndices.erase(animation);
    m_sheetSizes.erase(animation);
    m_spriteSizes.erase(animation);
    m_frequencies.erase(animation);
    m_timesUpdated.erase(animation);
    m_endingIndices.erase(animation);
}


void AnimationManager::setAnimationFrequency(std::string animation, int frequency)
{
    m_frequencies[animation] = frequency;
}


void AnimationManager::setAnimationIndex(std::string animation, sf::Vector2i index)
{
    m_indices[animation].x = index.x;
    m_indices[animation].y = index.y;
}


void AnimationManager::setAnimationSheetSize(std::string animation, sf::Vector2i size)
{
    m_sheetSizes[animation].x = size.x;
    m_sheetSizes[animation].y = size.y;
}


void AnimationManager::setAnimationSpriteSize(std::string animation, sf::Vector2i size)
{
    m_spriteSizes[animation].x = size.x;
    m_spriteSizes[animation].y = size.y;
}


void AnimationManager::setAnimationTexture(std::string animation, GLTexture texture)
{
    m_textures[animation] = texture;
}


void AnimationManager::resetAnimationIndex(std::string animation)
{
    m_indices[animation].x = m_startingIndices[animation].x;
    m_indices[animation].y = m_startingIndices[animation].y;
}


void AnimationManager::setAnimationStartingIndex(std::string animation, sf::Vector2i index)
{
    m_startingIndices[animation].x = index.x;
    m_startingIndices[animation].y = index.y;
}


void AnimationManager::setAnimationEndingIndex(std::string animation, sf::Vector2i index)
{
    m_endingIndices[animation].x = index.x;
    m_endingIndices[animation].y = index.y;
}


sf::Vector2i AnimationManager::getAnimationStartingIndex(std::string animation)
{
    return m_startingIndices[animation];
}


sf::Vector2i AnimationManager::getAnimationEndingIndex(std::string animation)
{
    return m_endingIndices[animation];
}
