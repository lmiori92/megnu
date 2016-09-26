/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Lorenzo Miori (C) 2015 [ 3M4|L: memoryS60<at>gmail.com ]

    Version History
        * 1.0 initial

*/

/**
 * @file menu.h
 * @author Lorenzo Miori
 * @date Apr 2016
 * @brief MENU: handling of simple menu logic for character displays
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

<<<<<<< HEAD
/* Add extra parenthesis, you never know what the mean programmer is able to do :-) */
#define MEGNU_MAX_PAGE_ENTRIES_INTERNAL     (MEGNU_MAX_PAGE_ENTRIES)

=======
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b
/** Enumeration of supported events the menu system can handle (input) */
typedef enum
{
    MENU_EVENT_NONE,
    MENU_EVENT_CLICK,
    MENU_EVENT_CLICK_LONG,
    MENU_EVENT_LEFT,
    MENU_EVENT_RIGHT
} e_menu_input_event;

/** Enumeration of supported events the menu system can generate (output) */
typedef enum _e_menu_output_event
{
    MENU_EVENT_OUTPUT_NONE,
    MENU_EVENT_OUTPUT_CLICK,
    MENU_EVENT_OUTPUT_CLICK_LONG,
    MENU_EVENT_OUTPUT_INDEX_EDIT,   /**< item index value has been changed */
    MENU_EVENT_OUTPUT_EXTRA_EDIT,   /**< item extra value has been changed */
    MENU_EVENT_OUTPUT_SELECT,       /**< item is now selected */
    MENU_EVENT_OUTPUT_DESELECT,     /**< item is not selected anymore */
<<<<<<< HEAD
    MENU_EVENT_OUTPUT_GOTO          /**< "goto" item has been clicked */
=======
    MENU_EVENT_OUTPUT_BACK     /**< "back" item has been clicked */
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b
} e_menu_output_event;

/** Enumeration of supported menu item types */
typedef enum
{
    MENU_TYPE_NONE,            /**< Label only */
    MENU_TYPE_LIST,            /**< Menu item extra type is a list */
    MENU_TYPE_NUMERIC_8,       /**<  */
    MENU_TYPE_NUMERIC_16,      /**<  */
    MENU_TYPE_NUMERIC_32,      /**<  */
<<<<<<< HEAD
    MENU_TYPE_GOTO,            /**< Menu item to get to another page */
=======
    MENU_TYPE_BACK             /**< Menu item to get to the previous page */
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b
} e_item_type;

/** Enumeration of the possible states a menu item can be */
typedef enum
{
    MENU_NOT_SELECTED,         /**< Menu item is not selected to perform actions on it */
    MENU_SELECTED              /**< Menu item is selected and actions can be performed on it */
} e_menu_item_state;

/** Structure for the extra LIST item information (labels and values) */
typedef struct
{
    uint8_t  count;           /**< Total number of labels */
    uint8_t  ptr;             /**< The index of the currently active label */
    char**   labels;         /**< Labels to associate the ptr value to */
    uint8_t* values;         /**< List of values to associate the ptr to */
} t_menu_extra_list;

/** Structure for describing the menu item */
typedef struct
{
    char*           label;  /**< Item has this string as label */
    void*           extra;  /**< When != NULL an extra is appended, depending on the type of menu */
    e_item_type     type;   /**< Menu type; determines extra field type */
} t_menu_item;

/** Structure for the current menu state */
typedef struct
{
    uint8_t index;              /**< Selected menu item */
    uint8_t prev;               /**< Selected menu item */
    e_menu_item_state   state;  /**< Item state */
    uint16_t diff;              /**< Determines scrolling "speed" / "velocity" (e.g. from an encoder or repeated keypress) */
} t_menu_state;

/** Event callback function */
typedef void (*t_menu_cb)(e_menu_output_event event, uint8_t index, uint8_t page);

<<<<<<< HEAD
/** EXTERNAL LINKAGE GLOBALS **/

/** Definition of the page entries, which are freely settable by the application */
//extern t_menu_item g_megnu_page_entries[];
//extern uint8_t     g_megnu_page_entries_max;

=======
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b
/**  EXTERNAL LINKAGE FUNCTIONS **/

void menu_init(t_menu_state *state);
void menu_init_bool_list(t_menu_extra_list *extra);
void menu_event_callback(t_menu_cb menu_cb);
void menu_set(t_menu_state *state, t_menu_item *item, uint8_t count, uint8_t page);
void menu_set_diff(uint16_t diff);
void menu_display(void);
e_menu_output_event menu_event(e_menu_input_event event);

#endif /* MENU_H_ */
