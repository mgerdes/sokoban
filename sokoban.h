static const GLfloat player_color[] = {1.0, 1.0, 1.0};
static const GLfloat wall_color[] = {0.2, 0.2, 0.7};
static const GLfloat crate_color[] = {0.5, 0.0, 0.5};
static const GLfloat crate_on_dest_color[] = {0.6, 0.0, 0.6};
static const GLfloat floor_color[] = {0.8, 0.8, 0.8};
static const GLfloat dest_color[] = {0.0, 0.6, 0.0};

static GLfloat cube_normals[] = {
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    -1.0, 0.0, -0.0,
    -1.0, 0.0, -0.0,
    -1.0, 0.0, -0.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    -1.0, -0.0, 0.0,
    -1.0, -0.0, 0.0,
    -1.0, -0.0, 0.0,
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
    1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,
    1.0,-1.0,-1.0,
    1.0,-1.0, 1.0,
    -1.0,-1.0, 1.0,
    -1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0,-1.0, 1.0,
    -1.0, 1.0, 1.0,
    1.0, 1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0,-1.0,
    1.0, 1.0,-1.0,
    1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0,-1.0,-1.0,
    -1.0, 1.0, 1.0,
    -1.0, 1.0,-1.0,
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
int current_level;
Warehouse* warehouse;