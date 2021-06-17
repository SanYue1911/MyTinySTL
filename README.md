# MyTinySTL
此库仅是记录学习《源码剖析》所阅读理解的代码 


原项目https://github.com/Alinshans/MyTinySTL


STL概述
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

1、空间配置器

	construct.h ：
对应对象的构造和析构。

	allocater.h ：
对应内存空间的分配和释放。（只做了简单的封装）

	uninitialized.h ：
定义了六个全局函数用与fill或copy大块内存数据。

	memory.h ：
配置器定义于此文件。

2、迭代器

	typr_traits.h：
用于提取类型信息。

	iterator.h：
定义了迭代器的五种型别，并使用typetraits萃取类型的型别。重载了指针移动及定位符号。

3、序列式容器

	vector.h：
	vector采用线性连续空间的数据结构，它以两个迭代器start和finish分别指向配置得来的连续空间中目前已被使用的围
，并以迭代器cap_指向整块连续空间(含备用空间)的尾端。支持随机数据访问。vector和array不同，采用和静态空间不同的动态空间，书中所述SGI空间配置策略为空间满载时将空间扩充为二倍甚至更多，但此文件中分配空间大小由get_new_cap（）函数决定。

	list.h：
	相对于vector的连续线性空间，list就显得复杂许多，它的好处就是插入或删除一个元素，就配置或删除一个元素空间。
对于任何位置的元素的插入或删除，list永远是常数时间。此文件实现的是双向链表，由一个迭代器node_指向尾节点，node_ -> next指向头节点。
  
	deque.h：
	deque和vector的最大差异，一在于deque允许常数时间内对起头端进行插入或移除操作，二在于deque没有所谓容量(capa
city)概念，因为它是以分段连续空间组合而成，随时可以增加一段新的空间连接起来。
	deque由一段一段连续空间组成，一旦有必要在deque的前端或尾端增加新空间，便配置一段连续空间，串接在整个deque
的前端或尾端。deque的最大任务，便是在这些分段的连续空间上，维护其整体连续的假象，并提供随机存取的接口，避开了“重新配置、复制、释放”的轮回，代价是复杂的迭代器结构。
	每个缓冲区（buffer）的大小为4096，缓冲区的个数至少为8个，由四个迭代器表示deque，begin_指向第一个节点，end_
指向最后一个节点，map_指向一块 map，map 中的每个元素都是一个指针，指向一个缓冲区，size_type map内指针的数目。
	
	stack.h：
	stack是一种先进后出FILO的数据结构，它只有一个出口。stack允许增加元素、移除元素、取得最顶端元素。但除了最顶
端外，没有任何其他方法可以存取，stack的其他元素，换言之，stack不允许有遍历行为。stack默认以deque为底层容器。
	
	queue.h：
	queue是一种先进先出FIFO的数据结构，它有两个出口，允许增加元素、移除元素、从最底端加入元素、取得最顶端元素
。但除了最底端可以加入、最顶端可以取出外，没有任何其他方法可以存取queue的其他元素，换言之，queue不允许有遍历行为。queue默认以deque为底层容器(此文件还包括一种以vector为底层容器的queue)。
	
	heap.h：
	heap并不归属于STL容器组件，它是个幕后英雄，扮演prority queue的助手。priority queue 
允许用户以任何次序将任何 元素推入容器内，但取出时一定是按照优先级最高的元素开始取。binary max heap正好具有这样的特性，适合作为 priority queue的底层机制。heap默认建立的是大堆。这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap
	
4、关联性容器

	rb_tree.h:
	红黑树是平衡树的一种，保证最坏情况下操作时间复杂度为O(lgo(n))。红黑树的应用比较广泛，比如作为C++中STL的se
和map的底层数据结构。
	rb树必须满足以下规则:
(1)每个节点是红色的，或者是黑色的
(2)根节点是黑色的
(3)如果一个节点是红色的，则它的两个子节点都是黑色的
(4)对每个节点，从该节点到其后代叶节点的简单路径上，均包含相同数目的黑色节点

	红黑树重要的操作旋转、插入、删除等都实现在此文件中。
  
	set.h：
	set的所有元素都会根据元素的键值自动排序。set的元素不像map那样可以同时拥有实值(value)和键值(key)，set元素的
键值就是实值，实值就是键值，set不允许有两个相同的元素。Set元素不能改变，在set源码中，set<T>::iterator被定义为
底层TB-tree的const_iterator，杜绝写入操作，也就是说，set iterator是一种constant iterators。	    		
	此文件还包括了multiset，multiset的特性以及用法和set完全相同，唯一的差别在于它允许键值重复，因此它的插入操
作采用的是底层机制RB-tree的insert_multi()而非insert_unique()。

	map.h：
	map的所有元素都会根据元素的键值自动排序。map的所有元素都是pair，同时拥有实值(value)和键值(key)。pair的第一
元素为键值，第二元素为实值。map不允许有两个相同的键值。
　　如果通过map的迭代器改变元素的键值，这样是不行的，因为map元素的键值关系到map元素的排列规则。任意改变map元素
键值都会破坏map组织。如果修改元素的实值，这是可以的，因为map元素的实值不影响map元素的排列规则。因此，map 
iterator既不是一种constant iterators，也不是一种mutable iterators。
	此文件还包括了multimap，multimap的特性以及用法和map完全相同，唯一的差别在于它允许键值重复，因此它的插入操
作采用的是底层机制RB-tree的insert_multi()而非insert_unique()。










	
	