/* Joseph Norkin
 * CIS 3207: Introduction to Systems Programming and Operating Systems
 * Prof: Dr. Eugene Kwatny
 *
 * application.c is a loop with the following task for each iteration:
 *
 *    writes REC_SEC strings of REC_LEN random characters (ascii 32 - 126)
 *    in a text file with each string on its own line. A random line is retreived
 *    for an arbitrary comparison of quadratic complexity.
 *
 * Text file is deleted before program ends.
 * 
 * Program can take 1 or 2 (optional) input arguments:
 *   
 *    1. time (in microseconds), in the range (0 - 999999) 
 *    2. an int to append to the name of the text file
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>           /* for stuct timeval and gettimeofday() */
#include <unistd.h>
#include <stdio.h>

#define REC_LEN          120
#define NUM_OF_ITER  2097152  // 2097152 = 2^21, will last about 2 minutes?
#define REC_SEQ           10
#define FILE_NAME_LEN     34

char *rand_chars(int);
char *read_rand_line(int, FILE *);
int compare(char *, FILE *);

int main(int argc, char **argv) {
    FILE *fp;
    struct timeval time_end;
    char *record;       /* string to be written to file */
    char *line_read;    /* line chosen for comparison */
    char filename[FILE_NAME_LEN];
    int time_start;
    int app_num = 0;
    int i, j;
    
    if (gettimeofday(&time_end, NULL) != 0) {
        printf("Error getting time.\n");
        return 1;
    } 
    if (argc > 1) {
        time_start = atoi(argv[1]);
        if (argc > 2)
            app_num = atoi(argv[2]);
        printf("\n(I am in the application, #%d)\n\n", app_num);
        printf("time_start: %d microsec, time end: %d microsec\ndiff: %d microseconds\n", 
               time_start, (int)time_end.tv_usec, (int)time_end.tv_usec - time_start);
    }

    srand(time(NULL));

    snprintf(filename, FILE_NAME_LEN, "created_file_for_application%d.txt", app_num);
    fp = fopen(filename, "w+");

    for (i=0; i<NUM_OF_ITER; i++) {
        rewind(fp);
        for (j=0; j<REC_SEQ; j++) {
            record = rand_chars(REC_LEN);
            fwrite(record, sizeof(char), REC_LEN, fp);
            free(record);
            if (j != REC_SEQ-1)
                fputc('\n', fp);
        }

        line_read = read_rand_line(REC_SEQ, fp);
        compare(line_read, fp);
        free(line_read);
    }
    fclose(fp);
    remove(filename);
    return 0;
}

/* returns pointer to a string of length len consisting 
   of random characters of ascii codes 32 - 126
  
   returns NULL if error occurs 

   user is responible for calling free() */
char *rand_chars(int len) {
    char *r, *rp;           /* r is a pointer to returned string */
    int i, c;

    if (( r = malloc((len+1)*sizeof(char)) ) == NULL)
        return NULL;

    rp = r;

    for (i=0; i<len; i++, rp++) {
        c = rand() % 95 + 32;
        *rp = (char) c;
    }
    *rp = '\0';

    return r;
}

/* returns pointer to randomly selected line of a text file pointed by fp,
   which can point anywhere on the file.

   lines specifies how many lines are in the file

   function ends with fp point at beginning of random line */
char *read_rand_line(int lines, FILE *fp) {
    int random, i=0;
    char *r, *rp;                       /* r is a pointer to returned string */
    int c;
    int len = 0;                        /* length of random line */ 
    int fp_not_pointing_to_line;
    long fp_pos;                        /* fp's position */

    random = rand() % lines; /* random number between 0 and lines-1 (inclusive) */
    rewind(fp);
    fp_not_pointing_to_line = (random > 0) ? 1 : 0;

    fflush(fp);
    fp_pos = ftell(fp);

    if (fp_not_pointing_to_line) {
    /* get fp pointing to chosen line */
        while(i < random) {
            while(fgetc(fp) != '\n');
            i++;
        }
    }

    fflush(fp);
    fp_pos = ftell(fp);

    while( (c = fgetc(fp)) != EOF && c != '\n')    /* get length of line */
        len++;
    
    if (( r = malloc((len+1)*sizeof(char)) ) == NULL)
        return NULL;

    rp = r;
    fseek(fp, fp_pos, SEEK_SET);
    while((( c = fgetc(fp) ) != EOF) && (c != '\n')) {
        *rp = c;
        rp++;
    }
    *rp = '\0';

    fseek(fp, fp_pos, SEEK_SET);

    return r;
}

/* returns an integer based on an exagerated count of repeated
   charactes for the string s and the line in the file pointed 
   by fp 

   one can view this function as an arbitrary comparision with
   quadratic complexity */
int compare(char *s, FILE *fp) {
    int c;
    char *sp;
    int count = 0;
    while ( (c = fgetc(fp)) != '\n' && c != EOF) {
        sp = s;
        while(*sp != '\0') {
            if (*sp == c)
                count++;
            sp++;
        }
    }

    return count;
}
