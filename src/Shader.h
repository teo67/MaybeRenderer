class Shader {
    GLuint program;
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        void end();
        void use();
        GLuint getLocation(const char* name);
        void transform(glm::mat4 trans, const char* transString);
};