/**
 * @file menu.h
 * @author Simon Zoltán
 * @brief Menu adatszerkezetek és funkciók
 * @version 0.1
 * @date 2019-11-29
 *
 * @copyright Copyright (c) 2019
 *
 */

// ################################################################
#ifndef _MENU_IMPORTALVA_
#define _MENU_IMPORTALVA_
// ################################################################

typedef enum jatek_allapotok {
    bevezeto,
    fomenu,
    beallitasok,
    beallitasok_menubol,
    jatek,
    jatek_kezdes,
    gyorsmenu,
    kilepes_elott_menubol,
    kilepes_elott_gymenubol,
    meretj
} jatek_allapotok;

typedef enum kilepes_elott_allapotok {
    kilep_ke,
    vissza_ke
} kilepes_elott_allapotok;

typedef enum jatek_kezdes_allapotok {
    solo_jk,
    duo_jk,
    vissza_jk
} jatek_kezdes_allapotok;

typedef enum beallitasok_allapotok {
    hangero_b,
    felbontas_b,
    vissza_b,
    meret_b
} beallitasok_allapotok;

typedef enum gyorsmenu_allapotok {
    vissza_gy,
    beallitasok_gy,
    menube_gy,
    kilep_gy,
    meret_gy
} gyorsmenu_allapotok;

typedef enum fomenu_allapotok {
    indit_f,
    beallitasok_f,
    kilep_f,
    meret_f
} fomenu_allapotok;

typedef enum menu_irany {
    fel,
    le
} menu_irany;

typedef struct gy_menupont {
    jatek_allapotok kov_allapot;
}menupont;

typedef struct menu_elem  {
    unsigned char szoveg[50];
    int  allapot;
} menu_elem;

// FOMENU:
static menu_elem fomenu_elemek[] = {
    { .szoveg  = "F\u0151men\u00FC",
      .allapot = -1 },
    { .szoveg  = "\u0170rrep\u00FCl\u00E9s megkezd\u00E9se",
      .allapot = indit_f },
    { .szoveg  = "Rendszer kalibr\u00E1l\u00E1sa",
      .allapot = beallitasok_f },
    { .szoveg  = "Fed\u00E9lzet elhagy\u00E1sa",
      .allapot = kilep_f }
};

// GYORSMENU:
static menu_elem gyorsmenu_elemek[] = {
    { .szoveg  = "Sz\u00FCnet",
      .allapot = -1 },
    { .szoveg  = "J\u00E1t\u00E9k folytat\u00E1sa",
      .allapot = vissza_gy },
    { .szoveg  = "Be\u00E1ll\u00EDt\u00E1sok",
      .allapot = beallitasok_gy },
    { .szoveg  = "Kil\u00E9p\u00E9s a f\u0151men\u00FCbe",
      .allapot = menube_gy },
    { .szoveg  = "Kil\u00E9p\u00E9s a programb\u00F3l",
      .allapot = kilep_gy }
};

// JATEK_KEZDES (JATEKMOD VALASZTO) MENU:
static menu_elem jatek_kezdes_menu_elemek[] = {
    { .szoveg  = "\u0170rrep\u00FCl\u00E9s megkezd\u00E9se",
      .allapot = -1 },
    { .szoveg  = "Sz\u00F3l\u00F3 rep\u00FCl\u00E9s",
      .allapot = solo_jk },
    { .szoveg  = "K\u00E9t pil\u00F3ta",
      .allapot = duo_jk },
    { .szoveg  = "Vissza",
      .allapot = vissza_jk }
};

// A KILEPEST JOVAHAGYO MENU:
static menu_elem kilepes_menu_elemek[] = {
    { .szoveg  = "Elhagyod a fed\u00E9lzetet",
      .allapot = -1 },
    { .szoveg  = "Jov\u00E1hagy\u00E1s",
      .allapot = kilep_ke },
    { .szoveg  = "Vissza",
      .allapot = vissza_ke }
};

// BEALLITASOK MENU:
static menu_elem beallitasok_menu_elemek[] = {
    { .szoveg  = "Be\u00E1ll\u00EDt\u00E1sok",
      .allapot = -1 },
    { .szoveg  = "Hanger\u0151 ",
      .allapot = hangero_b },
    { .szoveg  = "Ablakm\u00E9ret ",
      .allapot = felbontas_b },
    { .szoveg  = "Vissza",
      .allapot = vissza_b }
};

// ################################################################
#endif
// ################################################################