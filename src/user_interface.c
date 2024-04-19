#include "user_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "conversion_module.h"
#include "output_module.h"
#include "parsing_module.h"
#include "global_stuff.h"

char semester_start_date[MAX_LINE_LENGTH];
char semester_end_date[MAX_LINE_LENGTH];

void print_menu(void) 
{
    printf("\n--------------------------------------------------\n");
    printf("      Sheridan Schedule Converter Menu\n");
    printf("--------------------------------------------------\n");
    printf("1. Set the start date of the semester\n");
    printf("2. Set the end date of the semester\n");
    printf("3. Input the location of the CSV you want to convert\n");
    printf("4. Set the location of the converted file\n");
    printf("5. Convert my schedule!\n");
    printf("6. Exit\n");
    printf("--------------------------------------------------\n");
    printf("Please select an option: ");
    printf("-----------------------------------------------\n");
}

void print_header(void) 
{
    printf("\n");
    printf("  ────────────────────────────────────────────────────────────\n");
    printf("                 Sheridan College - Schedule Converter        \n");
    printf("  Convert your class schedules into universal .ICS format easily.\n");
    printf("  ────────────────────────────────────────────────────────────\n");
    printf("                  Course: Computer Programming                \n");
    printf("  ────────────────────────────────────────────────────────────\n");
    printf("  Instructor: Professor Jacob Smith                           \n");
    printf("  Authors: Jane Doe, John Smith, Alice Johnson                \n");
    printf("  ────────────────────────────────────────────────────────────\n");
    printf("  This software is part of the final project for the Computer \n");
    printf("  Programming course, designed to help students and faculty   \n");
    printf("  manage and convert their class schedules effectively.       \n");
    printf("  ────────────────────────────────────────────────────────────\n");
    printf("\n");
}

void handle_scanf_input(void) 
{
    int x;
    while ((x = getchar()) != '\n' && x != EOF) {} // Trims the input from the user
}

// Function to validate the format of the date string "YYYY/MM/DD"
int is_valid_date_format(const char *date) 
{

    if (strlen(date) != 10) return 0;
    if (date[4] != '/' || date[7] != '/') return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue; // Skip the '/' characters
        if (!isdigit(date[i])) return 0;
    }

    return 1; 
}

// Function to get a valid date from the user and store it in the provided variable
void get_input_date(char *date_storage) 
{
    char date[11];  // Temporary buffer to store input
    printf("Enter the date (YYYY/MM/DD):\n");
    scanf("%10s", date);  // Read exactly 10 characters
    getchar();

    // Validate the format of the date
    while (!is_valid_date_format(date)) {
        printf("Invalid date format. Please enter the date in the format YYYY/MM/DD:\n");
        scanf("%10s", date);
        getchar();
    }

    strcpy(date_storage, date);  // Copy the validated date into the provided storage
    printf("Date set to: %s\n", date_storage);  // Confirm the set date
}

void handle_input_file_path(void) 
{
    printf("Enter the full path for the CSV file you want to use:\n");
    scanf("%1023s", csvPath);  // Directly modify the global variable
    getchar();

    // Check if the path ends with .csv
    char *dot = strrchr(csvPath, '.');
    if (dot == NULL || strcmp(dot, ".csv") != 0) {
        printf("Invalid file. Please ensure the file is a .csv file.\n");
        return;  // exit if no .csv extension
    }

    // Try to open the file to see if it exists
    FILE *file = fopen(csvPath, "r");  // open for reading
    if (file) {
        printf("CSV file opened successfully: %s\n", csvPath);
        fclose(file);
    } else {
        printf("Failed to open the file. Check if the file exists and you have permission to read it.\n");
    }
}

void handle_output_file_path(void) 
{
    printf("Enter the full path for the folder where you want your converted schedule:\n");
    fgets(icsPath, MAX_PATH_LENGTH, stdin);  // Read user input

    // Remove newline character from the end of the path
    icsPath[strcspn(icsPath, "\n")] = 0;

    // make sure path ends with a slash, needed to add files to it
    if (icsPath[strlen(icsPath) - 1] != '/') {
        strcat(icsPath, "/");  // append a slash if not there
    }

    strcat(icsPath, "Sheridan Course Schedule.ics");  // append the filename to the path

    // Trying to create the file in the specified directory
    FILE *file = fopen(icsPath, "w");  // opens file for writing, creates if not exist
    if (file) {
        printf("File successfully created at %s\n", icsPath);
        fclose(file);
    } else {
        printf("Failed to create the file. Check the path and permissions.\n");
    }
}

//the main function that handles the user input and calls the appropriate function(s)
void handle_menu_choice(int choice) 
{
    switch (choice) 
    {
        case 1:
            // Get the start date of the semester, used for multiple calculations
            printf("Setting the start date of the semester.\n");
            get_input_date(semester_start_date);
            printf("Start date set to: %s\n", semester_start_date);
            break;

        case 2:
            // Get the end date of the semester, used for calculating end of RRULE in ICS
            printf("Setting the end date of the semester.\n");
            get_input_date(semester_end_date);
            printf("End date set to: %s\n", semester_end_date);
            break;

        case 3:
            // Input the location of the CSV to convert
            printf("Input the location of the CSV you want to convert.\n");
            handle_input_file_path();
            printf("CSV file path set to: %s\n", csvPath);
            break;

        case 4:
            // Set the location of the converted file
            printf("Set the location of the converted ICS file.\n");
            handle_output_file_path();
            printf("ICS file path set to: %s\n", icsPath);
            break;

        //parse the csv file, convert the days to calendar dates, print the ICS formatted times for each event, and write the ICS data to a file    
        case 5:
            if (strlen(csvPath) == 0) {
                printf("Please set the CSV file path first.\n");
            } else {
                printf("Parsing CSV file...\n");
                parse_main();
            }
            printf("Converting days to calendar dates...\n");
            if (courseCount == 0) {
                printf("No courses available. Please load courses first.\n");
            } else {
                int *weekday_offsets = weekday_to_offset(semester_start_date);
                if (weekday_offsets == NULL) {
                    printf("Failed to calculate weekday offsets. Please try again.\n");
                } else {
                    assign_dates_to_events(courses, courseCount, weekday_offsets);
                }
            }
            printf("Printing ICS formatted times for each event...\n");
            if (courseCount == 0) {
                printf("No courses loaded. Please load courses first.\n");
            } else {
                for (int i = 0; i < courseCount; i++) {
                    printf("Course: %s\n", courses[i].course_info);
                    convert_event_times(courses[i].events, courses[i].event_count);
                    for (int j = 0; j < courses[i].event_count; j++) {
                        printf("  Event %d: Start Time: %s, End Time: %s\n",
                               j + 1, courses[i].events[j].ics_start_time, courses[i].events[j].ics_end_time);
                    }
                }
            }
            printf("Writing ICS data to file...\n");
            if (courseCount == 0) {
                printf("No courses loaded. Please load courses first.\n");
            } else {
                // Define the output file path
                char *filepath = icsPath;
                // Open the file for writing
                FILE *fp = fopen(filepath, "w");
                if (fp == NULL) {
                    perror("Error opening file");
                } else {
                    // Call the function to write data to the file
                    generate_ics_event_to_file(courses, courseCount, semester_end_date, fp);
                    fclose(fp); // Close the file after writing
                    printf("ICS data successfully written to %s\n", filepath);
                }
            }
            break;
        
        //Exit the program
        case 6:
            printf("Exiting program.\n");
            exit(0);
        
        //for the default case I think it's best to assume the input was invalid and to prompt the user to try again
        default:
            printf("Invalid option. Please try again.\n");
            break;

    }

}
