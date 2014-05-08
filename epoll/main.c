#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "event.h"


#define ANY (void*)
static void sleep_thread(char* msg);
static void timer_thread(int duration);
static void mainloop_thread();

static pthread_t thread_producer;
static pthread_t thread_consumer;


static EventSystem* evsys;




int main()
{
	evsys = event_system();
	pthread_create(&thread_producer, NULL, ANY &sleep_thread, "\e[0;34mmousemove\e[0m");
	pthread_create(&thread_producer, NULL, ANY &sleep_thread, "\e[0;32mkeydown\e[0m");
	pthread_create(&thread_producer, NULL, ANY &sleep_thread, "\e[0;36mclick\e[0m");
	pthread_create(&thread_producer, NULL, ANY &timer_thread, ANY 321);
	pthread_create(&thread_producer, NULL, ANY &timer_thread, ANY 1111);
	pthread_create(&thread_consumer, NULL, ANY &mainloop_thread, NULL);
	pthread_join(thread_consumer, NULL);
	return 0;
}




static void sleep_thread(char* msg)
{
	Event* ev = event(msg);
	event_system_source(evsys, ev);
	while (1) {
		printf("pre-notify \"%s\"\n", msg);
		event_notify(ev);
		printf("pos-notify \"%s\"\n", msg);
		sleep(rand()&0b11);
	}
}

static void timer_thread(int duration)
{
	char msg[128];
	sprintf(msg, "\e[0;33mtimer \e[1;35m%d\e[0;35mms\e[0;33m timeout\e[0m", duration);
	Event* ev = event(msg);
	event_system_source(evsys, ev);
	while (1) {
		clock_t start = clock();
		while (clock()-start < duration*CLOCKS_PER_SEC/1000) {}
		event_notify(ev);
	}
}

static void mainloop_thread()
{
	while (1) {
		Event* ev = event_system_wait(evsys);
		printf("recv \"%s\"\n", (char*)ev->data);
	}
}

