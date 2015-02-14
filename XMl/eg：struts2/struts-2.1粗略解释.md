### struts-2.1.dtd 粗略解释： ###
	<!ELEMENT struts ((package|include|bean|constant)*, unknown-handler-stack?)>

	'()' 	将struts元素分为两组
	'|'		隔开的元素至少从中选一个
	'*'		表示元素可出现 0-n 次. 
	'?' 	列出的元素可以出现，但是只能出现一次
	','		列出的元素必须按顺序出现


	<!ATTLIST package
	    name CDATA #REQUIRED
	    extends CDATA #IMPLIED
	    namespace CDATA #IMPLIED
	    abstract CDATA #IMPLIED
	    externalReferenceResolver NMTOKEN #IMPLIED
	>
	ATTLIST 属性定义标志
	属性：
		CDATA	属性值为普通的字符数据
		NMTOKEN	属性值只能由字母，数字，下划线，连字符，圆点，冒号等字符组成
	附加属性：
		#REQUIRED	该元素的标记中必须给出此属性的属性值
		#IMPLIED	该元素的标记中可以不出现此属性
		