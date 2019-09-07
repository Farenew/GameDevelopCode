#include "GLSLprogram.h"
#include "Errors.h"

#include <fstream>
#include <vector>

namespace Bengine {

	GLSLprogram::GLSLprogram() : _numAttribute(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{

	}


	GLSLprogram::~GLSLprogram()
	{
	}

	GLint GLSLprogram::getUniformLocation(const std::string& uniformName)
	{
		// glGetUniformLocation means to find the variable in the program
		GLint Location = glGetUniformLocation(_programID, uniformName.c_str());
		if (Location == GL_INVALID_INDEX) {
			fatalError("uniform" + uniformName + "not found in shader!");
		}
		return Location;
	}


	void GLSLprogram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttribute; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLprogram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttribute; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLprogram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		// create a shader
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		// error handing
		if (_vertexShaderID == 0) {
			fatalError("Vertex shader failed to be created!");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		// error handing
		if (_fragmentShaderID == 0) {
			fatalError("Fragment shader failed to be created!");
		}

		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}

	// function to link shader, doc: https://www.khronos.org/opengl/wiki/Shader_Compilation
	void GLSLprogram::linkShaders()
	{
		// get a program object
		_programID = glCreateProgram();

		//Attach our shaders to our _programID
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our _programID
		glLinkProgram(_programID);

		// contents below are error handing
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the _programID anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			std::printf("%s\n", &errorLog[0]);

			fatalError("shader failed to link!");
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}


	//in colorShading.vert, there is in vec2 vertexPosition;
	// if it is :
	// vec2 0
	// vec2 1
	// vec2 2, 
	// so total _numAttribute is 2
	// so addAttribute is used to input our value

	void GLSLprogram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(_programID, _numAttribute++, attributeName.c_str());
	}

	// specific function to compile shader, reference doc: https://www.khronos.org/opengl/wiki/Shader_Compilation
	void GLSLprogram::compileShader(const std::string& filepath, GLuint id) {

		std::ifstream shaderFile(filepath);
		if (shaderFile.fail()) {
			perror(filepath.c_str());
			fatalError("Failed to open " + filepath);
		}

		std::string fileContents = "";
		std::string line;

		while (std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}

		shaderFile.close();

		const char* constPtr = fileContents.c_str();
		glShaderSource(id, 1, &constPtr, nullptr);

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		// error checking
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			std::printf("%s\n", &errorLog[0]);
			fatalError("shader" + filepath + "failed to compile!");
		}
	}

}