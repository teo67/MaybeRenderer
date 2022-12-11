#ifndef CAMERA_H
#define CAMERA_H
inline const float defaultYaw = -90.0f;
inline const float defaultPitch = 0.0f;
inline const float defaultSpeed = 2.5f;
inline const float defaultSensitivity = 0.1f;
inline const float defaultFov = 45.0f;
inline const glm::vec3 defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
inline const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
class Camera {
    private:
        float yaw;
        float pitch;
        float fov;
        glm::vec3 position;  
        glm::vec3 forwardVector;
        glm::vec3 rightVector;
        float sensitivity;
        float speed;
        void initDefault();
        void updateVectors();
    public:
        Camera();
        void handleMouse(Mouse mouse);
        void handleScroll(double xoffset, double yoffset);
        void forward(float dt);
        void backward(float dt);
        void left(float dt);
        void right(float dt);
        glm::mat4 getView();
        float getFOV();
        void setFOV(float fov);
        void setSensitivity(float sens);
        void setSpeed(float speed);
};
#endif