#ifndef MYTINYSTL_VRCTOR_H
#define MYTINYSTL_VRCTOR_H

#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"


namespace mystl
{

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

	template<class T>
	class vector
	{
		static_assert(!std::is_same<bool, T>::value, "vector<bool> is abandoned in mystl");
	public:

		typedef mystl::allocator<T>                       allocator_type;





	};








}














#endif // !MYTINYSTL_VRCTOR_H
