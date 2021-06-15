# MyTinySTL
此库仅是记录学习《源码剖析》所阅读理解的代码 


原项目https://github.com/Alinshans/MyTinySTL


1、STL概述
STL提供六大组件，彼此可以组合套用：

	容器（Containers）：
各种数据结构，如：vector、list、deque、set、map。用来存放数据。从实现的角度来看，STL容器是一种class template。

	算法（algorithms）：
各种常用算法，如：sort、search、copy、erase。从实现的角度来看，STL算法是一种 function template。

	迭代器（iterators）：
容器与算法之间的胶合剂，是所谓的“泛型指针”。共有五种类型，以及其他衍生变化。从实现的角度来看，迭代器是一种将 operator*、operator->、operator++、operator--等指针相关操作进行重载的classtemplate。所有STL容器都有自己专属的迭代器，只有容器本身才知道如何遍历自己的元素。原生指针(native pointer)也是一种迭代器。

	仿函数（functors）：
行为类似函数，可作为算法的某种策略（policy）。从实现的角度来看，仿函数是一种重载了operator()的class或class template。一般的函数指针也可视为狭义的仿函数。

	配接器（adapters）：
一种用来修饰容器、仿函数、迭代器接口的东西。例如：STL提供的queue和stack，虽然看似容器，但其实只能算是一种容器配接器，因为它们的底部完全借助deque，所有操作都由底层的deque供应。改变 functors接口者，称为function adapter；改变 container 接口者，称为container adapter；改变iterator接口者，称为iterator adapter。

	配置器（allocators）：
负责空间配置与管理。从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的class template。

2、空间配置器
	construct.h 
对应对象的构造和析构。
	allocater.h 
对应内存空间的分配和释放。（只做了简单的封装）
	uninitialized.h 
定义了六个全局函数用与fill或copy大块内存数据。
	memory.h 
配置器定义于此文件。

3、迭代器
	typr_traits.h
用于提取类型信息
	iterator.h
定义了迭代器的五种型别，并使用typetraits萃取类型的型别。重载了指针移动及定位符号。

4、序列式容器
	vector.h
vector采用线性连续空间的数据结构，它以两个迭代器start和finish分别指向配置得来的连续空间中目前已被使用的围，并以迭代器cap_指向整块连续空间(含备用空间)的尾端。支持随机数据访问。vector和array不同，采用和静态空间不同的动态空间，书中所述SGI空间配置策略为空间满载时将空间扩充为二倍甚至更多，但此文件中分配空间大小由get_new_cap（）函数决定。
	list.h
相对于vector的连续线性空间，list就显得复杂许多，它的好处就是插入或删除一个元素，就配置或删除一个元素空间。对于任何位置的元素的插入或删除，list永远是常数时间。此文件实现的是双向链表，由一个迭代器node_指向尾节点，node_ -> next指向头节点。
  
    
	
	
	
	
	
	
	
	
	