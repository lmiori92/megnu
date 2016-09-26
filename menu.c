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


<<<<<<< HEAD
#include "menu.h"       /* menu definitions */

/* External libraries */
#include "../deasplay/deasplay.h"    /* display primitives */
#include "../lorenzlib/lib.h"        /* utilities */
=======
#include "display.h"    /* display primitives */
#include "menu.h"       /* menu definitions */
#include "lib.h"        /* utilities */
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b

static char* BOOL_LABELS[2] = { "NO", "YES" };
static uint8_t BOOL_VALUES[2] = { (uint8_t)false, (uint8_t)true };

/** Static declarations **/
static void menu_extra_display(void *extra, e_item_type type);
static void menu_index_edit(t_menu_state *state, uint8_t count, bool increment);
static void menu_extra_edit(t_menu_item *item, bool increment);

/* State */
static t_menu_state *g_state;
static t_menu_item  *g_item;
static uint8_t       g_count;
static uint8_t       g_page;
static t_menu_cb     g_menu_event_cb;

void menu_init_bool_list(t_menu_extra_list *extra)
{
    extra->count = 2U;
    extra->labels = BOOL_LABELS;
    extra->values = BOOL_VALUES;
}

void menu_init(t_menu_state *state)
{
    if (g_state != NULL)
    {
        state->state = MENU_NOT_SELECTED;
        state->index = 0U;
        state->prev = 1U;
        state->diff = 1U;
    }
}

