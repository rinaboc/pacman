#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"
#include "player.h"
#include "gamemaster.h"


static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 360;
const unsigned int SCR_HEIGHT = 360;

GameMaster* GM;

int loadGlfwWindow(GLFWwindow * &window) {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pacman", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    return 0;
}

int main() {
    GLFWwindow* window;
    if (loadGlfwWindow(window) == -1) {
        return -1;
    }

    GM = new GameMaster(SCR_WIDTH, SCR_HEIGHT);

    static double limitUPS = 1.0 / 120.0;
    static double limitFPS = 1.0 / 60.0;

    double lastTime = glfwGetTime(), timer = lastTime;
    double deltaTime = 0, nowTime = 0, deltaFrame = 0;
    int frames = 0, updates = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {

        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitUPS;
        deltaFrame += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;

        // - Only update at 30 frames / s
        while (deltaTime >= 1.0) {
            GM->gameUpdate();   // - Update function
            updates++;
            deltaTime--;
        }

        while (deltaFrame >= 1.0) {
            // - Render at maximum possible frames
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GM->gameRender();

            frames++;
            deltaFrame--;
        }

        // - Reset after one second
        if (glfwGetTime() - timer > 1.0) {
            timer++;
            // std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
            updates = 0, frames = 0;
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        GM->getPlayer()->setDirection(left);
        GM->getPlayer()->setMoving(true);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        GM->getPlayer()->setDirection(down);
        GM->getPlayer()->setMoving(true);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        GM->getPlayer()->setDirection(up);
        GM->getPlayer()->setMoving(true);
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        GM->getPlayer()->setDirection(right);
        GM->getPlayer()->setMoving(true);
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Cursor Position at (" << xpos << " : " << ypos << "\n";
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

