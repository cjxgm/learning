#include "event.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>

#define ANY (void*)

EventSystem* event_system()
{
	EventSystem* evsys = malloc(sizeof(EventSystem));
	evsys->fd = epoll_create(1);
	return evsys;
}

void event_system_close(EventSystem* evsys)
{
	close(evsys->fd);
	free(evsys);
}

void event_system_source(EventSystem* evsys, Event* ev)
{
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.ptr = ev;
	epoll_ctl(evsys->fd, EPOLL_CTL_ADD, ev->fd_listener, &event);
}

Event* event_system_wait(EventSystem* evsys)
{
	struct epoll_event event;
	epoll_wait(evsys->fd, &event, 1, -1);

	Event* ev = event.data.ptr;
	char buf;
	read(ev->fd_listener, &buf, 1);

	return ev;
}


Event* event(void* data)
{
	Event* ev = malloc(sizeof(Event));
	pipe(ANY ev);
	ev->data = data;
	return ev;
}

void event_close(Event* ev)
{
	close(ev->fd_listener);
	close(ev->fd_notifier);
	free(ev);
}

void event_notify(Event* ev)
{
	write(ev->fd_notifier, "", 1);
}

