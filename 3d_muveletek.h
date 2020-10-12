/**
 * @file 3d_muveletek.h
 * @author Simon Zoltán
 * @brief 3D elemekkel dolgozó műveletek
 * @version 0.1
 * @date 2019-10-26
 *
 * @copyright Copyright (c) 2019
 *
 */

// ################################################################
#ifndef _3D_MUVELETEK_IMPORTALVA_
#define _3D_MUVELETEK_IMPORTALVA_
// ################################################################

#include <SDL2/SDL_mixer.h>
#include "3d_tipusok.h"

vek3d vektor_forgatas (vek3d vek, ang3d szog);

/**
 * @brief A kapott vektort osszeszorozza a kapott 4*4-es matrixal
 *
 * @param in
 * @param mx
 * @return vek3d
 */
vek3d matrix_szorzo(vek3d in, matrix4x4 mx);

/**
 * @brief
 *
 * @param pozv
 * @param celv
 * @param felv
 * @return matrix4x4
 */
matrix4x4 mutato_matrix(vek3d *pozv, vek3d *celv, vek3d *felv);

/**
 * @brief
 *
 * @param m
 * @return matrix4x4
 */
matrix4x4 specialis_matrix_inverz(matrix4x4 *m);

/**
 * @brief
 *
 * @param v1
 * @param v2
 * @return vek3d
 */
vek3d vektor_osszeadas(vek3d *v1, vek3d *v2);

/**
 * @brief
 *
 * @param v1
 * @param v2
 * @return vek3d
 */
vek3d vektor_kivonas(vek3d *v1, vek3d *v2);

/**
 * @brief
 *
 * @param v1
 * @param v2
 * @return vek3d
 */
vek3d vektor_szorzas(vek3d *v1, vek3d *v2);

/**
 * @brief
 *
 * @param iranyv1
 * @param iranyv2
 * @return vek3d
 */
vek3d normal_vektor(vek3d *iranyv1, vek3d *iranyv2);

/**
 * @brief
 *
 * @param vek
 * @param skal
 * @return vek3d
 */
vek3d vektor_skalarral_szorzas(vek3d *vek, double skal);

/**
 * @brief
 *
 * @param v1
 * @param v2
 * @return double
 */
double skalar_szorzat(vek3d *v1, vek3d *v2);

/**
 * @brief
 *
 * @param v1
 * @param v2
 * @return vek3d
 */
vek3d vektorialis_szorzat(vek3d *v1, vek3d *v2);

/**
 * @brief
 *
 * @param vek
 * @return double
 */
double vektor_hossz(vek3d *vek);

/**
 * @brief
 *
 * @param vek
 * @return vek3d
 */
vek3d vektor_normalizacio(vek3d vek);

/**
 * @brief
 *
 * @param matrix
 */
void matrix_nullazas(matrix4x4 *matrix);

/**
 * @brief
 *
 * @param matrix
 * @param aspect_ratio
 * @param fov_tangens
 * @param far
 * @param near
 */
void projekcio_matrix_generator(
    matrix4x4 *matrix,
    double *aspect_ratio,
    double *fov_tangens,
    double *far,
    double *near);

/**
 * @brief
 *
 * @param matrix
 * @param szog
 */
void z_forgatas_matrix_generator(matrix4x4 *matrix, double *szog);

/**
 * @brief
 *
 * @param matrix
 * @param szog
 */
void y_forgatas_matrix_generator(matrix4x4 *matrix, double *szog);

/**
 * @brief
 *
 * @param matrix
 * @param szog
 */
void x_forgatas_matrix_generator(matrix4x4 *matrix, double *szog);

/**
 * @brief A kapott polygonon elvegzi a kirajzolashoz szukseges
 * szamitasokat a kapott matrixokkal valo szorzas reven
 *
 * @param bemenet
 * @param pozicio
 * @param rotacio_z
 * @param rotacio_x
 * @param projekcio
 * @param kam
 * @param p_kep_szelesseg
 * @param p_kep_magassag
 * @param kimenet1
 * @param kimenet2
 * @return int
 */
int haromszog_feldolgozas(
    poli *bemenet,
    vek3d *pozicio,
    matrix4x4 *rotacio_x,
    matrix4x4 *rotacio_y,
    matrix4x4 *rotacio_z,
    matrix4x4 *projekcio,
    kamera *kam,
    int p_kep_szelesseg,
    int p_kep_magassag,
    poli *kimenet1,
    poli *kimenet2);

/**
 * @brief
 *
 * @param objektumok
 * @param kam
 * @param projekcio
 * @param kep_szelesseg
 * @param kep_magassag
 * @param offset
 * @param kirajzolasra_varo
 * @return int
 */
int mesh_feldolgozas (objektum_tomb *objektumok,
                      objektum *letiltott,
                      kamera *kam,
                      matrix4x4 *projekcio,
                      int kep_szelesseg,
                      int kep_magassag,
                      int *offset,
                      poli *kirajzolasra_varo);

/**
 * @brief
 *
 * @param ph1
 * @param ph2
 * @return int
 */
int osszehasonlit(
    const void *ph1,
    const void *ph2);

/**
 * @brief
 *
 * @param fizikak
 * @param db
 */
fizika fizika_letrehozo();

/**
 * @brief
 *
 * @param objektumok
 * @param meshek
 * @param fizikak
 * @param i
 */
void objektum_letrehozo(objektum *obj, mesh_tomb *meshek, rgba szin, obj_tipus tipus);

/**
 * @brief mozgas funkcio
 *
 * @param obj
 * @param t
 */
void fizikai_muveletek(objektum_tomb *objektumok, objektum_tomb *reszecskek, Mix_Chunk *robbanas_hang, int t);

matrix4x4 rotacio_matrix_tetszoleges_tengely(float szog_sin, float szog_cos, vek3d iranyv);

vek3d minimap_lekepezes (
        vek3d pozicio,
        matrix4x4 *projekciom,
        kamera kam,
        int p_kep_szelesseg,
        int p_kep_magassag);

vek3d hatter_lekepezes (
        vek3d pozicio,
        matrix4x4 *projekciom,
        kamera kam,
        int p_kep_szelesseg,
        int p_kep_magassag);

vek3d randomizalt_vektor(int offset, int tartomany);

// ################################################################
#endif
// ################################################################
