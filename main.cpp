#include <iostream>
#include <string>
#include <vector>

/*
Dear ImGui is used as the front end library, a middleware between this application and OpenGL.
Visit: https://github.com/ocornut/imgui

dependencies:
    - glfw3
    - opengl32

Mac OS X:
    brew install glfw
MSYS2:
    pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw

Build with included makefile
*/


#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> 

//#include "src/testcases.cpp"
#include "src/gameobj.hpp"
#include "src/module.hpp"
#include "src/player.hpp"


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

//Error callback for GLFW
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


int main(int, char**) {
    //bind callback function, initialize glfw, and set window hints
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
#if defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    //Get screen size to make game fullscreen
    size_t SCREENWIDTH = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    size_t SCREENHEIGHT = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "PA9 - Asteroids", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    //Set ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::GetStyle().AntiAliasedLines = true;
    ImGui::GetStyle().AntiAliasedFill = true;

    //create gameobject and initialize
    gameObject app(ImVec2(SCREENWIDTH, SCREENHEIGHT));
    app.initGame();
    //Tests playerTests;
    //playerTests.runTests();


    //menu flag
    bool menu = true;

    /******************************Main loop******************************/
    while (!glfwWindowShouldClose(window)) {

        //get io events
        glfwPollEvents();

        //instanciate new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //set window size and position
        ImGui::SetNextWindowSize(ImVec2(SCREENWIDTH, SCREENHEIGHT));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        //window flags
        ImGuiWindowFlags windowConfig = 0;
        windowConfig |= ImGuiWindowFlags_NoTitleBar;
        windowConfig |= ImGuiWindowFlags_NoResize;
        windowConfig |= ImGuiWindowFlags_NoMove;
        windowConfig |= ImGuiWindowFlags_NoCollapse;
        windowConfig |= ImGuiWindowFlags_NoNav;
        windowConfig |= ImGuiWindowFlags_AlwaysAutoResize;
        windowConfig |= ImGuiWindowFlags_NoDecoration;

        //main menu
        if (menu) {
            if (ImGui::Begin("Main Menu", NULL, windowConfig)) {
                int port;
                ImGui::SetWindowFontScale(1.5f);
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 100, SCREENHEIGHT / 2 - 200));
                ImGui::Text("Welcome to Asteroids!");
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 150, (SCREENHEIGHT / 2 - 150)));
                ImGui::Text("Press 'Play' to start the game");
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 60, (SCREENHEIGHT / 2)));
                if (ImGui::Button("How To Play?")) {
                    ImGui::OpenPopup("How To Play");
                }
                if (ImGui::BeginPopup("How To Play")) {
                    ImGui::Text("How To Play:\n");
                    ImGui::Text("The objective of the game is to control a spaceship and survive as long as possible in space while avoiding the enemy ships.\n\n"
                        "Here's a basic guide on how to play Asteroids:\n\n"

                        "Controls:\n"

                        "Use the 'WASD' to control the direction of your spaceship.\n"
                        "Use the mouse to control the direction of the ship and left click to shoot.\n"
                        "Use 'E' to pause the game and view your 'inventory' where you are able to modify your ship"

                        "\n\nGameplay :\n"

                        "Your spaceship is initially in the center of the screen.Enemy ships will appear and and try to shoot you.\n"
                        "Your goal is to shoot and destroy the enemies while avoiding then to stay out of their range.\n"
                        "As more enemies are killed the level will increase and more total enemies will spawn, and spawn faster .\n"

                        "You are able to build you ship with money earned.While in your inventory\n"
                        "clicking a node will allow you to add new modules.Clicking an existing module will give you the option too sell or repair it.\n"
                        "Repairing a module will cost money proportional to the health lost and increase its health. Selling a module will remove it from your ship and give you money back.\n"

                        "Truss modules are the building blocks and allow you to attach other modules to them\n"
                        "Thrusters increase your overall speed and acceleration.\n"
                        "Guns allow you to shoot more bullets at enemies.\n\n"

                        "Scoring : \n"
                        "Points and money are awarded based on the number of enemies killed.\n"
                        "The player will receive 10 points and $100 for each enemy killed.\n\n"
                        "Game Over : \n"

                        "The Game ends when your player gets hit.Modules can be repaired and built but your player cannot");
                    ImGui::EndPopup();
                }
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 20, (SCREENHEIGHT / 2) - 50));
                if (ImGui::Button("Play")) {
                    menu = false;
                }
                ImGui::SetCursorPos(ImVec2(SCREENWIDTH / 2 - 200, (SCREENHEIGHT / 2) - 100));

                ImGui::End();
            }
        }

        //Play Game
        if (menu == false && ImGui::Begin("Main Window", NULL, windowConfig)) {
            //std::cout << "Checkpoint 2" << std::endl;
            ImGui::SetWindowFontScale(1.2f);

            //hud stats
            ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
            ImGui::Text("Bands: %d", app.getPlayer()->getBandCount());
            ImGui::Text("Kills: %d", app.getPlayer()->getKills());
            ImGui::Text("Time Alive: %.2f", app.getPlayer()->getTimeAlive());

            //run game loop
            app.updateGame();
            ImGui::End();
        }

        //Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    //Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
