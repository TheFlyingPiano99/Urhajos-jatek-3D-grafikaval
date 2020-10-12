/**
 * @file kirajzolas.c
 * @author Simon Zoltán
 * @brief Kirajzolast vegzo funkciok
 * @version 0.1
 * @date 2019-11-30
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "kirajzolas.h"
#include "debugmalloc.h"

typedef struct _sor_{
    SDL_Color szin;
    char oszlopok[3][100];
} _sor_;

static _sor_ sugo_sorok[] = {
    { RGB_MENU_CIM , {"", "Billentyűzet kiosztás", "" } },
    { RGB_SZURKE   , {"", "", "" } },
    { RGB_FEHER    , {"Menüben:", "", "" } },
    { RGB_SZURKE   , {"  \"w\" vagy \"fel nyíl\": felfelé navigálás", "", "" } },
    { RGB_SZURKE   , {"  \"s\" vagy \"le nyíl\": lefelé navigálás",   "", "" } },
    { RGB_SZURKE   , {"  \"Enter\": menüpont kiválasztása",           "", "" } },
    { RGB_FEHER    , {"Űrhajó irányítása:",                           "", "" } },
    { RGB_FEHER    , {"Első űrhajó:",            "Második űrhajó:",         ""              } },
    { RGB_SZURKE   , {"  \"w\"",                 "\"fel nyíl\"",            "előre dőlés"   } },
    { RGB_SZURKE   , {"  \"s\"",                 "\"le nyíl\"",             "hátra dőlés"   } },
    { RGB_SZURKE   , {"  \"a\"",                 "\"balra nyíl\"",          "balra csavar"  } },
    { RGB_SZURKE   , {"  \"d\"",                 "\"jobbra nyíl\"",         "jobbra csavar" } },
    { RGB_SZURKE   , {"  \"z\" (angol bill.)",   "\",\" (vessző)",          "balra kanyar"  } },
    { RGB_SZURKE   , {"  \"x\"",                 "\".\" (pont)",            "jobbra kanyar" } },
    { RGB_SZURKE   , {"  \"Space\"",             "\"Enter\" (num. bill.)",  "lövés"         } },
    { RGB_SZURKE   , {"  \"bal Shift\"",         "\"0\" (num. bill.)",      "előre haladás" } },
    { RGB_FEHER    , {"Egyéb billentyűzetkiosztás:", "", "" } },
    { RGB_SZURKE   , {"  \"y\" (angol bill.): körvonalak és képfrissítési szám megjelenítése", "", "" } },
    { RGB_SZURKE   , {"  \"F1\": súgó megjelenítése", "", "" } },
    { RGB_SZURKE   , {"  \"Esc\": játékból gyorsmenübe váltás", "", "" } }
};


static int kep_szelesseg = 1024;
static int kep_magassag  = 768;
static TTF_Font *fontNormalMeret = NULL;
static TTF_Font *fontOriasMeret  = NULL;

int kep_szelesseg_ker() {
    return kep_szelesseg;
}

int kep_magassag_ker() {
    return kep_magassag;
}

void kep_szelesseg_beallit(int uj) {
    kep_szelesseg = uj;
}

void kep_magassag_beallit(int uj) {
    kep_magassag = uj;
}

void font_megnyitas() {
    TTF_Init();
    fontNormalMeret = TTF_OpenFont("fontok/manrope-medium.ttf", kep_magassag/25);
    fontOriasMeret  = TTF_OpenFont("fontok/manrope-medium.ttf", kep_magassag/12);
}

void font_felszabaditas() {
    TTF_CloseFont(fontNormalMeret);
    TTF_CloseFont(fontOriasMeret);
}

SDL_Texture *kepernyo_letrehoz(SDL_Renderer *renderer, SDL_Point meret) {
    return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, meret.x, meret.y);
}

void kiir_vegrehajt(SDL_Renderer *renderer, SDL_Point pozicio, char *szoveg, SDL_Color szin, TTF_Font *font) {
    //ezekkel vigyazni kell - nem megfeledkezni a felszabaditasrol!
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, szoveg, szin);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int xm = (pozicio.x < 0) ? (int)(kep_szelesseg - textSurface->w)/2 : pozicio.x;
    SDL_Rect rect = {.x=xm, .y=pozicio.y, .w=textSurface->w, .h = textSurface->h};

    //felszabaditjuk a SDL_Surface-t!
    SDL_FreeSurface(textSurface);
    textSurface = NULL;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    //felszbaditjuk a SDL_Texture-t is!
    SDL_DestroyTexture(texture);
}

void kiir(SDL_Renderer *renderer, SDL_Point pozicio, char *szoveg, SDL_Color szin) {
    kiir_vegrehajt(renderer, pozicio, szoveg, szin, fontNormalMeret);
}

void kiirOrias(SDL_Renderer *renderer, SDL_Point pozicio, char *szoveg, SDL_Color szin) {
    kiir_vegrehajt(renderer, pozicio, szoveg, szin, fontOriasMeret);
}

SDL_Rect kepkoordinata_szamol(SDL_Rect src_rect) {
    SDL_Rect dst_rect = {.x=0, .y=0, .w=kep_szelesseg, .h=kep_magassag};

    // keressuk a nagyitas merteket
    float szorzo = (float) dst_rect.w / (float) src_rect.w;
    if ((src_rect.h * szorzo) < dst_rect.h) {
        szorzo = (float) dst_rect.h / (float) src_rect.h;
    }

    // ha a kep kilog a kepernyorol, eltoljuk ugy, hogy a kep kozepe latszodjon
    if ((src_rect.w * szorzo) > dst_rect.w) {
        dst_rect.x = - ((src_rect.w * szorzo) - dst_rect.w) / 2;
    }
    if ((src_rect.h * szorzo) > dst_rect.h) {
        dst_rect.y = - ((src_rect.h * szorzo) - dst_rect.h) / 2;
    }

    // a kirajzolando kep aranyait megtartva kiszamoljuk a vegso mereteit
    dst_rect.w = (int) (src_rect.w * szorzo);
    dst_rect.h = (int) (src_rect.h * szorzo);

    return dst_rect;
}

void menu_parameter(bool feltetel, SDL_Point *p_pozicio, SDL_Color *pszin) {
    SDL_Point tolas = {.x = -100, .y = kep_magassag / 15};
    if (feltetel) {
        tolas.x = -95;
        *pszin = RGB_MENU_AKTIV;
    } else {
        *pszin = RGB_MENU;
    }
    p_pozicio->x = (int)(kep_szelesseg/2) + tolas.x;
    if (p_pozicio->y == 0) {
        p_pozicio->y = (int)(kep_magassag/3);
        *pszin = RGB_MENU_CIM;
    } else {
        p_pozicio->y += tolas.y;
    }
}

void fomenu_kiir (SDL_Renderer *renderer, fomenu_allapotok *allapot, SDL_Texture *img, SDL_Rect texr) {
    SDL_Rect dst_rect = kepkoordinata_szamol(texr);
    SDL_RenderCopy(renderer, img, &texr, &dst_rect);
    SDL_Point pozicio = {.x=0, .y=0};
    SDL_Color szin;
    const int elemszam = sizeof(fomenu_elemek) / sizeof(menu_elem);
    for (int i = 0; i < elemszam; i++)
    {
        menu_parameter((*allapot == fomenu_elemek[i].allapot), &pozicio, &szin);
        kiir(renderer, pozicio, fomenu_elemek[i].szoveg, szin);
    }
}

void gyorsmenu_kiir (SDL_Renderer *renderer, gyorsmenu_allapotok *gym_allapot) {
    boxRGBA(renderer, 0, 0, kep_szelesseg, kep_magassag, 0, 0, 0, 175);
    SDL_Point pozicio = {.x=0, .y=0};
    SDL_Color szin;
    const int elemszam = sizeof(gyorsmenu_elemek) / sizeof(menu_elem);
    for (int i = 0; i < elemszam; i++)
    {
        menu_parameter((*gym_allapot == gyorsmenu_elemek[i].allapot), &pozicio, &szin);
        kiir(renderer, pozicio, gyorsmenu_elemek[i].szoveg, szin);
    }
}

void jatek_kezdes_kiir(SDL_Renderer *renderer, jatek_kezdes_allapotok *allapot) {
    boxRGBA(renderer, 0, 0, kep_szelesseg, kep_magassag, 0, 0, 0, 255);
    SDL_Point pozicio = {.x=0, .y=0};
    SDL_Color szin;
    const int elemszam = sizeof(jatek_kezdes_menu_elemek) / sizeof(menu_elem);
    for (int i = 0; i < elemszam; i++)
    {
        menu_parameter((*allapot == jatek_kezdes_menu_elemek[i].allapot), &pozicio, &szin);
        kiir(renderer, pozicio, jatek_kezdes_menu_elemek[i].szoveg, szin);
    }
}

void kilepes_elott_kiir (SDL_Renderer *renderer, kilepes_elott_allapotok *allapot) {
    boxRGBA(renderer, 0, 0, kep_szelesseg, kep_magassag, 0, 0, 0, 255);
    SDL_Point pozicio = {.x=0, .y=0};
    SDL_Color szin;
    const int elemszam = sizeof(kilepes_menu_elemek) / sizeof(menu_elem);
    for (int i = 0; i < elemszam; i++)
    {
        menu_parameter((*allapot == kilepes_menu_elemek[i].allapot), &pozicio, &szin);
        kiir(renderer, pozicio, kilepes_menu_elemek[i].szoveg, szin);
    }
}

int hangero_allitas (bool allit) {
    static int hang = 128;
    static bool novel = false;
    if (allit) {
        hang += (novel ? 1 : -1) * 32;
        if (hang <= 0) {
            hang = 0;
            if (!novel) novel = !novel;
        } else if (hang >= 128) {
            hang = 128;
            if (novel) novel = !novel;
        }
        Mix_Volume(-1, hang);
        Mix_VolumeMusic(hang);
    }
    return hang;
}

void beallitasok_kiir (SDL_Renderer *renderer, beallitasok_allapotok *allapot) {
    boxRGBA(renderer, 0, 0, kep_szelesseg, kep_magassag, 0, 0, 0, 255);
    SDL_Point pozicio = {.x=0, .y=0};
    SDL_Color szin;
    char menupont[50];
    int kpoz = 0;
    const int elemszam = sizeof(beallitasok_menu_elemek) / sizeof(menu_elem);
    for (int i = 0; i < elemszam; i++)
    {
        menu_parameter((*allapot == beallitasok_menu_elemek[i].allapot), &pozicio, &szin);
        strcpy(menupont, beallitasok_menu_elemek[i].szoveg);
        if (i == 1) {
            int hangero = hangero_allitas(false)/32;
            kpoz = strlen(menupont);
            for (int i = 0; i < hangero; i++) {
                sprintf(menupont + kpoz + i, "+");
            }
        } else if (i == 2) {
            kpoz = strlen(menupont);
            sprintf(menupont + kpoz, " (%dx%d)", kep_szelesseg, kep_magassag);
        }
        kiir(renderer, pozicio, menupont, szin);
    }
}

void menu_kiir (
    SDL_Renderer *renderer,
    jatek_allapotok *jallapot,
    fomenu_allapotok *fomenu_allapot,
    beallitasok_allapotok *beall_allapot,
    jatek_kezdes_allapotok *jatek_kezdes_allapot,
    gyorsmenu_allapotok *gym_allapot,
    kilepes_elott_allapotok *kilepes_elotti_allapot,
    SDL_Texture *img,
    SDL_Rect texr) {

    switch(*jallapot) {
        case gyorsmenu:
            gyorsmenu_kiir(renderer, gym_allapot);
            break;
        case fomenu:
            fomenu_kiir(renderer, fomenu_allapot, img, texr);
            break;
        case beallitasok:
        case beallitasok_menubol:
            beallitasok_kiir(renderer, beall_allapot);
            break;
        case jatek_kezdes:
            jatek_kezdes_kiir(renderer, jatek_kezdes_allapot);
            break;
        case kilepes_elott_menubol:
        case kilepes_elott_gymenubol:
            kilepes_elott_kiir(renderer, kilepes_elotti_allapot);
            break;
    }
}

void intro_kirajzol(SDL_Renderer *renderer, int time) {
    boxRGBA(renderer, 0, 0, kep_szelesseg, kep_magassag, 0, 0, 0, 255);
    int lepes = (int)(time*0.1);
    if (lepes > 255)
        lepes = 255;
    SDL_Color szin = (SDL_Color) {.r = lepes, .g = lepes, .b = lepes, .a = 255};
    SDL_Point pozicio = (SDL_Point){.x=-1, .y=(int)(kep_magassag/2)};
    kiirOrias(renderer, pozicio, "3D Space Game", szin);

    lepes = (int)(time*0.05);
    if (lepes > 255)
        lepes = 255;
    szin = (SDL_Color) {.r = lepes, .g = lepes, .b = lepes, .a = 255};
    pozicio = (SDL_Point){.x=-1, .y=(int)((kep_magassag/4))*3+30};
    kiir(renderer, pozicio, "Simon Zolt\u00E1n (2019)", szin);
}

vek3d jatek_hatter_kirajzol(SDL_Renderer *renderer, kamera kam, vek3d* csillagok) {

    lekepezes_valtozok lekep;
    lekep.near = 1.0;
    lekep.far = 1000.0;
    lekep.fov = 70.0;
    lekep.aspect_ratio = 1;
    lekep.fov_tangens = 1 / tan(lekep.fov * 0.5 * pi /180.0);

    matrix4x4 projekciom;

    //Csillagok:
    projekcio_matrix_generator(&projekciom, &lekep.aspect_ratio, &lekep.fov_tangens, &lekep.far, &lekep.near);
    vek3d poz;
    for (int i = 0; i< 2000; i++) {
        poz = hatter_lekepezes(csillagok[i], &projekciom, kam, kep_szelesseg, kep_magassag);
        if (poz.x < kep_szelesseg &&
            poz.x > 0 &&
            poz.y < kep_magassag &&
            poz.y > 0) {
            filledCircleRGBA(renderer, poz.x,poz.y,2, 230,210,210,255);
        }
    }

    //Nap:
    poz = hatter_lekepezes((vek3d){0,-1, -1}, &projekciom, kam, kep_szelesseg, kep_magassag);
    if (poz.z > 0) {
        if (poz.x < kep_szelesseg &&
            poz.x > 0 &&
            poz.y < kep_magassag &&
            poz.y > 0) {
            filledCircleRGBA(renderer, poz.x, poz.y,20, 255,240,235,255);
        }

    }

    return poz;
}

//Kirajzolja a polygont:
void haromszog_rajz(SDL_Renderer *renderer, poli triangle, bool debug_mod, int offset) {
    rgba szin = (rgba){
        (int)(triangle.fenyesseg * triangle.szin.r)+20,
        (int)(triangle.fenyesseg * triangle.szin.g)+20,
        (int)(triangle.fenyesseg * triangle.szin.b)+20,
        triangle.szin.a
    };

    if (szin.r > 255) {
        szin.r = 255;
    }
    if (szin.g > 255) {
        szin.g = 255;
    }
    if (szin.b > 255) {
        szin.b = 255;
    }

    filledTrigonRGBA(
            renderer,
            (int)triangle.pv[0].x, (int)triangle.pv[0].y + offset,
            (int)triangle.pv[1].x, (int)triangle.pv[1].y + offset,
            (int)triangle.pv[2].x, (int)triangle.pv[2].y + offset,
            szin.r,
            szin.g,
            szin.b,
            szin.a);

    if (debug_mod) {
        trigonRGBA(
            renderer,
            (int)triangle.pv[0].x, (int)triangle.pv[0].y + offset,
            (int)triangle.pv[1].x, (int)triangle.pv[1].y + offset,
            (int)triangle.pv[2].x, (int)triangle.pv[2].y + offset,
            0, 255, 0, 255);
    }
}

void miniterkep(SDL_Renderer *renderer, int offs_x, int offs_y,int kep_x, int kep_y, objektum_tomb *objektumok, objektum *letiltott, kamera *kam) {
    int terkep_meret = kep_y / 5;
    int xpoz = offs_x + kep_x/2- terkep_meret/2;
    int ypoz = offs_y + kep_y - terkep_meret;

    lekepezes_valtozok lekep;
    lekep.near = 0.1;
    lekep.far = 1000.0;
    lekep.fov = 30.0;
    lekep.aspect_ratio = 1;
    lekep.fov_tangens = 1 / tan(lekep.fov * 0.5 * pi /180.0);

    matrix4x4 projekciom;
    projekcio_matrix_generator(&projekciom, &lekep.aspect_ratio, &lekep.fov_tangens, &lekep.far, &lekep.near);

    filledCircleRGBA(renderer,xpoz+terkep_meret/2, ypoz+terkep_meret/2, terkep_meret/2, 120,120,120,50);
    lineRGBA(renderer, xpoz+terkep_meret/2, ypoz+terkep_meret/2, xpoz+terkep_meret/2-terkep_meret/15, ypoz+terkep_meret/2+terkep_meret/10, 255,255,255,100);
    lineRGBA(renderer, xpoz+terkep_meret/2, ypoz+terkep_meret/2, xpoz+terkep_meret/2+terkep_meret/15, ypoz+terkep_meret/2+terkep_meret/10, 255,255,255,100);

    for (int i = 0; i < objektumok->meret; i++) {
        if (objektumok->objp[i].lathato && &objektumok->objp[i] != letiltott) {
            vek3d lekepezett = minimap_lekepezes(objektumok->objp[i].pozicio, &projekciom, *kam, terkep_meret, terkep_meret);
            if (objektumok->objp[i].tipus == urhajo) {
                lineRGBA(renderer, (int)(xpoz+lekepezett.x-3), (int)(ypoz+lekepezett.y), (int)(xpoz+lekepezett.x+3), (int)(ypoz+lekepezett.y),255, 255, 255, 100);
                lineRGBA(renderer, (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y), (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y - lekepezett.z),255,255,255,100);
                circleRGBA(renderer, (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y - lekepezett.z), 3, 255, 0, 0, 255);
            }
            if (objektumok->objp[i].tipus == bolygo) {
                lineRGBA(renderer, (int)(xpoz+lekepezett.x-3), (int)(ypoz+lekepezett.y), (int)(xpoz+lekepezett.x+3), (int)(ypoz+lekepezett.y),255,255,255,100);
                lineRGBA(renderer, (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y), (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y - lekepezett.z),255,255,255,100);
                circleRGBA(renderer, (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y - lekepezett.z), 6, 0, 0, 255, 255);
            }
            if (objektumok->objp[i].tipus == meteor) {
                lineRGBA(renderer, (int)(xpoz+lekepezett.x-3), (int)(ypoz+lekepezett.y), (int)(xpoz+lekepezett.x+3), (int)(ypoz+lekepezett.y),255,255,255,100);
                lineRGBA(renderer, (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y), (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y - lekepezett.z),255,255,255,100);
                circleRGBA(renderer, (int)(xpoz+lekepezett.x), (int)(ypoz+lekepezett.y - lekepezett.z), 2, 200, 200, 200, 255);
            }
        }
    }
    //HUD
    lineRGBA(renderer, kep_x/2, kep_y/2, kep_x/2-kep_x/25, kep_y/2+kep_y/50, 255,255,255,100);
    lineRGBA(renderer, kep_x/2, kep_y/2, kep_x/2+kep_x/25, kep_y/2+kep_y/50, 255,255,255,100);

    //Sebesseg vektor:
    lineRGBA(renderer, kep_x/2-3, kep_y/2+kep_y/20, kep_x/2+3, kep_y/2+kep_y/20, 255,255,255,100);
    lineRGBA(renderer, kep_x/2, kep_y/2+kep_y/20, kep_x/2+kam->iranyv.x*20, kep_y/2+kam->iranyv.y*20-kam->iranyv.z*10+kep_y/20, 255,255,255,100);
    lineRGBA(renderer, kep_x/2+kam->iranyv.x*20-6, kep_y/2+kam->iranyv.y*20-kam->iranyv.z*10+kep_y/20-kam->iranyv.y*3, kep_x/2+kam->iranyv.x*20, kep_y/2+kam->iranyv.y*20-kam->iranyv.z*10+kep_y/20, 255,255,255,100);
    lineRGBA(renderer, kep_x/2+kam->iranyv.x*20+6, kep_y/2+kam->iranyv.y*20-kam->iranyv.z*10+kep_y/20-kam->iranyv.y*3, kep_x/2+kam->iranyv.x*20, kep_y/2+kam->iranyv.y*20-kam->iranyv.z*10+kep_y/20, 255,255,255,100);

}

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
    bool megsemmisult) {

    if (kepernyo) {
        SDL_SetRenderTarget(renderer, kepernyo);
    }

    boxRGBA(renderer, 0, 0, kep_x, kep_y, 20, 20, 30, 255);

    int offset = 0;
    jatek_hatter_kirajzol(renderer, *kam, csillagok);

    *kir_varo_db = mesh_feldolgozas(
        objektumok, sajat, kam, projekcio,
        kep_x, kep_y,
        &offs_x, kirajzolasra_varo);

    for (int i = 0; i < *kir_varo_db; i++) {
        haromszog_rajz(renderer, kirajzolasra_varo[i], debug_mod, 0);
    }

    //Nap elvakitas:
    //double lathato = skalar_szorzat(&kam1->iranyv, &kamera_poz_tav);
    vek3d feny_forras_iranya = {0,-1, -1};
    feny_forras_iranya = vektor_normalizacio(feny_forras_iranya);
    double feny_beeses = skalar_szorzat(&kam->iranyv, &feny_forras_iranya);
    if (feny_beeses > 0.8) {
        boxRGBA(renderer, 0, 0, kep_x, kep_y, 255, 255, 255, feny_beeses*60);
    }

    //HUD kepernyo:
    miniterkep(renderer, offs_x, 0, kep_x, kep_y, objektumok, sajat, kam);

    //"Megsemmisult" kijelzese:
    if (megsemmisult) {
        boxRGBA(renderer, 0, 0, kep_x, kep_y, 250, 0, 50, 150);
    }

    if (kepernyo) {
        SDL_SetRenderTarget(renderer, NULL);
        SDL_Rect src_rect = { .x=0, .y=0, .w=kep_szelesseg, .h=kep_magassag/2 };
        SDL_Rect dst_rect = { .x=0, .y=offs_y, .w=kep_szelesseg, .h=kep_magassag/2 };
        SDL_RenderCopy(renderer, kepernyo, &src_rect, &dst_rect);
    }
}

SDL_Texture * sugo_letrehoz(SDL_Renderer *renderer, SDL_Point meret) {
    SDL_Texture *sugo = kepernyo_letrehoz(renderer, meret);
    if (sugo) {
        SDL_SetRenderTarget(renderer, sugo);
        boxRGBA(renderer, 0, 0, meret.x, meret.y, 50, 50, 50, 255);
        const int eltolas = kep_magassag / 22;
        const int sorok_szama = sizeof(sugo_sorok) / sizeof(_sor_);
        char oszlop1[100], oszlop2[100], oszlop3[100];
        SDL_Color betuszin;
        int x = 0, y = 0;
        for (int i = 0; i < sorok_szama; i++) {
            x = eltolas;
            y = eltolas + i*eltolas;
            strcpy(oszlop1, sugo_sorok[i].oszlopok[0]);
            strcpy(oszlop2, sugo_sorok[i].oszlopok[1]);
            strcpy(oszlop3, sugo_sorok[i].oszlopok[2]);
            betuszin = sugo_sorok[i].szin;
            if (oszlop1 && strlen(oszlop1) > 0) {
                kiir(renderer, (SDL_Point) {x, y}, oszlop1, betuszin);
            }
            x += kep_szelesseg / 3;
            if (oszlop2 && strlen(oszlop2) > 0) {
                kiir(renderer, (SDL_Point) {x, y}, oszlop2, betuszin);
            }
            x += kep_szelesseg / 3;
            if (oszlop3 && strlen(oszlop3) > 0) {
                kiir(renderer, (SDL_Point) {x, y}, oszlop3, betuszin);
            }
        }
        SDL_SetRenderTarget(renderer, NULL);
    }
    return sugo;
}

void sugo_kirajzol(SDL_Renderer *renderer, SDL_Texture *sugo) {
    SDL_RenderCopy(renderer, sugo, NULL, NULL);
}

void elvalaszto_kirajzol(SDL_Renderer *renderer) {
    boxRGBA(renderer, 0, kep_magassag/2, kep_szelesseg, (kep_magassag/2)+2, 75, 75, 75, 255);
}