#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
#define ONE_RAD_IN_DEG 360.0 / (2.0 * M_PI) //57.2957795

typedef struct Vec {
    double x, y, z, w;
} Vec;

typedef struct Mat {
    float* m;
} Mat;

Vec* create_vec(double, double, double, double);
void delete_vec(Vec*);
Vec* cross_vec(Vec*, Vec*);
Vec* normalize_vec(Vec*);
Vec* mat_times_vec(Mat*, Vec*);
double dot_vec(Vec*, Vec*);
void print_vec(Vec*);

Mat* create_mat(double, double, double, double,
        double, double, double, double,
        double, double, double, double,
        double, double, double, double);
void delete_mat(Mat*);
Mat* identity_mat();
Mat* perspective(double, double, double, double);
Mat* mat_times_mat(Mat*, Mat*);
Mat* look_at(Vec*, Vec*, Vec*);
Mat* translation_mat(double, double, double);
void print_mat(Mat*);
