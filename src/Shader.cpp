#include "Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "Assert.h"

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath)
	, m_RendererId(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererId = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GL_CALL(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
	GL_CALL(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
	GL_CALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
	GL_CALL(glUniform1i(GetUniformLocation(name), i0));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	GL_CALL(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    // ShaderTypes act as index for stringstream to load shader content
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int shaderId = glCreateShader(type);
    const char* src = source.c_str();

    GL_CALL(glShaderSource(shaderId, 1, &src, nullptr));
    GL_CALL(glCompileShader(shaderId));

    int ivResult;
    GL_CALL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ivResult));

    if (ivResult == GL_FALSE)
    {
        int length;
        GL_CALL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*) malloc(length * sizeof(char));
        GL_CALL(glGetShaderInfoLog(shaderId, length, &length, message));

        LOG("[ERROR] Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader");
		LOG("[ERROR] " << message);

        GL_CALL(glDeleteShader(shaderId));
        return 0;
    }

    return shaderId;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int glProgram = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach, Link, and Validate glProgram
    GL_CALL(glAttachShader(glProgram, vs));
    GL_CALL(glAttachShader(glProgram, fs));
    GL_CALL(glLinkProgram(glProgram));
    GL_CALL(glValidateProgram(glProgram));

    GL_CALL(glDeleteShader(vs));
    GL_CALL(glDeleteShader(fs));

    return glProgram;
}


int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	// Check if uniform exists and add it to the cache
	GL_CALL(int location = glGetUniformLocation(m_RendererId, name.c_str()));

	if (location == -1)
		LOG("[WARN] Uniform \"" << name << "\" not found on shader with ID " << m_RendererId);

	m_UniformLocations[name] = location;

	return location;
}
