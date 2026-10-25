# Pomodoro Application (C++ OOP & IPC)

[![University](https://img.shields.io/badge/University-University%20of%20Ioannina-blue.svg)](https://www.uoi.gr)
[![Department](https://img.shields.io/badge/Department-Informatics%20%26%20Telecommunications-lightgrey.svg)](https://www.dit.uoi.gr/)
[![Language](https://img.shields.io/badge/Language-C++-blueviolet.svg)]()
[![Library](https://img.shields.io/badge/Library-ncurses-orange.svg)]()

A robust terminal-based Pomodoro timer application written in C++. This project demonstrates advanced Object-Oriented Programming (OOP) principles alongside POSIX Inter-Process Communication (IPC) mechanisms. Developed as a semester project for the Object-Oriented Programming course at the Department of Computer Science and Telecommunications, University of Ioannina.

---

## 📚 Table of Contents
1. [Overview](#-overview)
2. [Theoretical Background & Architecture](#-theoretical-background--architecture)
3. [Project Structure](#-project-structure)
4. [Testing & Validation](#-testing--validation)
5. [Getting Started & Execution](#-getting-started--execution)
6. [Author & Credits](#-author--academic-use)

## 🔍 Overview
The application simulates the Pomodoro technique, allowing users to create, start, pause, and resume focus sessions. It features a custom Text-based User Interface (TUI) built with 
curses that provides real-time timer updates, configuration menus for work/break durations, and session statistics.

## 📐 Theoretical Background & Architecture

### Object-Oriented Design
The core logic is encapsulated within the Pomodoro class. Each focus session is instantiated as an object, utilizing:
- **Encapsulation:** Strict separation of data (work/break durations, elapsed times) and behaviors.
- **Static Members:** Class-level attributes track global statistics (total completed sessions, overall work time) across multiple object lifecycles.
- **State Machine:** Sessions transition gracefully between multiple states (\HOLD\, \STARTED\, \PAUSED\, \RESUMED\, \END\).

### Multi-processing & Inter-Process Communication (IPC)
To ensure the UI remains responsive while the timer runs, the application splits into two concurrent processes using \ork()\:
- **Parent Process (Interface):** Handles user input via \
curses\ and sends commands.
- **Child Process (Timer Engine):** Manages the \Pomodoro\ object lifecycle and time calculations.
- **Communication:** Processes exchange data (timer state, defaults, statistics) via **Shared Memory** (\shmget\, \shmat\) and synchronize events (Pause/Resume/Kill) using **Signals** (\SIGUSR1\, \SIGUSR2\, \SIGINT\).

## 🛠️ Project Structure

| File | Description |
|------|-------------|
| \Pomodoro.hpp\ / \.cpp\ | Class definition and implementation of the Pomodoro session logic. |
| \ergasia.cpp\ | Main application file handling UI rendering (\
curses\), process branching, and IPC. |
| \	est.cpp\ | Automated unit tests suite utilizing the Catch2 framework. |
| \Makefile\ | Compilation directives for building the main app and the test suite. |

## 📊 Testing & Validation
The project includes a comprehensive suite of unit tests verifying the integrity of the \Pomodoro\ class. Using the **Catch2** framework, the tests validate:
- Accurate initialization of default and custom time intervals.
- Correct status transitions during the session lifecycle (e.g., verifying \STATUS_STARTED\ after execution, or \STATUS_PAUSED\ upon interruption).
- Prevention of negative or invalid time accumulations.

## 🚀 Getting Started & Execution

3. **Run the Application:**
   You need to provide a text file containing the problem configuration as a command-line argument.
   ./jobs problem.txt

   
##📝 Author & Academic Use
- **Student:** Maria Alexandra Ndontou (ΜΑΡΙΑ ΑΛΕΞΑΝΔΡΑ ΝΤΟΝΤΟΥ)
- **Student ID:** 2799
- **Institution:** University of Ioannina — Department of Informatics and Telecommunications
- **Course:** Object-Oriented Programming (Αντικειμενοστραφής Προγραμματισμός) — 2nd Semester Project

*This document is provided for academic reference and educational purposes.*
