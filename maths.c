#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "maths.h"

Vec* create_vec(double x, double y, double z, double w) {
    Vec* v = malloc(sizeof(Vec));
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
    return v;
}

Vec* cross_vec(Vec* v1, Vec* v2) {
    double x = v1->y * v2->z - v1->z * v2->y;
    double y = v1->z * v2->x - v1->x * v2->z;
    double z = v1->x * v2->y - v1->y * v2->x;
    return create_vec(x, y, z, 1.0);
}

double dot_vec(Vec* v1, Vec* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

Vec* vec_minus_vec(Vec* v1, Vec* v2) {
    return create_vec(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, 1.0);
}

Vec* normalize_vec(Vec* v) {
    double length = sqrt(dot_vec(v, v));
    return create_vec(v->x / length, v->y / length, v->z / length, v->w);
}

/* stored column-major:
 *   a e i m 
 *   b f j n
 *   c g k o
 *   d h l p
 */
Mat* create_mat(double a, double b, double c, double d, 
        double e, double f, double g, double h, 
        double i, double j, double k, double l, 
        double m, double n, double o, double p) {
    Mat* mat = malloc(sizeof(Mat));
    mat->m = malloc(sizeof(double)*16);

    mat->m[0] = a;
    mat->m[1] = e;
    mat->m[2] = i;
    mat->m[3] = m;
    mat->m[4] = b;
    mat->m[5] = f;
    mat->m[6] = j;
    mat->m[7] = n;
    mat->m[8] = c;
    mat->m[9] = g;
    mat->m[10] = k;
    mat->m[11] = o;
    mat->m[12] = d;
    mat->m[13] = h;
    mat->m[14] = l;
    mat->m[15] = p;
    return mat;
}

Mat* zero_mat() {
    return create_mat(0.0,0.0,0.0,0.0,
            0.0,0.0,0.0,0.0,
            0.0,0.0,0.0,0.0,
            0.0,0.0,0.0,0.0);
}

Mat* identity_mat() {
    return create_mat(1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);
}

Mat* translate(Mat* m, Vec* v) {
    Mat* m_t = identity_mat();
    m_t->m[12] = v->x;
    m_t->m[13] = v->y;
    m_t->m[14] = v->z;
    return mat_times_mat(m_t, m);
}

Vec* mat_times_vec(Mat* m, Vec* v) {
    double x = m->m[0] * v->x + m->m[4] * v->y + m->m[8] * v->z + m->m[12] * v->w;
    double y = m->m[1] * v->x + m->m[5] * v->y + m->m[9] * v->z + m->m[13] * v->w;
    double z = m->m[2] * v->x + m->m[6] * v->y + m->m[10] * v->z + m->m[14] * v->w;
    double w = m->m[3] * v->x + m->m[7] * v->y + m->m[11] * v->z + m->m[15] * v->w;
    return create_vec(x, y, z, w);
}

Mat* mat_times_mat(Mat* m1, Mat* m2) {
    Mat* m = zero_mat();
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            double sum = 0;
            for (int i = 0; i < 4; i++) {
                sum += m1->m[row + i*4] * m2->m[i + col*4]; 
            }
            m->m[row + col*4] = sum;
        }
    }
    return m;
}

Mat* look_at (Vec* cam_pos, Vec* targ_pos, Vec* up) {
    Mat* p = identity_mat();
    p = translate(p, create_vec(-cam_pos->x, -cam_pos->y, -cam_pos->z, 1.0));

    Vec* d = vec_minus_vec(targ_pos, cam_pos);
    Vec* f = normalize_vec(d);
    Vec* r = normalize_vec(cross_vec(f, up));
    Vec* u = normalize_vec(cross_vec(r, f));

    Mat* ori = identity_mat();
    ori->m[0] = r->x;
    ori->m[4] = r->y;
    ori->m[8] = r->z;
    ori->m[1] = u->x;
    ori->m[5] = u->y;
    ori->m[9] = u->z;
    ori->m[2] = -f->x;
    ori->m[6] = -f->y;
    ori->m[10] = -f->z;

    return mat_times_mat(ori, p);
}

Mat* perspective (double fovy, double aspect, double near, double far) {
    double fov_rad = fovy * ONE_DEG_IN_RAD;
    double range = tan(fov_rad / 2.0f) * near;
    double sx = (2.0f * near) / (range * aspect + range * aspect);
    double sy = near / range;
    double sz = -(far + near) / (far - near);
    double pz = -(2.0f * far * near) / (far - near);
    Mat* m = zero_mat();
    m->m[0] = sx;
    m->m[5] = sy;
    m->m[10] = sz;
    m->m[14] = pz;
    m->m[11] = -1.0f;
    return m;
}

void print_mat(Mat* m) {
    printf("\n");
    printf ("[%.2f][%.2f][%.2f][%.2f]\n", m->m[0], m->m[4], m->m[8], m->m[12]);
    printf ("[%.2f][%.2f][%.2f][%.2f]\n", m->m[1], m->m[5], m->m[9], m->m[13]);
    printf ("[%.2f][%.2f][%.2f][%.2f]\n", m->m[2], m->m[6], m->m[10], m->m[14]);
    printf ("[%.2f][%.2f][%.2f][%.2f]\n", m->m[3], m->m[7], m->m[11], m->m[15]);
}

void print_vec(Vec* v) {
    printf ("[%.2f, %.2f, %.2f, %.2f]\n", v->x, v->y, v->z, v->w);
}
