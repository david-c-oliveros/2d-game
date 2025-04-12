#include <glad/glad.h>
#include "gl_error_manager.h"


void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "- ERROR::GL::(" << error << "):" << function
            << " " << file << ":" << line << std::endl;

        return false;
    }
    return true;
}
