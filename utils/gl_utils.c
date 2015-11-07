#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "gl_utils.h"

void gl_log(int severity_level, char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if (!file) {
        fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file\n", GL_LOG_FILE);
    }

    if (severity_level == ERROR) {
        fprintf(file, "ERROR: ");
    } else if (severity_level == INFO) {
        fprintf(file, "INFO: ");
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fprintf(file, "\n");

    // Also print to stderr if there is an error.
    if (severity_level == ERROR) {
        fprintf(stderr, "ERROR: ");
        va_start(argptr, message);
        vfprintf(stderr, message, argptr);
        va_end(argptr);
        fprintf(stderr, "\n");
    }

    fclose(file);
}

GLuint create_shader_program(char* frag_shader_filename, char* vert_shader_filename) {
    const char* frag_shader_str = read_file(frag_shader_filename);
    const char* vert_shader_str = read_file(vert_shader_filename);

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_str, NULL);
    glCompileShader(frag_shader);

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_str, NULL);
    glCompileShader(vert_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, frag_shader);
    glAttachShader(shader_program, vert_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    return shader_program;
}

const char* read_file(char* filename) {
    char* buffer = 0;
    long length;
    FILE* f = fopen(filename, "rb");

    if (!f) {
        gl_log(ERROR, "Could not read file %s\n", filename); 
        return 0;
    } 

    fseek(f, 0, SEEK_END);
    length = ftell (f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer) {
        fread (buffer, 1, length, f);
    }
    fclose(f);

    return buffer;
}
