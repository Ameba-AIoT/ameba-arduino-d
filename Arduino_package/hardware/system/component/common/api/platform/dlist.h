/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef __LIST_H
#define __LIST_H

#if defined ( __CC_ARM   )
#ifndef inline
#define inline			__inline
#endif
#endif


struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


static inline void __list_add(struct list_head *newitem,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = newitem;
	newitem->next = next;
	newitem->prev = prev;
	prev->next = newitem;
}

static inline void list_add(struct list_head *newitem, struct list_head *head)
{
	__list_add(newitem, head, head->next);
}

static inline void list_add_tail(struct list_head *newitem, struct list_head *head)
{
	__list_add(newitem, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (struct list_head *) 0;
	entry->prev = (struct list_head *) 0;
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}

static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

static inline void list_splice(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); \
        	pos = pos->prev)
        	
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member, type)				\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 					\
	     pos = list_entry(pos->member.next, type, member))

#define list_for_each_entry_safe(pos, n, head, member, type)			\
	for (pos = list_entry((head)->next, type, member),	\
		n = list_entry(pos->member.next, type, member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, type, member))

#endif

