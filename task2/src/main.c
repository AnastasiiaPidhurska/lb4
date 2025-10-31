#include "daemon_app.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    const char *config = "config.txt";
    if (argc > 1) config = argv[1];
    start_daemon(config);
    return 0;
}
