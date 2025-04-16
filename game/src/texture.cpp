#include <glad/glad.h>
#include "texture.h"

#include "gl_error_manager.h"



GLTexture::GLTexture()
    : nWidth(0), nHeight(0),
      nInternal_Format(GL_RGBA), nImage_Format(GL_RGBA),
      nWrap_S(GL_REPEAT), nWrap_T(GL_REPEAT),
      nFilter_Min(GL_LINEAR), nFilter_Max(GL_NEAREST)
{
    GLCall(glGenTextures(1, &nID));
}



GLTexture::~GLTexture()
{
}



void GLTexture::Bind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, nID));
}



void GLTexture::Generate(uint8_t* pData, uint32_t _nWidth, uint32_t _nHeight)
{
    nWidth = _nWidth;
    nHeight = _nHeight;

    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, nID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, nInternal_Format, nWidth, nHeight, 0, nImage_Format, GL_UNSIGNED_BYTE, pData));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, nWrap_S));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, nWrap_T));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nFilter_Min));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nFilter_Max));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}



const uint32_t GLTexture::GetID() const
{
    return nID;
}



glm::vec2 GLTexture::GetSize()
{
    return glm::ivec2(nWidth, nHeight);
}
