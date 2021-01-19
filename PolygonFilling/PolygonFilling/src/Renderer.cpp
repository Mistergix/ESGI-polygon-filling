#include "Renderer.h"
#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    bool show = false;
    if (show) {
        std::cout << function <<
            " " << file << ":" << line << std::endl;
    }

    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Resize(GLFWwindow* window, int* width, int* height)
{
    glfwGetWindowSize(window, width, height);

    GLCall(glViewport(0, 0, *width, *height));
}

void Renderer::Clear() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLenum mode) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    /*if (mode == GL_POLYGON) {
        glPolygonMode(GL_FRONT, GL_LINE);
    }
    if (mode == GL_POINT) {
        glPointSize(5.f);
        
    }
    if (mode == GL_LINES) {
        glLineWidth(5.f);
    }*/

    GLCall(glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
