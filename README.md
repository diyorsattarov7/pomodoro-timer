# pomodoro-timer

A simple Qt6-based Pomodoro timer application to help you stay focused and productive.

## Features

- **Work/Break Sessions**: Default 25-minute work sessions followed by 5-minute breaks
- **Customizable Durations**: Adjust work and break times through the settings dialog
- **Visual Status Indicators**: Clear color-coded labels for work sessions (red) and break time (green)
- **Timer Controls**: Start, pause, and reset functionality
- **Clean Interface**: Minimalist design with large, easy-to-read timer display

## Building

### Requirements

- Qt6 (Core and Widgets modules)
- CMake 3.16 or higher
- C++ compiler with C++17 support

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

1. Launch the application
2. Click "Start" to begin a work session
3. Use "Pause" to temporarily stop the timer
4. Click "Reset" to restart the current session
5. Access settings via the gear button (⚙) to customize work and break durations
