#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class GLShader
{
    public:
        GLShader();
        ~GLShader();

        void Compile(const char* sVertexSource,
                     const char* sFragmentSource,
                     const char* sGeometrySource);

        void Bind() const;
        void Unbind() const;

        void SetUniform(const std::string &name, bool value) const;
        void SetUniform(const std::string &name, unsigned int value) const;
        void SetUniform(const std::string &name, int value) const;
        void SetUniform(const std::string &name, float value) const;
        void SetUniform(const std::string &name, glm::vec2 vec) const;
        void SetUniform(const std::string &name, glm::vec3 vec) const;
        void SetUniform(const std::string &name, glm::vec4 vec) const;
        void SetUniform(const std::string &name, glm::mat3 matrix) const;
        void SetUniform(const std::string &name, glm::mat4 matrix) const;
        GLint GetUniform(const std::string &name) const;
        void ValidateShaderProgram() const;
        void PrintProgramParam(GLenum eParam) const;

    public:
        uint32_t nID;


    private:
        void checkCompileErrors(unsigned int nObject, std::string sType);
};
