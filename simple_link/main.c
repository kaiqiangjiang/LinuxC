#include <stdio.h>
#include <stdlib.h>
#include "link.h"

struct mylink *head = NULL;

struct mylink *headA = NULL;
struct mylink *headB = NULL;
struct mylink *headAB =NULL;



int main(void)
{
	//printf("sucess 1 !\n");

	int i = 0;
	head = (struct mylink*)malloc(sizeof(struct mylink));

	//合并用的链表
	headA = (struct mylink*)malloc(sizeof(struct mylink));
	headB = (struct mylink*)malloc(sizeof(struct mylink));
	
	
	if (head == NULL || headA==NULL || headB == NULL){
		printf("malloc error!\n");
		return -1;
	}
	//printf("sucess 2 !\n");

	head->value = 0;
	head->next =NULL;

	headA->value = 0;
	headA->next =NULL;

	headB->value = 0;
	headB->next =NULL;
	

	for (i = 0; i < 10;i++){
		insert_value(head,i+2);
	}
	for (i = 1;i < 10;i++){
		insert_value(headA,i*2-1);
		insert_value(headB,i*2);
	}

	printf("=================Test 1&2 ======!\n");
	#if 1
	//------------------测试1&2 

	printf("------------creat show--------\n");
	show_link(head);  //打印创建好的链表

	printf("----------reverst show----------\n");
	
	reverse_link(head); //翻转链表

	show_link(head); //打印翻转后的链表
	printf("-------------- kill 4 show------\n");

	delete_link(head,4); //删除链表中的某块数据

	show_link(head); //打印删除后的链表

	#endif
	
    //-------------测试 3
    
    printf("=================Test 3 ======!\n");

	printf("-------------- headA befor show------\n");
	reverse_link(headA);show_link(headA); //打印A 链表
	printf("-------------- headB befor show------\n");
	reverse_link(headB);show_link(headB); //打印B 链表

	headA = mergelink(headB,headA); //合并 A  B 链表

	printf("-------------- headAB befor show------\n");
	show_link(headA);//打印合并后的链表


	printf("=================Test 4 ======!\n");
	josephus(8,4,3);//问题4
	
	printf("sucess !\n");

	return 0;
}

