#include "../include/NEWATT/Game.hpp"

const char* Game::vertex_shader_text =
"#version 450 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"\n"
"layout (location = 2) uniform float horizontal_offset;\n"
"layout (location = 3) uniform float vertical_offset;\n"
"out vec4 vs_color;\n"
"\n"
"void main(){\n"
"    int horizontal_index = gl_InstanceID % 10;\n"
"    int vertical_index = gl_InstanceID / 10;\n"
"    vec2 offset = vec2(horizontal_index * horizontal_offset, vertical_index * vertical_offset);\n"
"    gl_Position = vec4(position.xy + offset, position.z, 1.0f);\n"
"    vs_color = vec4(color, 1.0f);\n"
"};\n";

const char* Game::fragment_shader_text =
"#version 450 core\n"
"in vec4 vs_color;\n"
"out vec4 color;\n"
"\n"
"void main() {\n"
"    color = vs_color;\n"
"};\n";

Game::Game(GLFWwindow* window):
    match{}, state{}, width{WIDTH}, height{HEIGHT}, window{window},
    program{}, vao{}, positions_buffer{}, colors_buffer{}
{
    for (int i = 0; i < Cell::COLOR_COMPONENTS; i++)
        this->grid_color[i] = 50;

    for (int i = 0; i < VERTICES_PER_CELL; i++){
        this->cell_positions[i] = 0.0f;
        this->cell_positions[i + 1] = 0.0f;
        this->cell_positions[i + 2] = -0.5f;
    }

    for (int i = 0; i < VERTICES_PER_CELL; i++){
        this->grid_positions[i] = 0.0f;
        this->grid_positions[i + 1] = 0.0f;
        this->grid_positions[i + 2] = 0.5f;
    }
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    Game* game = (Game*)(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS && game->state == State::InMenu){
        game->state = State::InMatch;
        game->match.start();
        std::cout << "match started" << std::endl;
    }

    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && game->state == State::InMatch){
        game->state = State::Paused;
        std::cout << "match paused" << std::endl;
    }

    else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && game->state == State::Paused){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        std::cout << "closing window" << std::endl;
    }

    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && game->state == State::Paused){
        game->state = State::InMatch;
        std::cout << "match resumed" << std::endl;
    }

    else if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.moveRight();
        std::cout << "moved right" << std::endl;
    }

    else if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.moveLeft();
        std::cout << "moved left" << std::endl;
    }

    else if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.holdPiece();
        std::cout << "piece held" << std::endl;
    }

    else if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.lowerPiece();
        std::cout << "lowered piece" << std::endl;
    }

    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.hardDrop();
        std::cout << "hard dropped" << std::endl;
    }

    else if (key == GLFW_KEY_Q && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.rotateCounterCW();
        std::cout << "rotated ccw" << std::endl;
    }

    else if (key == GLFW_KEY_W && action == GLFW_PRESS && game->state == State::InMatch){
        game->match.rotateCW();
        std::cout << "rotated cw" << std::endl;
    }
}

GLuint Game::compileShader(const char* shader_text, GLenum shader_name){
    const GLuint shader = glCreateShader(shader_name);
    glShaderSource(shader, 1, &shader_text, NULL);
    glCompileShader(shader);
    GLint shader_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_success);

    if (!shader_success) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        char* log = new char[log_length];
        glGetShaderInfoLog(shader, log_length, &log_length, log);
        fprintf(stderr, "Shader compilation failed:\n%s\n", log);
        delete[] log;
    }

    return shader;
}

Game::State Game::getState(){
    return this->state;
}

void Game::setState(State state){
    this->state = state;
}

float* Game::getCellVertexPositions(){
    return (float*)(this->cell_positions);
}

Cell* Game::getCells(){
    return (Cell*)(this->match.getGrid());
}

float* Game::getGridVertexPositions(){
    return (float*)(this->grid_positions);
}

uint8_t* Game::getGridColor(){
    return (uint8_t*)(this->grid_color);
}

