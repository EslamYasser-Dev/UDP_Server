#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "config.h"

pid_t ffplay_pid = -1;
void start_ffplay(char* url) {
    ffplay_pid = fork();
    if (ffplay_pid == 0) {
        if (url == NULL) {
            url = RTSP_URL; 
        }    
        execlp("ffplay", "ffplay", url, "-nodisp", "-autoexit", NULL);
        perror("Failed to launch ffplay");
        exit(EXIT_FAILURE); 
          start_ffplay(url);// Exit child process if execlp fails
    } else if (ffplay_pid < 0) {
        perror("Failed to fork for ffplay");
        exit(EXIT_FAILURE);
    }
}

void check_ffplay(char* url) {
    int status;
    pid_t result = waitpid(ffplay_pid, &status, WNOHANG); // Non-blocking check
    if (result == ffplay_pid) {
        printf("ffplay has stopped. Restarting...\n");
        start_ffplay(url);
    } else if (result == -1 && errno != ECHILD) {
        perror("waitpid error");
              

    }
}
