
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <signal.h>


// macro
// #define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
// backslashes to prevent newlines breaking macro
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// #x turns function passed into macro as string
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath)
{

    std::ifstream stream(filepath);


    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };


    std::string line;

    // create a stack allocated array for 2 sstreams
    std::stringstream ss[2];

    // current shadertype reading in
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // will not work for unknown reasons
        // char message[length];

        // will work
        char *message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "shader") << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error: Failed to initialize GL" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
        };

    unsigned int indices[] = {
        2,1,0,
        2,3,0
    };

    // buffer id
    unsigned int buffer;
    // creates a pointer to the buffer
    // gl<>(target, bind)
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2*sizeof(float), positions, GL_STATIC_DRAW);

    glenablevertexattribarray(0);
    glvertexattribpointer(0, 2, gl_FLOAT, GL_FALSE, sizeof(float)*2, 0);


    // index buffer ibo = index buffer object
    // must be unsigned !!
    unsigned int ibo;
    // creates a pointer to the buffer
    // gl<>(target, bind)
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // bounded to ibo buffer so don't need last argument
        // GLClearError();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        // ASSERT(GLLogCall());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    std::cout << "Hello Million Dollar World!!" << std::endl;

    return 0;
}
