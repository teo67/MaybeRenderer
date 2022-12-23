#ifndef CAMERA_H
#define CAMERA_H
inline const float defaultYaw = -90.0f;
inline const float defaultPitch = 0.0f;
inline const float defaultSensitivity = 0.1f;
inline const float defaultFov = 45.0f;
inline const glm::vec3 defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f);
inline const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
class Camera {
    private:
        float yaw;
        float pitch;
        glm::vec3 forwardVector;
        glm::vec3 rightVector;
        float sensitivity;
        void initDefault();
        void updateVectors();
    public:
        glm::vec3 position;     
        Camera();
        void handleMouse(Mouse mouse);
        void forward(float dt);
        void backward(float dt);
        void left(float dt);
        void right(float dt);
        void setPosition(float x, float y, float z);
        void setRotation(float yaw, float pitch);
        void setYaw(float yaw);
        void setPitch(float pitch);
        glm::mat4 getView();
        void setSensitivity(float sens);
};
#endif