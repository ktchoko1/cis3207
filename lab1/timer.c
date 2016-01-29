/* Joseph Norkin
 * CIS 3207: Introduction to Systems Programming and Operating Systems
 * Prof: Dr. Eugene Kwatny
 *
 * Timer.c runs the executable, "app", twice. "app" runs for about 2 minutes
 * by itself. Program waits 30 seconds before executing "app" for the second
 * time. End result is to have two distinct processes of "app" executing 
 * simulaneously. 
 * 
 * Program measures the time (in microseconds) it takes for "app" to begin
 * executing with respect to the time it was called. Calculations are sent
 * through stdout of "app".
 *
 * time difference is measured twice. The first measurement is started right
 * before the calling of fork(). The second measurement is starting in the child
 * process resulting from the calling of fork()
 * 
 * Results vary depending on how long timer.c sleeps after 1st fork/exec. Program
 * can be used to check general trends of how long it takes to fork/execute,
 * (t3 - t1), or to just execute (t3 - t2).
 */

#include <sys/time.h>           /* for stuct timeval and gettimeofday() */
#include <unistd.h>
#include <stdio.h>

#define TIME_BUFFER 7

int main() {
    pid_t childPID;
    struct timeval t1;              /* time from before fork */
    struct timeval t2;              /* time from child process created by fork */
    char t_start1[TIME_BUFFER];     /* t1 as an argument for app */
    char t_start2[TIME_BUFFER];     /* t2 an an argument for app */

    printf("1st time:\n");
    
    if (gettimeofday(&t1, NULL) != 0) {
        printf("Error getting time for t1.\n");
        return 1;
    }

    printf("t1 = %d microseconds\n", (int)t1.tv_usec);

    if ((childPID = fork()) < 0) {
        printf("fork failed\n");
        return 1;
    }

    if (childPID == 0) {
        if (gettimeofday(&t2, NULL) != 0) {
            printf("Error getting time for t2.\n");
            return 1;
        }

        printf("t2 = %d microseconds\n", (int)t2.tv_usec);

        snprintf(t_start1, TIME_BUFFER, "%d", (int)t1.tv_usec);
        snprintf(t_start2, TIME_BUFFER, "%d", (int)t2.tv_usec);

        if (execl("./app", "app", t_start1, t_start2, "1", NULL) == -1) {
            printf("Execution of app failed.\n");
            return 1;
        }
    }

    /* to execute app twice, include code between these comments */
    sleep(30);

    printf("\n2nd time:\n");

    if (gettimeofday(&t1, NULL) != 0) {
        printf("Error getting time.\n");
        return 1;
    }

    printf("2nd t1 = %d microseconds\n", (int)t1.tv_usec);
    
    if ((childPID = fork()) < 0) {
        printf("2nd fork failed\n");
        return 1;
    }

    if (childPID == 0) {
        if (gettimeofday(&t2, NULL) != 0) {
            printf("Error getting time for t2.\n");
            return 1;
        }

        printf("2nd t2 = %d microseconds\n", (int)t2.tv_usec);

        snprintf(t_start1, TIME_BUFFER, "%d", (int)t1.tv_usec);
        snprintf(t_start2, TIME_BUFFER, "%d", (int)t2.tv_usec);

        if (execl("./app", "app", t_start1, t_start2, "2", NULL) == -1) {
            printf("Execution of 2nd app failed.\n");
            return 1;
        }
    }
    /* to execute app twice, include code between these comments */

    sleep(1);
    return 0;
}
