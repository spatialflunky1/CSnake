#include <Windows.h>

/*
Menubar items
*/

MENUITEMINFO fileMenu = {
    sizeof(MENUITEMINFO),   // size of the structure (in bytes)                 Required flag
    MIIM_TYPE,              // indicates the member to be retrieved or set      ---------------------------------------
    MFT_STRING,             // menu item type                                   only retrieved if MIIM_TYPE is set
    NULL,                   // menu item state                                  only retrieved if MIIM_STATE is set
    NULL,                   // menu item id                                     only retrieved if MIIM_ID is set
    NULL,                   // handle to submenu                                only retrieved if MIIM_SUBMENU is set
    NULL,                   // bitmap handle if item is checked                 only retrieved if MIIM_CHECKMARKS is set
    NULL,                   // bitmap handle if item in unchecked               only retrieved if MIIM_CHECKMARKS is set
    NULL,                   // menu item data                                   only retrieved if MIIM_DATA is set
    L"File",                // menu item type contents                          only retrieved if MIIM_TYPE & MFT_STRING is set
    4,                      // length of the menu item text                     only retrieved if MIIM_TYPE & MFT_STRING is set
    NULL                    // handle to the bitmap                             only retrieved if MIIM_BITMAP
};

/*
File menu submenus
*/

MENUITEMINFO editMenu = {
    sizeof(MENUITEMINFO),   // size of the structure (in bytes)                 Required flag
    MIIM_TYPE,              // indicates the member to be retrieved or set      ---------------------------------------
    MFT_STRING,             // menu item type                                   only retrieved if MIIM_TYPE is set
    NULL,                   // menu item state                                  only retrieved if MIIM_STATE is set
    NULL,                   // menu item id                                     only retrieved if MIIM_ID is set
    NULL,                   // handle to submenu                                only retrieved if MIIM_SUBMENU is set
    NULL,                   // bitmap handle if item is checked                 only retrieved if MIIM_CHECKMARKS is set
    NULL,                   // bitmap handle if item in unchecked               only retrieved if MIIM_CHECKMARKS is set
    NULL,                   // menu item data                                   only retrieved if MIIM_DATA is set
    L"Edit",                // menu item type contents                          only retrieved if MIIM_TYPE & MFT_STRING is set
    4,                      // length of the menu item text                     only retrieved if MIIM_TYPE & MFT_STRING is set
    NULL                    // handle to the bitmap                             only retrieved if MIIM_BITMAP
};