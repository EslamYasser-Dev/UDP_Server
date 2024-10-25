#!/bin/bash

# Function to check the last command's success
check_success() {
    if [ $? -ne 0 ]; then
        echo "Error: $1"
        exit 1
    fi
}

# Update the system
echo "Updating the system..."
sudo apt update && sudo apt upgrade -y
check_success "System update failed."

# Install required packages
echo "Installing required packages..."
sudo apt install -y ffmpeg libsdl2-2.0-0 adb gcc git pkg-config meson ninja-build libavcodec-dev libavformat-dev libavutil-dev libsdl2-dev libswresample-dev libusb-1.0-0 libusb-1.0-0-dev
check_success "Failed to install required packages."

# Download scrcpy server
SCRCPY_VERSION="2.0"
echo "Downloading scrcpy server version $SCRCPY_VERSION..."
wget -c "https://github.com/Genymobile/scrcpy/releases/download/v$SCRCPY_VERSION/scrcpy-server-v$SCRCPY_VERSION" -O scrcpy-server.jar
check_success "Failed to download scrcpy server."

# Move the server file to /usr/local/bin/
echo "Moving scrcpy server to /usr/local/bin..."
sudo mv scrcpy-server.jar /usr/local/bin/
check_success "Failed to move scrcpy server."

# Clone the scrcpy repository
echo "Cloning scrcpy repository..."
git clone https://github.com/Genymobile/scrcpy.git
check_success "Failed to clone scrcpy repository."

# Build scrcpy
echo "Building scrcpy..."
cd scrcpy || { echo "Failed to enter scrcpy directory."; exit 1; }
meson x --buildtype release --strip -Db_lto=true -Dprebuilt_server=../scrcpy-server.jar
check_success "Building scrcpy failed."

ninja -Cx
check_success "Ninja build failed."

# Optional: Install scrcpy globally
echo "Installing scrcpy globally..."
sudo ninja -Cx install
check_success "Global installation of scrcpy failed."

# Clean up
cd ..
rm -rf scrcpy

echo "Installation complete! You can now run scrcpy by typing 'scrcpy' in your terminal."
