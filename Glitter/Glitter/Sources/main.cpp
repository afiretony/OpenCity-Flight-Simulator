// include basic library
#include <string>
#include <iostream>
#include <filesystem>

// include graphics and sound library(others)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <model.h>
#include "ysglfontdata.h"
#include "StringPlus.h"
#include "yssimplesound.h"
#include "yspng.h"
#include <limits.h>


// include our own library
#include "flightcontrol.h"
#include <camera.h>
#include "SimObject.h"
#include "obstacleavoid.h"
#include "map.h"
#include "detector.h"

// include gui library
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
// #include "imgui_impl_opengl3_loader.h"
//#define STB_IMAGE_IMPLEMENTATION

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, uav* UAV_fc);
void GameWindow(string Path_to_Project);

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
void StartInterface(GLFWwindow* window, ImVec4 clear_color, GLuint bg_img_texture, GLuint title_img_texture, GLuint start_btn_img_texture);

void GuiWindow(const char* glsl_version, string Path_to_Project);
// settings
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;

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

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{
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

    // find path to project for different user
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
    

    // load user interface
    GuiWindow(glsl_version, Path_to_Project);
    // load Game
    GameWindow(Path_to_Project);
    
    return 0;
}
void GuiWindow(const char* glsl_version, string Path_to_Project)
{
    // Create window with graphics context
    // GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        exit(-1);
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", primary, NULL);

    glfwSetWindowMonitor(window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
    
    SCR_WIDTH = mode->width;
    SCR_HEIGHT = mode->height;
    //GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        exit(-1);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    glfwMakeContextCurrent(window);


    //// tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //// glad: load all OpenGL function pointers
    //// ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
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

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // Our state
    bool show_demo_window = false;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /****** Load Images for the start-up menu ***************/
    //Load our texture for background
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint bg_img_texture = 0;
    GLuint title_img_texture = 0;
    GLuint start_btn_img_texture = 0;

    bool ret = LoadTextureFromFile((Path_to_Project + string("/figures/city_nobg.png")).c_str(), &bg_img_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
    ret = LoadTextureFromFile((Path_to_Project + string("/figures/start_interface-removebg.png")).c_str(), &title_img_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);
    ret = LoadTextureFromFile((Path_to_Project + string("/figures/start_button_nobg.png")).c_str(), &start_btn_img_texture, &my_image_width, &my_image_height);
    IM_ASSERT(ret);


    bool show_dropdown_menu = false;


    // Main loop
    StartInterface(window, clear_color, bg_img_texture, title_img_texture, start_btn_img_texture);
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void StartInterface(GLFWwindow* window, ImVec4 clear_color, GLuint bg_img_texture, GLuint title_img_texture, GLuint start_btn_img_texture) {

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

        // start button window
        //{
        //    ImVec2 pos_offset(SCR_WIDTH / 2, SCR_HEIGHT * 0.75); // center location of the window
        //    ImVec2 button_size(200, 100);  // size of the window
        //    ImVec2 button_pos(pos_offset.x - size.x / 2, pos_offset.y - size.y / 2); // top-left cornor of the win

        //    ImGui::SetNextWindowPos(pos);
        //    ImGui::SetNextWindowSize(size);
        //    ImGui::Begin("Title", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        //    ImGui::ImageButton((void*)(intptr_t)start_btn_img_texture, size);
        //    ImGui::End();
        //}
           // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (true)
        //     ImGui::ShowDemoWindow(&show_demo_window);
        // title window
        {
            static int counter = 0;

            ImVec2 pos_offset(SCR_WIDTH / 2, SCR_HEIGHT / 2); // center location of the window
            ImVec2 size(SCR_WIDTH * 0.8, SCR_HEIGHT * 0.8);  // size of the window
            ImVec2 pos(pos_offset.x - size.x / 2, pos_offset.y - size.y / 2);

            ImVec2 titleImgSize(size.x, size.y * 0.8);
            ImVec2 btnSize(size.x * 0.2, size.y * 0.2);

            ImGui::SetNextWindowPos(pos);
            ImGui::SetNextWindowSize(size);
            ImGui::Begin("Title", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::Image((void*)(intptr_t)title_img_texture, titleImgSize);
            ImGui::SetCursorPosX(pos_offset.x - btnSize.x);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - btnSize.y);
            if (ImGui::ImageButton((ImTextureID)start_btn_img_texture, btnSize)) {
                cout << "I Love 24780 soooooo much!!!! Let's Play the game!";
                // counter ++;
                break;
                //show_dropdown_menu ^= true;
            }
            //if(show_dropdown_menu){
            //// if (counter % 5 == 0)
            //    if (ImGui::Button("I")){
            //        cout<<"I"<<endl;
            //    }
            //    ImGui::SameLine();
            //    if (ImGui::Button("Love")){
            //        cout<<"Love\n";
            //    }
            //    ImGui::SameLine();
            //    if (ImGui::Button("24")){
            //        cout<<"24\n";
            //    }
            //    ImGui::SameLine();
            //    if (ImGui::Button("780")){
            //        cout<<"780\n";
            //    }
            //}
            ImGui::End();
        }

        //Try to load the image into ImGui
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("OpenGL Texture Text", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::Image((void*)(intptr_t)bg_img_texture, ImVec2(SCR_WIDTH, SCR_HEIGHT));
        ImGui::End();



        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
              //ImGui::ShowDemoWindow(&show_demo_window);

        //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        //{
        //   ImVec2 pos(100, 100);
        //   ImVec2 size(600, 500);
        //   bool Ankit = true; // Control if we need a dumy bool

        //   ImGui::Begin("Model Menu", &Ankit, ImGuiWindowFlags_NoTitleBar);                          // Create a window called "HModel Menu" and model should be loaded into it.

        //   ImGuiWindow* window = ImGui::GetCurrentWindow();

        //   ImGui::SetWindowPos(pos);
        //   ImGui::SetWindowSize(size);
        //   ImGui::End();
        //}
        // menu
        //{
        //    imvec2 pos(600, 0);
        //    //imvec2 size(200, 100);
        //    static float f = 0.0f;
        //    static int counter = 0;

        //    imgui::begin("hello, world!");                          // create a window called "hello, world!" and append into it.

        //    imgui::text("this is some useful text.");               // display some text (you can use a format strings too)
        //    imgui::checkbox("demo window", &show_demo_window);      // edit bools storing our window open/close state
        //    imgui::checkbox("another window", &show_another_window);

        //    imgui::sliderfloat("float", &f, 0.0f, 1.0f);            // edit 1 float using a slider from 0.0f to 1.0f
        //    imgui::coloredit3("clear color", (float*)&clear_color); // edit 3 floats representing a color

        //    if (imgui::button("button"))                            // buttons return true when clicked (most widgets return true when edited/activated)
        //        counter++;
        //    imgui::sameline();
        //    imgui::text("counter = %d", counter);

        //    imgui::text("application average %.3f ms/frame (%.1f fps)", 1000.0f / imgui::getio().framerate, imgui::getio().framerate);
        //    imguiwindow* window = imgui::getcurrentwindow();

        //    imgui::setwindowpos(pos);
        //    //imgui::setwindowsize(size);
        //    imgui::end();
        //}
        //  // 3. show another simple window.
        // if (show_another_window)
        // {
        //     imgui::begin("another window");   // pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //     imgui::text("hello from another window!");
        //     if (imgui::button("close me"))
        //         show_another_window = false;
        //     imgui::end();
        // }


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

}
// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    //int tmp = GL_TEXTURE_2D;
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void GameWindow(string Path_to_Project)
{

    string Path_to_Shader1 = Path_to_Project + "Glitter/Glitter/Shaders/modelvs.vs";
    const char* path1 = Path_to_Shader1.c_str(); // convert string to char, because Shader class input has to be char
    string Path_to_Shader2 = Path_to_Project + "Glitter/Glitter/Shaders/modelfs.fs";
    const char* path2 = Path_to_Shader2.c_str();


    // load city and uav model
    // Declear UAV Model
    string Path_to_Model = Path_to_Project + "Glitter/Glitter/Model/UAV2/uploads_files_893841_drone.obj";
    string Path_to_Sound1 = Path_to_Project + "Glitter/Glitter/Sounds/UAV1.wav";
    string Path_to_Sound2 = Path_to_Project + "Glitter/Glitter/Sounds/UAV2.wav";

    glfwInit();
    // Use OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);  // Enables sRGB framebuffer capability

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engineering Republic", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    // tell GLFW to make the context of our window the main context on the current thread
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);  // Actually enables sRGB framebuffer

    // load UAV sound effect
    YsSoundPlayer player1;
    YsSoundPlayer::SoundData myWav1;

    // store the filename of music
    string fileNames[] = { Path_to_Sound1, Path_to_Sound2 };

    if (true) {
        // build and compile shaders
        // -------------------------

        Shader ourShader(path1, path2);
        cout << "shader loaded" << endl;

        // load flight control and dynamics model
        // SimObject init: file path, scalar, position
        auto UAV_fc = new uav(Path_to_Model, glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(-10., 0., -0.));  //second glm change initial landed location of UAV

        // initialize UAV sound effects
        //set initial volume at first display of UAV
        float volume;
        // load user choice, note use of .c_str()
        if (YSOK == myWav1.LoadWav(fileNames[1].c_str())) {
            player1.Start();
            player1.SetVolume(myWav1, 0.5);
            player1.PlayBackground(myWav1);
        }
        else {
            cout << "Failed to read " << "UAV1.wav" << endl;
        }

        // City model
        const int maxID = 10;
        auto cityMap = new Map(14, 14, maxID, Path_to_Project);

        // Obstacle detector
        detector avoid(UAV_fc, cityMap);

        //intialize point of view status
        bool firstPOV = true;

        // camera parameters
        float distance = 0.5;
        glm::vec3 cameraPosition;
        float cameraYaw, cameraPitch, cameraRoll;
        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {

            // program dynamic UAV sound effect volume
            if (UAV_fc->GetNormVel() > 0)
                volume = UAV_fc->GetNormVel() / 2 + 0.5;
            else
                volume = 0.5;
            //cout << UAV_fc.GetNormVel() << endl;
            player1.SetVolume(myWav1, volume);
            // per-frame time logic
            // --------------------

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            UAV_fc->getdeltatime(deltaTime);
            // cout << currentFrame << endl;

            // input
            // -----
            processInput(window, UAV_fc);

            // obstacle avoid
            // -----


            // render
            // ------
            //glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
            glClearColor(0.8f, 0.5f, 0.f, 1.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            // enable shader before setting uniforms
            ourShader.use();

            // camera/view transformation
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();

            // view/projection transformations
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);


            // specificy Z and X key to switch between first and third POV
            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                firstPOV = true;
            else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                firstPOV = false;

            //distance from camera to UAV
            cameraPosition = UAV_fc->getUavPos();
            cameraYaw = UAV_fc->getUavTwist().y;
            cameraPitch = UAV_fc->getUavTwist().x;
            cameraRoll = UAV_fc->getUavTwist().z;

            if (firstPOV == false) {
                // draw UAV (physical update is integrated in class)
                UAV_fc->Draw(ourShader);

                camera.Position.x = cameraPosition.x - distance * cosf(cameraYaw);//cos
                camera.Position.y = cameraPosition.y + distance;//maybe add tilt with UAV
                camera.Position.z = cameraPosition.z + distance * sinf(cameraYaw); //sin
                camera.rotateWithUAV(-cameraYaw);
                camera.Pitch = -45.f;
                //camera.tiltHorizontalWithUAV(cameraPitch - 45.f);
            }
            else {
                // or perhaps write a unity function with verying distance (firstPOV distance=0, else distance=0.5)
                camera.Position = UAV_fc->getUavPos();
                camera.rotateWithUAV(-cameraYaw);
                camera.Pitch = 0.f;
                //camera.tiltHorizontalWithUAV(cameraPitch);
                //camera.tiltVerticalWithUAV(-cameraRoll);
            }


            // draw city
            for (auto& grid : cityMap->grids_map) {
                grid.block->Draw(ourShader);
            }

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();

            /*cout << UAV_fc.getUavPos().x << "  " << UAV_fc.getUavPos().y << "  " << UAV_fc.getUavPos().z << endl;*/
        }
    }




    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, uav* UAV_fc)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        UAV_fc->forward();
    }
        
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        UAV_fc->backward();
    }
        
         
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        UAV_fc->left();
    }
        
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        UAV_fc->right();
    }
        
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        UAV_fc->up();
    }
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        UAV_fc->down();
    }
        
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        UAV_fc->yawleft();
    }
        
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        UAV_fc->yawright();
    }

    UAV_fc->dynamics();
        
    if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS 
        && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS 
        && glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS 
        && glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS
        && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)
        UAV_fc->hold();

    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //    UAV_fc->hold();

    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    UAV_fc->hold();

    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    UAV_fc->hold();

    //if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //    UAV_fc->hold();

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