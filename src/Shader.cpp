#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
using namespace std;
string Shader::locationEquals(int& input) {
    input++;
    return "layout (location = " + to_string(input) + ") ";
}
void Shader::initShader(
    ColorOptions color, TextureOptions texture, bool transform, unsigned int numTextures
) {
    if(color == ColorOptions::NONE && texture == TextureOptions::NONE) {
        cout << "error generating texture: either a color or a texture must exist!" << endl;
        return;
    }
    if(texture == TextureOptions::NONE && numTextures > 0) {
        cout << "error generating texture: if this shader doesn't accept textures, there cannot be any textures." << endl;
    }
    string vertexCode;
    vertexCode += "#version 330 core\n";
    int location = -1;
    vertexCode += this->locationEquals(location) + "in vec3 aPos;\n";
    if(color == ColorOptions::VARIABLE_NO_LIGHT || color == ColorOptions::VARIABLE_WITH_LIGHT) {
        vertexCode += this->locationEquals(location) + "in vec3 aColor;\n";
        vertexCode += "out vec3 color;\n";
    }
    if(texture != TextureOptions::NONE) {
        vertexCode += this->locationEquals(location) + "in vec2 aTexture;\n";
        vertexCode += "out vec2 TexCoord;\n";
    }
    if(numTextures > 0) {
        vertexCode += this->locationEquals(location) + "in uint texIndex;\n";
        vertexCode += "out uint TexIndex;\n";
        if(texture == TextureOptions::MIX) {
            vertexCode += this->locationEquals(location) + "in uint texIndex2;\n";
            vertexCode += "out uint TexIndex2;\n";
        }
    }
    if(transform) {
        vertexCode += "uniform mat4 transform;\n";
    }
    vertexCode += "void main() {\n";
    vertexCode += "gl_Position = ";
    if(transform) {
        vertexCode += "transform * ";
    }
    vertexCode += "vec4(aPos, 1.0);\n";
    if(color == ColorOptions::VARIABLE_NO_LIGHT || color == ColorOptions::VARIABLE_WITH_LIGHT) {
        vertexCode += "color = aColor;\n";
    }
    if(texture != TextureOptions::NONE) {
        vertexCode += "TexCoord = aTexture;\n";
    }
    if(numTextures > 0) {
        vertexCode += "TexIndex = texIndex;\n";
        if(texture == TextureOptions::MIX) {
            vertexCode += "TexIndex2 = texIndex2;\n";
        }
    }
    vertexCode += "}";

    string fragmentCode;
    fragmentCode += "#version 330 core\n";
    fragmentCode += "out vec4 FragColor;\n";
    if(color == ColorOptions::VARIABLE_NO_LIGHT || color == ColorOptions::VARIABLE_WITH_LIGHT) {
        fragmentCode += "in vec3 color;\n";
        if(color == ColorOptions::VARIABLE_WITH_LIGHT) {
            fragmentCode += "uniform vec3 light;\n";
        }
    } else if(color == ColorOptions::UNIFORM) {
        fragmentCode += "uniform vec3 color;\n";
    }
    if(texture != TextureOptions::NONE) {
        fragmentCode += "in vec2 TexCoord;\n";
        fragmentCode += "uniform sampler2D texture1";
        if(numTextures > 0) {
            fragmentCode += "[" + std::to_string(numTextures) + "]";
        }
        fragmentCode += ";\n";
        if(texture == TextureOptions::MIX) {
            if(numTextures <= 0) {
                fragmentCode += "uniform sampler2D texture2;\n";
            }
            fragmentCode += "uniform float mixPercent;\n";
        }
    }
    if(numTextures > 0) {
        fragmentCode += "in uint TexIndex;\n";
        if(texture == TextureOptions::MIX) {
            fragmentCode += "in uint TexIndex2;\n";
        }
    }
    fragmentCode += "void main() {\n";
    fragmentCode += "FragColor = ";
    if(texture == TextureOptions::MIX) {
        fragmentCode += "mix(";
    }
    if(texture != TextureOptions::NONE) {
        fragmentCode += "texture(texture1";
        if(numTextures > 0) {
            fragmentCode += "[TexIndex]";
        }
        fragmentCode == ", TexCoord)";
    }
    if(texture == TextureOptions::MIX) {
        fragmentCode += ", texture(texture2";
        if(numTextures > 0) {
            fragmentCode += "[TexIndex2]";
        }
        fragmentCode += ", TexCoord), mixPercent)";
    }
    if(texture != TextureOptions::NONE && color != ColorOptions::NONE) {
        fragmentCode += " * ";
    }
    if(color != ColorOptions::NONE) {
        fragmentCode += "vec4(";
        if(color == ColorOptions::VARIABLE_WITH_LIGHT) {
            fragmentCode += "light * "; 
        }
        fragmentCode += "color, 1.0)";
    }
    fragmentCode += ";\n";
    fragmentCode += "}";
    // std::cout << vertexCode << std::endl;
    // std::cout << fragmentCode << std::endl;
    this->initShader(vertexCode, fragmentCode);
}
Shader::Shader(ColorOptions color, TextureOptions texture, bool transform, unsigned int numTextures) {
    this->initShader(color, texture, transform, numTextures);
}
Shader::Shader(ColorOptions color, bool transform) {
    this->initShader(color, TextureOptions::NONE, transform, 0);
}
Shader::Shader(TextureOptions texture, bool transform, unsigned int numTextures) {
    this->initShader(ColorOptions::NONE, texture, transform, numTextures);
}
Shader::Shader(ColorOptions color) {
    this->initShader(color, TextureOptions::NONE, true, 0);
}
Shader::Shader(TextureOptions texture, unsigned int numTextures) {
    this->initShader(ColorOptions::NONE, texture, true, numTextures);
}
Shader::Shader(ColorOptions color, TextureOptions texture, unsigned int numTextures) {
    this->initShader(color, texture, true, numTextures);
}
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    } catch(ifstream::failure e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << strerror(errno) << endl;
    }
    this->initShader(vertexCode, fragmentCode);
}
Shader::Shader(string vertexCode, string fragmentCode) {
    this->initShader(vertexCode, fragmentCode);
}
void Shader::initShader(string vertexCode, string fragmentCode) {
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return;
    }
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
        return;
    }
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        cout << "ERROR::SHADERPROGRAM::GENERATION_FAILED\n" << infoLog << endl;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  
}
void Shader::end() {
    glDeleteProgram(program);
}
void Shader::use() {
    glUseProgram(program);
}
GLuint Shader::getLocation(const char* name) {
    return glGetUniformLocation(program, name);
}
void Shader::transform(glm::mat4 trans, const char* transString) {
    GLuint transformLoc = this->getLocation(transString);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}
void Shader::sendVec3f(float a, float b, float c, const char* vecString) {
    GLuint vecLoc = this->getLocation(vecString);
    glUniform3f(vecLoc, a, b, c);
}