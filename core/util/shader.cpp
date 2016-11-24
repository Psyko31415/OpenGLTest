#include "shader.h"

GLuint createProgram(const char * vertPath, const char * fragPath)
{
	GLuint vert = loadShader(vertPath, GL_VERTEX_SHADER);
	GLuint frag = loadShader(fragPath, GL_FRAGMENT_SHADER);
	GLuint program;

	if (vert && frag)
	{
		program = glCreateProgram();
		glAttachShader(program, vert);
		glAttachShader(program, frag);
		glLinkProgram(program);

		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen)
		{
			char * buffer = new char[infoLen + 1];

			glGetProgramInfoLog(program, infoLen, NULL, buffer);
			buffer[infoLen] = '\0';
			std::cout << buffer << std::endl;

			delete buffer;
		}

		glDetachShader(program, vert);
		glDetachShader(program, frag);

		glDeleteShader(vert);
		glDeleteShader(frag);

		return program;
	}
	else
	{
		std::cout << "something faild when loading shaders" << std::endl;
	}

	return 0;
}

GLuint loadShader(const char * filepath, GLenum type)
{
	GLuint id = glCreateShader(type);

	std::string code;
	std::ifstream stream(filepath);

	if (stream.is_open())
	{
		std::string line = "";
		while (std::getline(stream, line))
		{
			code += line + "\n";
		}
	}
	else
	{
		std::cout << "can't open file " << filepath << std::endl;
		return 0;
	}

	const char * codeCstr = code.c_str();
	glShaderSource(id, 1, &codeCstr, NULL);
	glCompileShader(id);

	GLint infoLen = 0;
	GLint complied = GL_FALSE;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);
	glGetShaderiv(id, GL_COMPILE_STATUS, &complied);

	if (!complied)
	{
		char * buffer = new char[infoLen + 1];

		glGetShaderInfoLog(id, infoLen, NULL, buffer);
		std::cout << buffer << std::endl;

		delete buffer;
	}

	return id;
}