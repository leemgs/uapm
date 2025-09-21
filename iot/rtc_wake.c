// rtc_wake.c — reference sketch for RTC-assisted suspend/resume on MCUs
// Apache-2.0

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// ---- Board/HAL-specific stubs (replace with real ones) ----
void rtc_start_wakeup_in_ms(uint32_t ms) { (void)ms; /* HAL to arm RTC */ }
void enter_low_power_mode(void) { /* HAL to enter STOP/STANDBY */ }
void resume_from_low_power(void) { /* HAL to resume clocks/peripherals */ }
uint32_t millis(void) { static uint32_t t=0; return t+=100; } // mock
bool event_pending(void) { return false; } // replace with GPIO/IRQ check
// -----------------------------------------------------------

static uint32_t last_event_ms = 0;
static uint32_t T_ms = 2000; // initial threshold
static float alpha = 0.5f;    // smoothing factor
static float beta  = 0.5f;    // cost weight (not used directly in this sketch)

static inline uint32_t clamp(uint32_t v, uint32_t lo, uint32_t hi){
    return v<lo?lo:(v>hi?hi:v);
}

void on_activity_event(void){
    uint32_t now = millis();
    uint32_t idle = now - last_event_ms;
    last_event_ms = now;
    // T_new = α*T_old + (1-α)*t_idle
    float t_new = alpha * (float)T_ms + (1.0f - alpha) * (float)idle;
    T_ms = clamp((uint32_t)t_new, 250, 10000);
    rtc_start_wakeup_in_ms(T_ms);
    printf("[UTIMER] activity idle=%ums T=%ums\n", idle, T_ms);
}

void loop_once(void){
    uint32_t now = millis();
    uint32_t idle = now - last_event_ms;
    if (idle >= T_ms) {
        printf("[UTIMER] enter LP, idle=%u T=%u\n", idle, T_ms);
        enter_low_power_mode();
        // wakes on RTC or external IRQ
        resume_from_low_power();
        last_event_ms = millis();
        // after wake, schedule next check
        rtc_start_wakeup_in_ms(T_ms);
    }
    if (event_pending()) {
        on_activity_event();
    }
}

int main(void){
    last_event_ms = millis();
    rtc_start_wakeup_in_ms(T_ms);
    for (int i=0; i<100; ++i) loop_once();
    return 0;
}
