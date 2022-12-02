enum class ColorOptions { NONE, UNIFORM, VARIABLE_NO_LIGHT, VARIABLE_WITH_LIGHT };
enum class TextureOptions { NONE, SINGLE, MIX };
class Shader {
    private:
        GLuint program;
        void initShader(std::string vertexCode, std::string fragmentCode);
        void initShader(ColorOptions color, TextureOptions texture, bool transform);
        std::string locationEquals(int& input);
        unsigned int numSizes;
        unsigned int* sizes;
        unsigned int totalSize;
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(std::string vShaderCode, std::string fShaderCode);
        Shader(ColorOptions color, TextureOptions texture, bool transform);
        Shader(ColorOptions color, bool transform);
        Shader(TextureOptions texture, bool transform);
        Shader(ColorOptions color);
        Shader(TextureOptions texture);
        Shader(ColorOptions color, TextureOptions texture);
        void end();
        void use();
        GLuint getLocation(const char* name);
        void transform(glm::mat4 trans, const char* transString);
        void sendVec3f(float a, float b, float c, const char* vecString);
        unsigned int getNumSizes();
        unsigned int* getSizes();
        unsigned int getTotalSize();
};