void Game::init(){
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);

    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glCreateBuffers(1, &(this->positions_buffer));
    glNamedBufferStorage(this->positions_buffer, sizeof(this->cell_positions) + sizeof(this->grid_positions), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayVertexBuffer(vao, 0, this->positions_buffer, 0, 3 * sizeof(float));
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(vao, 0);

    glCreateBuffers(1, &colors_buffer);
    glNamedBufferStorage(colors_buffer, (Match::ROWS - 2) * Match::COLUMNS * sizeof(Cell), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayAttribBinding(vao, 1, 1);
    glVertexArrayVertexBuffer(vao, 1, colors_buffer, 0, sizeof(Cell));
    glVertexArrayAttribFormat(vao, 1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0);

    const GLuint vertex_shader = compileShader(vertex_shader_text, GL_VERTEX_SHADER);
    const GLuint fragment_shader = compileShader(fragment_shader_text, GL_FRAGMENT_SHADER);

    this->program = glCreateProgram();
    glAttachShader(this->program, vertex_shader);
    glAttachShader(this->program, fragment_shader);
    glLinkProgram(this->program);
    GLint program_success;
    glGetProgramiv(program, GL_LINK_STATUS, &program_success);

    if (!program_success) {
        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        char* log = new char[log_length];
        glGetProgramInfoLog(program, log_length, &log_length, log);
        fprintf(stderr, "Program linking failed:\n%s\n", log);
        delete[] log;
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(this->program);
}

void Game::run(){
    float final_time = glfwGetTime(); 
    int old_width = 0, old_height = 0;

    GLuint error = glGetError();
    if (error != GL_NO_ERROR) std::cout << "\nOpenGL error before the main loop: " << error << std::endl;
    error = GL_NO_ERROR;

    while (!glfwWindowShouldClose(window)){
        glfwGetFramebufferSize(window, &width, &height);

        error = glGetError();
        if (error != GL_NO_ERROR) std::cout << "\nOpenGL error after getting framebuffer size: " << error << std::endl;
        error = GL_NO_ERROR;

        while(old_width != width || old_height != height){
            old_height = height;
            old_width = width;

            this->calculateCellPositions();
            glNamedBufferSubData(positions_buffer, 0, sizeof(this->cell_positions), this->cell_positions);

            error = glGetError();
            if (error != GL_NO_ERROR) std::cout << "\nOpenGL error after updating cell positions: " << error << std::endl;
            error = GL_NO_ERROR;

            this->calculateGridPositions();
            glNamedBufferSubData(positions_buffer, sizeof(cell_positions), sizeof(grid_positions), this->grid_positions);

            error = glGetError();
            if (error != GL_NO_ERROR) std::cout << "\nOpenGL error after updating grid positions: " << error << std::endl;
            error = GL_NO_ERROR;
        }

        Cell* cells = this->getCells();
        glNamedBufferSubData(colors_buffer, 0, (Match::ROWS - 2) * Match::COLUMNS * sizeof(Cell), &(cells[2 * Match::COLUMNS]));

        error = glGetError();
        if (error != GL_NO_ERROR) std::cout << "\nOpenGL error after updating color buffer: " << error << std::endl;
        error = GL_NO_ERROR;

        const float ratio = width / (float)height;
        glViewport(0, 0, width, height);

        error = glGetError();
        if (error != GL_NO_ERROR) std::cout << "\nOpenGL error after changing the viewport: " << error << std::endl;
        error = GL_NO_ERROR;
        
        float delta_time = glfwGetTime() - final_time;
        this->update(delta_time);
        this->render();

        error = glGetError();
        if (error != GL_NO_ERROR) std::cout << "\nOpenGL error after rendering: " << error << std::endl;
        error = GL_NO_ERROR;

        glfwSwapBuffers(window);
        glfwPollEvents();
        final_time = glfwGetTime();

        error = glGetError();
        if (error != GL_NO_ERROR) std::cout << "\nOpenGL at the end of iteration: " << error << std::endl;
        error = GL_NO_ERROR;
    }
}

void Game::update(float dt){
    if (this->state != State::InMatch) return;

    if (this->match.getState() == Match::State::Finished){
        std::cout << "you fucking suck" << std::endl;
        this->state = State::Interim;
        return;
    }

    if (this->match.getState() == Match::State::PieceLocked){
        this->match.spawnNewPiece();
        return;
    }
}

void Game::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexAttribDivisor(1, 1);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, VERTICES_PER_CELL, (Match::ROWS - 2) * Match::COLUMNS);

    glDisableVertexArrayAttrib(vao, 1);
    glVertexAttribDivisor(1, 0);
    glVertexAttrib3f(1, this->grid_color[0] / 255.0f, this->grid_color[1] / 255.0f, this->grid_color[2] / 255.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, VERTICES_PER_CELL, VERTICES_PER_CELL);
}

void Game::calculateCellPositions(){
    float divider_width_ratio = 2.0 / this->width;
    float divider_height_ratio = 2.0 / this->height;
    float cell_size_pixels = this->height * 0.035;
    float cell_height_ratio = 0.07;
    float cell_width_ratio = 2 * cell_size_pixels / this->width;
    
    this->cell_positions[0] = -5 * cell_width_ratio - 4.5 * divider_width_ratio;
    this->cell_positions[1] = 10 * cell_height_ratio + 9.5 * divider_height_ratio;

    this->cell_positions[3] = -4 * cell_width_ratio - 4.5 * divider_width_ratio;
    this->cell_positions[4] = 10 * cell_height_ratio + 9.5 * divider_height_ratio;

    this->cell_positions[6] = -5 * cell_width_ratio - 4.5 * divider_width_ratio;
    this->cell_positions[7] = 9 * cell_height_ratio + 9.5 * divider_height_ratio;

    this->cell_positions[9] = -4 * cell_width_ratio - 4.5 * divider_width_ratio;
    this->cell_positions[10] = 9 * cell_height_ratio + 9.5 * divider_height_ratio;

    glUniform1f(2, cell_width_ratio + divider_width_ratio);
    glUniform1f(3, - cell_height_ratio - divider_height_ratio);
}

void Game::calculateGridPositions(){
    float divider_width_ratio = 2.0 / this->width;
    float divider_height_ratio = 2.0 / this->height;
    float cell_size_pixels = this->height * 0.035;
    float cell_width_ratio = 2 * cell_size_pixels / this->width;
    float cell_height_ratio = 0.07;
    float aspect_ratio = width / height;
    float width_offset_from_center = 5 * cell_width_ratio + 8 * divider_width_ratio;
    float height_offset_from_center = 10 * cell_height_ratio + 13.5 * divider_height_ratio;

    this->grid_positions[0] = - width_offset_from_center;
    this->grid_positions[1] = - height_offset_from_center;

    this->grid_positions[3] = width_offset_from_center;
    this->grid_positions[4] = - height_offset_from_center;

    this->grid_positions[6] = - width_offset_from_center;
    this->grid_positions[7] = height_offset_from_center;

    this->grid_positions[9] = width_offset_from_center;
    this->grid_positions[10] = height_offset_from_center;
}

void Game::printGridPositions(){
    std::cout << "Grid positions: " << std::endl;

    for (int i = 0; i < VERTICES_PER_CELL; i++)
        std::cout << "(" << this->grid_positions[i * VERTEX_DIMENSIONS] << "," << this->grid_positions[i * VERTEX_DIMENSIONS + 1] << ") ";
    
    std::cout << std::endl;
}
