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

/* Let this library to be also compilable in C++ projects */
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

/* Add extra parenthesis, you never know what the mean programmer is able to do :-) */
#define MEGNU_MAX_PAGE_ENTRIES_INTERNAL     (MEGNU_MAX_PAGE_ENTRIES)

/* Defnine the size of the internal buffer */
#define MEGNU_MAX_MENU_ITEMS    (20U)

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
    MENU_EVENT_OUTPUT_GOTO          /**< "goto" item has been clicked */
} e_menu_output_event;

/** Enumeration of supported menu item types */
typedef enum
{
    MENU_TYPE_NONE,              /**< Label only */
    MENU_TYPE_LIST,              /**< Menu item extra type is a list */
    MENU_TYPE_LIST_EDIT,         /**< Menu item extra type is a list */
    MENU_TYPE_NUMERIC_8,         /**<  */
    MENU_TYPE_NUMERIC_16,        /**<  */
    MENU_TYPE_NUMERIC_32,        /**<  */
    MENU_TYPE_NUMERIC_8_EDIT,    /**<  */
    MENU_TYPE_NUMERIC_16_EDIT,   /**<  */
    MENU_TYPE_NUMERIC_32_EDIT,   /**<  */
    MENU_TYPE_CALLBACK,          /**< Menu item that is bound to the return value of a given function */
    MENU_TYPE_GOTO,              /**< Menu item to get to another page */
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
    uint8_t  count;          /**< Total number of labels */
    uint8_t  *ptr;           /**< The index of the currently active label */
    uint8_t* values;         /**< List of values to associate the ptr to */
    char*   labels[];         /**< Labels to associate the ptr value to */
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
    uint8_t             index;       /**< Selected menu item */
    uint8_t             start;        /**< Selected menu item */
    e_menu_item_state   state;       /**< Item state */
    uint16_t            diff;        /**< Determines scrolling "speed" / "velocity" (e.g. from an encoder or repeated keypress) */
    t_menu_item         items[MEGNU_MAX_MENU_ITEMS];  /**< Menu items (i.e. elements) */
    uint8_t             item_count;    /**< Actual number of menu items */
    uint8_t             page;          /**< Selected page number */
    uint8_t             lines;         /**< The total lines of numbers to display */
} t_menu_state;

/** Event callback function
 *
 * @param event
 * @param index
 * @param page
 * @param info
 */
extern void menu_event_callback(e_menu_output_event event, uint8_t index, uint8_t page, uint8_t info);

/** EXTERNAL LINKAGE GLOBALS **/

/* None so far */

/**  EXTERNAL LINKAGE FUNCTIONS **/

/**
 * Initialize the internal structures to a known state
 * @param   lines   the number of physical lines of the destination display
 *                  (or a subset of them)
 */
void menu_init(uint8_t lines);

/**
 * Removes all the item from the internal state i.e.
 * sets the item count to 0 and indexes to 0 as well.
 */
void menu_clear(void);

/**
 * Add a new item to the current menu internal state
 * @param item
 */
void menu_item_add(t_menu_item *item);

/**
 * Set a new page number
 * @param page  the new page number
 */
void menu_set_page(uint8_t page);

/**
 * Return the current page value
 * @return  the page value
 */
uint8_t menu_get_page(void);

void menu_set_diff(uint16_t diff);
void menu_display(void);
e_menu_output_event menu_event(e_menu_input_event event);

#ifdef __cplusplus
}
#endif

#endif /* MENU_H_ */
