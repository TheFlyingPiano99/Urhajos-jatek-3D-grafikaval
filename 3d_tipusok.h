/**
 * @file 3d_tipusok.h
 * @author Simon Zoltán
 * @brief 3D elemek struktúrái
 * @version 0.1
 * @date 2019-10-26
 *
 * @copyright Copyright (c) 2019
 *
 */

// ################################################################
#ifndef _3D_TIPUSOK_IMPORTALVA_
#define _3D_TIPUSOK_IMPORTALVA_
// ################################################################

#include <stdbool.h>

static const double pi = 3.14159265359;

typedef struct ang3d {
    double x_ang, y_ang, z_ang;
} ang3d;

typedef struct vek3d {
    double x, y, z;
} vek3d;

typedef struct rgba {
    int r;
    int g;
    int b;
    int a;
} rgba;

typedef struct poli {
    vek3d pv[3];
    double fenyesseg;
    rgba szin;
} poli;

typedef struct vek3d_tomb {
    vek3d *elemek;
    size_t meret;
    size_t lefoglalt;
} vek3d_tomb;

typedef struct mesh {
    poli *elemek;
    size_t meret;
    size_t lefoglalt;
} mesh;

typedef struct mesh_tomb {
    mesh *elemek;
    size_t meret;
    size_t lefoglalt;
} mesh_tomb;

typedef struct lekepezes_valtozok {
    double near;
    double far;
    double fov;
    double aspect_ratio;
    double fov_tangens;
} lekepezes_valtozok;

typedef struct kamera {
    vek3d pozv;
    vek3d iranyv;
    vek3d felv;
    vek3d oldalra;
    ang3d szog_sebesseg;
    vek3d sebesseg;
} kamera;

typedef struct fizika {
    vek3d gyorsulas;
    vek3d sebesseg;
    ang3d szogsebesseg;
    ang3d szoggyorsulas;
    double tomeg;
} fizika;

typedef struct kontroll {
    bool up;
    bool down;
    bool left;
    bool right;
    bool w;
    bool a;
    bool s;
    bool d;
    bool space;
    bool lctrl;
    bool z;
    bool x;
    bool lshift;
    bool enter;
    bool esc;
} kontroll;

typedef enum obj_tipus {
    urhajo,
    bolygo,
    lovedek,
    meteor,
    reszecske
} obj_tipus;

typedef struct objektum {
    bool lathato;
    bool fantom;
    int elet;
    int letezes_ideje;
    bool vilagit;
    obj_tipus tipus;
    mesh *pmesh;
    rgba szin;
    fizika fizika;
    double sugar;
    vek3d pozicio;
    ang3d szog;
} objektum;

typedef struct objektum_tomb {
    objektum *objp;
    int meret;
} objektum_tomb;

typedef struct matrix4x4 {
    double m[4][4];
} matrix4x4;

// ################################################################
#endif
// ################################################################
