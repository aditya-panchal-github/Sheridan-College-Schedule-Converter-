
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "global_stuff.h"
#include "conversion_module.h"
#include "output_module.h"
#include "parsing_module.h"
#include "user_interface.h"

int day_abbr_to_weekday(const char *abbr) {
    const char *days[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(abbr, days[i]) == 0) return i;
    }
    return -1;
}

// Function to convert day abbreviation to weekday index
int* weekday_to_offset(const char *start_date_str) 
{
    struct tm start_date = {0};
    strptime(semester_start_date, "%Y/%m/%d", &start_date);
    mktime(&start_date);  // Normalize the tm structure

    int* offsets = malloc(7 * sizeof(int));  // Allocate memory for offsets
    int start_wday = start_date.tm_wday;  // Weekday of the semester start (0 = Sunday, ..., 6 = Saturday)

    // Calculate offsets for each weekday from the start day
    for (int i = 0; i < 7; i++) {
        offsets[i] = (i - start_wday + 7) % 7;
        if (offsets[i] == 0) offsets[i] = 7;  // If same day, schedule for next week
    }

    return offsets;
}

void assign_dates_to_events(CourseBlock *courses, int num_courses, int *offsets) {
    struct tm start_date = {0};
    strptime(semester_start_date, "%Y/%m/%d", &start_date);
    mktime(&start_date);  // Normalize start date

    for (int i = 0; i < num_courses; i++) {
        CourseBlock *course = &courses[i];
        for (int j = 0; j < course->event_count; j++) {
            Event *event = &course->events[j];
            struct tm event_date = start_date;
            int day_index = day_abbr_to_weekday(event->day);  // Convert day abbreviation to index

            // Add the correct number of days to the start date
            event_date.tm_mday += offsets[day_index];
            mktime(&event_date);  // Normalize the date

            // Format and store the date back in the event
            strftime(event->converted_event_dates, sizeof(event->converted_event_dates), "%Y%m%d", &event_date);

            // Print the conversion process
            printf("Converted %s to %s for event %d of course %d\n", event->day, event->converted_event_dates, j+1, i+1);
        }
    }

    free(offsets);  // Free the memory allocated for offsets
}

// Function to convert 12-hour AM/PM format to 24-hour time
void convert_time_to_24h(char *time12h, char *time24h) {
    int hours, minutes;
    char am_pm[3];
    sscanf(time12h, "%d:%d%s", &hours, &minutes, am_pm);

    if (strcmp(am_pm, "PM") == 0 && hours != 12) {
        hours += 12;
    } else if (strcmp(am_pm, "AM") == 0 && hours == 12) {
        hours = 0;
    }

    sprintf(time24h, "%02d%02d00", hours, minutes);  // Format as HHMMSS
}

// Main conversion function for event times
void convert_event_times(Event *events, int event_count) {
    for (int i = 0; i < event_count; i++) {
        char start_time[10], end_time[10];
        char start_24h[10], end_24h[10];

        // Split the time string into start and end times
        sscanf(events[i].time, "%s - %s", start_time, end_time);

        // Convert times to 24-hour format
        convert_time_to_24h(start_time, start_24h);
        convert_time_to_24h(end_time, end_24h);

        // Format for ICS (assuming UTC for simplicity)
        sprintf(events[i].ics_start_time, "%s", start_24h);
        sprintf(events[i].ics_end_time, "%s", end_24h);

        // Print the results for verification
        printf("Event %d: Start Time: %s, End Time: %s\n",
               i + 1, events[i].ics_start_time, events[i].ics_end_time);
    }
}
