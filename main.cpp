#include <iostream>
#include <string>
#include <vector>


#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "src/gameobj.hpp"
#include "src/module.hpp"
#include "src/player.hpp"


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    //Get screen size to make game fullscreen
    size_t SCREENWIDTH = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    size_t SCREENHEIGHT = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "PA9 - Asteroids", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //module mod1;

    gameObject app(ImVec2(SCREENWIDTH, SCREENHEIGHT));
    player* player1 = app.getPlayer();
    module* mod1 = new truss(ImVec2(0, 0), 0, (void*)player1, &app);
    //gun* newG = new gun(ImVec2(0, 0), 0, player1, &app);
    //mod1->module::setNodeModule((void*)newG, 1);
    player1->setNodeModule((void*)mod1, 0);
    app.addEnemy();
    //printf("Player: %p; Truss parent: %p", player1, mod1->getParent());
    //player1->addGun(newG);
    //app.addEnemy();
    bool menu = true;
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::SetNextWindowSize(ImVec2(SCREENWIDTH, SCREENHEIGHT));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGuiWindowFlags windowConfig = 0;
        windowConfig |= ImGuiWindowFlags_NoTitleBar;
        windowConfig |= ImGuiWindowFlags_NoResize;
        // windowConfig |= ImGuiWindowFlags_NoMove;
        windowConfig |= ImGuiWindowFlags_NoCollapse;
        windowConfig |= ImGuiWindowFlags_NoNav;
        windowConfig |= ImGuiWindowFlags_AlwaysAutoResize;
        windowConfig |= ImGuiWindowFlags_NoDecoration;

        if (menu) {
            if (ImGui::Begin("Main Menu", NULL, windowConfig)) {
                ImGui::SetWindowFontScale(1.5f);
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 100, SCREENHEIGHT / 2 - 100));
                ImGui::Text("Welcome to Asteroids!");
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 150, SCREENHEIGHT / 2));
                ImGui::Text("Press 'Play' to start the game");
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 10, SCREENHEIGHT / 2 + 100));

                if (ImGui::Button("Play")) {
                    //app.createServer();
                    menu = false;
                }
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 10, SCREENHEIGHT / 2 + 100));
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 10, SCREENHEIGHT / 2 + 100));
                ImGui::End();
            }
        }

        if (menu == false && ImGui::Begin("Main Window", NULL, windowConfig)) {

            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("Bands: %d", app.getPlayer()->getBandCount());
            ImGui::Text("Kills: %d", app.getPlayer()->getKills());
            ImGui::Text("Time Alive: %.2f", app.getPlayer()->getTimeAlive());
            //mod1.drawModule();
            app.updateGame();
            ImGui::End();
        }
            // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
