/************************************************************************************************************************
 * Course: Intro to Computer Programming - ENGR 183228D 
 * Final Project - Schedule Converter
 * Instructor: Prof. Jacob Chadoriwsky
 * Sheridan College - Davis Campus
 * ------------------------------------------------------------------------------------------------------------------------
 * Author(s)
 * Name: Aditya Panchal
 * Name: Suveer Singh Manj
 * Name: Conner Lampitt
 * -----------------------------------------------------------------------------------------------------------------------
 * Date: 2024-04-16
 * Description: This program converts a CSV file containing course schedules into an ICS file format. 
 * The CSV file has the course information for the semester, from the Sheridan myStudentCentre portal, 
 * and the program converts it into the ICS format (RFC 5545), which can be imported into calendar applications.
 * --------------------------------------------------------------------------------------------------------------------
 *
 * Code Structure and source files:
 * ---------------------------------------------------------------------------------------------------------------------
 * main.c
 * parsing_module.h
 * parsing_module.c
 * conversion_module.h
 * conversion_module.c
 * output_module.h
 * output_module.c
 * user_interface.h
 * user_interface.c
 * global_stuff.h
 * global_stuff.c
 * 
 * License: MIT License 
 ************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//header files for the modules
#include "conversion_module.h"
#include "output_module.h"
#include "parsing_module.h"
#include "user_interface.h"
#include "global_stuff.h"

//global variables
char icsPath[MAX_PATH_LENGTH] = {0};
char csvPath[MAX_PATH_LENGTH] = {0};
struct tm semester_start_tm = {0};
struct tm semester_end_tm = {0};
CourseBlock *courses = NULL;
int courseCount = 0;

//function to handle the user input, and call the appropriate function
int main(void) 
{
    int choice;

    //start of the program message
    print_header();

    //main loop for user input, will run until the user exits, or the program crashes lol 
    do 
    {
        print_menu(); //prints the menu options, cleaner and more maintainable
        printf("Enter the number from the menu options, and press Enter: ");
        scanf("%d", &choice); //reads the user input, tried using fgets but it caused issues, but this works fine with clearing using getchar
        handle_scanf_input();  
        handle_menu_choice(choice); //handles the user input, and calls the appropriate function
    } while (1);

    return 0;

}
