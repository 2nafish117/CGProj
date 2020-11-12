#include "ShaderProgram.h"

unsigned int ShaderProgram::boundProgram = UINT_MAX;

ShaderProgram::ShaderProgram(const std::string & vertexShaderPath, const std::string & fragmentShaderPath)
	: mRendererID(0), mVSPath(vertexShaderPath), mFSPath(fragmentShaderPath)
{
	using namespace std;
	stringstream vertexSource, fragmentSource;
	readShaderFileIntoStream(vertexShaderPath, vertexSource);
	readShaderFileIntoStream(fragmentShaderPath, fragmentSource);

	// cout << vertexSource.str() << "\n\n" << fragmentSource.str() << "\n\n";

	unsigned int vs = compileShader(vertexSource.str(), GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragmentSource.str(), GL_FRAGMENT_SHADER);

	mRendererID = createProgramAndLink(vs, fs);
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

unsigned int ShaderProgram::createProgramAndLink(unsigned int vs, unsigned int fs)
{
	unsigned int prog;
	GLCall(prog = glCreateProgram());
	GLCall(glAttachShader(prog, vs));
	GLCall(glAttachShader(prog, fs));
	GLCall(glLinkProgram(prog));
	int success;
	GLCall(glGetProgramiv(prog, GL_LINK_STATUS, &success));
	if (!success)
	{
		int length;
		GLCall(glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length));
		char* buff = (char*)alloca(length);
		GLCall(glGetProgramInfoLog(prog, length, &length, buff));
		std::cerr << "ERROR::PROGRAM\n" << buff << std::endl;
	}

	return prog;
}

bool ShaderProgram::readShaderFileIntoStream(const std::string & shaderPath, std::stringstream & sourceStream)
{
	using namespace std;
	ifstream shaderFile;

	shaderFile.open(shaderPath.c_str());
	if (!shaderFile.is_open())
	{
		cerr << "ERROR::SHADER: " << shaderPath << " not found\n";
		return false;
	}

	sourceStream << shaderFile.rdbuf();
	return true;
}

unsigned int ShaderProgram::compileShader(const std::string& src, GLenum shaderType)
{
	GLCall(unsigned int shader = glCreateShader(shaderType));
	const char* srcp = src.c_str();
	GLCall(glShaderSource(shader, 1, &srcp, NULL));
	GLCall(glCompileShader(shader));
	int success;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

	if (!success)
	{
		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
		char* buff = (char*)alloca(length);
		GLCall(glGetShaderInfoLog(shader, length, &length, buff));
		std::cerr << "ERROR::SHADER " << (shaderType == GL_VERTEX_SHADER ? "VERTEX: " + mVSPath : "FRAGMENT: " + mFSPath) << "\n" << buff << std::endl;
	}
	return shader;
}

ShaderProgram::~ShaderProgram()
{
	GLCall(glDeleteProgram(mRendererID));
}

void ShaderProgram::bind() const
{
	// if not currently bound then bind it
	if (mRendererID != boundProgram)
	{
		GLCall(glUseProgram(mRendererID));
		boundProgram = mRendererID;
	}

	return;
}

void ShaderProgram::unbind() const
{
	boundProgram = UINT_MAX;
	GLCall(glUseProgram(0));
}

void ShaderProgram::seti(const std::string & name, int value)
{
	int location = getUniformLocation(name);
	GLCall(glUniform1i(location, value));
}

void ShaderProgram::setf(const std::string & name, float value)
{
	int location = getUniformLocation(name);
	GLCall(glUniform1f(location, value));
}

void ShaderProgram::set3f(const std::string & name, float f0, float f1, float f2)
{
	int location = getUniformLocation(name);
	GLCall(glUniform3f(location, f0, f1, f2));
}

void ShaderProgram::set4f(const std::string & name, float f0, float f1, float f2, float f3)
{
	int location = getUniformLocation(name);
	GLCall(glUniform4f(location, f0, f1, f2, f3));
}

void ShaderProgram::set3fv(const std::string & name, const float * values)
{
	int location = getUniformLocation(name);
	GLCall(glUniform3fv(location, 1, values));
}

void ShaderProgram::setMatrix3fv(const std::string & name, const float * values)
{
	int location = getUniformLocation(name);
	GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, values));
}

void ShaderProgram::setMatrix4fv(const std::string & name, const float * values)
{
	int location = getUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, values));
}

void ShaderProgram::setDirLight(const std::string & name, DirLight & light)
{	// TODO: feels very inefficient to do this, but is layout independent
	// find a way to remain layout independent but set uniforms faster
	set3fv(name + ".direction", glm::value_ptr(light.mDirection));
	set3fv(name + ".ambient", glm::value_ptr(light.mAmbient));
	set3fv(name + ".diffuse", glm::value_ptr(light.mDiffuse));
	set3fv(name + ".specular", glm::value_ptr(light.mSpecular));
}

void ShaderProgram::setPointLight(const std::string & name, const PointLight & light)
{
	// TODO: feels very inefficient to do this, but is layout independent
	// find a way to remain layout independent but set uniforms faster
	set3fv(name + ".position", glm::value_ptr(light.mPosition));
	set3fv(name + ".ambient", glm::value_ptr(light.mAmbient));
	set3fv(name + ".diffuse", glm::value_ptr(light.mDiffuse));
	set3fv(name + ".specular", glm::value_ptr(light.mSpecular));
	setf  (name + ".kConstant", light.mKConstant);
	setf  (name + ".kLinear", light.mKLinear);
	setf  (name + ".kQuadratic", light.mKQuadratic);
}

void ShaderProgram::setSpotLight(const std::string & name, const SpotLight & light)
{
	// TODO: feels very inefficient to do this, but is layout independent
	// find a way to remain layout independent but set uniforms faster
	set3fv(name + ".position", glm::value_ptr(light.mPosition));
	set3fv(name + ".direction", glm::value_ptr(light.mDirection));
	setf  (name + ".innerCutOff", light.mInnerCutOff);
	setf  (name + ".outerCutOff", light.mOuterCutOff);
	set3fv(name + ".ambient", glm::value_ptr(light.mAmbient));
	set3fv(name + ".diffuse", glm::value_ptr(light.mDiffuse));
	set3fv(name + ".specular", glm::value_ptr(light.mSpecular));
}


int ShaderProgram::getUniformLocation(const std::string & name)
{
	// TODO: why is caching so slow?

	/*
	auto it = mUniformLocationCache.find(name);
	if (it != mUniformLocationCache.end())
	{
		return it->second;
	}*/

	assert(boundProgram == mRendererID && "setting uniform of unbound shader");

	GLCall(int location = glGetUniformLocation(mRendererID, name.c_str()));
	
	if(location == -1)
		std::cerr << "\nERROR: " << name << " is not a valid uniform\n";

	/*
	if (location != -1)
		mUniformLocationCache.insert({ name, location });
	else
		std::cerr << "\nERROR: " << name << " is not a valid uniform\n";
	*/
	return location;
}
