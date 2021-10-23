#ifndef LIST_H
#define LIST_H

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#endif // LIST_H
