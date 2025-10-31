#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static const char *g_timeout_msg = NULL;

static void alarm_handler(int signo)
{
    if (signo == SIGALRM)
    {
        if (g_timeout_msg && *g_timeout_msg)
        {
            ssize_t written = write(STDERR_FILENO, g_timeout_msg, (unsigned)strlen(g_timeout_msg));
            (void)written;
            written = write(STDERR_FILENO, "\n", 1);
            (void)written;
        }
        _exit(124);
    }
}

void set_time_limit(unsigned seconds, const char *message)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    g_timeout_msg = message;
    alarm(seconds);
}

void cancel_time_limit(void)
{
    alarm(0);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_DFL;
    sigaction(SIGALRM, &sa, NULL);
}
