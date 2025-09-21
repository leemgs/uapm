package com.example.utimer

import android.app.AlarmManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.os.SystemClock
import android.util.Log
import java.util.concurrent.atomic.AtomicLong
import kotlin.math.max
import kotlin.math.min

/**
 * UTimerManager: adaptive idle-threshold controller with simple context-aware rules.
 * Cost model: C(T) = α·E(T) + β·D(T); here we only adapt T using exponential smoothing:
 *   T_new = α*T_old + (1-α)*t_idle
 */
class UTimerManager(private val ctx: Context) {
    private val tag = "UTimer"
    private val am = ctx.getSystemService(Context.ALARM_SERVICE) as AlarmManager
    private val lastEvent = AtomicLong(SystemClock.elapsedRealtime())
    @Volatile private var T_ms: Long = 2000L // initial threshold
    @Volatile private var alpha: Double = 0.5
    @Volatile private var beta:  Double = 0.5

    fun setMode(mode: Mode) {
        when (mode) {
            Mode.ENERGY_CONSERVATIVE -> { alpha = 0.8; beta = 0.2 }
            Mode.BALANCED            -> { alpha = 0.5; beta = 0.5 }
            Mode.LATENCY_CRITICAL    -> { alpha = 0.2; beta = 0.8 }
        }
        Log.i(tag, "Mode=$mode α=$alpha β=$beta")
    }

    fun onActivityEvent() {
        val now = SystemClock.elapsedRealtime()
        val idle = now - lastEvent.getAndSet(now)
        // Exponential smoothing update rule
        val tNew = alpha * T_ms + (1.0 - alpha) * idle
        // Clamp to sane bounds
        T_ms = max(250L, min(tNew.toLong(), 10_000L))
        Log.d(tag, "Activity, idle=${idle}ms, T=${T_ms}ms")
        scheduleSuspendCheck()
    }

    fun scheduleSuspendCheck() {
        val intent = Intent(ctx, UTimerReceiver::class.java).setAction(ACTION_CHECK)
        val pi = PendingIntent.getBroadcast(ctx, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_IMMUTABLE)
        am.setExactAndAllowWhileIdle(AlarmManager.ELAPSED_REALTIME_WAKEUP, SystemClock.elapsedRealtime() + T_ms, pi)
        Log.d(tag, "Scheduled suspend check in ${T_ms}ms")
    }

    fun maybeSuspend() {
        val now = SystemClock.elapsedRealtime()
        val idle = now - lastEvent.get()
        if (idle >= T_ms) {
            Log.i(tag, "Entering light-sleep (simulated). idle=${idle}ms")
            // Integrators should map this to device-specific doze/light-sleep APIs.
            // Wakeup happens on next activity / broadcast.
        } else {
            Log.d(tag, "Skip suspend: idle=${idle}ms < T=${T_ms}ms")
        }
    }

    companion object {
        const val ACTION_CHECK = "com.example.utimer.ACTION_CHECK"
    }

    enum class Mode { ENERGY_CONSERVATIVE, BALANCED, LATENCY_CRITICAL }
}
