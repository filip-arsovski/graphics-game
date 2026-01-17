#include <Camera.hpp>
#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

#include "Mesh.hpp"
#include "collision.hpp"
#include "GLFW/glfw3.h"

template <typename T>
int findIndex(const std::vector<T>& vec, const T& target) {

  auto it = std::find(vec.begin(), vec.end(), target);

  if (it != vec.end()) return std::distance(vec.begin(), it);

  return -1;
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = position;
  previousPosition = Position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = glm::vec3(posX, posY, posZ);
  previousPosition = Position;
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() const
{
  return lookAt(Position, Position + Front, Up);
}

// Processes input received from any keyboard-like input system. Accepts input
// parameter in the form of camera defined ENUM (to abstract it from windowing
// systems)
void Camera::ProcessActions(std::vector<Camera_Action>& actions, Mirage::Mesh& M, float deltaTime, float currentTime) {
  float velocity = MovementSpeed * deltaTime;
  previousPosition = Position;

  for (auto& action : actions)
  {
    if (action == MOVE_FORWARD && findIndex(actions, MOVE_FORWARD) > findIndex(actions, MOVE_BACKWARD))
      Position += normalize(glm::vec3(Front.x, 0.0f, Front.z)) * velocity;
    if (action == MOVE_BACKWARD && findIndex(actions, MOVE_BACKWARD) > findIndex(actions, MOVE_FORWARD))
      Position -= normalize(glm::vec3(Front.x, 0.0f, Front.z)) * velocity;
    if (action == MOVE_LEFT && findIndex(actions, MOVE_LEFT) > findIndex(actions, MOVE_RIGHT))
      Position -= Right * velocity;
    if (action == MOVE_RIGHT && findIndex(actions, MOVE_RIGHT) > findIndex(actions, MOVE_LEFT))
      Position += Right * velocity;
    if (action == JUMP && !jumping)
    {
        jumping = true;
        jumpStartTime = currentTime;
    }
    if (action == SHOOT && !shooting)
    {
        shooting = true;
        shootStartTime = currentTime;
    }
  }

  Triangle T{};

  if (isColliding(Position, M, 0.2f, T))
  {
      Position += T.Normal * dot(T.Normal, previousPosition - Position) * 1.1f;
      if (isColliding(Position, M, 0.2f)) Position = previousPosition;
  }

  if (shooting)
  {
    constexpr float shootDuration = 0.3f;
    float timeElapsed = currentTime - shootStartTime;
    if (timeElapsed > shootDuration) shooting = false;
  }

  if (jumping) Jump(currentTime, deltaTime, jumpStartTime);

}

// Processes input received from a mouse input system. Expects the offset
// value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  const GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

float Quadratic(const float x, const float t) { return -2 * x + t; }

void Camera::Jump(const float currentTime, const float deltaTime, const float jumpStartTime)
{
  constexpr float jumpDuration = 0.8f;
  float timeElapsed = currentTime - jumpStartTime;

  Position += WorldUp * Quadratic(timeElapsed, jumpDuration) * deltaTime;

  if (timeElapsed > jumpDuration)
  {
    Position.y = 0.298243f; // the player returns to their original vertical position (defined in Maze)
    jumping = false;
  }
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = static_cast<float>(cos(glm::radians(static_cast<double>(Yaw))) *
                               cos(glm::radians(static_cast<double>(Pitch))));
  front.y = static_cast<float>(sin(glm::radians(static_cast<double>(Pitch))));
  front.z = static_cast<float>(sin(glm::radians(static_cast<double>(Yaw))) *
                               cos(glm::radians(static_cast<double>(Pitch))));
  Front = normalize(front);
  Right = normalize(cross(Front, WorldUp));
  Up = normalize(cross(Right, Front));
}
