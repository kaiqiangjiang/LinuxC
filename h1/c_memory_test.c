#include <stdio.h>



void question_1(void)
{
	char *p = "Hello";
	int n = 0;
	int count = 0;
	while(p[n] != 0){
		n++;
		count++;
	}
	printf("This size is:%d\n",count);
}
void question_2(void)
{
	int a[] = {1,2,3,4,5};
	int n = 0;
	int temp = 0;
	int m = sizeof(a); printf("This char size is m:%d\n",m);
	m = m/(sizeof(int));
	for (n =0;n < m;n++){
		printf("Before a[%d]:  %d\n",n,a[n]);
	}
	for (n =0;n < m-n-1;n++){
		//printf("a[%d]:  %d\n",n,a[n]);
		temp = a[n];
		a[n] = a[m-n-1];
		a[m-n-1] = temp;
	}
	for (n =0;n < m;n++){
		printf("After a[%d]:  %d\n",n,a[n]);
	}
	//printf("%d\n",m);

}
int question_3(int a,int b,int c)
{
	if (1 == c){
		return a+b;
	}else if(2 == c){
		return a-b;
	}else{
		return a*b;
	}
}
void question_4(void)
{
	char *Test = "H e";
	printf("空格的ASCII码是%x\n",Test[1]);
	
	char *p = "hhde  ji w";
	
	int count = 0;
	int n =0;
	while (p[n] != 0){
		if (p[n] == Test[1]){//空格加1Test[1]是空格
			count++;
		}
		n++;
	}
	printf("This char is :%s\n",p);
	printf("space:  %d\n",count);
}
int main(void)
{
	printf("----------------------This is 内存空间练习题----------------------------\n");
	
	printf("----------question_1  :----------\n");
	question_1();
	
	printf("----------question_2  :----------\n");
	question_2();

	printf("----------question_3  :----------\n");
	printf("question is :  %d\n",question_3(1,2,1));
	
	printf("----------question_4  :----------\n");
	question_4();
	
	printf("---------------------------Test End-------------------------------------\n");
	
	return 0;
}
