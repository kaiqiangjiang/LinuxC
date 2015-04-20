 #include <stdio.h>
  
 
 
 //计算任意一个数中 1 的个数   完成
 int main(void)
 {
	int test_num = 12311678;
	int n = 0;
	int test_num_temp = test_num;
	int count_1 = 0; //计算1的个数
	int temp_count = 0;
	
	//取最高位
	while (test_num_temp/10){
		test_num_temp = test_num_temp/10; 
		temp_count++;
	}
	n = temp_count + 1;
	printf("n :%d\n",n);
	
	int temp_array[n];//定义数组存int的每一位
	int temp_array_chu[n];//....1000 100 10...   产生的除数存放地方
	temp_array_chu[0] = 1;//可以确定的
	 
	 //产生 100 10 1 
	for (temp_count = 1;temp_count < n;temp_count++){
		temp_array_chu[temp_count] = 10*temp_array_chu[temp_count-1];
	 }
	 
	temp_array[n - 1] = test_num / temp_array_chu[n - 1];
	test_num = test_num -temp_array_chu[n - 1] * temp_array[n - 1];
	printf("Test Array debug xxxd  %d  \n",test_num);
	
	//
	temp_count = n - 1;
	for (;temp_count > 0;temp_count--){
		temp_array[temp_count -1] = test_num / temp_array_chu[temp_count - 1];
		
		printf("Test Array debug %d除数    is  %d\n",temp_count,temp_array_chu[temp_count - 1]);
		printf("Test Array debug before_test_num  %d  is  %d\n",temp_count,test_num);
		
		test_num = test_num -temp_array_chu[temp_count - 1] * temp_array[temp_count - 1];
		
		printf("Test Array debug after_test_num  %d  is  %d\n",temp_count,test_num);
	}
	
	 // 分别打印每个数 
	 for (temp_count = 0;temp_count < n;temp_count++){
		printf("Test Array %d  is  %d\n",temp_count,temp_array[temp_count]);
	 }
	 //判断 1个数
	 for (temp_count = 0;temp_count < n;temp_count++){
		//printf("Test Array %d  is  %d\n",temp_count,temp_array[temp_count]);
		if (temp_array[temp_count] == 1){
			count_1++;
		}
	 }
	 printf("The num with 1 is   %d\n",count_1);
	 return 0;
 }