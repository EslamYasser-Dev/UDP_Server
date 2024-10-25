#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "config.h"

pid_t ffplay_pid = -1; // To store the ffplay process ID

void start_ffplay() {
    ffplay_pid = fork(); // Create a child process for ffplay
    if (ffplay_pid == 0) {
        // In child process
        execlp("ffplay", "ffplay", RTSP_URL, "-nodisp", "-autoexit", NULL);
        perror("Failed to launch ffplay");
        exit(EXIT_FAILURE); // Exit child process if execlp fails
    } else if (ffplay_pid < 0) {
        perror("Failed to fork for ffplay");
        exit(EXIT_FAILURE);
    }
}

void check_ffplay() {
    int status;
    pid_t result = waitpid(ffplay_pid, &status, WNOHANG); // Non-blocking check

    if (result == ffplay_pid) { // If ffplay has exited
        printf("ffplay has stopped. Restarting...\n");
        start_ffplay(); // Restart ffplay if it has stopped
    } else if (result == -1 && errno != ECHILD) {
        perror("waitpid error");
    }
}
