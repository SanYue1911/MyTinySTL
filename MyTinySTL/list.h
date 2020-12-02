#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H

// 这个头文件包含了一个模板类 list
// list : 双向链表

// notes:
//
// 异常保证：
// mystl::list<T> 满足基本异常保证，部分函数无异常保证，并对以下等函数做强异常安全保证：
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{
	//list的节点
	template <class T> struct list_node_base;
	template <class T> struct list_node;

	template <class T>
	struct node_traits
	{
		typedef list_node_base<T>* base_ptr;
		typedef list_node<T>* node_ptr;
	};



















}











#endif // !MYTINYSTL_LIST_H

