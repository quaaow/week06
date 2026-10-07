#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    clock_t start, end;
    double time_taken;

    pid1 = fork();
    start = clock();

    if (pid1 == 0) {
        end = clock();
        time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        printf("Child 1: PID = %d, PPID = %d, Execution time = %f ms\n", getpid(), getppid(), time_taken);
        return 0;
    }

    pid2 = fork();
    if (pid2 == 0) {
        start = clock();
        end = clock();
        time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
        printf("Child 2: PID = %d, PPID = %d, Execution time = %f ms\n", getpid(), getppid(), time_taken);
        return 0;
    }

    wait(NULL);
    wait(NULL);

    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    printf("Main: PID = %d, PPID = %d, Execution time = %f ms\n", getpid(), getppid(), time_taken);

    return 0;
}