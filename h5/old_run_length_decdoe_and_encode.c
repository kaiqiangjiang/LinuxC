/*
*   游程编码问题
*/
#include <stdio.h>

#define SIZE 10


unsigned int ok_encode_src[SIZE];
unsigned int ok_decode_src[SIZE];
int g_size = 0;

void encoded(unsigned int src[],int arr_size)
{
	int i =0;
	int n = 0;
	int m = 0;
	int p_count = 0;
	int temp =0;
	
	unsigned int data1 = 0;
	unsigned int data2 = 0;
	int p[arr_size];
	unsigned int encode_src[arr_size];
	//unsigned int ok_encode_src[arr_size];
	//int *p = (int *)malloc(sizeof(int)*arr_size)
	n=m=0;
	i = 0;
	
	for (i = 0;i < arr_size;i++){
		printf("before encode is:%p\n",src[i]);
	}
	i = 0;
	do {
		data1 = src[i];
		i++;
		n++;
		if(data1 != data2){
			if(n != 1){	
				p[p_count++] = n-m;
				//printf("p_count num is:%d\n",p_count);
				encode_src[p_count-1] = data2;
			}
			m = n;
		}
		data2 =data1;
	}while(i <= arr_size);
//debug of king
/* for (i = 0;i < p_count; i++){    //debug
		printf("code :%d\n",p[i]);
	 }
	 for (i = 0;i < p_count; i++){
		printf("code :%p\n",encode_src[i]);
	 }*/
	 //组合
	 for (i = 0;i < (p_count + p_count);(i = i+2)){
		//printf("code :%d\n",i);
		//2
		ok_encode_src[i] = encode_src[temp];
		temp++;
	 }
	 temp = 0;
	 for (i = 1;i < (p_count + p_count);(i = i+2)){
		//printf("code :%d\n",i);
		//1
		ok_encode_src[i] = p[temp];
		temp++;	
	 }
	 for (i = 0;i < (p_count + p_count); i++){
		printf("after encode is  :%p\n",ok_encode_src[i]);
	 }
	 g_size = p_count + p_count;
	 //printf("g_size is %d\n",g_size);
}

void decode(void)
{
	int i =0;
	int n = 0;
	int m = 0;
	printf("start decode\n");
	//printf("g_size is %d\n",g_size);
	 for (i = 0;i < g_size;i++){
		if ((i % 2 ) == 0 ){
			//printf("%d\n",i);
			ok_decode_src[n] = ok_encode_src[i];
			n++;
		}else{
			//printf("%d\n",i);
			if(ok_encode_src[i] != 1){
				//printf("okkkk\n");
				for (m = 1;m < ok_encode_src[i];m++){
				   // printf("%p\n",ok_encode_src[i-1]);
					ok_decode_src[n] = ok_encode_src[i-1];
					n++;
				}
			}
		}
	 }
	 for (i = 0;i < SIZE; i++){
		printf("after decode is  :%p\n",ok_decode_src[i]);
	 }
}
int main(int argc,char *argv[])
{	
	//定义变量区
	unsigned int p[SIZE] = {0x11,0x11,0x11,0x22,0x33,0x33,0x33,0x33,0x33,0x55};
	//printf("p is :%d\n",sizeof(p));
	//测试子函数功能区
	encoded(p,SIZE);
	
	decode();
	//检验区-----打印
	return 0;
}








