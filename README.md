# Job Shop Scheduling Simulator (C++ OOP)

[![University](https://img.shields.io/badge/University-University%20of%20Ioannina-blue.svg)](https://www.uoi.gr)
[![Department](https://img.shields.io/badge/Department-Informatics%20%26%20Telecommunications-lightgrey.svg)](https://www.dit.uoi.gr/)
[![Language](https://img.shields.io/badge/Language-C++-blueviolet.svg)]()
[![Library](https://img.shields.io/badge/Library-ncurses-orange.svg)]()

A C++ terminal-based application simulating a Job Shop Scheduling problem. The system models the allocation of multiple jobs to a set of machines over time, respecting strict execution sequences and calculating the total time required for all processes to finish. Developed as a semester project for the Object-Oriented Programming course at the Department of Informatics and Telecommunications, University of Ioannina.

---

## 📚 Table of Contents
1. [Overview](#-overview)
2. [Theoretical Background & Architecture](#-theoretical-background--architecture)
3. [Project Structure](#-project-structure)
4. [Getting Started & Execution](#-getting-started--execution)
5. [Author & Credits](#-author--academic-use)

## 🔍 Overview
This project solves a scheduling problem where various jobs must visit a specific sequence of machines for a predetermined amount of time. The simulator steps through time units, assigns waiting jobs to available machines, and logs the entire operational timeline. It features an interactive menu allowing users to query specific job states, sort jobs/machines based on performance metrics, and visualize the machine allocation via a text-based Gantt chart.

## 📐 Theoretical Background & Architecture

### Object-Oriented Design
The system is built upon three core classes:
- **\`Job\`**: Represents an individual task. It encapsulates data such as execution time on each machine, the required sequence of machines, and its current status (\`WAIT\`, \`EXE\`, \`END\`).
- **\`Machine\`**: Represents an execution unit, tracking its availability (\`isFree\`) and total operational time.
- **\`Problem\`**: The core simulation engine. It reads the configuration from a text file, instantiates the objects, and drives the simulation loop tick-by-tick until all jobs reach the \`END\` state.

### Custom Algorithms & Visualization
- **Sorting**: Implements custom Bubble Sort algorithms to dynamically rank jobs by total execution time or desired time, and machines by total working time.
- **Graphing**: Generates a 2D grid in the terminal where rows represent machines, columns represent time units, and symbols denote which job is occupying a machine at any given tick.

## 🛠️ Project Structure

| File | Description |
|------|-------------|
| \`main.cpp\` | Entry point and interactive terminal menu. |
| \`Problem.h\` | Simulation engine, data parsing, sorting logic, and graph generation. |
| \`Job.h\` | Job entity definition and state transition logic. |
| \`Machine.h\` | Machine entity tracking utilization and availability. |

## 🚀 Getting Started & Execution

1. **Prerequisites:** Ensure \`g++\` is installed on your system.
2. **Compilation:**
   g++ main.cpp -o jobs

3. **Run the Application:**
   You need to provide a text file containing the problem configuration as a command-line argument.
   ./jobs problem.txt

   
##📝 Author & Academic Use
- **Student:** Maria Alexandra Ndontou (ΜΑΡΙΑ ΑΛΕΞΑΝΔΡΑ ΝΤΟΝΤΟΥ)
- **Student ID:** 2799
- **Institution:** University of Ioannina — Department of Informatics and Telecommunications
- **Course:** Object-Oriented Programming (Αντικειμενοστραφής Προγραμματισμός) — 2nd Semester Project

*This document is provided for academic reference and educational purposes.*
