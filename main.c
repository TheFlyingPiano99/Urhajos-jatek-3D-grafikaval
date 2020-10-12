/**
 * @file main.c
 * @author Simon Zoltán
 * @brief A főprogram
 * @version 0.1
 * @date 2019-10-27
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
#include "dinamikus_mezok.h"
#include "kirajzolas.h"

#define KONTROLL_INIT (kontroll)  {false,false,false,false,false,false,false,false,false}
#define VIRT_KEPMERET (SDL_Point) {kep_szelesseg_ker(), kep_magassag_ker()/2}
#define SUGO_KEPMERET (SDL_Point) {kep_szelesseg_ker(), kep_magassag_ker()}


// nehany funkciot elore deklaralunk
void kamera_mozgatas (kamera *kam, kontroll kont, double delta_t);
void loves(objektum_tomb *lovedekek, vek3d poz, vek3d irany, Mix_Chunk *hangok);

bool meret_allitas_vizsgalat(bool allit) {
    static bool kell = true;
    if (allit)
        kell = !kell;
    return kell;
}

void ablakmeret_allitas (SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Point meretek[] = {
        { 1024,  768 },
        { 1920, 1080 },
        { 3840, 2160 },
        {  640,  480 }
    };

    static int i = 0;
    if (i == 4)
        i = 0;
    kep_szelesseg_beallit(meretek[i].x);
    kep_magassag_beallit(meretek[i].y);
    SDL_SetWindowSize(window, kep_szelesseg_ker(), kep_magassag_ker());
    font_megnyitas();

    SDL_Rect kepernyo_meret;
    if (SDL_GetDisplayBounds(0, &kepernyo_meret) == 0) {
        SDL_SetWindowPosition(window,
            (kepernyo_meret.w - kep_szelesseg_ker())/2,
            (kepernyo_meret.h - kep_magassag_ker())/2);
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    i++;
}


//.obj allomanybol olvas be mesht.--------
mesh mesh_beolvasas(char *fajlnev) {
    mesh beolvasott = mesh_letrehoz();
    FILE *fp = fopen(fajlnev,"rt");
    if (fp != 0) {
        vek3d_tomb vektorok = vek3d_tomb_letrehoz();
        char sor[101];
        bool beolv = true;
        while (beolv) {
            if (fgets(sor,101,fp) != 0) {
                if (sor[0] == 'v') {
                    vek3d vek;
                    sscanf(sor+1,"%lf %lf %lf", &vek.x, &vek.y, &vek.z);
                    vek3d_tomb_hozzaad(&vektorok, vek);
                }
                else if (sor[0] == 'f') {
                    int f[3];
                    sscanf(sor+1,"%d %d %d", &f[0], &f[1], &f[2]);
                    mesh_hozzaad(&beolvasott, (poli){ vektorok.elemek[f[0]-1], vektorok.elemek[f[1]-1], vektorok.elemek[f[2]-1]} );
                }
            }
            else
                beolv = false;
        }
        fclose(fp);
        felszabadit(vektorok.elemek); // felszabaditjuk a vektorokat tartalmazo mezot
    }
    else {
        printf("Nem sikerult beolvasi \"%s\" modellt.\n", fajlnev);
    }
    return beolvasott;
}

void gyorsmenu_valto (menu_irany irany, gyorsmenu_allapotok *allapot) {
    if (irany == fel) {
        switch (*allapot) {
            case vissza_gy: *allapot = kilep_gy; break;
            case beallitasok_gy: *allapot = vissza_gy; break;
            case menube_gy: *allapot = beallitasok_gy; break;
            case kilep_gy: *allapot = menube_gy; break;
        }
    }
    else if (irany == le) {
        switch (*allapot) {
            case vissza_gy: *allapot = beallitasok_gy; break;
            case beallitasok_gy: *allapot = menube_gy; break;
            case menube_gy: *allapot = kilep_gy; break;
            case kilep_gy: *allapot = vissza_gy; break;
        }
    }
}

void fomenu_valto (menu_irany irany, fomenu_allapotok *allapot) {
    if (irany == fel) {
        switch(*allapot) {
            case indit_f: *allapot = kilep_f; break;
            case beallitasok_f: *allapot = indit_f; break;
            case kilep_f: *allapot = beallitasok_f; break;
        }
    }
    else if (irany == le) {
        switch(*allapot) {
            case indit_f: *allapot = beallitasok_f; break;
            case beallitasok_f: *allapot = kilep_f; break;
            case kilep_f: *allapot = indit_f; break;
        }
    }
}

void beallitasok_valto (menu_irany irany, beallitasok_allapotok *allapot) {
    if (irany == fel) {
        switch(*allapot) {
            case hangero_b: *allapot = vissza_b; break;
            case felbontas_b: *allapot = hangero_b; break;
            case vissza_b: *allapot = felbontas_b;
        }
    }
    else if (irany == le) {
        switch (*allapot) {
            case hangero_b: *allapot = felbontas_b; break;
            case felbontas_b: *allapot = vissza_b; break;
            case vissza_b: *allapot = hangero_b;
        }
    }
}

void kilepes_elott_valto (menu_irany irany, kilepes_elott_allapotok *allapot) {
    if (irany == fel || irany == le) {
        switch(*allapot) {
            case kilep_ke: *allapot = vissza_ke; break;
            case vissza_ke: *allapot = kilep_ke;
        }
    }
}

void jatek_kezdes_valto(menu_irany irany, jatek_kezdes_allapotok *allapot) {
    if (irany == fel) {
        switch(*allapot) {
            case solo_jk: *allapot = vissza_jk; break;
            case duo_jk: *allapot = solo_jk; break;
            case vissza_jk: *allapot = duo_jk;
        }
    }
    else if (irany == le) {
        switch(*allapot) {
            case solo_jk: *allapot = duo_jk; break;
            case duo_jk: *allapot = vissza_jk; break;
            case vissza_jk: *allapot = solo_jk;
        }
    }
}

int elfogadas (
    jatek_allapotok         *jallapot,
    fomenu_allapotok        *fomenu_allapot,
    jatek_kezdes_allapotok  *jatek_kezdes_allapot,
    gyorsmenu_allapotok     *gym_allapot,
    beallitasok_allapotok   *beall_allapot,
    kilepes_elott_allapotok *kilepes_elott_allapot,
    int                     *hany_jatekos,
    SDL_Window              *window) {

    switch(*jallapot) {
        case gyorsmenu:
            switch(*gym_allapot) {
                case vissza_gy: *jallapot = jatek; break;
                case beallitasok_gy: *jallapot = beallitasok; break;
                case menube_gy: *jallapot = fomenu; break;
                case kilep_gy: *jallapot = kilepes_elott_gymenubol;
            }
            *gym_allapot = vissza_gy;
            break;
        case fomenu:
            switch(*fomenu_allapot) {
                case indit_f: *jallapot = jatek_kezdes;  break;
                case beallitasok_f: *jallapot = beallitasok_menubol; break;
                case kilep_f: *jallapot = kilepes_elott_menubol;
            }
            *fomenu_allapot = indit_f;
            break;
        case beallitasok:
            switch(*beall_allapot) {
                case hangero_b: hangero_allitas(true); break;
                case felbontas_b: meret_allitas_vizsgalat(true); break;
                case vissza_b: *jallapot = gyorsmenu; *beall_allapot = hangero_b; break;
            }
            break;
        case beallitasok_menubol:
            switch(*beall_allapot) {
                case hangero_b: hangero_allitas(true); break;
                case felbontas_b: meret_allitas_vizsgalat(true); break;
                case vissza_b: *jallapot = fomenu; *beall_allapot = hangero_b; break;
            }
            break;
        case kilepes_elott_gymenubol:
            switch (*kilepes_elott_allapot) {
                case kilep_ke: return 1;
                case vissza_ke: *jallapot = gyorsmenu;
            }
            *kilepes_elott_allapot = kilep_ke;
            break;
        case kilepes_elott_menubol:
            switch (*kilepes_elott_allapot) {
                case kilep_ke: return 1;
                case vissza_ke: *jallapot = fomenu;
            }
            *kilepes_elott_allapot = kilep_ke;
            break;
        case jatek_kezdes:
            switch (*jatek_kezdes_allapot) {
                case solo_jk: *jallapot = jatek;
                    *hany_jatekos = 1;
                    break;
                case duo_jk: *jallapot = jatek;
                    *hany_jatekos = 2;
                    break;
                case vissza_jk: *jallapot = fomenu; break;
            }
            *jatek_kezdes_allapot = solo_jk;
    }

    return 0;
}

int billentyu_reakcio (
    kontroll              *kont,
    int                   *hany_jatekos,
    bool                  *megsemmisult,
    kamera                *kam,
    objektum_tomb         *objektumok,
    objektum_tomb         *lovedekek,
    jatek_allapotok       *jatek_allapot,
    fomenu_allapotok      *fomenu_allapot,
    jatek_kezdes_allapotok *jatek_kezdes_allapot,
    gyorsmenu_allapotok   *gymenu_allapot,
    beallitasok_allapotok *beall_allapot,
    kilepes_elott_allapotok *kilepes_elott_allapot,
    double                  delta_t,
    SDL_Window             *window,
    Mix_Chunk              *hangok) {

    static int loves_idokoz = 0;

    if (kont->w) {
        if (*jatek_allapot == gyorsmenu) {
            kont->w = false;
            gyorsmenu_valto(fel, gymenu_allapot);
        }
        else if (*jatek_allapot == fomenu) {
            kont->w = false;
            fomenu_valto(fel, fomenu_allapot);
        }
        else if (*jatek_allapot == beallitasok || *jatek_allapot == beallitasok_menubol) {
            kont->w = false;
            beallitasok_valto(fel, beall_allapot);
        }
        else if (*jatek_allapot == kilepes_elott_menubol ||*jatek_allapot == kilepes_elott_gymenubol) {
            kont->w = false;
            kilepes_elott_valto(fel, kilepes_elott_allapot);
        }
        else if (*jatek_allapot == jatek_kezdes) {
            kont->w = false;
            jatek_kezdes_valto(fel, jatek_kezdes_allapot);
        }
    }

    if (kont->s) {
        if (*jatek_allapot == gyorsmenu) {
            kont->s = false;
            gyorsmenu_valto(le, gymenu_allapot);
        }
        else if (*jatek_allapot == fomenu) {
            kont->s = false;
            fomenu_valto(le, fomenu_allapot);
        }
        else if (*jatek_allapot == beallitasok || *jatek_allapot == beallitasok_menubol) {
            kont->s = false;
            beallitasok_valto(le, beall_allapot);
        }
        else if (*jatek_allapot == kilepes_elott_menubol ||*jatek_allapot == kilepes_elott_gymenubol) {
            kont->s = false;
            kilepes_elott_valto(le, kilepes_elott_allapot);
        }
        else if (*jatek_allapot == jatek_kezdes) {
            kont->s = false;
            jatek_kezdes_valto(le, jatek_kezdes_allapot);
        }
    }

    if (kont->esc) {
        kont->esc = false;
        if (*jatek_allapot == jatek) {
            *gymenu_allapot = vissza_gy;
            *jatek_allapot = gyorsmenu;
        }
    }

    if (kont->enter) {
        kont->enter = false;
        return elfogadas(
                jatek_allapot,
                fomenu_allapot,
                jatek_kezdes_allapot,
                gymenu_allapot,
                beall_allapot,
                kilepes_elott_allapot,
                hany_jatekos,
                window);
    }
    if (*jatek_allapot == jatek && !*megsemmisult) {
        if (kont->space && loves_idokoz == 0) {
            loves(lovedekek, kam->pozv, kam->iranyv, hangok);
            loves_idokoz = 1;
        }
        if (loves_idokoz != 0) {
            loves_idokoz += delta_t;
        }
        if (loves_idokoz > 450) {
            loves_idokoz = 0;
        }

        kamera_mozgatas(kam, *kont, delta_t);
    }

    return 0;
}

int billentyu_lekerdezes (kontroll *kont1, kontroll *kont2, kontroll *mar_lenyomott1, kontroll *mar_lenyomott2, bool *debug_mod, bool *sugo_mod, int hany_jatekos) {
    SDL_Event event;
    bool egy_jatekos = (hany_jatekos != 2);
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    //a nyilak egy- es ketjatekos modban is mukodnek!
                    case SDLK_UP:       if (egy_jatekos) { mar_lenyomott1->w = false; kont1->w = false; }
                                        else             { mar_lenyomott2->w = false; kont2->w = false; }
                                        break;
                    case SDLK_DOWN:     if (egy_jatekos) { mar_lenyomott1->s = false; kont1->s = false; }
                                        else             { mar_lenyomott2->s = false; kont2->s = false; }
                                        break;
                    case SDLK_LEFT:     if (egy_jatekos) { mar_lenyomott1->a = false; kont1->a = false; }
                                        else             { mar_lenyomott2->a = false; kont2->a = false; }
                                        break;
                    case SDLK_RIGHT:    if (egy_jatekos) { mar_lenyomott1->d = false; kont1->d = false; }
                                        else             { mar_lenyomott2->d = false; kont2->d = false; }
                                        break;

                    case SDLK_w:        mar_lenyomott1->w = false;  kont1->w = false; break;
                    case SDLK_a:        mar_lenyomott1->a = false;  kont1->a = false; break;
                    case SDLK_s:        mar_lenyomott1->s = false;  kont1->s = false; break;
                    case SDLK_d:        mar_lenyomott1->d = false;  kont1->d = false; break;

                    case SDLK_z:        mar_lenyomott1->z = false; kont1->z = false; break;
                    case SDLK_x:        mar_lenyomott1->x = false; kont1->x = false; break;

                    case SDLK_COMMA:    mar_lenyomott2->z = false; kont2->z = false; break;
                    case SDLK_PERIOD:   mar_lenyomott2->x = false; kont2->x = false; break;

                    case SDLK_SPACE:    mar_lenyomott1->space = false; kont1->space = false; break;
                    case SDLK_LSHIFT:   mar_lenyomott1->lshift = false; kont1->lshift = false; break;

                    case SDLK_KP_0:     mar_lenyomott2->lshift = false; kont2->lshift = false; break;

                    case SDLK_ESCAPE:   mar_lenyomott1->esc   = false;  kont1->esc   = false; break;
                    case SDLK_RETURN:   mar_lenyomott1->enter = false;  kont1->enter = false; break;

                    case SDLK_KP_ENTER: mar_lenyomott2->space = false;  kont2->space = false; break;
                    default: break;
                }
                break;

            case SDL_KEYDOWN:

                if (*sugo_mod && SDLK_F1 != event.key.keysym.sym) {
                    *sugo_mod = false;
                }

                switch (event.key.keysym.sym) {
                    //a nyilak egy- es ketjatekos modban is mukodnek!
                    case SDLK_UP:   if (egy_jatekos) { if (mar_lenyomott1->w == false) { kont1->w = true; mar_lenyomott1->w = true; } }
                                    else             { if (mar_lenyomott2->w == false) { kont2->w = true; mar_lenyomott2->w = true; } }
                                    break;
                    case SDLK_DOWN: if (egy_jatekos) { if (mar_lenyomott1->s == false) { kont1->s = true; mar_lenyomott1->s = true; } }
                                    else             { if (mar_lenyomott2->s == false) { kont2->s = true; mar_lenyomott2->s = true; } }
                                    break;
                    case SDLK_LEFT: if (egy_jatekos) { if (mar_lenyomott1->a == false) { kont1->a = true; mar_lenyomott1->a = true; } }
                                    else             { if (mar_lenyomott2->a == false) { kont2->a = true; mar_lenyomott2->a = true; } }
                                    break;
                    case SDLK_RIGHT:if (egy_jatekos) { if (mar_lenyomott1->d == false) { kont1->d = true; mar_lenyomott1->d = true; } }
                                    else             { if (mar_lenyomott2->d == false) { kont2->d = true; mar_lenyomott2->d = true; } }
                                    break;

                    case SDLK_w:    if (mar_lenyomott1->w == false) kont1->w = true; mar_lenyomott1->w = true; break;
                    case SDLK_a:    if (mar_lenyomott1->a == false) kont1->a = true; mar_lenyomott1->a = true; break;
                    case SDLK_s:    if (mar_lenyomott1->s == false) kont1->s = true; mar_lenyomott1->s = true; break;
                    case SDLK_d:    if (mar_lenyomott1->d == false) kont1->d = true; mar_lenyomott1->d = true; break;

                    case SDLK_z:    if (mar_lenyomott1->z == false) kont1->z = true; mar_lenyomott1->z = true; break;
                    case SDLK_x:    if (mar_lenyomott1->x == false) kont1->x = true; mar_lenyomott1->x = true; break;

                    case SDLK_COMMA:  if (mar_lenyomott2->z == false) kont2->z = true; mar_lenyomott2->z = true; break;
                    case SDLK_PERIOD: if (mar_lenyomott2->x == false) kont2->x = true; mar_lenyomott2->x = true; break;

                    case SDLK_SPACE:  if (mar_lenyomott1->space == false) kont1->space = true; mar_lenyomott1->space = true; break;
                    case SDLK_LSHIFT: if (mar_lenyomott1->lshift== false) kont1->lshift = true; mar_lenyomott1->lshift= true; break;

                    case SDLK_KP_0:   if (mar_lenyomott2->lshift== false) kont2->lshift = true; mar_lenyomott2->lshift= true; break;

                    case SDLK_y:      *debug_mod = !*debug_mod; break;
                    case SDLK_F1:     *sugo_mod = !*sugo_mod; break;
                    case SDLK_ESCAPE: if (mar_lenyomott1->esc   == false) kont1->esc   = true; mar_lenyomott1->esc   = true; break;
                    case SDLK_RETURN: if (mar_lenyomott1->enter == false) kont1->enter = true; mar_lenyomott1->enter = true; break;

                    case SDLK_KP_ENTER: if (mar_lenyomott2->space == false) kont2->space = true; mar_lenyomott2->space = true; break;

                    default: break;
                }
                break;

            case SDL_QUIT:
                return 1;
        }
    }
    return 0;
}

void atmenet_kirajzol (SDL_Renderer *renderer, int time) {
    boxRGBA(renderer, 0, 0, kep_szelesseg_ker(), kep_magassag_ker(), 0, 0, 0, 255);
}

void fps_kiir(SDL_Renderer *renderer, unsigned int *rajzolas_szam, unsigned int *elozo_frissites) {
    *rajzolas_szam += 1;
    static char kiiras[20] = "";
    unsigned int eltelt_ido = SDL_GetTicks() - (*elozo_frissites);
    if (eltelt_ido > 250) {
        double eltelt_masodperc = eltelt_ido / 1000.0;
        double fps = *rajzolas_szam / eltelt_masodperc;
        sprintf(kiiras, "%3d fps", (int)fps);
        *elozo_frissites = SDL_GetTicks();
        *rajzolas_szam = 0;
    }

    SDL_Color szin = RGB_SZURKE;
    stringRGBA(renderer, 20, 20, kiiras, szin.r, szin.g, szin.b, szin.a);
}

void szog_sebesseg_modositas(ang3d *szog_sebesseg, double delta_t) {
    // sebesseg korlatozas a kulonbozo tengelyek koruli forgasnal
    const double max_v[3] = { 0.002, 0.001, 0.002 };
    double xyz[3] = {
        szog_sebesseg->x_ang,
        szog_sebesseg->y_ang,
        szog_sebesseg->z_ang
    };

    for (int i= 0; i < 3; i++) {
        if (xyz[i] > max_v[i]) {
            xyz[i] = max_v[i];
        }
        else if (xyz[i] < -max_v[i]) {
            xyz[i] = -max_v[i];
        }
        //-----------------------------------
        if (xyz[i] > 0) {
            xyz[i] -= 0.000001 * delta_t;
        }
        else if (xyz[i] < 0) {
            xyz[i] += 0.000001 * delta_t;
        }
    }
    szog_sebesseg->x_ang = xyz[0];
    szog_sebesseg->y_ang = xyz[1];
    szog_sebesseg->z_ang = xyz[2];
}

void sebesseg_modositas (vek3d *sebesseg, double delta_t) {
    double lassulas = 0.000008;
    double max = 0.05;
    double xyz[3] = {
        sebesseg->x,
        sebesseg->y,
        sebesseg->z
    };
    for (int i = 0; i < 3; i++) {
        if (xyz[i] > 0) {
            xyz[i] -= lassulas * delta_t;
        }
        else if (xyz[i] < 0) {
            xyz[i] += lassulas * delta_t;
        }
        if (xyz[i] > max) {
            xyz[i] = max;
        }
        else if (xyz[i] < -max) {
            xyz[i] = -max;
        }
    }
    sebesseg->x = xyz[0];
    sebesseg->y = xyz[1];
    sebesseg->z = xyz[2];
}

void kamera_mozgatas (kamera *kam, kontroll kont, double delta_t) {
    double szog_sinz_poz = sin(kam->szog_sebesseg.z_ang * delta_t);
    double szog_cosz_poz = cos(kam->szog_sebesseg.z_ang * delta_t);
    double szog_siny_poz = sin(kam->szog_sebesseg.y_ang * delta_t);
    double szog_cosy_poz = cos(kam->szog_sebesseg.y_ang * delta_t);
    double szog_sinx_poz = sin(kam->szog_sebesseg.x_ang * delta_t);
    double szog_cosx_poz = cos(kam->szog_sebesseg.x_ang * delta_t);

    //Matrix generalas:
    matrix4x4 mz = rotacio_matrix_tetszoleges_tengely(szog_sinz_poz, szog_cosz_poz, kam->iranyv);
    matrix4x4 my = rotacio_matrix_tetszoleges_tengely(szog_siny_poz, szog_cosy_poz, kam->felv);
    matrix4x4 mx = rotacio_matrix_tetszoleges_tengely(szog_sinx_poz, szog_cosx_poz, kam->oldalra);

    double szog_gyorsulas = 0.000002;
    double manover = vektor_hossz(&kam->sebesseg) + 1;
    //Szogek:-----------------------------------------------------------------
    //Y tengely:
    if (kont.z) {
        kam->szog_sebesseg.y_ang += szog_gyorsulas * delta_t * manover;
    }
    else if (kont.x) {
        kam->szog_sebesseg.y_ang -= szog_gyorsulas * delta_t * manover;
    }
    //X tengely:
    if (kont.s) {
        kam->szog_sebesseg.x_ang -= szog_gyorsulas * delta_t * manover;
    }
    if (kont.w) {
        kam->szog_sebesseg.x_ang += szog_gyorsulas * delta_t * manover;
    }
    //Z tengely:
    if (kont.d) {
        kam->szog_sebesseg.z_ang -= szog_gyorsulas * delta_t * manover;
    }
    if (kont.a) {
        kam->szog_sebesseg.z_ang += szog_gyorsulas * delta_t * manover;
    }

    szog_sebesseg_modositas(&kam->szog_sebesseg, delta_t);

    kam->iranyv = matrix_szorzo(kam->iranyv, mz);
    kam->felv = matrix_szorzo(kam->felv, mz);
    kam->oldalra = matrix_szorzo(kam->oldalra, mz);

    kam->iranyv = matrix_szorzo(kam->iranyv, my);
    kam->felv = matrix_szorzo(kam->felv, my);
    kam->oldalra = matrix_szorzo(kam->oldalra, my);

    kam->iranyv = matrix_szorzo(kam->iranyv, mx);
    kam->felv = matrix_szorzo(kam->felv, mx);
    kam->oldalra = matrix_szorzo(kam->oldalra, mx);

    //--------------------------------------------------
    //Elore mozgas
    if (kont.lshift){
        vek3d sebesseg_valtozas = vektor_skalarral_szorzas(&kam->iranyv, 0.00002*delta_t);
        kam->sebesseg = vektor_osszeadas(&kam->sebesseg, &sebesseg_valtozas);
    }
    sebesseg_modositas(&kam->sebesseg, delta_t);
    vek3d elmv = vektor_skalarral_szorzas(&kam->sebesseg, delta_t);
    kam->pozv = vektor_osszeadas(&kam->pozv, &elmv);
}

int inicializalas (
    objektum_tomb *objektumok,
    objektum_tomb *lovedekek,
    objektum_tomb *reszecskek,
    mesh_tomb *meshek,
    kamera *kam1,
    kamera *kam2,
    kontroll *kontroll1,
    kontroll *kontroll2,
    bool *megsemmisult1,
    bool *megsemmisult2,
    int alap_obj_szama,
    int lovedekek_szama,
    int meteorok_szama,
    int reszecskek_szama) {

    *megsemmisult1 = false;
    *megsemmisult2 = false;
    int j = alap_obj_szama;

    //Alap objektumok:
    objektum_letrehozo(&objektumok->objp[0], meshek, (rgba){255,50,50,255}, urhajo);
    objektum_letrehozo(&objektumok->objp[1], meshek, (rgba){255,50,50,255}, urhajo);
    objektum_letrehozo(&objektumok->objp[2], meshek, (rgba){50,100,150,255}, bolygo);

    //Lovedekek:
    lovedekek->objp = &objektumok->objp[j];
    lovedekek->meret = lovedekek_szama;
    for (int i = j; i< j+lovedekek_szama; i++) {
        objektum_letrehozo(&objektumok->objp[i], meshek, (rgba){255,255,255,255}, lovedek);
    }
    j += lovedekek_szama;

    //Meteorok:
    for (int i = j; i < j + meteorok_szama; i++) {
        objektum_letrehozo(&objektumok->objp[i], meshek, (rgba){120,120,120,255}, meteor);
        objektumok->objp[i].pozicio = randomizalt_vektor(10,60);
        objektumok->objp[i].fizika.szogsebesseg = (ang3d){(rand()%10)*0.00002,(rand()%10)*0.00002,(rand()%10)*0.00002};
        objektumok->objp[i].pozicio.y = 0;
    }
    j += meteorok_szama;

    //Reszecskek:
    reszecskek->objp = &objektumok->objp[j];
    reszecskek->meret = reszecskek_szama;
    for (int i = j; i < j + reszecskek_szama; i++) {
        objektum_letrehozo(&objektumok->objp[i], meshek, (rgba){240,240,100,255}, reszecske);
    }

    //Elore megadott poziciok es sebessegek:
    objektumok->objp[2].pozicio = (vek3d){0,0,50};
    objektumok->objp[2].fizika.szogsebesseg.y_ang = 0.0002;

    //Kamerak------------------------------
    kam1->pozv    = (vek3d){0.0, 0.0, 0.0};
    kam1->iranyv  = (vek3d){0.0, 0.0, 1.0};
    kam1->felv    = (vek3d){0.0, 1.0, 0.0};
    kam1->oldalra = (vek3d){1.0, 0.0, 0.0};
    kam1->szog_sebesseg = (ang3d){0,0,0};
    kam1->sebesseg = (vek3d){0,0,0};

    kam2->pozv    = (vek3d){0.0, 0.0, 100.0};
    kam2->iranyv  = (vek3d){0.0, 0.0, -1.0};
    kam2->felv    = (vek3d){0.0, 1.0, 0.0};
    kam2->oldalra = (vek3d){-1.0, 0.0, 0.0};
    kam2->szog_sebesseg = (ang3d){0,0,0};
    kam2->sebesseg = (vek3d){0,0,0};
    //------------------------------------

    return 0;
}

void loves(objektum_tomb *lovedekek, vek3d pozv, vek3d iranyv, Mix_Chunk *hangok) {
    static int db = 0;
    vek3d offsv = vektor_skalarral_szorzas(&iranyv, 3);
    pozv = vektor_osszeadas(&pozv, &offsv);
    lovedekek->objp[db].pozicio = pozv;
    lovedekek->objp[db].fizika.sebesseg = vektor_skalarral_szorzas(&iranyv, 0.05);
    lovedekek->objp[db].lathato = true;
    lovedekek->objp[db].elet = 1;
    Mix_PlayChannel(-1, hangok, 0);
    if (++db == lovedekek->meret) {
        db = 0;
    }
}

int main(int argc, char *argv[]) {
    /* SDL inicinicializálása és ablak megnyitása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(
        "3D SpaceGame",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kep_szelesseg_ker(),
        kep_magassag_ker(), 0);

    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Music *hatterzene    = Mix_LoadMUS("hangok/jatek_zeneje.mp3");
    Mix_Chunk *loves_hang    = Mix_LoadWAV("hangok/loves.wav");
    Mix_Chunk *robbanas_hang = Mix_LoadWAV("hangok/robbanas.wav");
    SDL_RenderClear(renderer);

    font_megnyitas();

    //Meshek allomanybol beolvasasa-----------------------------------
    mesh_tomb meshek = mesh_tomb_letrehoz();

    char* modellek[] = {
        "modellek/urhajo.txt",
        "modellek/bolygo.txt",
        "modellek/meteor.txt",
        "modellek/lovedek.txt",
        "modellek/reszecske.txt"
    };

    for (int i = 0; i < 5; i++)
    {
        mesh_tomb_hozzaad(&meshek, mesh_beolvasas(modellek[i]));
    }

    //Inic
    objektum_tomb objektumok;
    kamera kam1, kam2;

    //Objektumok szama:
    //----------------------------
    int alap_obj_szama   = 3;
    int lovedekek_szama  = 30;
    int meteorok_szama   = 30;
    int reszecskek_szama = 30;
    //----------------------------
    objektumok.meret = alap_obj_szama + lovedekek_szama + meteorok_szama + reszecskek_szama;
    objektumok.objp = (objektum*)malloc(objektumok.meret * sizeof(objektum));

    //Matrixok -------------------
    matrix4x4 projekcio, z_forgatas, y_forgatas, x_forgatas;

    //----------------------------
    bool quit      = false;
    bool debug_mod = false;
    bool sugo_mod  = false;
    kontroll kontroll1      = KONTROLL_INIT;
    kontroll kontroll2      = KONTROLL_INIT;
    kontroll mar_lenyomott1 = KONTROLL_INIT;
    kontroll mar_lenyomott2 = KONTROLL_INIT;

    int szam = 0;
    for (int i =0; i < meshek.meret; i++) {
        szam += meshek.elemek[i].meret;
    }
    szam *= 10;
    poli kirajzolasra_varo[szam];
    //Ido egysegek------------------------------------------------
    int elozo_t = 0;
    int jelenlegi_t = 0;
    int delta_t = 0;
    int offset;
    int kir_varo_db;
    unsigned int rajzolas_szam = 0;
    unsigned int elozo_fps_frissites = SDL_GetTicks();

    int intro_t = 0;
    int atmenet_t = 0;

    jatek_allapotok         jatek_allapot = bevezeto;
    jatek_allapotok         elozo_jatek_allapot = bevezeto;
    fomenu_allapotok        fomenu_allapot = indit_f;
    jatek_kezdes_allapotok  jatek_kezdes_allapot = solo_jk;
    gyorsmenu_allapotok     gymenu_allapot = vissza_gy;
    beallitasok_allapotok   beall_allapot = hangero_b;
    kilepes_elott_allapotok kilepes_elott_allapot = kilep_ke;

    SDL_Texture *menu_hatterkep = IMG_LoadTexture(renderer, MENU_HATTER);
    SDL_Surface *ikon = IMG_Load(ABLAK_IKON);
    SDL_SetWindowIcon(window, ikon);
    SDL_FreeSurface(ikon);

	int w, h;
	SDL_QueryTexture(menu_hatterkep, NULL, NULL, &w, &h);
    SDL_Rect texr = {.x = 0, .y = 0, .w = w*2, .h = h*2};
    bool legyen_atmenet = false;
    bool inicializalva = false;
    int hany_jatekos = 1;

    srand(time(0));
    vek3d csillagok[2000];
    for (int i = 0; i<2000;i++) {
        csillagok[i] = randomizalt_vektor(10,1000);
    }

    //Particle effektusok objektumaira mutato pointereket tarolo tombok:
    objektum_tomb lovedekek;
    objektum_tomb reszecskek;

    //Lekepezes inicializalas:
    lekepezes_valtozok lekep;
    lekep.near = 0.1;
    lekep.far = 1000.0;
    lekep.fov = 90.0;
    lekep.aspect_ratio = ((double)kep_magassag_ker()) / (double)kep_szelesseg_ker();
    lekep.fov_tangens = 1 / tan(lekep.fov * 0.5 * pi /180.0);

    bool megsemmisult1;
    bool megsemmisult2;

    SDL_Texture *virtualis_kepernyo = kepernyo_letrehoz(renderer, VIRT_KEPMERET);
    SDL_Texture *sugo               = sugo_letrehoz(renderer, SUGO_KEPMERET);

    while (!quit) {
        elozo_t = jelenlegi_t;
        jelenlegi_t = SDL_GetTicks();
        delta_t = jelenlegi_t - elozo_t;
        if (delta_t < 10) {
            SDL_Delay(10);
        }
        if (!inicializalva) {
            if (objektumok.objp != NULL) {
                inicializalas(
                    &objektumok,
                    &lovedekek,
                    &reszecskek,
                    &meshek,
                    &kam1,
                    &kam2,
                    &kontroll1,
                    &kontroll2,
                    &megsemmisult1,
                    &megsemmisult2,
                    alap_obj_szama,
                    lovedekek_szama,
                    meteorok_szama,
                    reszecskek_szama);
            }

            inicializalva = true;
        }
        if (jatek_allapot == fomenu) {
            inicializalva = false;
        }

        if(meret_allitas_vizsgalat(false)) {
            ablakmeret_allitas(window, renderer);
            meret_allitas_vizsgalat(true);

            // megszuntetjuk es ujra letrehozzuk a kepernyoket
            if (virtualis_kepernyo) { SDL_DestroyTexture(virtualis_kepernyo); }
            virtualis_kepernyo = kepernyo_letrehoz(renderer, VIRT_KEPMERET);

            if (sugo) { SDL_DestroyTexture(sugo); }
            sugo = sugo_letrehoz(renderer, SUGO_KEPMERET);
        }

        if (!legyen_atmenet) {
            //Input regisztralas:
            if (1 == billentyu_lekerdezes(
                &kontroll1, &kontroll2,
                &mar_lenyomott1, &mar_lenyomott2,
                &debug_mod, &sugo_mod, hany_jatekos)) { quit = true; }

            //Input reagalas:
            if (1 == billentyu_reakcio(
                &kontroll1,
                &hany_jatekos,
                &megsemmisult1,
                &kam1,
                &objektumok,
                &lovedekek,
                &jatek_allapot,
                &fomenu_allapot,
                &jatek_kezdes_allapot,
                &gymenu_allapot,
                &beall_allapot,
                &kilepes_elott_allapot,
                delta_t,
                window,
                loves_hang)) { quit = true; }
            if (hany_jatekos == 2) {
                if (1 == billentyu_reakcio(
                    &kontroll2,
                    &hany_jatekos,
                    &megsemmisult2,
                    &kam2,
                    &objektumok,
                    &lovedekek,
                    &jatek_allapot,
                    &fomenu_allapot,
                    &jatek_kezdes_allapot,
                    &gymenu_allapot,
                    &beall_allapot,
                    &kilepes_elott_allapot,
                    delta_t,
                    window,
                    loves_hang)) { quit = true; }
            }

            //------------------------------------------------------------------------------
            if (jatek_allapot == jatek) {
                //Urhajok kamerahoz igazitasa
                //1. jatekos:
                objektumok.objp[0].pozicio = kam1.pozv;
                //2. jatekos:
                if (hany_jatekos == 2) {
                    objektumok.objp[1].lathato = true;
                    objektumok.objp[1].pozicio = kam2.pozv;
                }
                else {
                    objektumok.objp[1].lathato = false;
                }

                fizikai_muveletek(&objektumok, &reszecskek, robbanas_hang, delta_t);
                if (!megsemmisult1 && !objektumok.objp[0].lathato) {
                    megsemmisult1 = true;
                }
                if (!megsemmisult2 && !objektumok.objp[1].lathato) {
                    megsemmisult2 = true;
                }
            }
        }
        if (jatek_allapot == jatek || jatek_allapot == gyorsmenu) {
            if (hany_jatekos == 1) {
                lekep.aspect_ratio = ((double)kep_magassag_ker()) / (double)kep_szelesseg_ker();
                projekcio_matrix_generator(&projekcio,&lekep.aspect_ratio,&lekep.fov_tangens,&lekep.far, &lekep.near);

                /* egyjatekos mod - nem kell virtualis kepernyo! */
                grafika_kirajzolas(renderer, NULL,
                    0, 0, kep_szelesseg_ker(), kep_magassag_ker(),
                    &kam1, &objektumok.objp[0], csillagok, &kir_varo_db, &objektumok, &projekcio,
                    kirajzolasra_varo, debug_mod, megsemmisult1);

            }
            else if (hany_jatekos == 2) {

                lekep.aspect_ratio = ((double)kep_magassag_ker()/2) / (double)kep_szelesseg_ker();
                projekcio_matrix_generator(&projekcio,&lekep.aspect_ratio,&lekep.fov_tangens,&lekep.far, &lekep.near);

                grafika_kirajzolas(renderer, virtualis_kepernyo,
                    0, 0, kep_szelesseg_ker(), kep_magassag_ker()/2,
                    &kam1, &objektumok.objp[0], csillagok, &kir_varo_db, &objektumok, &projekcio,
                    kirajzolasra_varo, debug_mod, megsemmisult1);

                grafika_kirajzolas(renderer, virtualis_kepernyo,
                    0, kep_magassag_ker()/2, kep_szelesseg_ker(), kep_magassag_ker()/2,
                    &kam2, &objektumok.objp[1], csillagok, &kir_varo_db, &objektumok, &projekcio,
                    kirajzolasra_varo, debug_mod, megsemmisult2);

                elvalaszto_kirajzol(renderer);

            }
        }
        if (jatek_allapot == bevezeto) {
            if (intro_t >= 3000) {
                jatek_allapot = fomenu;
            }
            else
                intro_kirajzol(renderer, intro_t);
            intro_t += delta_t;
        }

        if (jatek_allapot == fomenu && elozo_jatek_allapot == bevezeto) {
            Mix_PlayMusic(hatterzene, -1);
        }

        menu_kiir (
            renderer,
            &jatek_allapot,
            &fomenu_allapot,
            &beall_allapot,
            &jatek_kezdes_allapot,
            &gymenu_allapot,
            &kilepes_elott_allapot,
            menu_hatterkep,
            texr);

        if (jatek_allapot != elozo_jatek_allapot
           && jatek_allapot != gyorsmenu
           && elozo_jatek_allapot != gyorsmenu) {
            legyen_atmenet = true;
        }

        if (legyen_atmenet && atmenet_t < 250) {
            atmenet_kirajzol(renderer,atmenet_t);
            atmenet_t += delta_t;
        }
        else {
            legyen_atmenet = false;
            atmenet_t = 0;
        }

        if (sugo_mod) {
            sugo_kirajzol(renderer, sugo);
        }

        if (debug_mod) {
            fps_kiir(renderer, &rajzolas_szam, &elozo_fps_frissites);
        }

        SDL_RenderPresent(renderer);

        elozo_jatek_allapot = jatek_allapot;
    }

    free(objektumok.objp);
    for (int i = 0; i < meshek.meret; i++) {
        free(meshek.elemek[i].elemek);
    }
    free(meshek.elemek);

    font_felszabaditas();
    if (menu_hatterkep)     { SDL_DestroyTexture(menu_hatterkep);     }
    if (virtualis_kepernyo) { SDL_DestroyTexture(virtualis_kepernyo); }
    if (sugo)               { SDL_DestroyTexture(sugo);               }

    Mix_FreeMusic(hatterzene);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
