#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "global_stuff.h"

void parseEvent(char *eventData, Event *event);
void parseSchedule(char *schedule, Event *events, int *event_count);
void populateEventsFromSchedule(CourseBlock *courses, int courseCount);
void readCSVAndPopulate(const char* filename, CourseBlock** courses, int* count);
int isDayStart(const char *str);
int readQuotedField(FILE *file, char *field, int max_len);
int parse_main(void);

#endif // parsing_module.h
