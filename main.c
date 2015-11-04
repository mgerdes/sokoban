#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "warehouse.h"
#include "level_reader.h"
#include "maths.h"
#include "sokoban.h"

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

    current_level = 1;
    warehouse = read_in_level(current_level);

    camera_pos = create_vec(0.0, 32.0, 22.0, 1.0);
    center = create_vec(0.0, 0.0, 0.0, 1.0);
    up = create_vec(0.0, 1.0, 0.0, 1.0);

    return 0;
}

void draw_tile(Tile t, int row, int col) {
    double x = col*2.0 - warehouse->width + 1.0;
    double z = row*2.0 - warehouse->height + 1.0;

    Mat* translation = translation_mat(x, 0.0, z);
    Mat* look_at_mat = look_at(camera_pos, center, up); 
    Mat* view_mat = mat_times_mat(look_at_mat, translation);
    Mat* proj_mat = perspective(67.0,(float)width / height, 0.1, 100);

    glUniformMatrix4fv(view, 1, GL_FALSE, view_mat->m);
    glUniformMatrix4fv(proj, 1, GL_FALSE, proj_mat->m);

    switch(t) {
        case WALL:
            glUniform3fv(color, 1, wall_color);
            break;
        case CRATE:
            glUniform3fv(color, 1, crate_color);
            break;
        case CRATE_ON_DEST:
            glUniform3fv(color, 1, crate_on_dest_color);
            break;
        case PLAYER:
        case PLAYER_ON_DEST:
            glUniform3fv(color, 1, player_color);
            break;
        case EMPTY:
            glUniform3fv(color, 1, floor_color);
            break;
        case DEST:
            glUniform3fv(color, 1, dest_color);
            break;
    }

    glBindVertexArray(cube_vao);
    if (t == DEST || t == EMPTY) {
        // first two triangles is just 'floor' of cube.
        glDrawArrays(GL_TRIANGLES, 0, 2*3);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
    }

    delete_mat(translation);
    delete_mat(look_at_mat);
    delete_mat(view_mat);
    delete_mat(proj_mat);
}

void draw_warehouse() {
    for (int row = 0; row < warehouse->height; row++) {
        for (int col = 0; col < warehouse->width; col++) {
            draw_tile(warehouse->tiles[row][col], row, col);
        }
    }
}

void increase_level() {
    current_level = current_level < 9 ? current_level + 1 : 9;
    delete_warehouse(warehouse);
    warehouse = read_in_level(current_level);
}

void decrease_level() {
    current_level = current_level > 1 ? current_level - 1 : 1;
    delete_warehouse(warehouse);
    warehouse = read_in_level(current_level);
}

void cleanup() {
    delete_warehouse(warehouse);
    delete_vec(up);
    delete_vec(camera_pos);
    delete_vec(center);
}

void reset_camera() {
    delete_vec(up);
    delete_vec(camera_pos);
    camera_pos = create_vec(0.0, 32.0, 22.0, 1.0);
    up = create_vec(0.0, 1.0, 0.0, 1.0);
}

void handle_input() {
    double current_seconds = glfwGetTime();
    static double last_key_press;

    if (current_seconds > last_key_press + 0.15) {
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
            reset_camera();
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET)) {
            decrease_level();
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET)) {
            increase_level();
            last_key_press = current_seconds;
        }
        if (glfwGetKey(window, GLFW_KEY_A)) {
            Vec* old_camera_pos = camera_pos;
            camera_pos = rotate_vec_y(old_camera_pos, 0.02);
            delete_vec(old_camera_pos);
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            Vec* old_camera_pos = camera_pos;
            camera_pos = rotate_vec_y(old_camera_pos, -0.02);
            delete_vec(old_camera_pos);
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            Vec* old_camera_pos = camera_pos;
            Vec* old_up = up;
            camera_pos = rotate_vec_x(old_camera_pos, 0.02);
            up = rotate_vec_x(old_up, 0.02);
            delete_vec(old_camera_pos);
            delete_vec(old_up);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            Vec* old_camera_pos = camera_pos;
            Vec* old_up = up;
            camera_pos = rotate_vec_x(old_camera_pos, -0.02);
            up = rotate_vec_x(old_up, -0.02);
            delete_vec(old_camera_pos);
            delete_vec(old_up);
        }
        if (glfwGetKey(window, GLFW_KEY_Q)) {
            cleanup();
            glfwDestroyWindow(window);
            glfwTerminate();
            exit(0);
        }
    }
}

int main() {
    if (init_program() == 1) {
        return 1;
    }

    printf("|-----------------------------------------|\n");
    printf("|----------Welcome to Sokoban!------------|\n");
    printf("|-----------------------------------------|\n");
    printf("| The warehouse keeper has created you, a |\n");
    printf("| sentient crate tasked with placing your |\n");
    printf("|   fellow crates into their designated   |\n"); 
    printf("| positions in the warehouse. Good Luck.  |\n");
    printf("|-----------------------------------------|\n");
    printf("|--------------Instructions---------------|\n");
    printf("|-----------------------------------------|\n");
    printf("|  Move each of the crates (purple cubes) |\n");
    printf("|        onto the green tiles.            |\n");
    printf("|-----------------------------------------|\n");
    printf("|--------------Key Bindings---------------|\n");
    printf("|-----------------------------------------|\n");
    printf("|   Use the arrow keys to move yourself   |\n");
    printf("|            Press R to reset             |\n");
    printf("|      Use [ and ] to change levels       |\n");
    printf("| Use W, A, S, and D to rotate the camera |\n");
    printf("|             Press Q to quit             |\n");
    printf("|-----------------------------------------|\n");

    while (!glfwWindowShouldClose(window)) {
        if (is_warehouse_won(warehouse)) {
            increase_level();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        handle_input();
        draw_warehouse();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
