# UDP Server with FFmpeg on Raspberry Pi Zero

This project implements a UDP server that listens for incoming data and launches an `ffmpeg` stream using `ffplay`. The server is designed to restart the `ffplay` process automatically if it stops unexpectedly.

## Project Structure

- **src/**: Contains source code files.
- **include/**: Contains header files.
- **Makefile**: Automates the build process.

## Compilation

To compile the project, run:

```bash
make
```
## Requirmnets 
- ffmpeg 6.x

