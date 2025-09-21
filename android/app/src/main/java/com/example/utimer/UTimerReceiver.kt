package com.example.utimer

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent

class UTimerReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        if (intent.action == UTimerManager.ACTION_CHECK) {
            val svc = UTimerService.instance()
            svc?.maybeSuspend()
            svc?.scheduleSuspendCheck()
        }
    }
}
