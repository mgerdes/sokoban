#define GL_LOG_FILE "log"

enum SEVERITY_LEVELS { INFO, ERROR };

void gl_log(int severity_level, char* message, ...);
GLuint create_shader_program(char* frag_shader, char* vert_shader); 
const char* read_file(char* filename);
