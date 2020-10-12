/**
 * @file dinamikus_mezok.h
 * @author Simon Zoltán
 * @brief Dinamikus tárolást segítő adatszerkezetek és funkciók
 * @version 0.1
 * @date 2019-11-14
 *
 * @copyright Copyright (c) 2019
 *
 */

// ################################################################
#ifndef _DINAMIKUS_MEZOK_IMPORTALVA_
#define _DINAMIKUS_MEZOK_IMPORTALVA_
// ################################################################

#include <stddef.h>
#include "3d_tipusok.h"

void felszabadit(void *elemek);

// funkcio deklaraciok:
vek3d_tomb vek3d_tomb_letrehoz();
mesh       mesh_letrehoz();
mesh_tomb  mesh_tomb_letrehoz();

void vek3d_tomb_hozzaad(vek3d_tomb *m, vek3d uj);
void mesh_hozzaad(mesh *m, poli uj);
void mesh_tomb_hozzaad(mesh_tomb *m, mesh uj);

// ################################################################
#endif
// ################################################################