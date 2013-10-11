/**
 * drivers/keyboard.c
 *
 * Very simple incomplete keyboard driver.
 * Merely a PoC that it can be done.
 *
 * Author: Dennis J. McWherter, Jr.
 * MIT Lincoln Laboratory
 */

#ifndef DRIVERS_KEYBOARD_H__
#define DRIVERS_KEYBOARD_H__

/**
 * Initialize the keyboard driver
 *
 * @return  1 on success 0 otherwise.
 */
int keyboard_init();

/**
 * Enable the keyboard
 */
void keyboard_enable();

/**
 * Disable the keyboard
 */
void keyboard_disable();

#endif /** DRIVERS_KEYBOARD_H__ */

