 #include <stdio.h>
 
 //��������һ������   1234  to  4321 
 int main(void)
 {
	int test_num = 12345678;
	int n = 0;
	int test_num_temp = test_num;
	int count_1 = 0; //����1�ĸ���
	int temp_count = 0;
	int ok_int = 0;
	
	
	//ȡ���λ
	while (test_num_temp/10){
		test_num_temp = test_num_temp/10; 
		temp_count++;
	}
	n = temp_count + 1;
	//printf("n :%d\n",n);
	
	int temp_array[n];//���������int��ÿһλ
	int temp_array_chu[n];//....1000 100 10...   �����ĳ�����ŵط�
	temp_array_chu[0] = 1;//����ȷ����
	 
	 //���� 100 10 1 
	for (temp_count = 1;temp_count < n;temp_count++){
		temp_array_chu[temp_count] = 10*temp_array_chu[temp_count-1];
	 }
	 
	temp_array[n - 1] = test_num / temp_array_chu[n - 1];
	test_num = test_num -temp_array_chu[n - 1] * temp_array[n - 1];
	//printf("Test Array debug xxxd  %d  \n",test_num);
	
	//����λ��
	temp_count = n - 1;
	for (;temp_count > 0;temp_count--){
		temp_array[temp_count -1] = test_num / temp_array_chu[temp_count - 1];
		
		//printf("Test Array debug %d����    is  %d\n",temp_count,temp_array_chu[temp_count - 1]);
		//printf("Test Array debug before_test_num  %d  is  %d\n",temp_count,test_num);
		
		test_num = test_num -temp_array_chu[temp_count - 1] * temp_array[temp_count - 1];
		
		//printf("Test Array debug after_test_num  %d  is  %d\n",temp_count,test_num);
	}
	
	 // �ֱ��ӡÿ���� 
	 for (temp_count = 0;temp_count < n;temp_count++){
		printf("Test Array %d  is  %d\n",temp_count,temp_array[temp_count]);
	 }
	 
	 //1234 to  4321 
	 for (temp_count = 0;temp_count < n;temp_count++){
		 ok_int = ok_int + temp_array[temp_count] * temp_array_chu[(n - temp_count - 1)];
	 }

	 
	 printf("The num 1234to4321 is    %d\n",ok_int);
	 return 0;
 }