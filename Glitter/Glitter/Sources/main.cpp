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
#include "yssimplesound.h"
#include "map.h"
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, uav* UAV_fc);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

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
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    // load user interface
    
    string Path_to_Project = getCurrentDir();
    cout << "Current working directory : " << Path_to_Project << endl;
    // when compiling using visual studio, current directory is usually .../[Path to your project]/Glitter/Build/Debug
    // we can use feature to cd to shader files

    string t = "Glitter";
    string::size_type i = Path_to_Project.find(t);
    // delete everything after(including) "Glitter"
    if (i != string::npos)
        Path_to_Project.erase(i, Path_to_Project.length()-i);
    cout << "Path to Project is: " << Path_to_Project << endl;
    std::replace(Path_to_Project.begin(), Path_to_Project.end(), '\\', '/');

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
        return -1;
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
        return -1;
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
        uav UAV_fc(Path_to_Model, glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(-10., 0., -0.));  //second glm change initial landed location of UAV

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
        const int maxID = 5;
        Map cityMap(9, 9, maxID, Path_to_Project);

        //intialize point of view status
        bool firstPOV = true;

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {

            // program dynamic UAV sound effect volume
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
            processInput(window, &UAV_fc);

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
            float distance = 0.5;
            glm::vec3 cameraPosition;
            float cameraYaw, cameraPitch, cameraRoll;

            cameraPosition = UAV_fc.getUavPos();
            cameraYaw = UAV_fc.getUavTwist().y;
            cameraPitch = UAV_fc.getUavTwist().x;
            cameraRoll = UAV_fc.getUavTwist().z;

            if (firstPOV == false) {
                // draw UAV (physical update is integrated in class)
                UAV_fc.Draw(ourShader);

                camera.Position.x = cameraPosition.x - distance * cosf(cameraYaw);//cos
                camera.Position.y = cameraPosition.y + distance;//maybe add tilt with UAV
                camera.Position.z = cameraPosition.z + distance * sinf(cameraYaw); //sin
                camera.rotateWithUAV(-cameraYaw);
                camera.Pitch = -45.f;
                //camera.tiltHorizontalWithUAV(cameraPitch - 45.f);
            }
            else {
                // or perhaps write a unity function with verying distance (firstPOV distance=0, else distance=0.5)
                camera.Position = UAV_fc.getUavPos();
                camera.rotateWithUAV(-cameraYaw);
                //camera.tiltHorizontalWithUAV(cameraPitch);
                //camera.tiltVerticalWithUAV(-cameraRoll);
            }


            // draw city
            for (auto& grid : cityMap.grids_map) {
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
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, uav* UAV_fc)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        UAV_fc->forward();
        
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        UAV_fc->backward();
         
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        UAV_fc->left();
        
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        UAV_fc->right();
        
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        UAV_fc->up();
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        UAV_fc->down();
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        UAV_fc->yawleft();
        
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        UAV_fc->yawright();
        
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