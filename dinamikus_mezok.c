#include <stdlib.h>
#include <stdio.h>
#include "debugmalloc.h"
#include "dinamikus_mezok.h"

#define NOVELES 50

void felszabadit(void *elemek) {
    if (elemek) {
        free(elemek);
    }
}

inline mesh mesh_letrehoz() {
    return (mesh) { .elemek = NULL, .meret = 0, .lefoglalt = 0 };
}

inline vek3d_tomb vek3d_tomb_letrehoz() {
    return (vek3d_tomb) { .elemek = NULL, .meret = 0, .lefoglalt = 0 };
}

inline mesh_tomb mesh_tomb_letrehoz() {
    return (mesh_tomb) { .elemek = NULL, .meret = 0, .lefoglalt = 0 };
}

inline void vek3d_tomb_hozzaad(vek3d_tomb *m, vek3d uj) {
    if (m->lefoglalt == 0) {
        m->lefoglalt = NOVELES;
        m->elemek = malloc(m->lefoglalt * sizeof(vek3d));
    } else if (m->meret >= m->lefoglalt) {
        m->lefoglalt = m->meret + NOVELES;
        m->elemek = realloc(m->elemek, m->lefoglalt * sizeof(vek3d));
    }
    m->elemek[m->meret++] = uj;
}

inline void mesh_hozzaad(mesh *m, poli uj) {
    if (m->lefoglalt == 0) {
        m->lefoglalt = NOVELES;
        m->elemek = malloc(m->lefoglalt * sizeof(poli));
    } else if (m->meret >= m->lefoglalt) {
        m->lefoglalt = m->meret + NOVELES;
        m->elemek = realloc(m->elemek, m->lefoglalt * sizeof(poli));
    }
    m->elemek[m->meret++] = uj;
}

inline void mesh_tomb_hozzaad(mesh_tomb *m, mesh uj) {
    if (m->lefoglalt == 0) {
        m->lefoglalt = NOVELES;
        m->elemek = malloc(m->lefoglalt * sizeof(mesh));
    } else if (m->meret >= m->lefoglalt) {
        m->lefoglalt = m->meret + NOVELES;
        m->elemek = realloc(m->elemek, m->lefoglalt * sizeof(mesh));
    }
    m->elemek[m->meret++] = uj;
}
