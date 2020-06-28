#include "Shader.h"

//check shader compile status
void checkShaderStatus(GLuint shader)
{
    int success;

    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "shader error shader compile failed " << infoLog << std::endl;
    }
}

//check program link status
void checkProgramStatus(GLuint program)
{
    int success;

    char infoLog[512];
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "program error link failed " << infoLog << std::endl;
    }
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    //read shader from file path
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        //open file
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        //read file cache in data stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //close file processer
        vShaderFile.close();
        fShaderFile.close();
        //send data to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "error shader file not success read" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    //Compile shader
    unsigned int vertex, fragment;

    //vertexshader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkShaderStatus(vertex);

    //fragemnt shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(vertex);
    checkShaderStatus(vertex);

    //shader program
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkProgramStatus(programID);

    //delete shader those shader already link in the program no more need;
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return;
}

void Shader::use()
{
    glUseProgram(programID);
    return;
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID,name.c_str()),(int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID,name.c_str()),value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(programID,name.c_str()),value);
}
