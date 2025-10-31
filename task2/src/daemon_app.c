#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "daemon_app.h"

static char symbol = '*';
static int repeat = 5;
static const char *cfg_path = NULL;
static volatile sig_atomic_t reload_flag = 0;
static volatile sig_atomic_t stop_flag = 0;

static void read_config() {
    FILE *f = fopen(cfg_path, "r");
    if (!f) return;
    char line[64];
    if (fgets(line, sizeof(line), f)) symbol = line[0];
    if (fgets(line, sizeof(line), f)) repeat = atoi(line);
    fclose(f);
}

static void hup_handler(int signo) { (void)signo; reload_flag = 1; }
static void term_handler(int signo) { (void)signo; stop_flag = 1; }

void start_daemon(const char *config_path) {
    cfg_path = config_path;
    read_config();

    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    setsid();
    umask(0);
    if (chdir("/") != 0) _exit(EXIT_FAILURE);


    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    signal(SIGHUP, hup_handler);
    signal(SIGTERM, term_handler);

    FILE *log = fopen("/tmp/lab4_task2.log", "a+");
    if (!log) exit(EXIT_FAILURE);

    while (!stop_flag) {
        if (reload_flag) {
            read_config();
            reload_flag = 0;
            fprintf(log, "[INFO] Config reloaded: symbol=%c repeat=%d\n", symbol, repeat);
            fflush(log);
        }

        for (int i = 0; i < repeat; ++i) fputc(symbol, log);
        fputc('\n', log);
        fflush(log);

        sleep(3);
    }

    fprintf(log, "[INFO] Terminated\n");
    fclose(log);
}
