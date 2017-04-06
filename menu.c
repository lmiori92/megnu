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
 * @file menu.c
 * @author Lorenzo Miori
 * @date Jun 2016
 * @brief MENU: handling of simple menu logic for character displays
 */

#include "menu.h"       /* menu definitions */

/* External libraries */
#include "../deasplay/deasplay.h"    /* display primitives */
#include "../lorenzlib/lib.h"        /* utilities */

/** Static declarations **/
static void menu_extra_display(void *extra, e_item_type type);
static void menu_index_edit(bool increment);
static void menu_extra_edit(t_menu_item *item, bool increment);

/* State */
static t_menu_state g_state;

void menu_init(uint8_t lines)
{
    g_state.diff = 1;
    g_state.page = 0;
    g_state.lines = lines;

    menu_clear();
}

void menu_clear(void)
{
    g_state.index = 0;
    g_state.start = 0;
    g_state.state = MENU_NOT_SELECTED;
    g_state.item_count = 0;
}

static void menu_extra_display(void *extra, e_item_type type)
{

    uint8_t u8_tmp;
    uint16_t u16_tmp;
    uint32_t u32_tmp;
    t_menu_extra_list *ext_ptr_tmp;

    if (extra != NULL)
    {
        /* dereference and operate the pointer based on the defined type */
        switch(type)
        {
            case MENU_TYPE_LIST:
            case MENU_TYPE_LIST_EDIT:
                /* 8-bit array index */
                /* display the "enum-like" element */
                ext_ptr_tmp = (t_menu_extra_list*)extra;
                u8_tmp = (*((uint8_t*)ext_ptr_tmp->ptr));
                display_write_string(ext_ptr_tmp->labels[u8_tmp]);
                break;
            case MENU_TYPE_NUMERIC_8:
            case MENU_TYPE_NUMERIC_8_EDIT:
                /* 8-bit number */
                u8_tmp = (*((uint8_t*)extra));
                display_write_number((uint16_t)u8_tmp, false);
                break;
            case MENU_TYPE_NUMERIC_16:
            case MENU_TYPE_NUMERIC_16_EDIT:
                /* 16-bit number */
                u16_tmp = (*((uint16_t*)extra));
                display_write_number(u16_tmp, false);
                break;
            case MENU_TYPE_NUMERIC_32:
            case MENU_TYPE_NUMERIC_32_EDIT:
                /* 16-bit number */
                u32_tmp = (*((uint32_t*)extra));
                display_write_number(u32_tmp, false);
                break;
            default:
                /* not implemented */
                break;
        }
    }
}

static void menu_index_edit(bool increment)
{
    if (g_state.item_count > 0)
    {
        if (increment == true)
        {
            /* increment - "menu down" */
            if (g_state.index < (g_state.item_count - 1U))
            {
                g_state.index++;                    /* increment value */
                if (g_state.index - g_state.start >= g_state.lines)
                {
                    g_state.start = g_state.index;
                }
            }
        }
        else
        {
            /* decrement - "menu up" */
            if (g_state.index > 0U)
            {
                g_state.index--;                    /* increment value */
                if (g_state.index < g_state.start)
                {
                    g_state.start = g_state.index;
                }
            }
        }
    }
}

static void menu_extra_edit(t_menu_item *item, bool increment)
{
    t_menu_extra_list* tmp_extra;
    uint16_t tmp;

    if (item->extra != NULL)
    {
        /* dereference and operate the pointer based on the defined type */
        switch(item->type)
        {
            case MENU_TYPE_LIST_EDIT:
                /* 8-bit number for indexing the type "list" */
                tmp_extra = (t_menu_extra_list*)item->extra;
                if (increment == true)
                {
                    if (*((uint8_t*)tmp_extra->ptr) < (tmp_extra->count - 1))
                        (*((uint8_t*)tmp_extra->ptr))++;
                }
                else
                {
                    if ((*((uint8_t*)tmp_extra->ptr)) > 0U)
                        (*((uint8_t*)tmp_extra->ptr))--;
                }
                break;
            case MENU_TYPE_NUMERIC_8_EDIT:
                /* 8-bit number */
                tmp = (uint16_t)(*((uint8_t*)item->extra));
                if (increment == true) lib_sum(&tmp, UINT8_MAX, g_state.diff);
                else lib_diff(&tmp, g_state.diff);
                *((uint8_t*)item->extra) = (uint8_t)tmp;
                break;
            case MENU_TYPE_NUMERIC_16_EDIT:
                /* 16-bit number */
                tmp = (*((uint16_t*)item->extra));
                if (increment == true) lib_sum(&tmp, UINT16_MAX, g_state.diff);
                else lib_diff(&tmp, g_state.diff);
                *((uint16_t*)item->extra) = tmp;
                break;
            default:
                /* not implemented */
                break;
        }
    }
}

