#ifndef MYTINYSTL_UTIL_H
#define MYTINYSTL_UTIL_H

#include<cstddef>
#include"type_traits.h"

namespace mystl
{
	//move函数将参数强制转换为右值
	template<class T>
	typename std::remove_reference<T>::type&& move(T&& arg) noexcept 
	{
		return static_cast<typename std::remove_reference<T>::type&&>(arg);
	}

	// forward
	// 使用move（）作为参数时会无法调用报错，所以使用forward（）作为过渡使用
	template <class T>
	T&& forward(typename std::remove_reference<T>::type& arg) noexcept
	{
		return static_cast<T&&>(arg);
	}

	//std::is_lvalue_reference<T>::value判断T是否为引用，T&返回ture,T&&和T返回false
	template <class T>
	T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
	{
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
		return static_cast<T&&>(arg);
	}

	//swap
	template <class Tp>
	void swap(Tp& lhs, Tp& rhs)
	{
		auto tmp(mystl::move(lhs));
		lhs = mystl::move(rhs);
		rhs = mystl::move(tmp);
	}

	template <class ForwardIter1, class ForwardIter2>
	ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
	{
		for (; first1 != last1; ++first1, (void) ++first2)
			mystl::swap(*first1, *first2);
		return first2;
	}

	template <class Tp, size_t N>
	void swap(Tp(&a)[N], Tp(&b)[N])
	{
		mystl::swap_range(a, a + N, b);
	}

	// 结构体模板 : pair
    // 两个模板参数分别表示两个数据的类型
	// 用 first 和 second 来分别取出第一个数据和第二个数据
	template<class Ty1,class Ty2>
	struct pair 
	{
		typedef Ty1 first_type;
		typedef Ty2 second_type;

		first_type first;    // 保存第一个数据
		second_type second;  // 保存第二个数据

		// std::is_default_constructible判断是否有默认构造函数
		// enble_if限定必须为ture，否则报错
		template <class Other1 = Ty1, class Other2 = Ty2,
		typename = typename std::enable_if<std::is_default_constructible<Other1>::value&&
		std::is_default_constructible<Other2>::value, void>::type>
		constexpr pair()
		: first(), second()
		{
		}

		//std::is_copy_constructible判断是否具有拷贝构造函数
		template <class U1 = Ty1, class U2 = Ty2,
			typename std::enable_if<
			std::is_copy_constructible<U1>::value&&
			std::is_copy_constructible<U2>::value&&
			std::is_convertible<const U1&, Ty1>::value&&
			std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
			constexpr pair(const Ty1& a, const Ty2& b)
			: first(a), second(b)
		{
		}

		// 若std::is_convertible返回false则禁止隐式转换
		template <class U1 = Ty1, class U2 = Ty2,
			typename std::enable_if<
			std::is_copy_constructible<U1>::value&&
			std::is_copy_constructible<U2>::value &&
			(!std::is_convertible<const U1&, Ty1>::value ||
			!std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
			explicit constexpr pair(const Ty1& a, const Ty2& b)
			: first(a), second(b)
		{
		}

		pair(const pair& rhs) = default;
		pair(pair && rhs) = default;

		//std::is_constructible第一个参数类型可否由剩余参数（转换或代入参数调用第一个参数的构造函数）
		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty2, Other2>::value&&
			std::is_convertible<Other1&&, Ty1>::value&&
			std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
			constexpr pair(Other1&& a, Other2&& b)
			: first(mystl::forward<Other1>(a)),
			second(mystl::forward<Other2>(b))
		{
		}

		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty2, Other2>::value &&
			(!std::is_convertible<Other1, Ty1>::value ||
			!std::is_convertible<Other2, Ty2>::value), int>::type = 0>
			explicit constexpr pair(Other1&& a, Other2&& b)
			: first(mystl::forward<Other1>(a)),
			second(mystl::forward<Other2>(b))
		{
		}

		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, const Other1&>::value&&
			std::is_constructible<Ty2, const Other2&>::value&&
			std::is_convertible<const Other1&, Ty1>::value&&
			std::is_convertible<const Other2&, Ty2>::value, int>::type = 0>
			constexpr pair(const pair<Other1, Other2>& other)
			: first(other.first),
			second(other.second)
		{
		}

		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, const Other1&>::value&&
			std::is_constructible<Ty2, const Other2&>::value &&
			(!std::is_convertible<const Other1&, Ty1>::value ||
				!std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
			explicit constexpr pair(const pair<Other1, Other2>& other)
			: first(other.first),
			second(other.second)
		{
		}

		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty2, Other2>::value&&
			std::is_convertible<Other1, Ty1>::value&&
			std::is_convertible<Other2, Ty2>::value, int>::type = 0>
			constexpr pair(pair<Other1, Other2>&& other)
			: first(mystl::forward<Other1>(other.first)),
			second(mystl::forward<Other2>(other.second))
		{
		}

		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty2, Other2>::value &&
			(!std::is_convertible<Other1, Ty1>::value ||
			!std::is_convertible<Other2, Ty2>::value), int>::type = 0>
			explicit constexpr pair(pair<Other1, Other2>&& other)
			: first(mystl::forward<Other1>(other.first)),
			second(mystl::forward<Other2>(other.second))
		{
		}

		pair& operator=(const pair& rhs)
		{
			if (rhs != this)
			{
				first = rhs.first;
				second = rhs.second;
			}
			return *this;
		}

		pair& operator=(pair&& rhs)
		{
			if (this != &rhs)
			{
				first = mystl::move(rhs.first);
				second = mystl::move(rhs.second);
			}
			return *this;
		}

		template <class Other1, class Other2>
		pair& operator=(const pair<Other1, Other2>& other)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		template <class Other1, class Other2>
		pair& operator=(pair<Other1, Other2>&& other)
		{
			first = mystl::forward<Other1>(other.first);
			second = mystl::forward<Other2>(other.second);
			return *this;
		}

		~pair() = default;

		void swap(pair & other)
		{
			if (this != &other)
			{
				mystl::swap(first, other.first);
				mystl::swap(second, other.second);
			}
		}
	};

	// 重载比较操作符 
	template <class Ty1, class Ty2>
	bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template <class Ty1, class Ty2>
	bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
	{
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	}

	template <class Ty1, class Ty2>
	bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Ty1, class Ty2>
	bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
	{
		return rhs < lhs;
	}

	template <class Ty1, class Ty2>
	bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class Ty1, class Ty2>
	bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
	{
		return !(lhs < rhs);
	}

	// 重载 mystl 的 swap
	template <class Ty1, class Ty2>
	void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs)
	{
		lhs.swap(rhs);
	}

	// 全局函数，让两个数据成为一个 pair
	template <class Ty1, class Ty2>
	pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second)
	{
		return pair<Ty1, Ty2>(mystl::forward<Ty1>(first), mystl::forward<Ty2>(second));
	}

}

#endif //MYTINYSTL_UTIL_H