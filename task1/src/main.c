#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "timer.h"

#define BUF_SMALL  128
#define BUF_MEDIUM 256

static unsigned parse_uint_or_exit(const char *s) {
    if (!s || !*s) {
        fprintf(stderr, "Помилка: очікувалось число.\n");
        exit(EXIT_FAILURE);
    }
    char *end = NULL;
    unsigned long v = strtoul(s, &end, 10);
    if (end == s || *end != '\0') {
        fprintf(stderr, "Помилка: некоректне число: '%s'\n", s);
        exit(EXIT_FAILURE);
    }
    if (v > 3600ul) {
        fprintf(stderr, "Занадто велике значення тайм-ауту (макс 3600).\n");
        exit(EXIT_FAILURE);
    }
    return (unsigned)v;
}

static void trim_nl(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n && (s[n-1] == '\n')) s[n-1] = '\0';
}

static int read_line_with_timeout(const char *prompt, char *buf, size_t cap, unsigned timeout_sec) {
    if (prompt) {
        fputs(prompt, stdout);
        fflush(stdout);
    }

    set_time_limit(timeout_sec, "⏳ Час очікування введення вичерпано. Спробуйте знову або збільште ліміт (-t).");

    char *res = fgets(buf, (int)cap, stdin);

    cancel_time_limit();

    if (!res) {
        if (feof(stdin)) {
            fprintf(stderr, "Ввід завершено (EOF).\n");
        } else if (ferror(stdin)) {
            perror("Помилка читання");
        } else {
            fprintf(stderr, "Невідома помилка читання.\n");
        }
        return -1;
    }
    trim_nl(buf);
    return 0;
}

int main(int argc, char **argv) {
    unsigned timeout_sec = 15;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--timeout") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Використання: %s [-t SEC]\n", argv[0]);
                return EXIT_FAILURE;
            }
            timeout_sec = parse_uint_or_exit(argv[++i]);
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Використання: %s [-t SEC]\n\n", argv[0]);
            printf("  -t, --timeout  Ліміт часу (секунди) на введення кожного поля (за замовчуванням: %u)\n", timeout_sec);
            return EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Невідомий параметр: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    char full_name[BUF_MEDIUM];
    char email[BUF_MEDIUM];
    char age_str[BUF_SMALL];

    printf("Реєстрація користувача (тайм-аут на поле: %u с)\n", timeout_sec);
    printf("Щоб вийти — натисніть Ctrl+D.\n\n");

    if (read_line_with_timeout("Введіть ПІБ: ", full_name, sizeof(full_name), timeout_sec) != 0)
        return EXIT_FAILURE;

    if (read_line_with_timeout("Введіть email: ", email, sizeof(email), timeout_sec) != 0)
        return EXIT_FAILURE;

    if (read_line_with_timeout("Введіть вік: ", age_str, sizeof(age_str), timeout_sec) != 0)
        return EXIT_FAILURE;

    for (const char *p = age_str; *p; ++p) {
        if (!isdigit((unsigned char)*p)) {
            fprintf(stderr, "Помилка: вік має бути числом.\n");
            return EXIT_FAILURE;
        }
    }

    unsigned age = (unsigned)strtoul(age_str, NULL, 10);

    printf("\n Дані прийнято!\n");
    printf("ПІБ  : %s\n", full_name);
    printf("Email: %s\n", email);
    printf("Вік  : %u\n", age);

    return EXIT_SUCCESS;
}
