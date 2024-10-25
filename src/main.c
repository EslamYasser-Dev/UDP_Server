#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "config.h"
#include "ffplay_handler.h"

int sockfd;

void cleanup() {
    if (ffplay_pid > 0) {
        kill(ffplay_pid, SIGTERM); // Terminate ffplay if running
        printf("ffplay process terminated.\n");
    }
    close(sockfd);
    printf("Socket closed.\n");
}

void handle_signal(int signal) {
    printf("Received signal %d, shutting down...\n", signal);
    cleanup();
    exit(0);
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set SO_REUSEADDR option
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Define server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        cleanup();
        exit(EXIT_FAILURE);
    }

    printf("Listening for UDP stream on port %d...\n", PORT);

    // Setup signal handling for graceful shutdown
    signal(SIGINT, handle_signal);

    start_ffplay(); // Start the ffplay process

    // Loop to receive data
    while (1) {
        char buffer[1024] = {0};  // Initialize buffer
        int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);

        if (len < 0) {
            perror("Receive failed");
            continue; // Continue listening on error
        }

        printf("Received %d bytes\n", len);
        
        check_ffplay(); // Check if ffplay is still running and restart if necessary

        // Optionally process the received data here
    }

    cleanup(); // Cleanup resources before exiting (though this will never be reached)
    return 0;
}