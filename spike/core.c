#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

// Pseudocode from the paper's Algorithm 1 
// Context-Aware Adaptive Timer Implementation

#define BASELINE_TIMEOUT_MS 5000 // T0 in milliseconds
#define IDLE_TIME_UPDATE_ALPHA 0.8 // Alpha for T update rule [cite: 60]

// Define context modes and their corresponding alpha/beta values [cite: 66, 92]
typedef enum {
    ENERGY_CONSERVATIVE,
    BALANCED,
    LATENCY_CRITICAL
} PriorityMode;

typedef struct {
    float alpha;
    float beta;
} ModeWeights;

// Note: These values are examples and should be tuned based on real-world data
ModeWeights mode_weights[] = {
    {0.8, 0.2}, // ENERGY_CONSERVATIVE [cite: 66, 92]
    {0.5, 0.5}, // BALANCED [cite: 92]
    {0.2, 0.8}  // LATENCY_CRITICAL [cite: 66, 92]
};

// Global variables for the timer
long long last_event_time = 0;
long long current_idle_time = 0;
float current_T = BASELINE_TIMEOUT_MS;
float observed_t_idle_avg = 0;
bool is_suspended = false;

// Function to get current time in milliseconds
long long get_current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
}

// Function to simulate suspend/resume operations
void suspend_system() {
    if (!is_suspended) {
        printf("[Power Controller] Suspending system...\n");
        is_suspended = true;
    }
}

void resume_system() {
    if (is_suspended) {
        printf("[Power Controller] Resuming system...\n");
        is_suspended = false;
    }
}

// Function to handle a new activity event
void on_activity_event(PriorityMode current_context) {
    long long now = get_current_time_ms();
    
    // Resume system immediately if it was suspended
    if (is_suspended) {
        printf("[Activity Monitor] Event detected. Resuming...\n");
        resume_system();
        // Record the actual idle duration
        long long actual_idle_duration = now - last_event_time;
        observed_t_idle_avg = (observed_t_idle_avg * 0.9) + (actual_idle_duration * 0.1); // Simple moving average
    }

    last_event_time = now;
    current_idle_time = 0;

    // Adjust T based on context
    float alpha = mode_weights[current_context].alpha;
    current_T = (alpha * current_T) + ((1 - alpha) * observed_t_idle_avg);
    
    printf("[Timer Manager] New T: %.2f ms (alpha: %.1f)\n", current_T, alpha);
}

// Main simulation loop
int main() {
    // Initial state
    last_event_time = get_current_time_ms();
    printf("Starting Adaptive Timer simulation...\n");

    // Simulate different workloads/contexts [cite: 80-89]
    on_activity_event(LATENCY_CRITICAL); // Simulate AR/VR Gaming [cite: 88]
    usleep(500000); // 0.5 sec of inactivity
    on_activity_event(LATENCY_CRITICAL);
    usleep(250000); // 0.25 sec of inactivity
    on_activity_event(LATENCY_CRITICAL);
    usleep(500000);

    // Switch to a new context [cite: 11]
    printf("\n--- Context Switch: Energy-Conservative Mode (e.g., Night Hours) ---\n\n");
    on_activity_event(ENERGY_CONSERVATIVE); // Simulate IoT Sensing or Night Hours [cite: 89]
    usleep(3000000); // 3 sec of inactivity
    // Now, let's simulate a long period of inactivity to trigger suspend
    current_idle_time = get_current_time_ms() - last_event_time;
    if (current_idle_time >= current_T) {
        suspend_system();
    }
    usleep(2000000); // System is suspended here

    printf("\n--- End of simulation ---\n");

    return 0;
}
