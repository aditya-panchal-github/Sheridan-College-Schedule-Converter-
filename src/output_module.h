#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "global_stuff.h"

void print_ics_preamble(FILE *icsfilepointer);
void generate_ics_event_to_file(CourseBlock *courses, int courseCount, const char* semesterEndDate, FILE *fp);

#endif //output_module.h
