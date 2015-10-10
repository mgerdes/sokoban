#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "warehouse.h"
#include "level_reader.h"
#include "maths.h"

static const GLfloat player_color[] = {1.0, 1.0, 1.0};
static const GLfloat wall_color[] = {0.2, 0.2, 0.7};
static const GLfloat crate_color[] = {0.5, 0.0, 0.5};

static GLfloat cube_normals[] = {
    -1.0, 0.0, -0.0,
    -1.0, 0.0, -0.0,
    -1.0, 0.0, -0.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    -1.0, -0.0, 0.0,
    -1.0, -0.0, 0.0,
    -1.0, -0.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -0.0, 1.0,
    0.0, -0.0, 1.0,
    0.0, -0.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
};

static GLfloat cube_points[] = {
    -1.0,-1.0,-1.0,
    -1.0,-1.0, 1.0,
    -1.0, 1.0, 1.0,
    1.0, 1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0,-1.0,
    1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,
    1.0,-1.0,-1.0,
    1.0, 1.0,-1.0,
    1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,
    1.0,-1.0, 1.0,
    -1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0, 1.0,
    -1.0,-1.0, 1.0,
    1.0,-1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0,-1.0,-1.0,
    1.0, 1.0,-1.0,
    1.0,-1.0,-1.0,
    1.0, 1.0, 1.0,
    1.0,-1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0,-1.0,
    -1.0, 1.0,-1.0,
    1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,
    -1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    1.0,-1.0, 1.0
};

const char* vertex_shader_source =
    "#version 330\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_normal;"
    "out vec3 color;"
    "uniform vec3 vertex_color;"
    "uniform mat4 view, proj;"
    "void main () {"
    "   vec3 light_position = vec3(15.0, 15.0, -15.0);"
    "   vec3 L = normalize(light_position - vertex_position);"
    "   float dot_prod = max(dot(L, vertex_normal), 0.4);"
    "   color = dot_prod * vertex_color;"
    "	gl_Position = proj * view * vec4 (vertex_position, 1.0);"
    "}";

const char* fragment_shader_source =
    "#version 330\n"
    "in vec3 color;"
    "out vec4 frag_colour;"
    "void main () {"
    "	frag_colour = vec4(color, 1.0);"
    "}";

int width, height;
GLuint cube_vao;
GLuint cube_points_vbo, cube_normals_vbo;
GLuint vertex_shader, fragment_shader;
GLuint shader_program;
GLuint view, proj, color;
GLFWwindow* window;
Warehouse* warehouse;

int init_program() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    width = height = 1000;
    window = glfwCreateWindow(width, height, "Sokoban!", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glGenBuffers(1, &cube_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points), cube_points, GL_STATIC_DRAW);

    glGenBuffers(1, &cube_normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    glBindBuffer(GL_ARRAY_BUFFER, cube_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, vertex_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    view = glGetUniformLocation(shader_program, "view");
    proj = glGetUniformLocation(shader_program, "proj");
    color = glGetUniformLocation(shader_program, "vertex_color");

    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);

    warehouse = read_in_level(1);

    return 0;
}

void draw_cube(int row, int col) {
    float x = col*2.0 - warehouse->width + 1.0;
    float z = row*2.0 - warehouse->height + 1.0;

    Mat* translation = translate(identity_mat(), create_vec(x, 0.0, z, 1.0));

    Vec* camera_pos = create_vec(0.0, 32.0, 22.0, 1.0);
    Vec* up = cross_vec(create_vec(-1.0, 0.0, 0.0, 1.0), normalize_vec(camera_pos));
    Mat* view_mat = look_at(camera_pos, create_vec(0.0, 0.0, 0.0, 1.0), up); 
    view_mat = mat_times_mat(view_mat, translation);
    Mat* proj_mat = perspective(67.0,(float)width / height, 0.1, 100);

    glUniformMatrix4fv(view, 1, GL_FALSE, view_mat->m);
    glUniformMatrix4fv(proj, 1, GL_FALSE, proj_mat->m);

    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
}

void draw_warehouse() {
    for (int row = 0; row < warehouse->height; row++) {
        for (int col = 0; col < warehouse->width; col++) {
            if (warehouse->tiles[row][col] == WALL) {
                glUniform3fv(color, 1, wall_color);
                draw_cube(row, col);
            } else if (warehouse->tiles[row][col] == CRATE 
                    || warehouse->tiles[row][col] == CRATE_ON_DEST) {
                glUniform3fv(color, 1, crate_color);
                draw_cube(row, col);
            } else if (warehouse->tiles[row][col] == PLAYER 
                    || warehouse->tiles[row][col] == PLAYER_ON_DEST) {
                glUniform3fv(color, 1, player_color);
                draw_cube(row, col);
            }
        }
    }
}

void handle_input() {
    double current_seconds = glfwGetTime();
    static double last_key_press;

    if (current_seconds > last_key_press + 0.12) {
        if (glfwGetKey(window, GLFW_KEY_UP)) {
            move_player_up(warehouse);
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            move_player_down(warehouse);
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            move_player_left(warehouse);
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            move_player_right(warehouse);
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_R)) {
            reset_warehouse(warehouse);
            last_key_press = current_seconds;
        }
    }
}

int main() {
    if (init_program() == 1) {
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        handle_input();
        draw_warehouse();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
