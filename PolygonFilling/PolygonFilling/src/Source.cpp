#include <iostream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <math.h>

#include "Maths/Polygon.h"
#include "Maths/Sutherland.h"
#include "Drawing/Drawing.h"

#define PI 3.141592653589793f



int main(void) {
    std::vector<Vector> points;
    points.push_back(Vector(0.0f, 0.0f));
    points.push_back(Vector(1.0f, 0.0f));
    points.push_back(Vector(1.0f, 1.0f));
    points.push_back(Vector(0.0f, 1.0f));
    Polygon poly(points);

    bool inside = poly.IsInside(Vector(1.5f, 0.5f));

    Sutherland s;

    bool inter1 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.5f), Vector(3.0f, 0.5f)); // inter sommet du milieu
    bool inter2 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(3.0f, 0.0f)); // inter sommet du bas
    bool inter3 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 1.0f), Vector(3.0f, 1.0f)); // inter sommet du haut

    bool inter4 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, -1.0f), Vector(3.0f, -1.0f)); // en dessous
    bool inter5 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 2.0f), Vector(3.0f, 2.0f)); // au dessus

    bool inter6 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(2.0f, 2.0f)); // paralelle
    bool inter7 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(0.0f, -1.0f), Vector(0.0f, 0.5f)); // confondue

    bool inter8 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 2.0f / 5.0f), Vector(4.0f / 5.0f, 3.0f / 5.0f)); // fenetre oblique
    bool inter9 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 1.0f / 5.0f), Vector(4.0f / 5.0f, 2.0f / 5.0f)); // fenetre oblique, inter bas
    bool inter10 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 0.0f / 5.0f), Vector(4.0f / 5.0f, 1.0f / 5.0f)); // fenetre oblique pas d'inter

    bool inter11 = s.Cut(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(-1.0f, 0.5f), Vector(1.0f, 0.5f)); // inter segments

    ASSERT(inter1);
    ASSERT(inter2);
    ASSERT(inter3);
    ASSERT(! inter4);
    ASSERT(! inter5);
    ASSERT(! inter6);
    ASSERT(! inter7);
    ASSERT(inter8);
    ASSERT(inter9);
    ASSERT(! inter10);
    ASSERT(inter11);

    Vector inter1v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.5f), Vector(3.0f, 0.5f)); // inter sommet du milieu
    Vector inter2v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(3.0f, 0.0f)); // inter sommet du bas
    Vector inter3v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 1.0f), Vector(3.0f, 1.0f)); // inter sommet du haut

    Vector inter8v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 2.0f / 5.0f), Vector(4.0f / 5.0f, 3.0f / 5.0f)); // fenetre oblique
    Vector inter9v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 1.0f / 5.0f), Vector(4.0f / 5.0f, 2.0f / 5.0f)); // fenetre oblique, inter bas

    Vector inter11v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(-1.0f, 0.5f), Vector(1.0f, 0.5f)); // inter segments

    return 0;
    /*
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;


    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(1000, 1000, "Clipping and Windowing", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current 
    glfwMakeContextCurrent(window);

    // vsync
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    {
        const float vertices[] = {
            0.0f, 0.0f
        };

        const unsigned int indices[] = {
            0
        };

        // BLENDING

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VERTEX ARRAY

        VertexArray va;

        // VERTEX BUFFER

        VertexBuffer vb(vertices, sizeof(vertices));

        VertexBufferLayout layout;
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        // INDEX BUFFER
        IndexBuffer ib(indices, 1);

        // SHADER
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        // UNBIND
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // IMGUI

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);


        bool my_tool_active = true;
        Polygon polygon, windowPolygon, currentPolygon;
        Color polygonColor, windowPolygonColor, cutPolygonColor;
        Sutherland sutherland;
        Drawing drawing(window);

        currentPolygon = polygon;

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            renderer.Resize(window, &width, &height);

            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            //Polygon cutPolygon = sutherland.Clip(polygon, windowPolygon);

            //drawing.DrawPolygon(windowPolygon, windowPolygonColor);
            //drawing.DrawPolygon(polygon, polygonColor);
            //drawing.Fill(cutPolygon, cutPolygonColor);

            renderer.Draw(va, ib, shader);

            {
                ImGui::Begin("Polygon clipping and filling", &my_tool_active, ImGuiWindowFlags_MenuBar);
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("Menu"))
                    {
                        if (ImGui::MenuItem("Colors", "")) {  }
                        if (ImGui::MenuItem("Polygon to cut", "")) {  }
                        if (ImGui::MenuItem("Draw window", "")) {  }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            //Swap front and back buffers 
            glfwSwapBuffers(window);

            //Poll for and process events
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
    */
}