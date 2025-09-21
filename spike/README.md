# User-Aware Adaptive Timer for Low-Overhead Power Management

This repository contains the source code and documentation for the user-aware, context-extended timer framework, as detailed in the paper "User-Aware Adaptive Timer for Low-Overhead Power Management in Mobile/IoT Systems."

The framework is a lightweight solution designed to reduce energy consumption in mobile and IoT devices by aligning suspend/resume operations with actual usage patterns, without the need for complex machine learning models or kernel modifications. [cite_start]It has been shown to achieve 15-25% energy savings while maintaining sub-50 ms latency and preserving user experience[cite: 6].

## Features

- **Lightweight:** Requires <500 lines of code and <1 KB of memory.
- [cite_start]**Context-Aware:** Dynamically adapts timer thresholds based on the application context (e.g., AR/VR gaming, background sensing, night hours)[cite: 62].
- [cite_start]**Low Overhead:** Avoids costly CPU polling and complex stacks[cite: 65, 159].
- [cite_start]**High Portability:** Compatible with standard Linux/Android power management stacks[cite: 51, 75].
- [cite_start]**Proven Performance:** Achieves similar energy-delay trade-offs as a lightweight ML baseline at orders-of-magnitude lower overhead[cite: 8, 145].

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)
- A Linux-based system (for `unistd.h` and `sys/time.h`)

### Building the Code

To compile the provided `main.c` file, run the following command in your terminal:

```bash
gcc main.c -o adaptive_timer
