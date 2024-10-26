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

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        cleanup();
        exit(EXIT_FAILURE);
    }

    printf("Listening for UDP stream on port :: %d...\n", PORT);
    signal(SIGINT, handle_signal);
    // start_ffplay();

    // Loop to receive data
    while (1) {
        char buffer[512] = {0};  
        int len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (len < 0) {
            perror("Receive failed");
            continue; // Continue listening on error
        }

        // Get client IP and port
        char client_IP[INET_ADDRSTRLEN]; // Ensure it's large enough
        inet_ntop(AF_INET, &client_addr.sin_addr, client_IP, sizeof(client_IP));
        int client_port = ntohs(client_addr.sin_port);

        printf("Received %d bytes from %s:%d\n", len, client_IP, client_port);
        printf("Content: %s\n", buffer);
        
        // Prepare a formatted string for check_ffplay
        char client_info[30]; 
        snprintf(client_info, sizeof(client_info), "rtsp://%s:%d", client_IP, 1945);
        if(buffer[1]) check_ffplay(client_info);
    }

    cleanup(); // Cleanup resources before exiting (though this will never be reached)
    return 0;
}
