/* Joseph Norkin
 * CIS 3207: Introduction to Systems Programming and Operating Systems
 * Prof: Dr. Eugene Kwatny
 *
 * Timer.c runs the executable, "app", twice. "app" runs for about 2 minutes
 * by itself. Program waits 30 seconds before executing "app" for the second
 * time. End result is to have two distinct processes of "app" executing 
 * simulaneously. 
 * Program measures the time (in microseconds) it takes for "app" to begin
 * executing with respect to the time it was called. Calculations are sent
 * through stdout of "app".
 */

#include <sys/time.h>           /* for stuct timeval and gettimeofday() */
#include <unistd.h>
#include <stdio.h>

#define T_START_LEN 7

int main() {
    pid_t childPID;
    struct timeval time_start;
    char t_start[T_START_LEN];

    printf("in timer about to fork.\n");
    if (gettimeofday(&time_start, NULL) != 0) {
        printf("Error getting time.\n");
        return 1;
    }
    printf("TIME START: %d microseconds\n", (int)time_start.tv_usec);
    childPID = fork();
    if (childPID < 0) {
        printf("fork failed\n");
        return 1;
    }
    if (childPID == 0) {
        printf("\n\tin child about to execute application\n\n");
        snprintf(t_start, T_START_LEN, "%d", (int)time_start.tv_usec);
        if (execl("./app", "app", t_start, "1", NULL) == -1) {
            printf("Execution of app failed.\n");
            return 1;
        }
    }

    /* to execute app twice, include code between these comments */
    sleep(30);
    if (gettimeofday(&time_start, NULL) != 0) {
        printf("Error getting time.\n");
        return 1;
    }
    printf("2nd time... TIME START: %d microseconds\n", (int)time_start.tv_usec);
    childPID = fork();
    if (childPID < 0) {
        printf("2nd fork failed\n");
        return 1;
    }
    if (childPID == 0) {
        printf("\n\tin 2nd child about to execute 2nd application\n\n");
        snprintf(t_start, T_START_LEN, "%d", (int)time_start.tv_usec);
        if (execl("./app", "app", t_start, "2", NULL) == -1) {
            printf("Execution of 2nd app failed.\n");
            return 1;
        }
    }
    /* to execute app twice, include code between these comments */

    sleep(1);
    return 0;
}
