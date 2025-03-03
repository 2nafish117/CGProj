#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLDependencies.h"
#include "GLError.h"

#include "Lighting/Lighting.h"

class ShaderProgram
{
public:
	ShaderProgram(const std::string & vertexShaderPath, const std::string & fragmentShaderPath);
	ShaderProgram() {  };
	~ShaderProgram();

	void bind() const;
	void unbind() const;

	// set uniforms
	void seti(const std::string & name, int value);

	void setf(const std::string & name, float value);
	void set3f(const std::string& name, float f0, float f1, float f2);
	void set4f(const std::string& name, float f0, float f1, float f2, float f3);
	void set3fv(const std::string& name, const float * values);

	void setMatrix3fv(const std::string & name, const float * values);
	void setMatrix4fv(const std::string & name, const float * values);

	void setDirLight(const std::string& name, DirLight& light);
	void setPointLight(const std::string& name, const PointLight& light);
	void setSpotLight(const std::string& name, const SpotLight& light);

private:
	unsigned int compileShader(const std::string& src, GLenum shaderType);
	bool readShaderFileIntoStream(const std::string & shaderPath, std::stringstream& sourceStream);
	unsigned int createProgramAndLink(unsigned int vs, unsigned int fs);

	int getUniformLocation(const std::string& name);
private:
	unsigned int mRendererID;
	// keeps track of globally bound program for caching
	static unsigned int boundProgram;
	std::unordered_map<std::string, int> mUniformLocationCache;
	std::string mVSPath, mFSPath;
};