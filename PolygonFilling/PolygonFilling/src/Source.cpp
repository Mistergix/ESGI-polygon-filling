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

#include "Transform.h"
#include "Camera.h"

#include "Dragon.h"

enum MODE { POLYGON, CLIPPING };

int main(void) {
    // ON SUPPOSE SENS TRIGO POUR LE MOMENT

    std::vector<Vector> points;
    points.push_back(Vector(0.0f, 0.0f));
    points.push_back(Vector(1.0f, 0.0f));
    points.push_back(Vector(1.0f, 1.0f));
    points.push_back(Vector(0.0f, 1.0f));
    Polygon poly(points);

    poly.SetTrigonometric(true);

    bool inside = poly.IsInside(Vector(1.5f, 0.5f));

    Sutherland s;

    // TEST INTERSECTION

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


    // TEST INTERSECTION CALCUL POINT
    Vector inter1v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.5f), Vector(3.0f, 0.5f)); // inter sommet du milieu
    Vector inter2v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 0.0f), Vector(3.0f, 0.0f)); // inter sommet du bas
    Vector inter3v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f, 1.0f), Vector(3.0f, 1.0f)); // inter sommet du haut

    Vector inter8v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 2.0f / 5.0f), Vector(4.0f / 5.0f, 3.0f / 5.0f)); // fenetre oblique
    Vector inter9v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(2.0f / 5.0f, 1.0f / 5.0f), Vector(4.0f / 5.0f, 2.0f / 5.0f)); // fenetre oblique, inter bas

    Vector inter11v = s.Intersection(Vector(0.0f, 0.0f), Vector(0.0f, 1.0f), Vector(-1.0f, 0.5f), Vector(1.0f, 0.5f)); // inter segments


    // TEST VISIBLE 
    Polygon subject;

    subject.SetTrigonometric(true);

    subject.Add(Vector(0.0f, 0.0f));
    subject.Add(Vector(1.0f, 0.0f));
    subject.Add(Vector(2.0f, 2.0f));
    subject.Add(Vector(0.5f, 1.0f));
    subject.Add(Vector(-1.0f, 2.0f));

    Polygon clipPolygon;

    clipPolygon.SetTrigonometric(true);

    clipPolygon.Add(Vector(-1.0f, 0.5f));
    clipPolygon.Add(Vector(2.0f, 0.5f));
    clipPolygon.Add(Vector(2.0f, 1.25f));
    clipPolygon.Add(Vector(-1.0f, 1.25f));

    bool visible1 = s.Visible(Vector(0.0f, 0.0f), Vector(-1.0f, 0.5f), Vector(2.0f, 0.5f), clipPolygon); // NOT VISIBLE
    bool visible2 = s.Visible(Vector(0.0f, 0.0f), Vector(2.0f, 0.5f), Vector(2.0f, 1.25f), clipPolygon); // VISIBLE

    bool visible3 = s.Visible(Vector(0.5f, 1.0f), Vector(2.0f, 0.5f), Vector(2.0f, 1.25f), clipPolygon); // VISIBLE
    bool visible4 = s.Visible(Vector(0.5f, 1.0f), Vector(2.0f, 1.25f), Vector(-1.0f, 1.25f), clipPolygon); // VISIBLE
    bool visible5 = s.Visible(Vector(0.5f, 1.0f), Vector(-1.0f, 1.25f), Vector(-1.0f, 0.5f), clipPolygon); // VISIBLE
    bool visible6 = s.Visible(Vector(0.5f, 1.0f), Vector(-1.0f, 0.5f), Vector(2.0f, 0.5f), clipPolygon); // VISIBLE

    bool visible7 = s.Visible(Vector(2.0f, 2.0f), Vector(2.0f, 0.5f), Vector(2.0f, 1.25f), clipPolygon); // VISIBLE
    bool visible8 = s.Visible(Vector(2.0f, 2.0f), Vector(2.0f, 1.25f), Vector(-1.0f, 1.25f), clipPolygon); // NOT VISIBLE
    bool visible9 = s.Visible(Vector(2.0f, 2.0f), Vector(-1.0f, 1.25f), Vector(-1.0f, 0.5f), clipPolygon); // VISIBLE
    bool visible10 = s.Visible(Vector(2.0f, 2.0f), Vector(-1.0f, 0.5f), Vector(2.0f, 0.5f), clipPolygon); // VISIBLE

    ASSERT(! visible1);
    ASSERT(visible2);

    ASSERT(visible3);
    ASSERT(visible4);
    ASSERT(visible5);
    ASSERT(visible6);

    ASSERT(visible7);
    ASSERT(! visible8);
    ASSERT(visible9);
    ASSERT(visible10);

    Polygon cut = s.Clip(subject, clipPolygon);

    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;


    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clipping and Windowing", NULL, NULL);
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
        GLCall(glEnable(GL_DEPTH_TEST));
        Drawing drawing(SCR_WIDTH, SCR_HEIGHT);

        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
        };

        const unsigned int indices[] = {
            0, 1, 2, 3, 4, 5
        };

        // BLENDING

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VERTEX ARRAY

        VertexArray va;

        // VERTEX BUFFER

        VertexBuffer vb(quadVertices, sizeof(quadVertices));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        va.AddBuffer(vb, layout);

        // INDEX BUFFER
        IndexBuffer ib(indices, 6);

        // RENDER TEXTURE
        unsigned int renderTextureId;
        static GLubyte renderTexture[SCR_HEIGHT][SCR_WIDTH][4];
        for (int i = 0; i < SCR_HEIGHT; i++)
        {
            for (int j = 0; j < SCR_WIDTH; j++)
            {
                drawing.DrawPixel(j, i, Color{ 255, 255, 255, 255 }, renderTexture);
            }
        }
        unsigned int slot = 0;
        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GLCall(glGenTextures(1, &renderTextureId));
        GLCall(glActiveTexture(GL_TEXTURE0 + slot))
        GLCall(glBindTexture(GL_TEXTURE_2D, renderTextureId));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));
        

        // SHADER
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        

        // draw as wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        Color polygonColor{ 255, 0, 0, 255 }, windowPolygonColor{ 0, 255, 0, 255 }, cutPolygonColor{ 0, 0, 255, 255 };
        Sutherland sutherland;
        


        bool clicked = false;
        MODE mode = POLYGON;

        bool show_colors = false;

        Transform camTransform, objectTransform;
        Camera cam;
        camTransform.SetPos(0.0f, 0.0f, -10.0f);

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            renderer.Resize(window, &width, &height);

            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            Polygon cutPolygon = sutherland.Clip(polygon, windowPolygon);

            //drawing.DrawPolygon(windowPolygon, windowPolygonColor);
            //drawing.DrawPolygon(polygon, polygonColor);
            //drawing.Fill(cutPolygon, cutPolygonColor);
            /*
            for (int i = 0; i < 100; i++) // height
            {
                for (int j = 0; j < 300; j++) // width
                {
                    drawing.DrawPixel(j, i, windowPolygonColor, renderTexture);
                }
            }*/

            
            
            GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderTexture));
            shader.SetUniform1i("u_Texture", slot);
            

            renderer.Draw(va, ib, shader);

            {
                ImGui::Begin("Polygon clipping and filling", &my_tool_active, ImGuiWindowFlags_MenuBar);
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("Menu"))
                    {
                        if (ImGui::MenuItem("Colors", "")) { 
                            show_colors = !show_colors;  
                        }
                        if (ImGui::MenuItem("Polygon to cut", "")) { 
                            mode = POLYGON; 
                            std::cout << "The mode is " << mode << std::endl; 
                        }
                        if (ImGui::MenuItem("Draw window", "")) { 
                            mode = CLIPPING;  
                            std::cout << "The mode is " << mode << std::endl;
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::End();

                if (show_colors) {
                    ImGui::Begin("Colors");
                    ImGui::SliderInt3("Polygon Color", &polygonColor.r, 0, 255);
                    ImGui::SliderInt3("Window Polygon Color", &windowPolygonColor.r, 0, 255);
                    ImGui::SliderInt3("Cut Polygon Color", &cutPolygonColor.r, 0, 255);
                    ImGui::End();
                }
            }


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            //Swap front and back buffers 
            glfwSwapBuffers(window);

            //Poll for and process events
            glfwPollEvents();

            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
            if (state == GLFW_PRESS)
            {
                if (!clicked) {
                    clicked = true;
                    double xpos, ypos;
                    //getting cursor position
                    glfwGetCursorPos(window, &xpos, &ypos);
                    std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;

                    ypos = SCR_HEIGHT - ypos;
                    std::cout << "Texture Position at (" << xpos << " : " << ypos << std::endl;

                    if (mode == POLYGON) {
                        polygon.Add(Vector(xpos + 0.5f, ypos + 0.5f)); // + 0.5 because we want them centered
                        std::cout << polygon.PointCount() << std::endl;
                    }
                    else if (mode == CLIPPING) {
                        windowPolygon.Add(Vector(xpos + 0.5f, ypos + 0.5f));
                        std::cout << windowPolygon.PointCount() << std::endl;
                    }
                }
            }
            else {
                clicked = false;
            }
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
   }