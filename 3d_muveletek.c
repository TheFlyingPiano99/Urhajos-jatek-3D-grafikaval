/**
 * @file 3d_muveletek.c
 * @author Simon Zoltán
 * @brief 3D elemekkel dolgozó funkciók
 * @version 0.1
 * @date 2019-10-26
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"

#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "3d_tipusok.h"
#include "3d_muveletek.h"

vek3d matrix_szorzo (vek3d in, matrix4x4 mx) {
    vek3d out;
    out.x = in.x * mx.m[0][0] + in.y * mx.m[1][0] + in.z * mx.m[2][0] + mx.m[3][0];
    out.y = in.x * mx.m[0][1] + in.y * mx.m[1][1] + in.z * mx.m[2][1] + mx.m[3][1];
    out.z = in.x * mx.m[0][2] + in.y * mx.m[1][2] + in.z * mx.m[2][2] + mx.m[3][2];
    double w =  in.x * mx.m[0][3] + in.y * mx.m[1][3] + in.z * mx.m[2][3] + mx.m[3][3];
    if (w != 0.0) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
    return out;
}

vek3d randomizalt_vektor(int offset, int tartomany) {
    vek3d vek;
    vek.x = rand() %tartomany;
    vek.x += offset;
    int neg = rand() % 2;
    if (neg == 0) {
        vek.x *= -1;
    }
    vek.y = rand() %tartomany;
    vek.y += offset;
    neg = rand() % 2;
    if (neg == 0) {
        vek.y *= -1;
    }
    vek.z = rand() %tartomany;
    vek.z += offset;
    neg = rand() % 2;
    if (neg == 0) {
        vek.z *= -1;
    }
    return vek;
}

vek3d vektor_forgatas (vek3d vek, ang3d szog) {
    matrix4x4 x_forgatas, y_forgatas, z_forgatas;
    x_forgatas_matrix_generator(&x_forgatas, &szog.x_ang);
    y_forgatas_matrix_generator(&y_forgatas, &szog.y_ang);
    z_forgatas_matrix_generator(&z_forgatas, &szog.z_ang);

    vek = matrix_szorzo(vek, z_forgatas);
    vek = matrix_szorzo(vek, y_forgatas);
    vek = matrix_szorzo(vek, x_forgatas);
    return vek;
}

matrix4x4 mutato_matrix(vek3d *pozv, vek3d *celv, vek3d *felv) {
    //Uj elore vektor:
    vek3d uj_elorev = vektor_normalizacio(vektor_kivonas(celv, pozv));

    //Uj fel vektor:
    double skal_szorzat = skalar_szorzat(felv, &uj_elorev);
    vek3d fel_temp = vektor_skalarral_szorzas(&uj_elorev, skal_szorzat);
    vek3d uj_felv = vektor_normalizacio(vektor_kivonas(felv, &fel_temp));

    //Uj jobbra vektor:
    vek3d uj_jobbrav = vektorialis_szorzat(&uj_felv, &uj_elorev);

    //Mutato matrix letrehozasa:
    matrix4x4 matrix;
    matrix.m[0][0] = uj_jobbrav.x;
    matrix.m[0][1] = uj_jobbrav.y;
    matrix.m[0][2] = uj_jobbrav.z;
    matrix.m[0][3] = 0;
    matrix.m[1][0] = uj_felv.x;
    matrix.m[1][1] = uj_felv.y;
    matrix.m[1][2] = uj_felv.z;
    matrix.m[1][3] = 0;
    matrix.m[2][0] = uj_elorev.x;
    matrix.m[2][1] = uj_elorev.y;
    matrix.m[2][2] = uj_elorev.z;
    matrix.m[2][3] = 0;
    matrix.m[3][0] = pozv->x;
    matrix.m[3][1] = pozv->y;
    matrix.m[3][2] = pozv->z;
    matrix.m[3][3] = 1;
    return matrix;
}

matrix4x4 specialis_matrix_inverz(matrix4x4 *m) {
    matrix4x4 kim;
    kim.m[0][0] = m->m[0][0];
    kim.m[0][1] = m->m[1][0];
    kim.m[0][2] = m->m[2][0];
    kim.m[0][3] = 0.0;
    kim.m[1][0] = m->m[0][1];
    kim.m[1][1] = m->m[1][1];
    kim.m[1][2] = m->m[2][1];
    kim.m[1][3] = 0.0;
    kim.m[2][0] = m->m[0][2];
    kim.m[2][1] = m->m[1][2];
    kim.m[2][2] = m->m[2][2];
    kim.m[2][3] = 0.0;
    kim.m[3][0] = -(m->m[3][0] * kim.m[0][0] + m->m[3][1] * kim.m[1][0] + m->m[3][2] * kim.m[2][0]);
    kim.m[3][1] = -(m->m[3][0] * kim.m[0][1] + m->m[3][1] * kim.m[1][1] + m->m[3][2] * kim.m[2][1]);
    kim.m[3][2] = -(m->m[3][0] * kim.m[0][2] + m->m[3][1] * kim.m[1][2] + m->m[3][2] * kim.m[2][2]);
    kim.m[3][3] = 1.0;
    return kim;
}

vek3d vektor_osszeadas(vek3d *v1, vek3d *v2) {
    return (vek3d) {
        .x = v1->x + v2->x,
        .y = v1->y + v2->y,
        .z = v1->z + v2->z
        };
}

vek3d vektor_kivonas(vek3d *v1, vek3d *v2) {
    return (vek3d) {
        .x = v1->x - v2->x,
        .y = v1->y - v2->y,
        .z = v1->z - v2->z
        };
}

vek3d vektor_szorzas(vek3d *v1, vek3d *v2) {
    return (vek3d) {
        .x = v1->x * v2->x,
        .y = v1->y * v2->y,
        .z = v1->z * v2->z };
}

vek3d normal_vektor(vek3d *iranyv1, vek3d *iranyv2) {
    return vektor_normalizacio(
        vektorialis_szorzat(iranyv1, iranyv2)
    );
}

vek3d vektor_skalarral_szorzas(vek3d *vek, double skal) {
    return (vek3d) {
        .x = vek->x * skal,
        .y = vek->y * skal,
        .z = vek->z * skal };
}

double skalar_szorzat(vek3d *v1, vek3d *v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

vek3d vektorialis_szorzat(vek3d *v1, vek3d *v2) {
    return (vek3d) {
        .x = v1->y * v2->z - v1->z * v2->y,
        .y = v1->z * v2->x - v1->x * v2->z,
        .z = v1->x * v2->y - v1->y * v2->x
    };
}

double vektor_hossz(vek3d *vek) {
    return sqrt(vek->x * vek->x + vek->y * vek->y + vek->z * vek->z);
}

vek3d vektor_normalizacio(vek3d vek) {
    double hossz = vektor_hossz(&vek);
    return vektor_skalarral_szorzas(&vek, 1 / hossz);
}

void matrix_nullazas (matrix4x4 *matrix){
    for (int i = 0; i<4; i++) {
        for (int j = 0; j<4; j++) {
            matrix->m[i][j] = 0;
        }
    }
}

void projekcio_matrix_generator(
    matrix4x4 *matrix,
    double *aspect_ratio,
    double *fov_tangens,
    double *far,
    double *near)
{
    matrix_nullazas(matrix);
    matrix->m[0][0] = *aspect_ratio * *fov_tangens;
    matrix->m[1][1] = *fov_tangens;
    matrix->m[2][2] = *far / (*far - *near);
    matrix->m[3][2] = (-*near * *far) / (*far - *near);
    matrix->m[2][3] = 1;
}

void z_forgatas_matrix_generator(matrix4x4 *matrix, double *szog) {
        matrix_nullazas(matrix);
        matrix->m[0][0] = cos(*szog);
        matrix->m[0][1] = sin(*szog);
        matrix->m[1][0] = -sin(*szog);
        matrix->m[1][1] = cos(*szog);
        matrix->m[2][2] = 1;
        matrix->m[3][3] = 1;
}

void y_forgatas_matrix_generator(matrix4x4 *matrix, double *szog) {
        matrix_nullazas(matrix);
		matrix->m[0][0] = cos(*szog);
		matrix->m[0][2] = sin(*szog);
		matrix->m[2][0] = -sin(*szog);
		matrix->m[1][1] = 1.0;
		matrix->m[2][2] = cos(*szog);
		matrix->m[3][3] = 1.0;
}

void x_forgatas_matrix_generator(matrix4x4 *matrix, double *szog) {
        matrix_nullazas(matrix);
        matrix->m[0][0] = 1;
        matrix->m[1][1] = cos(*szog);
        matrix->m[1][2] = sin(*szog);
        matrix->m[2][1] = -sin(*szog);
        matrix->m[2][2] = cos(*szog);
        matrix->m[3][3] = 1;
}

matrix4x4 nezet_matrix_generator(kamera *kam) {

    vek3d kamera_cel_vek;
    kamera_cel_vek = vektor_osszeadas(&kam->iranyv, &kam->pozv);
    matrix4x4 kamera_matrix = mutato_matrix(&kam->pozv, &kamera_cel_vek, &kam->felv);
    matrix4x4 nezet_matrix  = specialis_matrix_inverz(&kamera_matrix);
    return nezet_matrix;
}


vek3d sik_metszes (vek3d sik_pont, vek3d sik_normal, vek3d vonal_kezdet, vek3d vonal_veg) {
    //Biztosagi normalizalas:
    sik_normal = vektor_normalizacio(sik_normal);
    double sik_d = -skalar_szorzat(&sik_normal, &sik_pont);
    double ad = skalar_szorzat(&vonal_kezdet, &sik_normal);
    double bd = skalar_szorzat(&vonal_veg, &sik_normal);
    double t = (-sik_d - ad) / (bd - ad);
    vek3d vonal_kezd_veg = vektor_kivonas(&vonal_veg, &vonal_kezdet);
    vek3d vonal_metszesig = vektor_skalarral_szorzas(&vonal_kezd_veg, t);
    return vektor_osszeadas(&vonal_kezdet, &vonal_metszesig);
}

double siktol_tavolsag (vek3d sik_pont,vek3d sik_normal, vek3d p) {
    vek3d n = vektor_normalizacio(p);
    double skalar = skalar_szorzat(&sik_normal, &sik_pont);
    return (sik_normal.x * p.x + sik_normal.y * p.y + sik_normal.z * p.z - skalar);
}

int clip(vek3d *sik_pont, vek3d *sik_normal, poli *be_haromsz, poli *ki_haromsz1,  poli *ki_haromsz2) {
    //Biztosagi normalizalas:
    *sik_normal = vektor_normalizacio(*sik_normal);

    //Atmeneti tarolok:
    vek3d belso_pontok[3];
    int belso_pont_szamlalo = 0;
    vek3d kulso_pontok[3];
    int kulso_pont_szamlalo = 0;

    //Elojeles tavolsag minden pontra:
    double tav0 = siktol_tavolsag(*sik_pont, *sik_normal, be_haromsz->pv[0]);
    double tav1 = siktol_tavolsag(*sik_pont, *sik_normal, be_haromsz->pv[1]);
    double tav2 = siktol_tavolsag(*sik_pont, *sik_normal, be_haromsz->pv[2]);

    //Ha a "tav" pozitiv, a pont a sikon belul fekszik.
    if (tav0 >= 0)
        belso_pontok[belso_pont_szamlalo++] = be_haromsz->pv[0];
    else
        kulso_pontok[kulso_pont_szamlalo++] = be_haromsz->pv[0];
    if (tav1 >= 0)
        belso_pontok[belso_pont_szamlalo++] = be_haromsz->pv[1];
    else
        kulso_pontok[kulso_pont_szamlalo++] = be_haromsz->pv[1];
    if (tav2 >= 0)
        belso_pontok[belso_pont_szamlalo++] = be_haromsz->pv[2];
    else
        kulso_pontok[kulso_pont_szamlalo++] = be_haromsz->pv[2];

    //Az egesz haromszog clip-elodott:
    if (belso_pont_szamlalo == 0)
        return 0;
    //Az egesz haromszog bent van:
    if (belso_pont_szamlalo == 3) {
        *ki_haromsz1 = *be_haromsz;
        return 1;
    }
    //2 pont log ki, 1 uj haromszogre kell bontani az eredetit:
    if (belso_pont_szamlalo == 1 && kulso_pont_szamlalo == 2) {
        ki_haromsz1->fenyesseg = be_haromsz->fenyesseg;
        ki_haromsz1->szin = be_haromsz->szin;
        ki_haromsz1->pv[0] = belso_pontok[0];
        ki_haromsz1->pv[1] = sik_metszes(*sik_pont, *sik_normal, belso_pontok[0], kulso_pontok[0]);
        ki_haromsz1->pv[2] = sik_metszes(*sik_pont, *sik_normal, belso_pontok[0], kulso_pontok[1]);
        return 1;
    }
    //1 pont log ki, 2 uj haromszogre kell bontani az eredetit:
    if (belso_pont_szamlalo == 2 && kulso_pont_szamlalo == 1) {

        ki_haromsz1->fenyesseg = be_haromsz->fenyesseg;
        ki_haromsz1->szin = be_haromsz->szin;
        ki_haromsz2->fenyesseg = be_haromsz->fenyesseg;
        ki_haromsz2->szin = be_haromsz->szin;

        ki_haromsz1->pv[0] = belso_pontok[0];
        ki_haromsz1->pv[1] = belso_pontok[1];
        ki_haromsz1->pv[2] = sik_metszes(*sik_pont, *sik_normal, belso_pontok[0], kulso_pontok[0]);

        ki_haromsz2->pv[0] = belso_pontok[1];
        ki_haromsz2->pv[1] = ki_haromsz1->pv[2];
        ki_haromsz2->pv[2] = sik_metszes(*sik_pont, *sik_normal, belso_pontok[1], kulso_pontok[0]);
        return 2;
    }
}

matrix4x4 rotacio_matrix_tetszoleges_tengely(float szog_sin, float szog_cos, vek3d iranyv) {
    float x2 = iranyv.x * iranyv.x;
    float y2 = iranyv.y * iranyv.y;
    float z2 = iranyv.z * iranyv.z;
    float L = (x2 + y2 + z2);
    matrix4x4 m;
    m.m[0][0] = (x2 + (y2 + z2) * szog_cos) / L;
    m.m[0][1] = (iranyv.x * iranyv.y * (1 - szog_cos) - iranyv.z * sqrt(L) * szog_sin) / L;
    m.m[0][2] = (iranyv.x * iranyv.z * (1 - szog_cos) + iranyv.y * sqrt(L) * szog_sin) / L;
    m.m[0][3] = 0.0;

    m.m[1][0] = (iranyv.x * iranyv.y * (1 - szog_cos) + iranyv.z * sqrt(L) * szog_sin) / L;
    m.m[1][1] = (y2 + (x2 + z2) * szog_cos) / L;
    m.m[1][2] = (iranyv.y * iranyv.z * (1 - szog_cos) - iranyv.x * sqrt(L) * szog_sin) / L;
    m.m[1][3] = 0.0;

    m.m[2][0] = (iranyv.x * iranyv.z * (1 - szog_cos) - iranyv.y * sqrt(L) * szog_sin) / L;
    m.m[2][1] = (iranyv.y * iranyv.z * (1 - szog_cos) + iranyv.x * sqrt(L) * szog_sin) / L;
    m.m[2][2] = (z2 + (x2 + y2) * szog_cos) / L;
    m.m[2][3] = 0.0;

    m.m[3][0] = 0.0;
    m.m[3][1] = 0.0;
    m.m[3][2] = 0.0;
    m.m[3][3] = 1.0;

    return m;
}

//A kapott polygonon elvegzi a kirajzolashoz szukseges szamitasokat a kapott matrixokkal valo szorzas reven:
int haromszog_lekepezes (
        poli *bemenet,
        vek3d *pozicio,
        bool *vilagit,
        matrix4x4 *rotacio_x,
        matrix4x4 *rotacio_y,
        matrix4x4 *rotacio_z,
        matrix4x4 *projekcio,
        kamera *kam,
        int p_kep_szelesseg,
        int p_kep_magassag,
        poli *kimenet1,
        poli *kimenet2) {

    poli forgatott_z, forgatott_y, forgatott_x, betolt, nezett;

    for (int vek = 0; vek < 3; vek++) {
        forgatott_z.pv[vek] = matrix_szorzo(bemenet->pv[vek], *rotacio_z);
        forgatott_y.pv[vek] = matrix_szorzo(forgatott_z.pv[vek], *rotacio_y);
        forgatott_x.pv[vek] = matrix_szorzo(forgatott_y.pv[vek], *rotacio_x);

        betolt.pv[vek] = forgatott_x.pv[vek];
        betolt.pv[vek] = vektor_osszeadas(&betolt.pv[vek], pozicio);
        betolt.pv[vek].z += 1;
    }

    vek3d iranyv1 = vektor_kivonas(&betolt.pv[1], &betolt.pv[0]);
    vek3d iranyv2 = vektor_kivonas(&betolt.pv[2], &betolt.pv[0]);
    vek3d normalv = normal_vektor(&iranyv1, &iranyv2);

    vek3d kamera_poz_tav = vektor_kivonas(&betolt.pv[0], &kam->pozv);
    double lathato = skalar_szorzat(&normalv, &kamera_poz_tav);
    int clipelt_haromszogek_szama = 0;
    if (lathato < 0.0) {
        //Feny-szamitasok---------------------------------
        if (*vilagit) {
            nezett.fenyesseg = 1;
        }
        else {
            vek3d feny_forras_iranya = {0,-1, -1};
            feny_forras_iranya = vektor_normalizacio(feny_forras_iranya);

            double feny_beeses = skalar_szorzat(&normalv, &feny_forras_iranya);
            nezett.fenyesseg = feny_beeses;
            if (nezett.fenyesseg > 1) {
                nezett.fenyesseg = 1;
            }
            else if (nezett.fenyesseg < 0) {
                nezett.fenyesseg = 0;
            }
        }
        nezett.szin = bemenet->szin;

        //Kamera nezet:------------------------------
        matrix4x4 nezet_matrix = nezet_matrix_generator(kam);

        //Vetites------------------------------------
        for (int vek = 0; vek < 3; vek++)
            nezett.pv[vek] = matrix_szorzo(betolt.pv[vek], nezet_matrix);

        //Clipeles a terben:
        poli clipelt_h[2];
        clipelt_haromszogek_szama = clip(&(vek3d){0.0, 0.0, 1.0}, &(vek3d){0.0, 0.0, 1.0}, &nezett, &clipelt_h[0], &clipelt_h[1]);

        poli projektalt[2];
        for (int n = 0; n < clipelt_haromszogek_szama; n++) {
                projektalt[n].fenyesseg = clipelt_h[n].fenyesseg;
                projektalt[n].szin = clipelt_h[n].szin;
            for (int vek = 0; vek < 3; vek++) {
                projektalt[n].pv[vek] = matrix_szorzo(clipelt_h[n].pv[vek], *projekcio);
                //Meretezes----------------------------------

                projektalt[n].pv[vek].x += 1;
                projektalt[n].pv[vek].y += 1;
                projektalt[n].pv[vek].x *= 0.5 * p_kep_szelesseg;
                projektalt[n].pv[vek].y *= 0.5 * p_kep_magassag;
            }
        }
        if (clipelt_haromszogek_szama == 1)
            *kimenet1 = projektalt[0];
        else if (clipelt_haromszogek_szama == 2) {
            *kimenet1 = projektalt[0];
            *kimenet2 = projektalt[1];
        }
    }
    return clipelt_haromszogek_szama;
}

vek3d minimap_lekepezes (
        vek3d pozicio,
        matrix4x4 *projekciom,
        kamera kam,
        int p_kep_szelesseg,
        int p_kep_magassag) {

    vek3d temp = (vek3d){-kam.iranyv.x, -kam.iranyv.y, -kam.iranyv.z};
    kam.iranyv = kam.felv;
    kam.felv = temp;

    //Kamera nezet:------------------------------
    matrix4x4 nezet_matrix = nezet_matrix_generator(&kam);

    //Vetites------------------------------------
    pozicio = matrix_szorzo(pozicio, nezet_matrix);
    float tav = siktol_tavolsag((vek3d){0.0,0.0,0.0}, (vek3d){0.0, 0.0, 1.0}, pozicio);
    pozicio = matrix_szorzo(pozicio, *projekciom);
    pozicio.z = tav * -1;

    //Meretezes----------------------------------
    if (pozicio.z > 0) {
        pozicio.x *= -1;
        pozicio.y *= -1;
    }

    pozicio.x *= 0.05;
    pozicio.y *= 0.05;
    pozicio.z *= 0.01;

    double hossz = vektor_hossz(&pozicio);
    if (hossz > 1) {
        pozicio = vektor_normalizacio(pozicio);
    }
    pozicio.x += 1;
    pozicio.y += 1;

    pozicio.x *= 0.5 * p_kep_szelesseg;
    pozicio.y *= 0.5 * p_kep_magassag;
    pozicio.z *= 0.5 * p_kep_magassag;

    return pozicio;
}

vek3d hatter_lekepezes (
        vek3d pozicio,
        matrix4x4 *projekciom,
        kamera kam,
        int p_kep_szelesseg,
        int p_kep_magassag) {


    kam.pozv = (vek3d){0,0,0};
    //Kamera nezet:------------------------------
    matrix4x4 nezet_matrix = nezet_matrix_generator(&kam);

    //Vetites------------------------------------
    pozicio = matrix_szorzo(pozicio, nezet_matrix);
    float tav = siktol_tavolsag((vek3d){0.0,0.0,0.0}, (vek3d){0.0, 0.0, 1.0}, pozicio);
    pozicio = matrix_szorzo(pozicio, *projekciom);
    pozicio.z = tav;

    //Meretezes----------------------------------

    pozicio.x += 1;
    pozicio.y += 1;

    pozicio.x *= 0.5 * p_kep_szelesseg;
    pozicio.y *= 0.5 * p_kep_magassag;
//    pozicio.x = fmod(abs(pozicio.x, p_kep_szelesseg);
//    pozicio.y = fmod(pozicio.y, p_kep_magassag);

    return pozicio;
}


int osszehasonlit(const void *ph1,const void *ph2) {
    poli *h1 = (poli*)ph1;
    poli *h2 = (poli*)ph2;
    double elem1 = (h1->pv[0].z + h1->pv[1].z + h1->pv[2].z)/3.0;
    double elem2 = (h2->pv[0].z + h2->pv[1].z + h2->pv[2].z)/3.0;
    return (elem1 > elem2) ? -1 : 1;
}

fizika fizika_letrehozo(double tomeg) {
    fizika fiz;
    fiz.gyorsulas = (vek3d){0,0,0};
    fiz.sebesseg = (vek3d){0,0,0};
    fiz.szogsebesseg = (ang3d){0,0,0};
    fiz.szoggyorsulas = (ang3d){0,0,0};
    fiz.tomeg = tomeg;
    return fiz;
}

void objektum_letrehozo(objektum *obj, mesh_tomb *meshek, rgba szin, obj_tipus tipus) {
    mesh* meshp;
    int elet;
    float tomeg;
    double sugar;
    bool lathato;
    bool vilagit;
    //Ha igen, akkor nem utkozik:
    bool fantom;
    switch (tipus) {
        case urhajo:
            meshp = &meshek->elemek[0];
            elet = 10;
            tomeg = 1000;
            sugar = 2;
            lathato = true;
            vilagit = false;
            fantom = false;
            break;
        case bolygo:
            meshp = &meshek->elemek[1];
            elet = 50;
            tomeg = 10000;
            sugar = 10;
            lathato = true;
            vilagit = false;
            fantom = false;
            break;
        case lovedek:
            meshp = &meshek->elemek[3];
            elet = 1;
            tomeg = 5;
            sugar = 0.5;
            lathato = false;
            vilagit = true;
            fantom = false;
            break;
        case meteor:
            meshp = &meshek->elemek[2];
            elet = 4;
            tomeg = 100;
            sugar = 1;
            lathato = true;
            vilagit = false;
            fantom = false;
            break;
        case reszecske:
            meshp = &meshek->elemek[4];
            elet = 1;
            tomeg = 2;
            sugar = 1;
            lathato = false;
            vilagit = true;
            fantom = true;
            break;
    }
    obj->fantom = fantom;
    obj->letezes_ideje = 0;
    obj->vilagit = vilagit;
    obj->elet = elet;
    obj->tipus = tipus;
    obj->pmesh = meshp;
    obj->szin = szin;
    for (int j = 0; j < obj->pmesh->meret; j++) {
        obj->pmesh->elemek[j].szin = obj->szin;
    }
    obj->fizika = fizika_letrehozo(tomeg);
    obj->pozicio   = (vek3d){0,0,0};
    obj->szog      = (ang3d){0,0,0};
    obj->lathato = lathato;
    obj->sugar = sugar;
}

bool utkozes (objektum *obj1, objektum *obj2) {
    vek3d kulonbsegv = vektor_kivonas(&obj1->pozicio, &obj2->pozicio);
    float hossz = vektor_hossz(&kulonbsegv);
    return (hossz < (obj1->sugar+obj2->sugar))? true:false;
}

void mozgas (objektum *obj, int t) {
    obj->pozicio.x += obj->fizika.sebesseg.x * t;
    obj->pozicio.y += obj->fizika.sebesseg.y * t;
    obj->pozicio.z += obj->fizika.sebesseg.z * t;

    obj->szog.x_ang += obj->fizika.szogsebesseg.x_ang * t;
    obj->szog.y_ang += obj->fizika.szogsebesseg.y_ang * t;
    obj->szog.z_ang += obj->fizika.szogsebesseg.z_ang * t;
}

void mesh_atszinez (mesh *m, rgba szin) {
        for (int i = 0; i < m->meret; i++) {
            m->elemek[i].szin = szin;
        }
}

void reszecske_effektus (vek3d poz, rgba kapott_szin, objektum_tomb *reszecskek) {
    static int j = 0;
    for (int i = j; i < j+10; i++) {
        reszecskek->objp[i].elet = 1;
        reszecskek->objp[i].lathato = true;
        reszecskek->objp[i].pozicio = poz;
        reszecskek->objp[i].szin = kapott_szin;
        mesh_atszinez(reszecskek->objp[i].pmesh, kapott_szin);
        vek3d veletlen = randomizalt_vektor(0, 10);
        reszecskek->objp[i].fizika.sebesseg = vektor_skalarral_szorzas(&veletlen, 0.005);
        reszecskek->objp[i].fizika.szogsebesseg = (ang3d){(rand()%10)*0.001,(rand()%10)*0.001,(rand()%10)*0.001};
    }
    j += 10;
    if (j >= 20)
        j = 0;
}

//Fo fizikai funkcio:
void fizikai_muveletek(objektum_tomb *objektumok, objektum_tomb *reszecskek, Mix_Chunk *robbanas_hang, int t) {
    for (int i = 0; i < objektumok->meret; i++) {
        for (int j = 0; j < objektumok->meret; j++) {
            if (&objektumok->objp[i] != &objektumok->objp[j]) {
                if (objektumok->objp[i].lathato && objektumok->objp[j].lathato) {
                    if (!objektumok->objp[i].fantom && !objektumok->objp[j].fantom) {
                        if (utkozes(&objektumok->objp[i], &objektumok->objp[j])) {
                            objektumok->objp[i].elet--;
                            objektumok->objp[j].elet--;
                            reszecske_effektus(objektumok->objp[i].pozicio, (rgba){255, 230,100,255}, reszecskek);
                            break;
                        }
                    }
                }
            }
        }
        if (objektumok->objp[i].lathato) {
            if (objektumok->objp[i].elet <= 0 && objektumok->objp[i].lathato) {
                Mix_PlayChannel(-1,robbanas_hang,0);
                objektumok->objp[i].lathato = false;
            }
            mozgas(&objektumok->objp[i],t);
            if (objektumok->objp[i].tipus == lovedek) {
                objektumok->objp[i].letezes_ideje += t;
                if (objektumok->objp[i].letezes_ideje > 2000) {
                    objektumok->objp[i].lathato = false;
                    objektumok->objp[i].letezes_ideje = 0;
                }
            }
            if (objektumok->objp[i].tipus == reszecske) {
                objektumok->objp[i].letezes_ideje += t;
                mesh_atszinez(objektumok->objp[i].pmesh, (rgba){objektumok->objp[i].szin.r,
                                                                objektumok->objp[i].szin.g,
                                                                objektumok->objp[i].szin.b,
                                                                255 - objektumok->objp[i].letezes_ideje*0.5});
                if (objektumok->objp[i].letezes_ideje > 500) {
                    objektumok->objp[i].lathato = false;
                    objektumok->objp[i].letezes_ideje = 0;
                    objektumok->objp[i].szin.a = 255;
                }
            }
        }
    }
}

int mesh_feldolgozas (objektum_tomb *objektumok,
                      objektum *letiltott,
                      kamera *kam,
                      matrix4x4 *projekcio,
                      int kep_szelesseg,
                      int kep_magassag,
                      int *offset,
                      poli *kirajzolasra_varo) {

    int kir_varo_db = 0;

    for (int obj=0; obj < objektumok->meret; obj++) {
        if (objektumok->objp[obj].lathato && &objektumok->objp[obj] != letiltott) {
            matrix4x4 x_forgatas, y_forgatas, z_forgatas;
            x_forgatas_matrix_generator(&x_forgatas, &objektumok->objp[obj].szog.x_ang);
            y_forgatas_matrix_generator(&y_forgatas, &objektumok->objp[obj].szog.y_ang);
            z_forgatas_matrix_generator(&z_forgatas, &objektumok->objp[obj].szog.z_ang);

            for (int tri = 0; tri < objektumok->objp[obj].pmesh->meret; tri++) {
                poli visszakapott[2];
                int db = haromszog_lekepezes(
                    &objektumok->objp[obj].pmesh->elemek[tri],
                    &objektumok->objp[obj].pozicio,
                    &objektumok->objp[obj].vilagit,
                    &x_forgatas,
                    &y_forgatas,
                    &z_forgatas,
                    projekcio,
                    kam,
                    kep_szelesseg,
                    kep_magassag,
                    &visszakapott[0],
                    &visszakapott[1]);

                for (int i = 0; i < db; i++) {
                    if (visszakapott[i].pv[0].x < kep_szelesseg+1000 && visszakapott[i].pv[0].x > -1000) {
                        if (visszakapott[i].pv[0].y < kep_magassag+1000 && visszakapott[i].pv[0].y > -1000) {
                            kirajzolasra_varo[kir_varo_db++] = visszakapott[i];
                        }
                    }
                }
            }
        }
    }
    qsort(kirajzolasra_varo, kir_varo_db, sizeof(poli),osszehasonlit);
    return kir_varo_db;
}
