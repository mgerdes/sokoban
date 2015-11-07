static const GLfloat player_color[] = {0.1, 0.1, 0.1};
static const GLfloat wall_color[] = {0.2, 0.2, 0.7};
static const GLfloat crate_color[] = {0.5, 0.0, 0.5};
static const GLfloat crate_on_dest_color[] = {0.6, 0.0, 0.6};
static const GLfloat floor_color[] = {0.8, 0.8, 0.8};
static const GLfloat dest_color[] = {0.4, 0.9, 0.4};


GLuint cube_vao;
GLuint shader_program;
GLuint view, proj, color, texture;

Vec* camera_pos;
Vec* center;
Vec* up;

int current_level;
Warehouse* warehouse;
