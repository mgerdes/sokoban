#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "warehouse.h"
#include "level_reader.h"
#include "maths.h"
#include "sokoban.h"
#include "gl_utils.h"
#include "cube.h"

void init_program() {
    init_gl("Sokoban");

    create_texture("textures/crate001.jpg");
    cube_vao = create_cube();
    shader_program = create_shader_program("shaders/frag.glsl", "shaders/vert.glsl");

    view = glGetUniformLocation(shader_program, "view");
    proj = glGetUniformLocation(shader_program, "proj");
    color = glGetUniformLocation(shader_program, "vertex_color");
    texture = glGetUniformLocation(shader_program, "basic_texture");

    camera_pos = create_vec(0.0, 32.0, 22.0, 1.0);
    center = create_vec(0.0, 0.0, 0.0, 1.0);
    up = cross_vec(camera_pos, &x_axis);

    current_level = 1;
    warehouse = read_in_level(current_level);

    Mat* proj_mat = create_perspective_mat(67.0,(float)window_height / window_width, 0.1, 100);
    glUniformMatrix4fv(proj, 1, GL_FALSE, proj_mat->m);
    delete_mat(proj_mat);
    glUniform1i(texture, 0);
}

void draw_tile(Tile t, int row, int col) {
    double x = col*2.0 - warehouse->width + 1.0;
    double z = row*2.0 - warehouse->height + 1.0;

    Mat* translation = create_translation_mat(x, 0.0, z);
    Mat* look_at_mat = create_look_at_mat(camera_pos, center, up); 
    Mat* view_mat = mat_times_mat(look_at_mat, translation);

    glUniformMatrix4fv(view, 1, GL_FALSE, view_mat->m);

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
}

void draw_warehouse() {
    for (int row = 0; row < warehouse->height; row++) {
        for (int col = 0; col < warehouse->width; col++) {
            draw_tile(warehouse->tiles[row][col], row, col);
        }
    }
}

void increase_level() {
    current_level = current_level < 60 ? current_level + 1 : 60;
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
    up = cross_vec(camera_pos, &x_axis);
}

void rotate_camera_up(double degrees) {
    Vec* old_camera_pos = camera_pos;
    Vec* old_up = up;
    Vec* cross = cross_vec(camera_pos, up);
    Vec* u_hat = normalize_vec(cross);

    camera_pos = rotate_vec(old_camera_pos, u_hat, degrees);
    up = rotate_vec(old_up, u_hat, degrees);

    delete_vec(old_camera_pos);
    delete_vec(old_up);
    delete_vec(cross);
    delete_vec(u_hat);
}

void rotate_camera_left(double degrees) {
    Vec* old_camera_pos = camera_pos;
    Vec* up_hat = normalize_vec(up);

    camera_pos = rotate_vec(old_camera_pos, up_hat, degrees);

    delete_vec(old_camera_pos);
    delete_vec(up_hat);
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
            rotate_camera_left(0.02);
        }
        if (glfwGetKey(window, GLFW_KEY_D)) {
            rotate_camera_left(-0.02);
        }
        if (glfwGetKey(window, GLFW_KEY_W)) {
            rotate_camera_up(-0.02);
        }
        if (glfwGetKey(window, GLFW_KEY_S)) {
            rotate_camera_up(0.02);
        }
        if (glfwGetKey(window, GLFW_KEY_COMMA)) {
            Vec* old_camera_pos = camera_pos;
            camera_pos = scale_vec(camera_pos, 1.01);
            free(old_camera_pos);
        }
        if (glfwGetKey(window, GLFW_KEY_PERIOD)) {
            Vec* old_camera_pos = camera_pos;
            camera_pos = scale_vec(camera_pos, 0.99);
            free(old_camera_pos);
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
    printf("|       Use comma and period to zoom      |\n");
    printf("|-----------------------------------------|\n");

    init_program();

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
