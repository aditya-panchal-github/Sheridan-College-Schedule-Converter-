#ifndef CONVERT_H
#define CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "global_stuff.h"

void convert_time_to_24h(char *time12h, char *time24h);
void convert_event_times(Event *events, int event_count);
int* weekday_to_offset(const char *start_date_str);
void assign_dates_to_events(CourseBlock *courses, int num_courses, int *offsets);
int day_abbr_to_weekday(const char *abbr);
#endif 
