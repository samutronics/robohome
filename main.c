/*
 * main.c
 */

#include "list.h"
#include "task.h"
#include "queue.h"

int main(void) {
	vTaskStartScheduler();
	while(1);
	return 0;
}
