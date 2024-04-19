
#include "parsing_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "global_stuff.h"

//had to make this seperate function to read the quoted fields
int readQuotedField(FILE *file, char *field, int max_len) {
    int ch, i = 0;
    int in_quotes = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '"') {
            in_quotes = !in_quotes;
            if (!in_quotes) break;
            continue;
        }
        if (in_quotes && i < max_len - 1) field[i++] = (char)ch;
    }
    field[i] = '\0';
    return in_quotes == 0;
}

//checks if the string is the start of a day might remove this later
int isDayStart(const char *str) {
    static const char *days[] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su", NULL};
    for (int i = 0; days[i] != NULL; i++) {
        if (strncmp(str, days[i], 2) == 0) return 1;
    }
    return 0;
}

//parses the schedule and calls the parseEvent function
void parseSchedule(char *schedule, Event *events, int *event_count) {
    char *scheduleCopy = strdup(schedule);
    if (!scheduleCopy) {
        perror("Failed to allocate memory for schedule copy");
        return;
    }

    printf("original schedule: %s\n", scheduleCopy);

    char *current = scheduleCopy;
    while (*current && *event_count < MAX_EVENTS_PER_COURSE) {
        while (isspace(*current)) current++;  // Skip whitespace

        if (!isDayStart(current)) {
            current++;  // Continue if it's not the start of an event
            continue;
        }

        char *next = strchr(current, '\n');
        while (next && !isDayStart(next + 1)) {
            char *temp = strchr(next + 1, '\n');
            if (!temp) break;
            next = temp;
        }

        if (next) *next = '\0';

        printf("Processing event: \n%s\n", current);

        parseEvent(current, &events[*event_count]);
        (*event_count)++;
        current = next ? next + 1 : NULL;
    }

    free(scheduleCopy); 
}

//populates the events from the schedule
void populateEventsFromSchedule(CourseBlock *courses, int courseCount) {
    for (int i = 0; i < courseCount; i++) {
        parseSchedule(courses[i].course_schedule, courses[i].events, &courses[i].event_count);
    }
}

//parses the event and extracts the day, time, and location
void parseEvent(char *eventData, Event *event) 
{
    int amCount = 0, pmCount = 0;
    char *ptr = eventData; // Pointer to iterate through eventData

    // Extract the day
    sscanf(eventData, "%s", event->day); // Assumming day is the first word

    while (*ptr && !isspace(*ptr)) ptr++; // Skip the day part
    while (*ptr && isspace(*ptr)) ptr++; // Skip any spaces after the day
    char *timeStart = ptr; // Start of the time
    char *lastAM = NULL, *lastPM = NULL;

    while (*ptr) {
        if (strncmp(ptr, "AM", 2) == 0) {
            amCount++;
            lastAM = ptr + 2; // Move past "AM"
        } else if (strncmp(ptr, "PM", 2) == 0) {
            pmCount++;
            lastPM = ptr + 2; // Move past "PM"
        }
        ptr++;
    }

    // Check for errors 
    if (amCount + pmCount != 2) {
        printf("Error: Incorrect number of AM/PM occurrences. Found AM: %d, PM: %d\n", amCount, pmCount);
        printf("Current parse: %s\n", eventData);
        return;
    }

    // Determine end of time part based on the last occurrence of AM/PM
    char *timeEnd = (lastAM > lastPM) ? lastAM : lastPM;

    // Copy the time into the event structure
    int timeLength = timeEnd - timeStart;
    if (timeLength > 0 && timeLength < sizeof(event->time)) {
        strncpy(event->time, timeStart, timeLength);
        event->time[timeLength] = '\0'; // Ensure null-termination
    } else {
        printf("Error: Time string is too long or empty.\n");
        return;
    }

    // The rest of the string is location
    while (*timeEnd && isspace(*timeEnd)) timeEnd++; // this skips the spaces just after the time
    strncpy(event->location, timeEnd, sizeof(event->location) - 1);
    event->location[sizeof(event->location) - 1] = '\0'; 
}

//reads the csv file and populates the course info and schedule
void readCSVAndPopulate(const char* filename, CourseBlock** courses, int* count) {
    printf("Reading CSV file: %s\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    *courses = malloc(sizeof(CourseBlock) * 100); 
    if (!*courses) {
        fclose(file);
        return;
    }
    //doesn't work globally NOTE TO SELF figure out why
    char course_info[MAX_COURSE_INFO];
    char schedule[MAX_SCHEDULE_LENGTH];
    int courseCount = 0;
    while (!feof(file)) {
        if (readQuotedField(file, course_info, sizeof(course_info)) && 
            fgetc(file) == ',' && // checks if theres a comma after the course info
            readQuotedField(file, schedule, sizeof(schedule))) {
            strcpy((*courses)[courseCount].course_info, course_info);
            strcpy((*courses)[courseCount].course_schedule, schedule);
            courseCount++;
        }
    }

    fclose(file);
    *count = courseCount;
}

//main 
int parse_main(void) {
    // Populate course data from CSV
    readCSVAndPopulate(csvPath, &courses, &courseCount);

    // Populate events from schedule
    populateEventsFromSchedule(courses, courseCount);

    // Print final results
    for (int i = 0; i < courseCount; i++) {
        printf("Course: %s\n", courses[i].course_info);
        printf("Full Schedule: %s\n", courses[i].course_schedule);
        for (int j = 0; j < courses[i].event_count; j++) {
            printf("  Event %d: Day: %s, Time: %s, Location: %s\n",
                   j + 1, courses[i].events[j].day, courses[i].events[j].time, courses[i].events[j].location);
        }
        printf("\n");
    }

    free(courses);
    return 0;
}
