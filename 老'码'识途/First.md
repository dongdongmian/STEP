##第一章：欲向码途问大道，铿锵bit是吴刀

-------------------------
**VS新建项目**

文件 -> 新建 -> 项目 -> VC++ -> win32控制台应用程序(修改名称位置)<br> -> win32应用程序向导(控制台应用程序,空项目)

选择项目资源文件(添加->新建项) -> C++文件(.cpp  修改文件名) ->完成

-----------------------
**调试方法**

1. 监视窗口：监视程序中可能用到的变量
2. 设置断点
3. 内存和寄存器的监测
4. 反汇编代码


----------------
**全局变量**

>**基本认识：**

	int gi，i;
	void main(int argc,char* argv[])
	{
		gi = 12;	//C7 05(mov指令) 30 81 0A 01（address:010A8130h  小端机存放方式，数值从小到大存放） 0C 00 00 00	(data)	
		//013A13BE C7 05 30 81 3A 01 0C 00 00 00 mov         dword ptr ds:[013A8130h],0Ch   013A13BE是mov指令存放地址
		//printf("gi address = %x\n",&gi);
	}

>**书中小代码**

	// asmjmp.cpp : 定义控制台应用程序的入口点。
	//
	#include <stdio.h>
	#include <malloc.h>
	int gi;
	void * address;
	
	void * buildCode()
	{
		//我们要构建的代码如下
		// mov gi, 18
		// jmp address;
	
		//mov gi, 被赋值整数；指令是10个字节组成C7 05 gi的地址四字节 被赋值整数四字节
		//jmp address;6字节，前两个为ff 25，后4字节为地址
		//因此我们需要分配16个字节
		char * code = (char *)malloc(16);
		char * pMov = code;
		char * pJmp = code + 10;
		char * pAddress;
		//mov gi, 18
		pMov[0] = 0xc7;
		pMov[1] = 0x05;
		pAddress = pMov + 2;
		*((int *)pAddress) = (int)&gi;
		*((int *)(pAddress + 4)) = 18;
		//jmp address
		pJmp[0] = 0xff;
		pJmp[1] = 0x25;
		*((int *)(&pJmp[2])) = (int)&address;
		return code;
	}
	
	void main()
	{	
		void * code = buildCode();	//获取新构建代码的首地址
		_asm {
			mov address, offset _lb1;	
		}
		gi = 12;
		printf("gi = %d\n", gi);
		_asm jmp code;//执行我们自己构建的代码
		gi = 13;
		_lb1:
		printf("gi = %d\n", gi);//打印的结果为18，而不是12或者13，
								//因为在上面构建的代码会跳转懂啊address处执行
		getchar();
	}

----------------------
**理解指针**

* 指针存储内存的地址，且有类型，但是 sizeof(int *) 只有4字节，但是全局变量的地址就已经占用了4字节
>>解释：其实在指针的内存中只存放了要指向内容的地址，而在编译中会根据这个指针的类型决定在内存中读取和写入的字节数,最终会反应到汇编中的指令中的 dword 和 word 等

**指针强制转换**

* 为啥 short * 指针 ps 和 int * 指针 pi （pi = ps）时发生编译错误</p>

>>pi 是 int 型指针，那么在汇编中赋值指令的时候是 dword   <br>
>>ps 是 short 型指针，那么就是 word ，2个字节          <br>
当 ps 指向的地址给 pi 时，那么 pi 的赋值操作就变成了 2字节却赋值给4字节，导致越界

* 为啥可以强制转换 "pi = (int *)ps"?
>> 强制转换的时候地址放入指针变量中的 指令 是没有差别的  <br>
>> 但是在进行赋值操作的时候，将各个指针变量指向的地址取出，各自赋值的字节大小是不一样的

	eg
	int i;
	int *pi;
	short *ps;
	
	pi = &i;
	ps = (short *)&i;
	//eg: mov dword ptr [pi(417148h)],offset i(41714ch)
	
	*pi = 0x1234;
	*ps = 0x1234;
	//eg:	mov dword [eax],1234h
	//eg:	mov eax,1234h;	mov ecx,dword ptr [pc(417140h)];	mov word ptr [ecx],ax

------------
**函数调用和局部变量**

>1. call 指令有 5 个字节，一个字节是代表自己是 call，其余的4个字节为偏移地址
>2. call 指令要跳转的地址为： call指令地址+指令中的偏移
>3. 本代码采用的传值方式是 压栈 参数压栈位置当然是要反过来988
>4. 压栈是指针减小

>1. 局部变量在程序中是保存在栈中的

	#include <stdio.h>

	int Add(int x,int y){
		int sum;
		sum = x+y;
		return sum;
	}
	
	void main(){
		int z;
		z = Add(1,2);	
		//003B29E2 E8 F9 E7 FF FF   call  Add (03B11E0h)	
		//(F9 E7 FF FF) 是偏移地址的补码，用 003B29E2 即指令地址加上这个偏移地址就是 add 的入口地址，即 03B11E0h，E8指令call
		printf("z = %d\n",z);
	}


---------
**函数指针**

* 函数指针赋值原则：只能将于指针原型匹配的函数入口地址赋给它

--------------------

**数组**

* 数组为什么是以0开始为索引？
>> 因为数组在进行查找操作的时候可以直接以 下标*字节 寻址<br>

* 为什么C语言容易发生数组越界错误呢？
>> 数组寻址的时候是根据首地址+偏移量，如果偏移量超过了范围就会发生越界错误

**结构体**

>**?**

	struct Person{
		int age;
		char no;
		char a;
	};
	//为什么在这两个结构体中得到其占用的空间都是8？
	struct Person{
		int age;
		char no;
	};
>>解答：这便是对齐啊
><br>在结构体中变量的内存是挨着分配的，一般分配为偶字节数，最后字节是4的倍数，如果像是int，char5个字节，分配8个字节；<br>
>但是如果是short，char[2],这样加起来刚好为4个字节，则只分配4个字节就可以了。<br>
>但如果是 int char int char 这样分开写的，则要占用16个字节，int int char char只占用12个字节，还是比较讲究的！

>结构体按4字节对齐<br>
>结构体在内存中没有特殊性，只是编译器提供给我们的一个自动求取偏移量的方法