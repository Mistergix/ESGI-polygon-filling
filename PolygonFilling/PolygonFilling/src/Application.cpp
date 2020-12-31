#include <iostream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Dragon.h"

#include <math.h>

#define PI 3.141592653589793f

struct Vector4
{
    float x, y, z, w;
};

static Vector4 GetPosition(const float* vertices, int count) {
    float x = 0.f, y = 0.f, z = 0.f;
    for (int i = 0; i < count; i += 8)
    {
        x += vertices[i];
        y += vertices[i + 1];
        z += vertices[i + 2];
    }
    float denom = count / 8.f;
    return Vector4{ x / denom, y / denom, z / denom, 1.f };
}

struct Light {
    glm::vec3 position;
    glm::vec3 intensities; //a.k.a. the color of the light
};


struct Vertex {
    Vector4 Position;
};

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Couscous Tajine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // vsync
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    {
        Vertex vertices[] = {
            {{-50.0f, -50.0f, 0.0f, 1.0f}},
            {{50.0f, -50.0f, 0.0f, 1.0f}},
            {{50.0f, 50.0f, 0.0f, 1.0f}},
            {{-50.0f, 50.0f, 0.0f, 1.0f}}
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // BLENDING

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VERTEX ARRAY

        VertexArray va;

        // VERTEX BUFFER

        VertexBuffer vb(DragonVertices, sizeof(DragonVertices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        // INDEX BUFFER
        IndexBuffer ib(DragonIndices, 45000);

        // MATRIX
        //glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
        //::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); // move cam to the right (so we have to translate to the left)

        // SHADER
        Shader shader("res/shaders/transform.shader");
        shader.Bind();

        //TEXTURE
        Texture texture("res/textures/tricolor.png");
        unsigned int slot = 0;
        texture.Bind(slot);
        shader.SetUniform1i("u_Texture", slot);
        shader.SetUniform1f("u_Shininess", 10);
        shader.SetUniform3f("u_SpecularColor", 1.f, 1.f, 1.f);

        // UNBIND
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        Vector4 dragonPos{ 0.f, 0.f, 15.f };
        Vector4 camPos{ 0.f, 15.f, 0.f };
        Light gLight;

        gLight.position = glm::vec3(camPos.x, camPos.y, camPos.z);
        gLight.intensities = glm::vec3(1, 1, 1); //white

        glm::vec3 translation(200, 200, 0);
        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            renderer.Resize(window, &width, &height);
            renderer.Clear();

            //glm::mat4 model = glm::translate(glm::mat4(1.0f), translation); // object translate up and right
            //MODEL = TRANSLATE * ROTATION * SCALE
            //glm::mat4 mvp = proj * view * model; // reversed because opengl
            float time = static_cast<float>(glfwGetTime());
            //time = 0.f;

            float translationMatrix[] = {
                1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                dragonPos.x, dragonPos.y, dragonPos.z, 1.f
            };
            float rotationMatrixX[] = {
            1.f, 0.f, 0.f, 0.f, // 1ere colonne
            0.f, cos(time), sin(time), 0.f, // 2eme colonne
            0.f, -sin(time), cos(time), 0.f, // 3eme colonne
            0.f, 0.f, 0.f, 1.f // 4eme colonne
            };
            float rotationMatrixY[] = {
            cos(time), 0.f, -sin(time), 0.f, // 1ere colonne
            0.f, 1.f, 0.f, 0.f, // 2eme colonne
            sin(time), 0.f, cos(time), 0.f, // 3eme colonne
            0.f, 0.f, 0.f, 1.f // 4eme colonne
            };
            float rotationMatrixZ[] = {
            cos(time), sin(time), 0.f, 0.f, // 1ere colonne
            -sin(time), cos(time), 0.f, 0.f, // 2eme colonne
            0.f, 0.f, 1.f, 0.f, // 3eme colonne
            0.f, 0.f, 0.f, 1.f // 4eme colonne
            };
            float scaleMatrix[] = {
                1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f
            };

            // proj
            float fov = 45.f * 3.14159f / 180.f;
            float f = 1.f / tan(fov / 2.f);
            float a = (float)width / (float)height;
            float far = 100.f;
            float near = 0.5f;
            float projectionMatrix[] = {
                f / a, 0.f, 0.f, 0.f, // 1ere colonne
                0.f, f, 0.f, 0.f, // 2eme colonne
                0.f, 0.f, (far + near) / (near - far), -1.f, // 3eme colonne
                0.f, 0.f, (2.f * far * near) / (near - far), 0.f // 4eme colonne
            };

            shader.Bind();

            shader.SetUniformMat4f("u_TranslationMatrix", translationMatrix);
            shader.SetUniformMat4f("u_RotationMatrix", rotationMatrixY);
            shader.SetUniformMat4f("u_ScaleMatrix", scaleMatrix);

            shader.SetUniform3f("u_CameraPosition", camPos.x, camPos.y, camPos.z);
            shader.SetUniform3f("u_TargetPosition", dragonPos.x, dragonPos.y, dragonPos.z);

            shader.SetUniformMat4f("u_ProjectionMatrix", projectionMatrix);

            shader.SetUniform3f("light.position", gLight.position.x, gLight.position.y, gLight.position.z);
            shader.SetUniform3f("light.intensities", gLight.intensities.x, gLight.intensities.y, gLight.intensities.z);


            renderer.Draw(va, ib, shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}