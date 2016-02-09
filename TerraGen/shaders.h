#pragma once

#include <string>
#include <map>
#include <QOpenGLShaderProgram>

class Shaders
{
public:

    /// <summary>
    /// Initializes all shaders
    /// </summary>
    static bool InitializeShaders();

	/// <summary>
	/// Finds the shader for the given key. 
	/// TODO : Add other keys to comment
	/// </summary>
	/// <param name="key">The name of the shader
	/// Current keys are: 
	/// main
	/// </param>
	/// <returns>Returns a pointer to the Shader object if found</returns>
    static QOpenGLShaderProgram* Find(std::string key);

    /// <summary>
    /// Removes the shader with the key from the collection and deletes the program
    /// </summary>
    static void deleteShader(std::string key);

	/// <summary>
	/// Deletes all shaders and frees the memory.
	/// </summary>
	static void DeleteAll();

    Shaders();
    ~Shaders();
private:
	/// <summary>
	/// Map that stores all shaders created on Initialization
	/// </summary>
    static std::map<std::string, QOpenGLShaderProgram*> ShaderMap;
};
