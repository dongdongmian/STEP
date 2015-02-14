# struts学习： #
### 六个基本包：###
	struts2-core-2.1.6.jar ：开发的核心类库
	freemarker-2.3.13.jar ：struts2的UI标签的模板使用freemarker编写
	commons-logging-1.0.4.jar ：日志包
	ognl-2.6.11.jar ：对象图导航语言，通过它来读写对象属性
	xwork-2.1.2.jar ：xwork类库，struts2在其上进行构建
	commons-fileupload-1.2.1.jar：文件上传组件，2.1.6版本后必须加入此jar包

### 初识struts2配置文件:
	（1）．web.xml文件  
	主要完成对StrutsPrepareAndExecuteFilter的配置（在以前的版本中是对FilterDispatcher配置，新版本同样支持用FilterDispatcher配置），它的实质是一个过滤器，它负责初始化整个Struts框架并且处理所有的请求。这个过滤器可以包括一些初始化参数，有的参数指定了要加载哪些额外的xml配置文件，还有的会影响struts框架的行为。除了StrutsPrepareAndExecuteFilter外，Struts还提供了一个ActionContexCleanUp类，它的主要任务是当有其它一些过滤器要访问一个初始化好了的struts框架的时候，负责处理一些特殊的清除任务。

	（2）．struts.xml文件  
	框架的核心配置文件就是这个默认的struts.xml文件，在这个默认的配置文件里面我们可以根据需要再包括其它一些配置文件。在通常的应用开发中，我们可能想为每个不同的模块单独配置一个struts.xml文件，这样也利于管理和维护。这也是我们要配置的主要文件。 


	（3）．struts.properties（参default.properties）  
	在Struts框架使用了很多属性，我们可以通过改变这些属性来满足我们的需求。要改变这些属性，只需在struts.properties文件中指定属性的key和value即可。属性文件可以放在任何一个包含在classpath中的路径上，但是通常我们都把它放在/WEB-INF/classes目录下面。我们可以在struts-default.properties文件中找到一个属性的列表。 

	（4）struts-default.xml  
	此文件是struts2框架默认加载的配置文件，它定义了struts2一些核心bean和拦截器，它会自动包含(included)到struts.xml文件中(实质是通过<package  extends="struts-default">)，并为我们提供了一些标准的配置。我们可以在struts2-core.jar中找到这个文件。 

	（5）其它配置文件 
	velocity.properties，struts-default.vm，struts-plugin.xml