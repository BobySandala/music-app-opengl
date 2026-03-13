#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "graphics/quad.h"
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <iostream>

std::string readShaderFile(const char* path);
GLuint compileShader(GLenum type, const char* source);

class Renderer
{
public:
    bool init();
    void drawQuad(const Quad& quad);

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int shader;
};
