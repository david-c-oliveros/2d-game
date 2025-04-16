#include <map>

#include "shader.h"
#include "gl_error_manager.h"


GLShader::GLShader()
{
}



GLShader::~GLShader()
{
}


void GLShader::Compile(const char* sVertexSource, const char* sFragmentSource, const char* sGeometrySource)
{
    unsigned int sVertex;
    unsigned int sFragment;
//    unsigned int sGeometry;
    int success;
    char infoLog[512];


    /***************************************/
    /*        Compile Vertex Shader        */
    /***************************************/
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(sVertex, 1, &sVertexSource, NULL));
    GLCall(glCompileShader(sVertex));
    checkCompileErrors(sVertex, "VERTEX");


    /*****************************************/
    /*        Compile Fragment Shader        */
    /*****************************************/
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(sFragment, 1, &sFragmentSource, NULL));
    GLCall(glCompileShader(sFragment));
    checkCompileErrors(sFragment, "FRAGMENT");


    /*****************************************/
    /*        Compile Geometry Shader        */
    /*****************************************/
//    if (sGeometrySource !=  nullptr)
//    {
//        sGeometry = glCreateShader(GL_GEOMETRY_SHADER);
//        GLCall(glShaderSource(sGeometry, 1, &sGeometrySource, NULL));
//        GLCall(glCompileShader(sGeometry));
//        checkCompileErrors(sGeometry, "GEOMETRY");
//    }


    /***************************************/
    /*        Create Shader Program        */
    /***************************************/
    nID = glCreateProgram();
    GLCall(glAttachShader(nID, sVertex));
    GLCall(glAttachShader(nID, sFragment));

//    if (sGeometrySource != nullptr)
//        GLCall(glAttachShader(nID, sGeometry));

    GLCall(glLinkProgram(nID));
    GLCall(glValidateProgram(nID));

    std::cout << "- Linking shaders\n";
    checkCompileErrors(nID, "PROGRAM");
    std::cout << "- Deleting shaders\n";

    GLCall(glDeleteShader(sVertex));
    GLCall(glDeleteShader(sFragment));


//    if (sGeometrySource != nullptr)
//        GLCall(glDeleteShader(sFragment));
}


void GLShader::Bind() const
{
    GLCall(glUseProgram(nID));
}



void GLShader::Unbind() const
{
    GLCall(glUseProgram(0));
}



/**********************************/
/*        Set Bool Uniform        */
/**********************************/
void GLShader::SetUniform(const std::string &name, const bool value) const
{
    GLCall(glUniform1i(glGetUniformLocation(nID, name.c_str()), value));
}



/******************************************/
/*        Set Unsigned Int Uniform        */
/******************************************/
void GLShader::SetUniform(const std::string &name, const unsigned int value) const
{
    GLCall(glUniform1i(glGetUniformLocation(nID, name.c_str()), value));
}



/*********************************/
/*        Set Int Uniform        */
/*********************************/
void GLShader::SetUniform(const std::string &name, const int value) const
{
    GLCall(glUniform1i(glGetUniformLocation(nID, name.c_str()), value));
}



/***********************************/
/*        Set Float Uniform        */
/***********************************/
void GLShader::SetUniform(const std::string &name, const float value) const
{
    GLCall(glUniform1f(glGetUniformLocation(nID, name.c_str()), value));
}



/**********************************/
/*        Set Vec2 Uniform        */
/**********************************/
void GLShader::SetUniform(const std::string &name, const glm::vec2 vec) const
{
    GLCall(glUniform2fv(glGetUniformLocation(nID, name.c_str()), 1, glm::value_ptr(vec)));
}



/**********************************/
/*        Set Vec3 Uniform        */
/**********************************/
void GLShader::SetUniform(const std::string &name, const glm::vec3 vec) const
{
    GLCall(glUniform3fv(glGetUniformLocation(nID, name.c_str()), 1, glm::value_ptr(vec)));
}



/**********************************/
/*        Set Vec4 Uniform        */
/**********************************/
void GLShader::SetUniform(const std::string &name, const glm::vec4 vec) const
{
    GLCall(glUniform4fv(glGetUniformLocation(nID, name.c_str()), 1, glm::value_ptr(vec)));
}



/**********************************/
/*        Set Mat3 Uniform        */
/**********************************/
void GLShader::SetUniform(const std::string &name, const glm::mat3 matrix) const
{
    GLCall(glUniformMatrix3fv(glGetUniformLocation(nID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix)));
}



/**********************************/
/*        Set Mat4 Uniform        */
/**********************************/
void GLShader::SetUniform(const std::string &name, const glm::mat4 matrix) const
{
    GLCall(glUniformMatrix4fv(glGetUniformLocation(nID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix)));
}



