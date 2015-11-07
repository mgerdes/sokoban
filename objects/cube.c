#include <GL/glew.h>
#include "cube.h"

static GLfloat cube_normals[] = {
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,

    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,

    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, -0.0, 0.0,
    -1.0, -0.0, 0.0,
    -1.0, -0.0, 0.0,

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
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
};

static GLfloat cube_points[] = {
    1.0, -1.0, 1.0,
    -1.0,-1.0,-1.0,
    1.0, -1.0,-1.0,
    1.0, -1.0, 1.0,
    -1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,

    1.0,  1.0,-1.0,
    -1.0,-1.0,-1.0,
    1.0, -1.0,-1.0,
    1.0,  1.0,-1.0,
    -1.0, 1.0,-1.0,
    -1.0,-1.0,-1.0,

    -1.0, 1.0, 1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0,-1.0,
    -1.0, 1.0, 1.0,
    -1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,

    1.0, 1.0, 1.0,
    1.0,-1.0,-1.0,
    1.0, 1.0,-1.0,
    1.0, 1.0, 1.0,
    1.0,-1.0, 1.0,
    1.0,-1.0,-1.0,

    1.0,  1.0, 1.0,
    -1.0, 1.0,-1.0,
    1.0,  1.0,-1.0,
    1.0,  1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,

    1.0,  1.0, 1.0,
    -1.0,-1.0, 1.0,
    1.0, -1.0, 1.0,
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    -1.0, -1.0, 1.0
};

static GLfloat texture_coords[] = {
    1.0, 1.0, 
    0.0, 0.0, 
    1.0, 0.0, 
    1.0, 1.0, 
    0.0, 1.0, 
    0.0, 0.0,

    1.0, 1.0, 
    0.0, 0.0, 
    1.0, 0.0, 
    1.0, 1.0, 
    0.0, 1.0, 
    0.0, 0.0,

    1.0, 1.0, 
    0.0, 0.0, 
    1.0, 0.0, 
    1.0, 1.0, 
    0.0, 1.0, 
    0.0, 0.0,

    1.0, 1.0, 
    0.0, 0.0, 
    1.0, 0.0, 
    1.0, 1.0, 
    0.0, 1.0, 
    0.0, 0.0,

    1.0, 1.0, 
    0.0, 0.0, 
    1.0, 0.0, 
    1.0, 1.0, 
    0.0, 1.0, 
    0.0, 0.0,

    1.0, 1.0, 
    0.0, 0.0, 
    1.0, 0.0, 
    1.0, 1.0, 
    0.0, 1.0, 
    0.0, 0.0,
};

GLuint create_cube() {
    GLuint cube_points_vbo, cube_normals_vbo, texture_vbo, cube_vao;

    glGenBuffers(1, &cube_points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_points), cube_points, GL_STATIC_DRAW);

    glGenBuffers(1, &cube_normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);

    glGenBuffers(1, &texture_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    glBindBuffer(GL_ARRAY_BUFFER, cube_points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cube_normals_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    return cube_vao;
}