void menu_event_callback(t_menu_cb menu_cb)
{
    g_menu_event_cb = menu_cb;
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
                /* 8-bit array index */
                /* display the "enum-like" element */
                ext_ptr_tmp = (t_menu_extra_list*)extra;
                u8_tmp = ext_ptr_tmp->ptr;
                display_write_string(ext_ptr_tmp->labels[u8_tmp]);
                break;
            case MENU_TYPE_NUMERIC_8:
                /* 8-bit number */
                u8_tmp = (*((uint8_t*)extra));
                display_write_number((uint16_t)u8_tmp, false);
                break;
            case MENU_TYPE_NUMERIC_16:
                /* 16-bit number */
                u16_tmp = (*((uint16_t*)extra));
                display_write_number(u16_tmp, false);
                break;
            case MENU_TYPE_NUMERIC_32:
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

static void menu_index_edit(t_menu_state *state, uint8_t count, bool increment)
{
    if (count > 0)
    {
        if (increment == true)
        {
            /* increment - "menu down" */
            if (state->index < (count - 1U))
            {
                state->prev = state->index;
                state->index++;                    /* increment value */
            }
        }
        else
        {
            /* decrement - "menu up" */
            if (state->index > 0U)
            {
                state->prev = state->index;
                state->index--;                    /* increment value */
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
            case MENU_TYPE_LIST:
                /* 8-bit number for indexing the type "list" */
                tmp_extra = (t_menu_extra_list*)item->extra;
                if (increment == true)
                {
                    if (tmp_extra->ptr < (tmp_extra->count - 1))
                        tmp_extra->ptr++;
                }
                else
                {
                    if (tmp_extra->ptr > 0U)
                        tmp_extra->ptr--;
                }
                break;
            case MENU_TYPE_NUMERIC_8:
                /* 8-bit number */
                tmp = (uint16_t)(*((uint8_t*)item->extra));
                if (increment == true) lib_sum(&tmp, UINT8_MAX, g_state->diff);
                else lib_diff(&tmp, g_state->diff);
                *((uint8_t*)item->extra) = (uint8_t)tmp;
                break;
            case MENU_TYPE_NUMERIC_16:
                /* 16-bit number */
                tmp = (*((uint16_t*)item->extra));
                if (increment == true) lib_sum(&tmp, UINT16_MAX, g_state->diff);
                else lib_diff(&tmp, g_state->diff);
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

    uint8_t id1;
    uint8_t id2;

    if ((g_state == NULL ) || (g_item == NULL))
    {
    	/* No menu page is selected */
    	return;
    }

    display_clean();

    /* selected item arrow display */
    if (g_state->index > g_state->prev)
    {
        /* "down" */
        display_set_cursor(1, 0);
    }
    else
    {
        /* "up" */
        display_set_cursor(0, 0);
    }

    display_write_char(0x7EU);

    id1 = g_state->prev > g_state->index ? g_state->index : g_state->prev;
    id2 = g_state->prev > g_state->index ? g_state->prev : g_state->index;

    /* display labels and extra data */
    /* for faster execution time, work with the pointer of the items */

    display_set_cursor(0U, 1U);                                       /* cursor right after the (possible) arrow */
    display_write_string((g_item + id1)->label);         /* label */
    display_advance_cursor(1U);

    /* [ or space */
    display_write_char(((id1 == g_state->index) && (g_state->state == MENU_SELECTED)) ? '[' : ' ');
    menu_extra_display((g_item + id1)->extra, (g_item + id1)->type);
    /* ] or space */
    display_write_char(((id1 == g_state->index) && (g_state->state == MENU_SELECTED)) ? ']' : ' ');

    display_set_cursor(1U, 1U);                                       /* cursor right after the (possible) arrow */
    display_write_string((g_item + id2)->label);    /* label */
    display_advance_cursor(1U);

    /* [ or space */
    display_write_char(((id2 == g_state->index) && (g_state->state == MENU_SELECTED)) ? '[' : ' ');
    menu_extra_display((g_item + id2)->extra, (g_item + id2)->type);
    /* ] or space */
    display_write_char(((id2 == g_state->index) && (g_state->state == MENU_SELECTED)) ? ']' : ' ');

}

e_menu_output_event menu_event(e_menu_input_event event)
{

    t_menu_item *item;
    e_menu_output_event output_event = MENU_EVENT_OUTPUT_NONE;

    if ((g_state == NULL ) || (g_item == NULL))
    {
    	/* No menu page is selected */
    }
    else
    {

        item = g_item + g_state->index;

        switch(event)
        {
            case MENU_EVENT_NONE:
                /* no event; NOOP */
                break;
            case MENU_EVENT_CLICK:
<<<<<<< HEAD
                if (item->type == MENU_TYPE_GOTO)
                {
                    output_event = MENU_EVENT_OUTPUT_GOTO;
                }
                else if (item->extra != NULL)
=======
                if (item->extra != NULL)
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b
                {
                    /* complex entry (editable) -> toggle selected / unselected */
                    g_state->state = (g_state->state == MENU_SELECTED) ? MENU_NOT_SELECTED : MENU_SELECTED;
                    output_event = (g_state->state == MENU_SELECTED) ? MENU_EVENT_OUTPUT_SELECT : MENU_EVENT_OUTPUT_DESELECT;
                }
<<<<<<< HEAD
=======
                else if (item->type == MENU_TYPE_BACK)
                {
                    output_event = MENU_EVENT_OUTPUT_BACK;
                }
>>>>>>> 6a81c6ec8d587e3005a3bc3852deda94968ef53b
                else
                {
                    /* if a simple entry, do nothing (probably changing menu anyhow) */
                    g_state->state = MENU_NOT_SELECTED;
                    output_event = MENU_EVENT_OUTPUT_CLICK;
                }
                break;
            case MENU_EVENT_CLICK_LONG:
                /* NOOP for the moment */
                output_event = MENU_EVENT_OUTPUT_CLICK_LONG;
                break;
            case MENU_EVENT_LEFT:
                if (g_state->state == MENU_SELECTED)
                {
                    menu_extra_edit(item, false);
                    output_event = MENU_EVENT_OUTPUT_EXTRA_EDIT;
                }
                else
                {
                    menu_index_edit(g_state, g_count, false);
                    output_event = MENU_EVENT_OUTPUT_INDEX_EDIT;
                }
                break;
            case MENU_EVENT_RIGHT:
                if (g_state->state == MENU_SELECTED)
                {
                    menu_extra_edit(item, true);
                    output_event = MENU_EVENT_OUTPUT_EXTRA_EDIT;
                }
                else
                {
                    menu_index_edit(g_state, g_count, true);
                    output_event = MENU_EVENT_OUTPUT_INDEX_EDIT;
                }
                break;
            default:
                /* no event; NOOP */
                break;
        }

        /* Callback for the external world (if any) */
        if ((output_event != MENU_EVENT_OUTPUT_NONE) && (g_menu_event_cb != NULL))
        {
            g_menu_event_cb(output_event, g_state->index, g_page);
        }
    }

    return output_event;
}

void menu_set(t_menu_state *state, t_menu_item *item, uint8_t count, uint8_t page)
{
    g_state = state;
    g_item  = item;
    g_count = count;
    g_page  = page;

    /* init to a known state */
    menu_init(state);
}

void menu_set_diff(uint16_t diff)
{
    if (g_state != NULL)
        g_state->diff = (diff == 0U) ? 1U : diff;
}
