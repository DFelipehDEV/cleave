#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

// Helper function to load a file into a string
std::string LoadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND (" << path << ")\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::CreateFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = LoadFile(vertexPath);
    if (vertexSource.empty()) {
        std::cerr << "ERROR::SHADER::VERTEX_SHADER_LOAD_FAILED (" << vertexPath << ")\n";
        return;
    }

    std::string fragmentSource = LoadFile(fragmentPath);
    if (fragmentSource.empty()) {
        std::cerr << "ERROR::SHADER::FRAGMENT_SHADER_LOAD_FAILED (" << fragmentPath << ")\n";
        return;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSourceCStr = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX_SHADER_COMPILATION_FAILED (" << vertexPath << ")\n" << infoLog << std::endl;

        // Clean up
        glDeleteShader(vertexShader);
        return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSourceCStr = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT_SHADER_COMPILATION_FAILED (" << fragmentPath << ")\n" << infoLog << std::endl;

        // Clean up 
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        // Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    SetId(program);
}

void Shader::CreateFromString(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSourceCStr = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(vertexShader); // Clean up the failed shader
        return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSourceCStr = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
        
        // Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        // Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    SetId(program);
}

void Shader::Destroy() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}

void Shader::Use() const {
    if (m_id != 0) {
        glUseProgram(m_id);
    } else {
        std::cerr << "ERROR::SHADER::USE::PROGRAM_NOT_CREATED\n";
    }
}

void Shader::SetUniformInt(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform1i(location, value);
}

void Shader::SetUniformFloat(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform1f(location, value);
}

void Shader::SetUniformVector2f(const std::string& name, float x, float y) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform2f(location, x, y);
}

void Shader::SetUniformVector3f(const std::string& name, float x, float y, float z) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform3f(location, x, y, z);
}

void Shader::SetUniformVector4f(const std::string& name, float x, float y, float z, float w) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniform4f(location, x, y, z, w);
}

void Shader::SetUniformMatrix4(const std::string& name, const float* matrix) const {
    GLint location = glGetUniformLocation(m_id, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND (" << name << ")\n";
        return;
    }
    glUniformMatrix4fv(location, 1, false, matrix);
}