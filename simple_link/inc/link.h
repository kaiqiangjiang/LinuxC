#ifndef _LINK_H
#define _LINK_H

struct mylink{
	int value;
	struct mylink * next;
};

extern struct mylink *head;
extern struct mylink *headA;
extern struct mylink *headB;
extern struct mylink *headAB;





int insert_value(struct mylink *node,int value);
int delete_link(struct mylink *node,int id);
void show_link(struct mylink *node);
int reverse_link(struct mylink *node);
struct mylink * mergelink(struct mylink *nodeA,struct mylink *nodeB);
void josephus(int n, int m, int k);






#endif


