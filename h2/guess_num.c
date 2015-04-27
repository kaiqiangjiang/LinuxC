#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	printf("-----------------------This is 思维逻辑练习题 01-------------------\n");
	
	int in_data;
//	int set_data = 57;
	
	int set_data = 0;
	srand((int)time(NULL));
	set_data = rand()%100;	
	
	printf("输入数字【猜数字游戏0-100】：\n ");
	printf("-----------------------------\n");
	scanf("%d",&in_data);
	printf("你猜的数字是 %d\n",in_data);
	//printf("-----------------------------\n ");
	/*if (in_data > in=set_data){
		printf("Sorry BIG!!!\n ");
	}else if (in_data < set_data) {
		printf("Sorry little!!!\n ");
	}else {
		printf("Well done!\n");
	}*/
	int count = 0;
	while(!(in_data == set_data)){
		if (7 == count){
			printf("没有猜对哦！");
			break;
		}
		printf("你猜数字比较");
		(in_data > set_data) ? printf("大\n"):printf("小\n");
		printf("请继续输入.........\n");
		count++;
		scanf("%d",&in_data);
	}
	if (count < 8){
		printf("Right set_data is %d\n",set_data);
	}
	printf("---------------------------Test End-----------------------------\n");
	
	return 0;
}
