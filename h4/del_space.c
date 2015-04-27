#include <stdio.h>


void fun1(void)
{
	char hh[] = "a d";
	char p[]  = "J o J d";   //删除字字符串空格  间隔一格空格可以删，多个删不了。。后面改成fun2了
	int n = 0;
	int temp = 0;
	while (!(p[n] == '\0')){
		if (p[n] == hh[1]){   //如果是空格将后面的字符全部移动一格
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
	char *head = p;    //保留开头
	char *hh = "a d";
	
	while (!(*p == '\0')){  //将非空字符转移到p_temp中
		if (!(*p == hh[1])){
			*p_temp = *p;
			*p_temp++ ;
		}
		p++;
	}
	*p_temp = '\0';//补尾巴
	
	p_temp = head;//回到开头
	printf("del ok : %s\n",p_temp);
}
int main(void) 
{
	char Str_test[] = "dss   s a j k";
	
	fun1();
	
	fun2(Str_test);
	
	return 0;
}
