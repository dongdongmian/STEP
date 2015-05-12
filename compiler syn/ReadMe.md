## 程序说明
		在本程序中，需要有一个输入文件，文件的名字在 realmain 主函数中定义

		realmain 函数将文件名传到 LEX.c 函数中

		LEX.c 函数主要的功能是将读入的文件根据  token.c 中的定义，将输入的各类型转换为对应的 token 字
	并将 token 的返回值保存在一个 结构体链表 中

		LEX.c 函数 返回链表头指针，作为 grammer.c 函数的输入

		grammer.c 函数的 action_table 和 goto_table 是由读入的 1.txt 文档建立而成
		1.txt 是由 1.python 处理 1.html 文件而来

		1.html 由编译工作台根据自己定义的语法规则，生成的 LALR（1）表

		语法规则为  .grm   表格输出为：LALR分析表.htm 即1.html

		
---------
注：

	作为输入的程序中，应注意不让两个符号紧挨着输入，否则识别为 unknow token
	
	其他的要根据具体的语法规则来输入！