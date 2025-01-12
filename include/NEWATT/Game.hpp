#ifndef GAME_HPP
#define GAME_HPP

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../include/NEWATT/Match.hpp"

class Game{
    public:
        enum class State{
            InMenu, Paused, InMatch, Interim
        };

        static const std::string state_strings[4];

        static const int VERTICES_PER_CELL = 4;
        static const int VERTEX_DIMENSIONS = 3;
        static const int WIDTH = 800;
        static const int HEIGHT = 450;

        static const char* vertex_shader_text;
        static const char* fragment_shader_text;

        Game(GLFWwindow* window);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        GLuint compileShader(const char* shader_text, GLenum shader_name);

        State getState();
        void setState(State state);
        float* getCellVertexPositions();
        Cell* getCells();
        float* getGridVertexPositions();
        uint8_t* getGridColor();

        void init();
        void run();
        void update(float dt);
        void render();

    private:
        State state;
        Match match;

        GLFWwindow* window;
        int width;
        int height;

        GLuint program;
        GLuint vao;
        GLuint positions_buffer;
        GLuint colors_buffer;
        uint8_t grid_color[Cell::COLOR_COMPONENTS];
        float cell_positions[VERTICES_PER_CELL * VERTEX_DIMENSIONS];
        float grid_positions[VERTICES_PER_CELL * VERTEX_DIMENSIONS];

        void calculateCellPositions();
        void calculateGridPositions();
        void printGridPositions();
};

#endif
