#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h> // For cross-platform access to OpenGL functions 
#include <GLFW/glfw3.h> // For creating windows, handling user input, and manage OpenGL context

#include "includes/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "includes/filesystem.h"
#include "includes/Shader.h"
#include "includes/Camera.h"
#include "includes/Model.h" // for model importing
#include "includes/Entity.h"

#ifndef ENTITY_H
#define ENTITY_H

#include <list> //std::list
#include <memory> //std::unique_ptr

class Entity : public Model
{
public:
    std::list<std::unique_ptr<Entity>> children;
    Entity* parent;
};
#endif

#include <iostream>

// Forward declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// settings
// change to native resolution here:
const unsigned int SCR_WIDTH = 800; // window width
const unsigned int SCR_HEIGHT = 600; // window height

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 0.0f));
Camera cameraSpy(glm::vec3(0.0f, 10.0f, 0.f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// cursor settings
bool cursorEnabled = false;
bool tabPressedLastFrame = false;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(8.2f, 8.0f, 9.0f);

int main()
{
    // glfw: initialize and configure
    // create openGL context version 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile

    // For apple compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Rendering Engine", NULL, NULL);
    if (window == NULL) // if window creation doesn't work
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // for backpack
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    camera.MovementSpeed = 20.f;

    // build and compile our shader source code
    // change to your specified directory here:
    Shader shader("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/src/shaders/basic.vs", "C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/src/shaders/basic.fs");
    Shader skyboxShader("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/src/shaders/skybox.vs", "C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/src/shaders/skybox.fs");
    Shader lightingCubeShader("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/src/shaders/light_cube.vs", "C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/src/shaders/light_cube.fs");

    Model model(FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/objects/planet/planet.obj"));
    Entity ourEntity(model);
    ourEntity.transform.setLocalPosition({ 0, 0, 0 });
    const float scale = 1.0;
    ourEntity.transform.setLocalScale({ scale, scale, scale });

    {
        Entity* lastEntity = &ourEntity;

        for (unsigned int x = 0; x < 1; ++x)
        {
            for (unsigned int z = 0; z < 1; ++z)
            {
                ourEntity.addChild(model);
                lastEntity = ourEntity.children.back().get();

                //Set transform values
                lastEntity->transform.setLocalPosition({ x * 10.f - 100.f,  0.f, z * 10.f - 100.f });
            }
        }
    }
    ourEntity.updateSelfAndChild();

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // lightCube
    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // orient skybox texture correctly
    stbi_set_flip_vertically_on_load(false);

    // load textures
    std::vector<std::string> faces
    {
        FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/skybox/space/right.png"),
        FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/skybox/space/left.png"),
        FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/skybox/space/top.png"),
        FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/skybox/space/bottom.png"),
        FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/skybox/space/front.png"),
        FileSystem::getPath("C:/Users/dakot/OneDrive/Desktop/OpenGL Projects/OpenGL_Engine/OpenGL_Engine/resources/skybox/space/back.png")
    };
    unsigned int cubeMapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // initialize ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // imgui settings
    bool drawLamp = true;

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // Background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color + depth buffer every frame

        // create imgui window
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.use();

        shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        const Frustum camFrustum = createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, glm::radians(camera.Zoom), 0.1f, 100.0f);

        cameraSpy.ProcessMouseMovement(2, 0);
        //static float acc = 0;
        //acc += deltaTime * 0.0001;
        //cameraSpy.Position = { cos(acc) * 10, 0.f, sin(acc) * 10 };
        glm::mat4 view = camera.GetViewMatrix();

        // project + view matrices
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        // draw our scene graph
        unsigned int total = 0, display = 0;
        ourEntity.drawSelfAndChild(camFrustum, shader, display, total);
        // std::cout << "Total process in CPU : " << total << " / Total send to GPU : " << display << std::endl;

        // also draw the lamp object
        lightingCubeShader.use();
        lightingCubeShader.setMat4("projection", projection);
        lightingCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightingCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        if (drawLamp)
            glDrawArrays(GL_TRIANGLES, 0, 36);

        // rotate entity
        ourEntity.transform.setLocalRotation({ 0.0f, ourEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.0f });
        ourEntity.updateSelfAndChild();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        ImGui::Begin("ImGUI window");
        ImGui::Checkbox("Light", &drawLamp);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // shutdown imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &lightCubeVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    static bool tabPressedLastFrame = false;

    // If user presses escape, close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Toggle cursor with TAB
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressedLastFrame)
    {
        cursorEnabled = !cursorEnabled;
        glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        tabPressedLastFrame = true;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        tabPressedLastFrame = false;
    }

    // don't allow for movement if cursor is toggled off
    if (!cursorEnabled)
    {
        // User input for moving
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (cursorEnabled)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// loads texture
unsigned int loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}