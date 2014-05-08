#pragma once

typedef struct EventSystem
{
	int fd;
}
EventSystem;

typedef struct Event
{
	int fd_listener;
	int fd_notifier;
	void* data;
}
Event;

EventSystem* event_system();
void event_system_close(EventSystem* evsys);
void event_system_source(EventSystem* evsys, Event* ev);
Event* event_system_wait(EventSystem* ev);

Event* event(void* data);
void event_close(Event* ev);
void event_notify(Event* ev);

