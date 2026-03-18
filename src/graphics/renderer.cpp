#include "renderer.h"

bool Renderer::init()
{
    float vertices[12] = {0};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // read shaders
    std::string vsSource = readShaderFile("src/graphics/shader/vertex_shader.vs");
    std::string fsSource = readShaderFile("src/graphics/shader/fragment_shader.fs");
    
    // compile shaders
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource.c_str());
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource.c_str());

    // create programs
    shader = glCreateProgram();

    // attach the shaders
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);

    // link program
    glLinkProgram(shader);

    // delete unused shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}

void Renderer::drawQuad(const Quad& q)
{
    // 2 triangles for 1 quad
    float vertices[] =
    {
        q.x1, q.y1,
        q.x2, q.y1,
        q.x2, q.y2,

        q.x1, q.y1,
        q.x2, q.y2,
        q.x1, q.y2
    };

    glUseProgram(shader);

    glUniform3f(
        glGetUniformLocation(shader, "color"),
        q.r, q.g, q.b
    );

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        sizeof(vertices),
        vertices
    );

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

std::string readShaderFile(const char* path)
{
    std::cout << std::filesystem::current_path() << std::endl;
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Failed to open shader: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

GLuint compileShader(GLuint type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    return shader;
}