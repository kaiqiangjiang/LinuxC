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

	//�ϲ��õ�����
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
	//------------------����1&2 

	printf("------------creat show--------\n");
	show_link(head);  //��ӡ�����õ�����

	printf("----------reverst show----------\n");
	
	reverse_link(head); //��ת����

	show_link(head); //��ӡ��ת�������
	printf("-------------- kill 4 show------\n");

	delete_link(head,4); //ɾ�������е�ĳ������

	show_link(head); //��ӡɾ���������

	#endif
	
    //-------------���� 3
    
    printf("=================Test 3 ======!\n");

	printf("-------------- headA befor show------\n");
	reverse_link(headA);show_link(headA); //��ӡA ����
	printf("-------------- headB befor show------\n");
	reverse_link(headB);show_link(headB); //��ӡB ����

	headA = mergelink(headB,headA); //�ϲ� A  B ����

	printf("-------------- headAB befor show------\n");
	show_link(headA);//��ӡ�ϲ��������


	printf("=================Test 4 ======!\n");
	josephus(8,4,3);//����4
	
	printf("sucess !\n");

	return 0;
}

