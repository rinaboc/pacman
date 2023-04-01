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
const unsigned int SCR_WIDTH = 648;
const unsigned int SCR_HEIGHT = 648;

float r = 0.0, g = 0.0, b = 0.0;

void setUpTriangle(unsigned int& VBO, unsigned int& VAO) {

    float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}


void setUpSquare(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO) {

    float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
     -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };


    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

void render(Shader shader, unsigned int& VAO) {
    glClearColor(r, g, b, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float intensity = sin(timeValue) / 2 + 0.5f;

    shader.setFloat("intensity", intensity);

    shader.use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

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

    unsigned int VBO2, VAO2, EBO;
    setUpTriangle(VBO2, VAO2);

    //setUpSquare(VBO2, VAO2, EBO);

    Shader shader("4.0.shader.vs", "4.0.shader.fs");

    //unsigned int VBO, VAO;
    GM = new GameMaster();


    static double limitUPS = 1.0 / 30.0;
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
            glClearColor(r, g, b, 1.0);
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
    } else {
        GM->getPlayer()->setMoving(false);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (r < 1.0)
            r += 0.1;
        else
            r = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (g < 1.0)
            g += 0.1;
        else
            g = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        if (b < 1.0)
            b += 0.1;
        else
            b = 0.0;
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

