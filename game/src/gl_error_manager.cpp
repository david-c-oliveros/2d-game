#include <glad/glad.h>
#include "gl_error_manager.h"
#include "util.h"


void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        util::Log("ERROR::GL::(", false);
        util::Log(error, false);
        util::Log("):", false);
        util::Log(function, false);
        util::Log(" ", false);
        util::Log(file, false);
        util::Log(":", false);
        util::Log(line);

        return false;
    }
    return true;
}
