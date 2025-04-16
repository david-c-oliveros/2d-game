#pragma once


#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>



class GLTexture
{
    public:
        GLTexture();
        ~GLTexture();

        void Bind() const;
        void Generate(uint8_t* pData, uint32_t _nWidth, uint32_t _nHeight);

        glm::vec2 GetSize();
        const uint32_t GetID() const;


    public:
        uint32_t nID;
        uint32_t nWidth, nHeight;

        uint32_t nInternal_Format;
        uint32_t nImage_Format;

        uint32_t nWrap_S;
        uint32_t nWrap_T;
        uint32_t nFilter_Min;
        uint32_t nFilter_Max;
};
