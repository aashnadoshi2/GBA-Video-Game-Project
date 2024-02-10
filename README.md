# GBA Graphical Application Project

## Overview

This project is a Game Boy Advance (GBA) graphical application developed. It's designed to run on a GBA emulator and demonstrates the use of Direct Memory Access (DMA), Mode 3 video, button input, and collision detection among other GBA functionalities.

## Features

- **Start and Win Screens:** Utilizes two full-screen DMA images to display the start and win screens.
- **Movement and Collision:** Implements 2-dimensional movement using the GBA's button inputs and collision detection between objects on the screen.
- **Text Display:** Shows on-screen text to display information like timers and scores.
- **Efficient Graphics Handling:** Ensures there's no screen tearing by adhering to efficient graphics drawing practices.

## How to Build and Run

1. **Setup:** Ensure you have a GBA emulator and the necessary development tools installed on your system.
2. **Compilation:** Use the provided Makefile to compile the project. Run `make` in the terminal within the project directory.
3. **Running:** Load the compiled ROM file into your GBA emulator to start the application.

## Controls

- **Movement:** Use the arrow keys to move the player or navigate through the application.
- **Reset:** Press the "Select" button (mapped to the Backspace key on most emulators) to reset the application to the title screen.
- **Other Actions:** Specific buttons (e.g., "Start", "A", "B") are used for various in-game actions, such as advancing screens or confirming selections.
