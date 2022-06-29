#ifndef Shader_h
#define Shader_h

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererId;
	std::unordered_map<std::string, int> m_UniformLocations;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	/* Set Uniform Data */
	void SetUniform1i(const std::string& name, int i0);
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniformMatrix3fv(const std::string& name, const float* matrix);
	void SetUniformMatrix4fv(const std::string& name, const float* matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

#endif /* Shader_h */
