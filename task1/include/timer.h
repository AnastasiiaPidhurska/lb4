#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void set_time_limit(unsigned seconds, const char *message);
void cancel_time_limit(void);

#ifdef __cplusplus
}
#endif
