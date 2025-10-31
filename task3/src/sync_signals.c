#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "sync_signals.h"

static volatile sig_atomic_t got_signal = 0;
static volatile sig_atomic_t received_value = 0;

static void child_handler(int signo, siginfo_t *info, void *ctx) {
    (void)ctx;
    got_signal = 1;
    if (info) received_value = info->si_value.sival_int;
    printf("[CHILD] Отримано сигнал %d (%s), значення: %d\n", signo, strsignal(signo), received_value);
    fflush(stdout);
}

void run_sync_demo(void) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = child_handler;
        sigaction(SIGRTMIN, &sa, NULL);

        printf("[CHILD] PID = %d, очікую сигнали...\n", getpid());
        fflush(stdout);

        while (1) pause();
    } else {
        sleep(1);
        printf("[PARENT] PID = %d, нащадок PID = %d\n", getpid(), pid);
        fflush(stdout);

        union sigval val;
        for (int i = 1; i <= 5; ++i) {
            val.sival_int = i;
            sigqueue(pid, SIGRTMIN, val);
            printf("[PARENT] Надіслано сигнал %d (%s) із значенням %d\n", SIGRTMIN, strsignal(SIGRTMIN), i);
            fflush(stdout);
            sleep(1);
        }

        sleep(2);
        kill(pid, SIGTERM);
        printf("[PARENT] Завершив роботу дочірнього процесу.\n");
        fflush(stdout);
    }
}
