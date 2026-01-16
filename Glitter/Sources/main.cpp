#include <memory>
#include "cube.hpp"
#include "Enemy.hpp"
#include "enemydata.hpp"
#include "glitter.hpp"
#include "GunView.hpp"
#include "Lamp.hpp"
#include "Player.hpp"
#include "rect.hpp"
#include "Sky.hpp"

const std::string program_name = ("Maze game");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window, std::vector<Camera_Action>& actions);

template <typename T>
bool contains(std::vector<T>& vector, T element);
template <typename T>
void remove(std::vector<T>& vector, T element);

// camera
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = mWidth / 2.0f;
static float lastY = mHeight / 2.0f;
static bool firstMouse = true;

// timing
static float deltaTime = 0.0f; // time between current frame and last frame
static float lastFrame = 0.0f;

// global variables
bool leftClick = false;
bool rightClick = false;
int counter = 0;

bool gameOver = false;
bool gameStart = true;

enum GameStatus {WIN, LOSE};
GameStatus gameStatus;

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, program_name.c_str(), nullptr, nullptr);
    glfwSetWindowPos(mWindow, (mode->width - mWidth)/2, (mode->height - mHeight)/2);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(mWindow, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // wireframe for testing
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // load shaders
    Shader shader("shader.vert", "shader.frag");
    Shader basic("basic.vert", "basic.frag");
    Shader healthBar("healthbar.vert", "healthbar.frag");
    Shader gray3Dshader("gray3Dshader.vert", "gray3Dshader.frag");

    // create maze
    Maze maze("Maze", "maze.obj");

    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;

    // create gun model
    GunView gunView("Gun", "gun.fbx");

    // other meshes
    Mirage::Mesh exitArea("Maze", "exit.obj");
    Mirage::Mesh crosshairMesh(crosshairVertices);
    Mirage::Mesh healthBarMesh(rectangle);

    // sky plane
    Sky sky("Maze", "sky.obj");

    // test light bulb
    Lamp lamp("Enemy", "cube.obj");


    std::vector<Camera_Action> actions;

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 1. process input
        processInput(mWindow, actions);

        if (gameStart)
        {
            gameStart = false;
            gameOver = false;

            camera.Yaw = YAW;
            camera.Pitch = PITCH;
            camera.Position = maze.PlayerSpawnPoint;

            // create player
            player = std::unique_ptr<Player>(new Player(cubeVertices, cubeIndices,
                "Bullet", "bullet.fbx", "player", camera));

            // create enemies
            enemies.clear();

            int i = 1;
            for (const auto& data: enemyData)
            {
                PositionData spawn{
                    .location = data.enemySpawn,
                    .prevLocation = glm::vec3(0.0f), // the origin of the cube model
                    .direction = normalize(data.patrolPath[1] - data.patrolPath[0])
                };
                enemies.push_back(std::unique_ptr<Enemy>(new Enemy("Enemy", "cube.obj", "Bullet", "bullet.fbx",
                    "enemy " + std::to_string(i), spawn, data.patrolPath)));
                i++;
            }
        }

        if (!gameOver)
        {
            // 2. Update game state
            player->playerCamera.ProcessActions(actions, maze.mesh, deltaTime, currentFrame);
            player->Move();
            player->Shoot(currentFrame, deltaTime, maze);

            Enemy::playerPosData = player->posData;
            for (auto &enemy: enemies) enemy->Shoot(currentFrame, deltaTime, maze);
            for (auto &enemy: enemies) enemy->Move(deltaTime, maze);
            for (auto &enemy: enemies) enemy->CheckHits(player->bullets, maze);
            for (auto &enemy: enemies) player->CheckHits(enemy->bullets, maze);

            std::vector<int> enemyIndices;
            for (int j = 0; j < enemies.size(); j++) {if (enemies[j]->IsDead()) enemyIndices.push_back(j);}
            for (auto& index: enemyIndices)
                enemies.erase(std::remove(enemies.begin(), enemies.end(), enemies.at(index)), enemies.end());

            if (isColliding(player->posData.location, exitArea, 0.01f))
            {
                gameStatus = WIN;
                gameOver = true;
            }

            if (player->health <= 0)
            {
                gameStatus = LOSE;
                gameOver = true;
            }
        }

        if (!gameOver)
        {
            // 3. Draw screen

            // Background Fill Color
            glClearColor(.0f, .0f, .0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            maze.UpdateShaderMatrix(camera, mWidth, mHeight);
            maze.drawMesh(shader, player->playerCamera.Position, lamp.position);
            gunView.UpdateShaderMatrix(camera, mWidth, mHeight);
            gunView.drawMesh(shader, player->playerCamera.Position, lamp.position);

            for (auto &bullet: player->bullets)
            {
                bullet->UpdateShaderMatrix(camera, mWidth, mHeight);
                bullet->drawMesh(shader, player->playerCamera.Position, lamp.position);
            }

            for (auto &enemy: enemies)
            {
                enemy->UpdateShaderMatrix(camera, mWidth, mHeight);
                enemy->drawMesh(shader, player->playerCamera.Position, lamp.position);

                for (auto &bullet: enemy->bullets)
                {
                    bullet->UpdateShaderMatrix(camera, mWidth, mHeight);
                    bullet->drawMesh(shader, player->playerCamera.Position, lamp.position);
                }
            }

            sky.UpdateShaderMatrix(camera, mWidth, mHeight);
            sky.drawMesh(gray3Dshader, player->playerCamera.Position, lamp.position);

            basic.use();
            float aspect = mWidth * 1.0f / mHeight;
            basic.setFloat("uAspect", aspect);
            glDisable(GL_DEPTH_TEST);
            crosshairMesh.draw(basic.ID);
            glEnable(GL_DEPTH_TEST);

            healthBar.use();
            healthBar.setFloat("playerHealth", player->health);
            healthBar.setFloat("barWidth", width);
            healthBar.setFloat("leftmostBarXpos", offset.x);
            healthBar.setFloat("uAspect", aspect);
            glDisable(GL_DEPTH_TEST);
            healthBarMesh.draw(healthBar.ID);
            glEnable(GL_DEPTH_TEST);


        }
        else
        {
            if (gameStatus == LOSE)
            {
                glClearColor(1.0f, .0f, .0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            } else
            {
                glClearColor(.0f, 1.0f, .0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}

void processInput(GLFWwindow *window, std::vector<Camera_Action>& actions) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && gameOver) gameStart = true;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !contains(actions, SHOOT))
        actions.push_back(SHOOT);
    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && contains(actions, SHOOT))
        remove(actions, SHOOT);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !contains(actions, MOVE_FORWARD))
        actions.push_back(MOVE_FORWARD);
    if (!glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && contains(actions, MOVE_FORWARD))
        remove(actions, MOVE_FORWARD);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !contains(actions, MOVE_BACKWARD))
        actions.push_back(MOVE_BACKWARD);
    if (!glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && contains(actions, MOVE_BACKWARD))
        remove(actions, MOVE_BACKWARD);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !contains(actions, MOVE_LEFT))
        actions.push_back(MOVE_LEFT);
    if (!glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && contains(actions, MOVE_LEFT))
        remove(actions, MOVE_LEFT);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !contains(actions, MOVE_RIGHT))
        actions.push_back(MOVE_RIGHT);
    if (!glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && contains(actions, MOVE_RIGHT))
        remove(actions, MOVE_RIGHT);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !contains(actions, JUMP))
        actions.push_back(JUMP);
    if (!glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && contains(actions, JUMP))
        remove(actions, JUMP);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposd, double yposd) {
    float xpos = static_cast<float>(xposd);
    float ypos = static_cast<float>(yposd);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

template <typename T>
bool contains(std::vector<T>& vector, T element)
{
    return std::find(vector.begin(), vector.end(), element) != vector.end();
}

template <typename T>
void remove(std::vector<T>& vector, T element)
{
    vector.erase(std::remove(vector.begin(), vector.end(), element), vector.end());
}