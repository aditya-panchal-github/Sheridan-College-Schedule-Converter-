#include "output_module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "global_stuff.h"
#include "conversion_module.h"

void print_ics_preamble(FILE *icsfilepointer)
{
    fprintf(icsfilepointer, "BEGIN:VCALENDAR\n");
    fprintf(icsfilepointer, "VERSION:2.0\n");
    fprintf(icsfilepointer, "PRODID:-//Class Schedule//Sheridan College//EN\n");
    fprintf(icsfilepointer, "BEGIN:VTIMEZONE\n");
    fprintf(icsfilepointer, "TZID:America/Toronto\n");
    fprintf(icsfilepointer, "BEGIN:STANDARD\n");
    fprintf(icsfilepointer, "DTSTART:19701101T020000\n");
    fprintf(icsfilepointer, "RRULE:FREQ=YEARLY;BYMONTH=11;BYDAY=1SU\n");
    fprintf(icsfilepointer, "TZOFFSETFROM:-0400\n");
    fprintf(icsfilepointer, "TZOFFSETTO:-0500\n");
    fprintf(icsfilepointer, "TZNAME:EST\n");
    fprintf(icsfilepointer, "END:STANDARD\n");
    fprintf(icsfilepointer, "BEGIN:DAYLIGHT\n");
    fprintf(icsfilepointer, "DTSTART:19700308T020000\n");
    fprintf(icsfilepointer, "RRULE:FREQ=YEARLY;BYMONTH=3;BYDAY=2SU\n");
    fprintf(icsfilepointer, "TZOFFSETFROM:-0500\n");
    fprintf(icsfilepointer, "TZOFFSETTO:-0400\n");
    fprintf(icsfilepointer, "TZNAME:EDT\n");
    fprintf(icsfilepointer, "END:DAYLIGHT\n");
    fprintf(icsfilepointer, "END:VTIMEZONE\n");
}


// Generates the ICS event and writes it to the file
void generate_ics_event_to_file(CourseBlock *courses, int courseCount, const char *semesterEndDate, FILE *icsfilepointer)
{
    print_ics_preamble(icsfilepointer); 

    for (int i = 0; i < courseCount; i++)
    {
        for (int j = 0; j < courses[i].event_count; j++)
        {
            Event *event = &courses[i].events[j];
            fprintf(icsfilepointer, "BEGIN:VEVENT\n");
            fprintf(icsfilepointer, "UID:%d-%d@example.com\n", i, j);
            fprintf(icsfilepointer, "DTSTAMP:%sT%sZ\n", event->converted_event_dates, event->ics_start_time);
            fprintf(icsfilepointer, "DTSTART;TZID=America/Toronto:%sT%sZ\n", event->converted_event_dates, event->ics_start_time);
            fprintf(icsfilepointer, "DTEND;TZID=America/Toronto:%sT%sZ\n", event->converted_event_dates, event->ics_end_time);
            fprintf(icsfilepointer, "SUMMARY:%s\n", courses[i].course_info);
            fprintf(icsfilepointer, "LOCATION:%s\n", event->location);
            fprintf(icsfilepointer, "RRULE:FREQ=WEEKLY;UNTIL=%sT%sZ\n", semesterEndDate, event->ics_end_time);
            fprintf(icsfilepointer, "END:VEVENT\n");
        }
    }
    fprintf(icsfilepointer, "END:VCALENDAR\n");
}
