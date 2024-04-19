//this is the header file for user_interface.c
#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "global_stuff.h"

void print_menu(void);
void print_header(void);
void handle_scanf_input(void);
void handle_menu_choice(int choice);
int is_valid_date_format(const char *date);
void get_input_date(char *date_storage);
void handle_input_file_path(void);
void handle_output_file_path(void);

#endif // user_interface.h
