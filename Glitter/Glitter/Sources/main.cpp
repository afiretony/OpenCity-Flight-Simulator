#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "flightcontrol.h"
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
//#include "yssimplesound.h"
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawGrid();

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const char* vertexSource = R"glsl(
    #version 150 core

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";
const char* fragmentSource = R"glsl(
    #version 150 core

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

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
    //YsSoundPlayer player1;
    //YsSoundPlayer::SoundData myWav1;
    //if (YSOK == myWav1.LoadWav("C:/Users/14846/Desktop/24780/HW/IndividualProject/Demo_zhanfany/Glitter/Glitter/Sources/solarsystem.wav")) {
    //    canPlaySound = true;
    //    // start the player after opening window
    //    player1.Start();
    //    player1.PlayOneShot(myWav1);
    //    player1.PlayBackground(myWav1);
    //}
    //else {
    //    cout << "Failed to read " << "click.wav" << endl;
    //    return 1;
    //}

    // glfw: initialize and configure
    // ------------------------------
    // load user interface
    
    string Path_to_Project = getCurrentDir();
    cout << "Current working directory : " << Path_to_Project << endl;
    // when compiling using visual studio, current directory is usually .../Glitter/Glitter/build/debug
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
    string Path_to_Model = Path_to_Project + "Glitter/Glitter/Model/UAV/quadcop.obj";

    // load flight control and dynamics model
    //uav uav_fc;
    
    // load sound
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engineering Republic", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
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

    YsSoundPlayer player1;
    YsSoundPlayer::SoundData myWav1;

    // store the filename of music
    string fileNames[] = { "UAV1.wav", "UAV2.wav" };
    // load user choice, note use of .c_str()
    // if (YSOK == myWav1.LoadWav(fileNames[0].c_str())) {
    if (true) {

        //player1.Start();
        //player1.PlayBackground(myWav1);
        // build and compile shaders
        // -------------------------
        
        Shader ourShader(path1, path2);
        cout << "shader loaded" << endl;
        Model UAV(Path_to_Model);
        Model UAV2(Path_to_Model);

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {

            // per-frame time logic
            // --------------------
            
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            // cout << currentFrame << endl;

            // input
            // -----
            processInput(window);

            // render
            // ------
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            // enable shader before setting uniforms
            ourShader.use();

            // camera/view transformation
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            
            // view/projection transformations
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);

            glm::mat4 trans = glm::mat4(1.0f); // what is this?
            trans = glm::translate(trans, glm::vec3(0., -0.1, 0.)); // translate
            trans = glm::scale(trans, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", trans);
            UAV.Draw(ourShader);

            // draw another one
            trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(0., 0.1, 0.));
            trans = glm::scale(trans, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", trans);
            UAV2.Draw(ourShader);
            // Draw grid
            drawGrid();
            
            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    else {
        cout << "Failed to read " << "UAV1.wav" << endl;
    }
    
    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

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

void drawGrid() {
    int i = 0;
    for (int i = 0; i < 40; i++) {
        glPushMatrix();
        if (i < 20)
            glTranslatef(0, 0, i);
        else {
            glTranslatef(i - 20, 0, 0);
            glRotatef(-90, 0, 1, 0);
        }

        glBegin(GL_LINES);
        glColor3f(1, 1, 1); glVertex3f(19, -0.1, 0);
        glEnd();
        glPopMatrix();
    }
}