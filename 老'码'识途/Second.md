##庖丁解“码”：底层的力量和乐趣
-----------
**指针的指针**

	#include <stdio.h>
	#include <malloc.h>
	
	
	void allocateInt(int** s)		
	{
	  int * pi = (int *) malloc(sizeof(int));
	  *pi = 3;
	  *s = pi;
	} 
	
	void main()
	{
		int * i;
		allocateInt(&i);
		printf("*i = %d", *i);
	}

//我只是想说 只要把**s 看做是 &i 就可以了<br>
//那么在这里: *s == *(&i) 即 *s = i<br>
//那么这里转换一下就变成了 i = pi，那么pi指向的单元也变为了i指向的单元