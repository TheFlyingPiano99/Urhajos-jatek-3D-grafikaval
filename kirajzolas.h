/**
 * @file kirajzolas.h
 * @author Simon Zoltán
 * @brief kirajzolást végző funkciók
 * @version 0.1
 * @date 2019-11-29
 *
 * @copyright Copyright (c) 2019
 *
 */

// ################################################################
#ifndef _KIRAJZOLAS_IMPORTALVA_
#define _KIRAJZOLAS_IMPORTALVA_
// ################################################################

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "3d_muveletek.h"
#include "menu.h"

#define MENU_HATTER "kepek/menu_hatter_pixeles.png"
#define ABLAK_IKON "kepek/ikon_64x64.png"

#define RGB_FEHER      (SDL_Color) {.r=255, .g=255, .b=255, .a=255}
#define RGB_SZURKE     (SDL_Color) {.r=195, .g=195, .b=195, .a=255}
#define RGB_MENU_CIM   (SDL_Color) {.r=255, .g=40,  .b=40,  .a=255}
#define RGB_MENU       (SDL_Color) {.r=175, .g=175, .b=175, .a=255}
#define RGB_MENU_AKTIV RGB_FEHER

int  kep_szelesseg_ker();
int  kep_magassag_ker();
void kep_szelesseg_beallit(int uj);
void kep_magassag_beallit(int uj);

int  hangero_allitas(bool allit);
void font_megnyitas();
void font_felszabaditas();
void intro_kirajzol(SDL_Renderer *renderer, int time);
void sugo_kirajzol(SDL_Renderer *renderer, SDL_Texture *sugo);
SDL_Texture *kepernyo_letrehoz(SDL_Renderer *renderer, SDL_Point meret);
SDL_Texture *sugo_letrehoz(SDL_Renderer *renderer, SDL_Point meret);
void elvalaszto_kirajzol(SDL_Renderer *renderer);

void menu_kiir (
    SDL_Renderer *renderer,
    jatek_allapotok *jallapot,
    fomenu_allapotok *fomenu_allapot,
    beallitasok_allapotok *beall_allapot,
    jatek_kezdes_allapotok *jatek_kezdes_allapot,
    gyorsmenu_allapotok *gym_allapot,
    kilepes_elott_allapotok *kilepes_elotti_allapot,
    SDL_Texture *img,
    SDL_Rect texr);

void grafika_kirajzolas (
    SDL_Renderer *renderer,
    SDL_Texture *kepernyo,
    int offs_x,
    int offs_y,
    int kep_x,
    int kep_y,
    kamera *kam,
    objektum *sajat,
    vek3d *csillagok,
    int *kir_varo_db,
    objektum_tomb *objektumok,
    matrix4x4 *projekcio,
    poli *kirajzolasra_varo,
    bool debug_mod,
    bool megsemmisult);

// ################################################################
#endif
// ################################################################