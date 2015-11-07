#define GL_LOG_FILE "log"

enum SEVERITY_LEVELS { INFO, ERROR };

void gl_log(int severity_level, char* message, ...);
GLuint compile_shader(char* filename, GLenum shader_type);
GLuint create_shader_program(char* frag_shader, char* vert_shader); 
const char* read_file(char* filename);
GLFWwindow* create_window(int width, int height, char* title);
GLuint create_texture(char* texture_file);
