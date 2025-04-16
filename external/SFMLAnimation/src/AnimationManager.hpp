/*
MIT License

Copyright (c) 2018 Jack Featherstone

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

DEPENDENCIES:
std::string
std::map
sf::Sprite
sf::Texture
sf::sf::Vector2i/f

*/

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

#include "texture.h"
#include "sprite.h"

class AnimationManager {
    private:
        static std::map<std::string, GLTexture> m_textures;
        static std::map<std::string, sf::Vector2i> m_indices;
        static std::map<std::string, sf::Vector2i> m_startingIndices;
        static std::map<std::string, sf::Vector2i> m_endingIndices;
        static std::map<std::string, sf::Vector2i> m_sheetSizes;
        static std::map<std::string, sf::Vector2i> m_spriteSizes;
        static std::map<std::string, int> m_frequencies;
        static std::map<std::string, int> m_timesUpdated;
        static std::map<std::string, int> m_timesBetweenUpdate;
        static std::map<uint32_t, std::string> m_animations;


    public:

        static void printTextures();
        static void forceUpdate(std::string animation, const std::shared_ptr<GLSprite> &sprite);

        static bool update(std::string animation, const std::shared_ptr<GLSprite> &sprite);

        static void addAnimation(std::string animation, GLTexture texture, sf::Vector2i sheetSize,
        sf::Vector2i spriteSize, sf::Vector2i index = sf::Vector2i(0, 0), int frequency = 0,
        sf::Vector2i startingIndex = sf::Vector2i(0, 0));

        static void deleteAnimation(std::string animation);

        static void setAnimationFrequency(std::string animation, int frequency);
        static void setAnimationSpriteSize(std::string animation, sf::Vector2i size);
        static void setAnimationSheetSize(std::string animation, sf::Vector2i size);
        static void setAnimationIndex(std::string animation, sf::Vector2i index);
        static void setAnimationTexture(std::string animation, GLTexture texture);
        static void setAnimationStartingIndex(std::string animation, sf::Vector2i index);
        static void setAnimationEndingIndex(std::string animation, sf::Vector2i index);

        static sf::Vector2i getAnimationStartingIndex(std::string animation);
        static sf::Vector2i getAnimationEndingIndex(std::string animation);


        static void resetAnimationIndex(std::string animation);
};
