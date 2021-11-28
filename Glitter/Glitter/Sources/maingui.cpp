// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "flightcontrol.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include "ysglfontdata.h"
#include "StringPlus.h"
#include "yssimplesound.h"
#include "yspng.h"
#include <string>
#include <limits.h>
#include <iostream>
#include <filesystem>
#include <glm/gtx/norm.hpp>
#include "SimObject.h"
#include "obstacleavoid.h"

// include gui library
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//#include "yssimplesound.h"
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, uav* UAV_fc);
void drawGrid();

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.f, 10.f, 10.f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//// sound player
//bool canPlaySound = false;
string getCurrentDir() {
    char buff[MAX_PATH];
    GetModuleFileName(NULL, buff, MAX_PATH);
    string::size_type position = string(buff).find_last_of("\\/");
    return string(buff).substr(0, position);
}
int main(int, char**)
{
    string Path_to_Project = getCurrentDir();
    cout << "Current working directory : " << Path_to_Project << endl;
    // when compiling using visual studio, current directory is usually .../[Path to your project]/Glitter/Build/Debug
    // we can use feature to cd to shader files

    string t = "Glitter";
    string::size_type i = Path_to_Project.find(t);
    // delete everything after(including) "Glitter"
    if (i != string::npos)
        Path_to_Project.erase(i, Path_to_Project.length() - i);
    cout << "Path to Project is: " << Path_to_Project << endl;
    std::replace(Path_to_Project.begin(), Path_to_Project.end(), '\\', '/');

    string Path_to_Shader1 = Path_to_Project + "Glitter/Glitter/Shaders/modelvs.vs";
    const char* path1 = Path_to_Shader1.c_str(); // convert string to char, because Shader class input has to be char
    string Path_to_Shader2 = Path_to_Project + "Glitter/Glitter/Shaders/modelfs.fs";
    const char* path2 = Path_to_Shader2.c_str();


    // load city and uav model
    // Declear UAV Model
    string Path_to_Model = Path_to_Project + "Glitter/Glitter/Model/UAV/quadcop.obj";
    string Path_to_Sound1 = Path_to_Project + "Glitter/Glitter/Sounds/UAV1.wav";
    string Path_to_Sound2 = Path_to_Project + "Glitter/Glitter/Sounds/UAV2.wav";

    // Declear City Model
    string Path_to_City1 = Path_to_Project + "Glitter/Glitter/Model/Building/building01.obj";
    string Path_to_City2 = Path_to_Project + "Glitter/Glitter/Model/Building/building02.obj";
    string Path_to_City3 = Path_to_Project + "Glitter/Glitter/Model/Building/building03.obj";

    //setup sound
    YsSoundPlayer player1;
    YsSoundPlayer::SoundData myWav1;

    // store the filename of music
    string fileNames[] = { Path_to_Sound1, Path_to_Sound2 };

    // Setup window
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
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //// tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //// glad: load all OpenGL function pointers
    //// ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    Shader ourShader(path1, path2);
    cout << "shader loaded" << endl;

    // load flight control and dynamics model
    // SimObject init: file path, scalar, position
    uav UAV_fc(Path_to_Model, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0., 8., 0.));

    float volume;

    if (YSOK == myWav1.LoadWav(fileNames[1].c_str())) {
        player1.Start();
        player1.SetVolume(myWav1, 0.5);
        player1.PlayBackground(myWav1);
    }
    else {
        cout << "Failed to read " << "UAV1.wav" << endl;
    }
    // City model
    obstacle CITY1(4.0f, Path_to_City1, glm::vec3(1.0f, 4.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    obstacle CITY2(4.0f, Path_to_City2, glm::vec3(1.0f, 3.0f, 1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    obstacle CITY3(4.0f, Path_to_City3, glm::vec3(1.0f, 3.5f, 1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            ImVec2 pos(100, 100);
            ImVec2 size(600, 500);
            bool Ankit = true; // Control if we need a dumy bool

            ImGui::Begin("Model Menu", &Ankit, ImGuiWindowFlags_NoTitleBar);                          // Create a window called "HModel Menu" and model should be loaded into it.

            ImGuiWindow* window = ImGui::GetCurrentWindow();

            ImGui::SetWindowPos(pos);
            ImGui::SetWindowSize(size);
            // processInput(window, &UAV_fc);

            // render
            // ------
            //glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
            // glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            // enable shader before setting uniforms
            ourShader.use();

            // camera/view transformation
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();

            // view/projection transformations
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);



            // draw UAV (physical update is integrated in class)
            UAV_fc.Draw(ourShader);


            // draw city
            CITY1.Draw(ourShader);

            CITY2.Draw(ourShader);

            CITY3.Draw(ourShader);

            // copy obstacle
            obstacle CITY4 = CITY3;
            CITY4.UpdatePos(glm::vec3(0.0f, 0.0f, 3.0f));
            CITY4.UpdateScale(glm::vec3(1.f, 4.f, 1.f));
            CITY4.Draw(ourShader);
            ImGui::End();
        }
        // manu
        {
            ImVec2 pos(0, 0);
            ImVec2 size(200, 100);
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGuiWindow* window = ImGui::GetCurrentWindow();

            ImGui::SetWindowPos(pos);
            ImGui::SetWindowSize(size);
            ImGui::End();
        }
         // 3. Show another simple window.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}
        ////////////////////////////////////
        if (UAV_fc.GetNormVel() > 0)
            volume = UAV_fc.GetNormVel() / 2 + 0.5;
        else
            volume = 0.5;
        //cout << UAV_fc.GetNormVel() << endl;
        player1.SetVolume(myWav1, volume);
        // per-frame time logic
        // --------------------

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        UAV_fc.getdeltatime(deltaTime);
        // cout << currentFrame << endl;

        // input
        // -----
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //glfwSwapBuffers(window);
        //glfwPollEvents();
        ////////////////////////////////////

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window, uav* UAV_fc)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        UAV_fc->forward();
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        UAV_fc->backward();
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        UAV_fc->left();
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        UAV_fc->right();
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        UAV_fc->up();
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        UAV_fc->down();
    else if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
        UAV_fc->yawright();
    else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
        UAV_fc->yawleft();
    else
        UAV_fc->hold();
    UAV_fc->dynamics();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
