#pragma once

#include <string>
#include <GL/glew.h>

namespace Bengine {

	class GLSLprogram
	{
	public:
		GLSLprogram();
		~GLSLprogram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		// accorting to the name, find the location of the variable
		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

	private:

		int _numAttribute;

		void compileShader(const std::string& filepath, GLuint id);

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

	};


}