/**
This routine displays the current state of the menu i.e. labels, selection
and possibly extra data like ON-OFF labels or numeric values.
*/
void menu_display(void)
{

    bool selected;
    uint8_t i = 0;
    uint8_t id = 0;

    display_clean();

    for (i = 0; i < g_state.lines; i++)
    {
        id = g_state.start+i;
        if (id < g_state.item_count)
        {
            selected = (id == g_state.index);
            /* write menu entries on the display */
            display_set_cursor(i, 0U);
            display_write_char(selected ? '-' : ' ');
            display_write_string(g_state.items[id].label);         /* label */
            display_advance_cursor(1U);
            /* [ or space */
            display_write_char(((selected) && (g_state.state == MENU_SELECTED)) ? '[' : ' ');
            menu_extra_display(g_state.items[id].extra, g_state.items[id].type);
            /* ] or space */
            display_write_char(((selected) && (g_state.state == MENU_SELECTED)) ? ']' : ' ');
        }
    }

//    /* selected item arrow display */
//    display_write_char(0x7EU);
}

static inline bool menu_is_editable(e_item_type type)
{
    if ((type == MENU_TYPE_NUMERIC_8_EDIT) || (type == MENU_TYPE_NUMERIC_16_EDIT) ||
        (type == MENU_TYPE_NUMERIC_32_EDIT) || (type == MENU_TYPE_LIST_EDIT))
    {
        return true;
    }
    else
    {
        return false;
    }
}

e_menu_output_event menu_event(e_menu_input_event event)
{

    t_menu_item *item;
    e_menu_output_event output_event = MENU_EVENT_OUTPUT_NONE;
    uint8_t info;

    if (g_state.item_count == 0U)
    {
    	/* No menu page is selected */
    }
    else
    {

        item = &g_state.items[g_state.index];

        switch(event)
        {
            case MENU_EVENT_NONE:
                /* no event; NOOP */
                break;
            case MENU_EVENT_CLICK:
                if (item->type == MENU_TYPE_GOTO)
                {
                    output_event = MENU_EVENT_OUTPUT_GOTO;
                }
                else if (menu_is_editable(item->type) == true)
                {
                    /* complex entry (editable) -> toggle selected / unselected */
                    g_state.state = (g_state.state == MENU_SELECTED) ? MENU_NOT_SELECTED : MENU_SELECTED;
                    output_event = (g_state.state == MENU_SELECTED) ? MENU_EVENT_OUTPUT_SELECT : MENU_EVENT_OUTPUT_DESELECT;
                }
                else
                {
                    /* if a simple entry, do nothing (probably changing menu anyhow) */
                    g_state.state = MENU_NOT_SELECTED;
                    output_event = MENU_EVENT_OUTPUT_CLICK;
                }
                break;
            case MENU_EVENT_CLICK_LONG:
                /* NOOP for the moment */
                output_event = MENU_EVENT_OUTPUT_CLICK_LONG;
                break;
            case MENU_EVENT_LEFT:
                if (g_state.state == MENU_SELECTED)
                {
                    menu_extra_edit(item, false);
                    output_event = MENU_EVENT_OUTPUT_EXTRA_EDIT;
                }
                else
                {
                    menu_index_edit(false);
                    output_event = MENU_EVENT_OUTPUT_INDEX_EDIT;
                }
                break;
            case MENU_EVENT_RIGHT:
                if (g_state.state == MENU_SELECTED)
                {
                    menu_extra_edit(item, true);
                    output_event = MENU_EVENT_OUTPUT_EXTRA_EDIT;
                }
                else
                {
                    menu_index_edit(true);
                    output_event = MENU_EVENT_OUTPUT_INDEX_EDIT;
                }
                break;
            default:
                /* no event; NOOP */
                break;
        }

        /* Callback for the external world (if any) */
        if (output_event != MENU_EVENT_OUTPUT_NONE)
        {
            if (output_event == MENU_EVENT_OUTPUT_GOTO)
            {
                /* additional info: new page number */
                info = ((uint8_t)(item->extra));
            }
            else
            {
                /* no additional info about the item event */
                info = 0;
            }

            menu_event_callback(output_event, g_state.index, g_state.page, info);
        }
    }

    return output_event;
}

void menu_item_add(t_menu_item *item)
{
    if ((item != NULL) && (g_state.item_count < MEGNU_MAX_MENU_ITEMS))
    {
        memcpy(&g_state.items[g_state.item_count], item, sizeof(t_menu_item));
        g_state.item_count++;
    }
    else
    {
        /* NULL pointer ! */
    }
}

void menu_set_page(uint8_t page)
{
    g_state.page = page;
}

uint8_t menu_get_page(void)
{
    return g_state.page;
}

void menu_set_diff(uint16_t diff)
{
    g_state.diff = (diff == 0U) ? 1U : diff;
}
