#include <stdio.h>


void fun1(void)
{
	char hh[] = "a d";
	char p[]  = "J o J d";   //ɾ�����ַ����ո�  ���һ��ո����ɾ�����ɾ���ˡ�������ĳ�fun2��
	int n = 0;
	int temp = 0;
	while (!(p[n] == '\0')){
		if (p[n] == hh[1]){   //����ǿո񽫺�����ַ�ȫ���ƶ�һ��
			temp = n;
			while (!(p[temp] == '\0')){
				p[temp] = p[temp+1];
				temp++;
			}
		    temp = 0; 
		}
		n++;
	}
	printf("Good!! %s\n",p);
}
void fun2(char *p)
{
	char *p_temp = p;
	char *head = p;    //������ͷ
	char *hh = "a d";
	
	while (!(*p == '\0')){  //���ǿ��ַ�ת�Ƶ�p_temp��
		if (!(*p == hh[1])){
			*p_temp = *p;
			*p_temp++ ;
		}
		p++;
	}
	*p_temp = '\0';//��β��
	
	p_temp = head;//�ص���ͷ
	printf("del ok : %s\n",p_temp);
}
int main(void) 
{
	char Str_test[] = "dss   s a j k";
	
	fun1();
	
	fun2(Str_test);
	
	return 0;
}
