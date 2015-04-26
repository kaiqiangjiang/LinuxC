#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "link.h"

int insert_value(struct mylink *node,int value)
{
	struct mylink * new_link;
	struct mylink * temp;
	if(node == NULL){
		return -1;
	}

	new_link = (struct mylink*)malloc(sizeof(struct mylink));
	if(new_link == NULL){
		printf("insert malloc error!\n");
	}

	new_link->value = value;

	temp = node->next;
	node->next = new_link;
	new_link->next = temp;

	return 0;
	
}
int delete_link(struct mylink *node,int id)
{
	//printf(" 66666 delete!\n");

	struct mylink * temp;
	struct mylink * temp2;
	temp = node;

	if(node == NULL){
		return ;
	}

	while(temp->next){
		if(temp->next->value == id){
			break;
		} 
		temp = temp->next;
	}
	   
	temp2 = temp->next;
	temp->next = temp->next->next;

	free(temp2);
	
	return 0;
	
}
void show_link(struct mylink *node)
{

	if(node == NULL){
		return ;
	}
	
	struct mylink * temp;
	temp = node;

	//printf(" show ============!\n");
	while(temp){
		//printf(" 5555!\n");
		printf("mylink test %d\n",temp->value);
		temp = temp->next;
	}
}

int reverse_link(struct mylink *node)
{
    struct mylink *current;
	struct mylink *pnext;
	struct mylink *prev;
    if(node == NULL || node->next == NULL)
        return -1;
    current = node->next;  /* p1ָ������ͷ�ڵ����һ���ڵ� */
    pnext = current->next;
    current->next = NULL;
    while(pnext)
    {
        prev = pnext->next;
        pnext->next = current;
        current = pnext;
        pnext = prev;
        //printf("after:current = %d,next = %d \n",current->value,current->next->value);
    }
    //printf("current = %d,next = %d \n",current->data,current->next->data);
    node->next = current;  /* ������ͷ�ڵ�ָ��p1 */
    return 0;
}

#if 0
int mergelink(struct mylink *nodeA,struct mylink *nodeB)
 {
 
	  struct mylink *currentA;
	  struct mylink *temp;
	  struct mylink *temp2;
  
 	  if(nodeA == NULL || nodeA->next == NULL || nodeB == NULL || nodeB->next == NULL)
        return -1;

	  currentA = nodeA->next;
	
	  while(currentA){
	  	
		temp = currentA->next;
		temp2 = nodeB->next->next;
		
		currentA->next = nodeB->next;
		nodeB->next->next = temp;


		nodeB->next = temp2;
		currentA = currentA->next;
		
	  }
	  
	  return 0;

 }
#endif

struct mylink * mergelink(struct mylink *nodeA,struct mylink *nodeB)
{
	struct mylink *head,*p1,*p2,*pos;

	if(nodeA == NULL || nodeA->next == NULL || nodeB == NULL || nodeB->next == NULL)
			return NULL;

	head=p1 = nodeA;
	p2 = nodeB->next;

	pos = head;

	while(p2!=NULL){
		p1 = p1->next;
		pos->next = p2;
		pos = p2;
		p2 = p2->next;
		pos->next = p1;
		pos = p1;
	}

	return head;
}
void josephus(int n, int m, int k)
{

 struct mylink * p, *r, *list = NULL;
 
 int i;

  //����һ��ѭ������
 for(i = 1; i <= n; i++){ 
 	p = (struct mylink *)malloc(sizeof(struct mylink));
  	p->value= i;
  	if(list == NULL){
   		list = p;
  	}
  	else{
  		 r->next = p;
  	}
  	r = p;
 }
 p->next = list;                    


 //pָ���1��������
 p = list;
 for(i = 1; i < k; i++){
 	r = p;
 	p = p->next;
 }                                



 while(p->next != p){
 	for(i=1; i<m; i++){
 		r = p;
 		p = p->next;
 	 }                               //pָ���m����㣬rָ���m-1�����
  	r->next = p->next;              //ɾ����m�����
  	printf("%4d", p->value);         //���һ�������
  	free(p);                        //�ͷű�ɾ�����Ŀռ�
  	p = r->next;                    //pָ���µĳ�����
 }

 printf("%4d\n", p->value);

 free(p);
 
}




