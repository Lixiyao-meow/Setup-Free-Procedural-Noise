// ----------------------------------------------
// Polytechnique - INF584 "Image Synthesis"
//
// Base code for practical assignments.
//
// Copyright (C) 2022 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------
#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <exception>
#include <ios>

using namespace std;

// Create a GPU program i.e., a graphics pipeline
ShaderProgram::ShaderProgram () : m_id (glCreateProgram ()) {}


ShaderProgram::~ShaderProgram () {
	glDeleteProgram (m_id); 
}

std::string ShaderProgram::file2String (const std::string & filename) {
	std::ifstream input (filename.c_str ());
	if (!input)
		throw std::ios_base::failure ("[Shader Program][file2String] Error: cannot open " + filename);
	std::stringstream buffer;
	buffer << input.rdbuf ();
	return buffer.str ();
}

void ShaderProgram::loadShader (GLenum type, const std::string & shaderFilename) {
	GLuint shader = glCreateShader (type); // Create the shader, e.g., a vertex shader to be applied to every single vertex of a mesh
	std::string shaderSourceString = file2String (shaderFilename); // Loads the shader source from a file to a C++ string
	const GLchar * shaderSource = (const GLchar *)shaderSourceString.c_str (); // Interface the C++ string through a C pointer
	glShaderSource (shader, 1, &shaderSource, NULL); // Load the vertex shader source code
	glCompileShader (shader);  // THe GPU driver compile the shader
	glAttachShader (m_id, shader); // Set the vertex shader as the one ot be used with the program/pipeline
	glDeleteShader (shader);
}

std::shared_ptr<ShaderProgram> ShaderProgram::genBasicShaderProgram (const std::string & vertexShaderFilename,
															 	 	 const std::string & fragmentShaderFilename,
																	 Material & material, 
																	 LightSource & lightSource) {
	std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram> ();
	shaderProgramPtr->loadShader (GL_VERTEX_SHADER, vertexShaderFilename);
	shaderProgramPtr->loadShader (GL_FRAGMENT_SHADER, fragmentShaderFilename);
	shaderProgramPtr->link ();

	shaderProgramPtr->set("material.matAlbedo", material.get_Albedo());
	shaderProgramPtr->set("material.roughness", material.get_roughness());
	shaderProgramPtr->set("material.metallicness", material.get_metallicness());

	shaderProgramPtr->set("lightSource.lightPos[0]", lightSource.get_Pos()[0]);
	shaderProgramPtr->set("lightSource.lightIntensity[0]", lightSource.get_Intensity()[0]);
	shaderProgramPtr->set("lightSource.lightColor[0]", lightSource.get_Color()[0]);
	shaderProgramPtr->set("lightSource.lightPos[1]", lightSource.get_Pos()[1]);
	shaderProgramPtr->set("lightSource.lightIntensity[1]", lightSource.get_Intensity()[1]);
	shaderProgramPtr->set("lightSource.lightColor[1]", lightSource.get_Color()[1]);
	shaderProgramPtr->set("lightSource.lightPos[2]", lightSource.get_Pos()[2]);
	shaderProgramPtr->set("lightSource.lightIntensity[2]", lightSource.get_Intensity()[2]);
	shaderProgramPtr->set("lightSource.lightColor[2]", lightSource.get_Color()[2]);
	
	shaderProgramPtr->set("lightSource.att_const", lightSource.get_Const());
	shaderProgramPtr->set("lightSource.att_lin", lightSource.get_Lin());
	shaderProgramPtr->set("lightSource.att_quad", lightSource.get_quad());

	return shaderProgramPtr;
}