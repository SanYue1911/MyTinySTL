#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H

// ���ͷ�ļ�������һ��ģ���� list
// list : ˫������

// notes:
//
// �쳣��֤��
// mystl::list<T> ��������쳣��֤�����ֺ������쳣��֤���������µȺ�����ǿ�쳣��ȫ��֤��
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
	//list�Ľڵ�
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

