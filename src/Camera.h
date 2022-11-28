const float defaultYaw = -90.0f;
const float defaultPitch = 0.0f;
const float defaultSpeed = 2.5f;
const float defaultSensitivity = 0.1f;
const float defaultFov = 45.0f;
const glm::vec3 defaultPosition = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

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
        Camera(float yaw, float pitch, float fov, float speed, float sensitivity);
        Camera(float fov, float speed, float sensitivity);
        Camera(float yaw, float pitch);
        Camera();
        void handleMouse(Mouse* mouse);
        void handleScroll(double xoffset, double yoffset);
        void forward(float dt);
        void backward(float dt);
        void left(float dt);
        void right(float dt);
        float getFOV();
        glm::mat4 getView();
};