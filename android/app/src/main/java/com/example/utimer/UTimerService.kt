package com.example.utimer

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.IBinder
import android.util.Log

class UTimerService : Service() {
    private lateinit var mgr: UTimerManager

    override fun onCreate() {
        super.onCreate()
        mgr = UTimerManager(this)
        mgr.setMode(UTimerManager.Mode.BALANCED)
        startForeground(1, buildNotification())
        instance = this
        mgr.scheduleSuspendCheck()
        Log.i("UTimer", "Service started")
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        // In real integration, call mgr.onActivityEvent() from input/sensor hooks.
        return START_STICKY
    }

    override fun onDestroy() {
        super.onDestroy()
        instance = null
    }

    override fun onBind(intent: Intent?): IBinder? = null

    private fun buildNotification(): Notification {
        val nm = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        val ch = "utimer"
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            nm.createNotificationChannel(NotificationChannel(ch, "UTimer", NotificationManager.IMPORTANCE_LOW))
        }
        val b = Notification.Builder(this).setContentTitle("UTimer").setContentText("Adaptive timer running")
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) b.setChannelId(ch)
        return b.build()
    }

    fun maybeSuspend() = mgr.maybeSuspend()
    fun scheduleSuspendCheck() = mgr.scheduleSuspendCheck()

    companion object {
        private var instance: UTimerService? = null
        fun instance(): UTimerService? = instance
    }
}
