#include <stdio.h>

int count_int(unsigned int a)
{
		int count = 0;
		
		while(a){
			if(a&0x01){
				count++;
			}
			a>>=1;
		}
		return count;
}

int main()
{
	unsigned int a = 0xff;

	printf("the %x space num'1s is %d\n",a,count_int(a));
	
	return 0;
}