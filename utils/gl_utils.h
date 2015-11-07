#define GL_LOG_FILE "log"

enum SEVERITY_LEVELS { INFO, ERROR };

void gl_log(int severity_level, char* message, ...);
void init_gl();
GLuint compile_shader(char* filename, GLenum shader_type);
GLuint create_shader_program(char* frag_shader, char* vert_shader); 
const char* read_file(char* filename);
GLuint create_texture(char* texture_file);

int window_width, window_height;
GLFWwindow* window;