/**************************************/
/*        Get Uniform Location        */
/**************************************/
GLint GLShader::GetUniform(const std::string &name) const
{
    return glGetUniformLocation(nID, name.c_str());
}



void GLShader::ValidateShaderProgram() const
{
    GLint current_program = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
    if (0 == current_program)
        std::cout << "- ERROR::No current program is set\n";
    else if(current_program != nID)
        std::cout << "ERROR::Current program ("
            << current_program
            << ") doesn't match active program ("
            << nID << ")\n";
    else
        std::cout << "ERROR::Current program ("
            << current_program
            << ") matches active program ("
            << nID << ")\n";
}



void GLShader::PrintProgramParam(GLenum eParam) const
{
    std::map<GLenum, std::string> glEnumStrings = {
        { GL_LINK_STATUS, "GL_LINK_STATUS" },
        { GL_VALIDATE_STATUS, "GL_VALIDATE_STATUS" },
        { GL_INFO_LOG_LENGTH, "GL_INFO_LOG_LENGTH" },
        { GL_ATTACHED_SHADERS, "GL_ATTACHED_SHADERS" },
        { GL_ACTIVE_ATOMIC_COUNTER_BUFFERS, "GL_ACTIVE_ATOMIC_COUNTER_BUFFERS" },
        { GL_ACTIVE_ATTRIBUTES, "GL_ACTIVE_ATTRIBUTES" },
        { GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH" },
        { GL_ACTIVE_UNIFORMS, "GL_ACTIVE_UNIFORMS" },
        { GL_ACTIVE_UNIFORM_BLOCKS, "GL_ACTIVE_UNIFORM_BLOCKS" },
        { GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, "GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH" },
        { GL_ACTIVE_UNIFORM_MAX_LENGTH, "GL_ACTIVE_UNIFORM_MAX_LENGTH" },
        { GL_COMPUTE_WORK_GROUP_SIZE, "GL_COMPUTE_WORK_GROUP_SIZE" },
        { GL_PROGRAM_BINARY_LENGTH, "GL_PROGRAM_BINARY_LENGTH" },
        { GL_TRANSFORM_FEEDBACK_BUFFER_MODE, "GL_TRANSFORM_FEEDBACK_BUFFER_MODE" },
        { GL_TRANSFORM_FEEDBACK_VARYINGS, "GL_TRANSFORM_FEEDBACK_VARYINGS" },
        { GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, "GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH" },
        { GL_GEOMETRY_VERTICES_OUT, "GL_GEOMETRY_VERTICES_OUT" },
        { GL_GEOMETRY_INPUT_TYPE, "GL_GEOMETRY_INPUT_TYPE" },
        { GL_GEOMETRY_OUTPUT_TYPE, "GL_GEOMETRY_OUTPUT_TYPE" }
    };

    GLint iSuccess;
    char cInfoLog[1024];
    GLCall(glGetProgramiv(nID, eParam, &iSuccess));
    GLCall(glGetProgramInfoLog(nID, 1024, NULL, cInfoLog));
    std::cout << "- SHADER PROGRAM::PARAMETER: " << glEnumStrings[eParam]
        << cInfoLog << "\n-- --------------------------------------------------- -- "
        << "\nValue: " << iSuccess << "\n-- --------------------------------------------------- -- "
        << std::endl;
}



void GLShader::checkCompileErrors(GLuint nObject, std::string sType)
{
    GLint iSuccess;
    char cInfoLog[1024];
    if (sType != "PROGRAM")
    {
        GLCall(glGetShaderiv(nObject, GL_COMPILE_STATUS, &iSuccess));
        if (!iSuccess)
        {
            GLCall(glGetShaderInfoLog(nObject, 1024, NULL, cInfoLog));
            std::cout << "- ERROR::SHADER: Compile-time error: Type: " << sType << "\n"
                << cInfoLog << "\n-- --------------------------------------------------- -- "
                << std::endl;
        }
        else
        {
            std::cout << "- Shader compilation successful\n";
        }
    }
    else
    {
        GLCall(glGetProgramiv(nObject, GL_LINK_STATUS, &iSuccess));
        if (!iSuccess)
        {
            GLCall(glGetProgramInfoLog(nObject, 1024, NULL, cInfoLog));
            std::cout << "| ERROR::SHADER: Link-time error: Type: " << sType << "\n"
                << cInfoLog << "\n-- --------------------------------------------------- -- "
                << std::endl;
        }
        else
        {
            std::cout << "- Shader linking successful\n";
        }
    }
}
