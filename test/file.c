
#include <vlc/vlc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Initialize VLC instance
    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *mediaPlayer;
    libvlc_media_t *media;
    
    // VLC options for screen capture and RTSP streaming
    const char *vlc_args[] = {
        "--no-audio",  // No audio required
        "--screen-fps=25",  // Frame rate
        "--screen-follow-mouse"  // Follow the mouse
    };
    
    // Initialize the VLC instance
    vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    if (vlcInstance == NULL) {
        fprintf(stderr, "Failed to create VLC instance\n");
        return -1;
    }
    
    // Create a new media with screen capture input and RTSP output
    media = libvlc_media_new_location(vlcInstance, "screen://");
    if (media == NULL) {
        fprintf(stderr, "Failed to create media\n");
        libvlc_release(vlcInstance);
        return -1;
    }
    
    // Configure the RTSP output
    libvlc_media_add_option(media, "sout=#transcode{vcodec=h264,vb=800,fps=25}:rtp{sdp=rtsp://:8554/screen}");

    // Create a media player
    mediaPlayer = libvlc_media_player_new_from_media(media);
    libvlc_media_release(media);
    
    if (mediaPlayer == NULL) {
        fprintf(stderr, "Failed to create media player\n");
        libvlc_release(vlcInstance);
        return -1;
    }
    
    // Start the media player
    if (libvlc_media_player_play(mediaPlayer) < 0) {
        fprintf(stderr, "Failed to start media player\n");
        libvlc_media_player_release(mediaPlayer);
        libvlc_release(vlcInstance);
        return -1;
    }
    
    printf("Streaming on rtsp://<Raspberry_Pi_IP>:8554/screen\n");
    printf("Press Ctrl+C to stop\n");

    // Wait until the user stops the program
    getchar();
    
    // Cleanup
    libvlc_media_player_stop(mediaPlayer);
    libvlc_media_player_release(mediaPlayer);
    libvlc_release(vlcInstance);
    
    return 0;
}
