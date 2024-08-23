#include "Shader.hpp"
#include <cerrno>
#include <string>

std::string getFileContents(const char* filename)
{
    std::cout << filename;
    std::ifstream in(filename, std::ios::binary);
    std::cout << "Loading\n";
    if(!in) throw errno;
    std::cout << "Loaded\n";

    std::string contents((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
    in.close();
    return contents;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexShaderSourceString = getFileContents(vertexFile);
    std::string fragmentShaderSourceString = getFileContents(fragmentFile);

    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "VERTEX");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activate()
{
    glUseProgram(this->ID);
}

void Shader::destroy()
{
    glDeleteProgram(this->ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
    std::string typeString(type);
	if (typeString != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}