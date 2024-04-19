#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include <stdio.h>
#include <time.h>

// some of these might redundant but due to time constraints wasnt able to clean them up
// i will clean them up later, plus might be future proofing 
#define MAX_LINE_LENGTH 75
#define MAX_COURSE_INFO 200
#define MAX_SCHEDULE_INFO 1024
#define MAX_EVENTS_PER_COURSE 10
#define MAX_SCHEDULE_LENGTH 1024
#define MAX_PATH_LENGTH 1024

// Structure to hold event information, within a course block
typedef struct 
{
    char day[20];
    char time[40];
    char location[200];
    char* course_info;
    char converted_event_dates[20];
    char ics_start_time[20];
    char ics_end_time[20];
} Event;

// Structure to hold course information and event count
typedef struct 
{
    char course_info[200];
    char course_schedule[1024];
    Event events[10];
    int event_count;
} CourseBlock;

// Global variables
extern char csvPath[MAX_PATH_LENGTH];
extern char icsPath[MAX_PATH_LENGTH];

extern char semester_start_date[MAX_LINE_LENGTH];
extern char semester_end_date[MAX_LINE_LENGTH];

extern struct tm semester_end_tm;
extern struct tm semester_start_tm;

extern CourseBlock *courses;
extern int courseCount;

#endif // global_stuff.h
