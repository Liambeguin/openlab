#ifndef BOOT_MSG_H_
#define BOOT_MSG_H_

#include "nvic.h"
#include "event.h"
#include "soft_timer.h"
#include "printf.h"

#define boot_success(...) do {printf("[\x1b[32m  OK \x1b[0m ] "); printf(__VA_ARGS__);}while(0)
#define boot_failure(...) do {printf("[\x1b[31m FAIL\x1b[0m ] "); printf(__VA_ARGS__);}while(0)


#endif /* BOOT_MSG_H */
