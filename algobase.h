#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H

#include<cstring>
#include<type_traits>

#include"iterator.h"
#include"util.h"

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

	//max
	//max���ؽϴ��ߣ����ʱ���ص�һ������
	template <class T>
	const T& max(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? rhs : lhs;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class T, class Compare>
	const T& max(const T& lhs, const T& rhs, Compare comp)
	{
		return comp(lhs, rhs) ? rhs : lhs;
	}

	//min
	//min���ؽ�С�ţ����ʱ���ص�һ������
	template <class T>
	const T& min(const T& lhs, const T& rhs)
	{
		return rhs < lhs ? rhs : lhs;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class T, class Compare>
	const T& min(const T& lhs, const T& rhs, Compare comp)
	{
		return comp(rhs, lhs) ? rhs : lhs;
	}

	// iter_swap
	// ��������������ָ����Ե�
	template <class FIter1, class FIter2>
	void iter_swap(FIter1 lhs, FIter2 rhs)
	{
		mystl::swap(*lhs, *rhs);
	}

	//************************************************************************
	// copy
    // �� [first, last)�����ڵ�Ԫ�ؿ����� [result, result + (last - first))��

	// input_iterator_tag �汾
	template <class InputIter, class OutputIter>
	OutputIter
	unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
	mystl::input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
		{
			*result = *first;
		}
		return result;
	}

	// ramdom_access_iterator_tag �汾
	template <class RandomIter, class OutputIter>
	OutputIter
	unchecked_copy_cat(RandomIter first, RandomIter last, OutputIter result,
	mystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n, ++first, ++result)
		{
			*result = *first;
		}
		return result;
	}

	template <class InputIter, class OutputIter>
	OutputIter
	unchecked_copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy_cat(first, last, result, iterator_category(first));
	}

	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
	template <class Tp, class Up>
	typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
	std::is_trivially_copy_assignable<Up>::value,
	Up*>::type
	unchecked_copy(Tp* first, Tp* last, Up* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up));
		return result + n;
	}

	template <class InputIter, class OutputIter>
	OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy(first, last, result);
	}

	//************************************************************************
	// copy_backward
	// �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��

	// unchecked_copy_backward_cat �� bidirectional_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
	{
		while (first != last)
			*--result = *--last;
		return result;
	}

	// unchecked_copy_backward_cat �� random_access_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result, mystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
			*--result = *--last;
		return result;
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result)
	{
		return unchecked_copy_backward_cat(first, last, result,iterator_category(first));
	}

	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
	template <class Tp, class Up>
	typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
	std::is_trivially_copy_assignable<Up>::value,
	Up*>::type
	unchecked_copy_backward(Tp* first, Tp* last, Up* result)
	{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
		{
			result -= n;
			std::memmove(result, first, n * sizeof(Up));
		}
		return result;
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_copy_backward(first, last, result);
	}

	//************************************************************************
	// copy_if
	// ��[first, last)������һԪ���� unary_pred ��Ԫ�ؿ������� result Ϊ��ʼ��λ����
	template <class InputIter, class OutputIter, class UnaryPredicate>
	OutputIter
	copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				*result++ = *first;
		}
		return result;
	}

	//************************************************************************
	// copy_n
	// �� [first, first + n)�����ϵ�Ԫ�ؿ����� [result, result + n)��
	// ����һ�� pair �ֱ�ָ�򿽱�������β��
	template <class InputIter, class Size, class OutputIter>
	mystl::pair<InputIter, OutputIter>
	unchecked_copy_n(InputIter first, Size n, OutputIter result, mystl::input_iterator_tag)
	{
		for (; n > 0; --n, ++first, ++result)
		{
			*result = *first;
		}
		return mystl::pair<InputIter, OutputIter>(first, result);
	}

	template <class RandomIter, class Size, class OutputIter>
	mystl::pair<RandomIter, OutputIter>
	unchecked_copy_n(RandomIter first, Size n, OutputIter result,mystl::random_access_iterator_tag)
	{
		auto last = first + n;
		return mystl::pair<RandomIter, OutputIter>(last, mystl::copy(first, last, result));
	}

	template <class InputIter, class Size, class OutputIter>
	mystl::pair<InputIter, OutputIter>
		copy_n(InputIter first, Size n, OutputIter result)
	{
		return unchecked_copy_n(first, n, result, iterator_category(first));
	}

	//************************************************************************
	// move
	// �� [first, last)�����ڵ�Ԫ���ƶ��� [result, result + (last - first))��
	// input_iterator_tag �汾
	template <class InputIter, class OutputIter>
	OutputIter
	unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
	mystl::input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
		{
			*result = mystl::move(*first);
		}
		return result;
	}

	// ramdom_access_iterator_tag �汾
	template <class RandomIter, class OutputIter>
	OutputIter
	unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
	mystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n, ++first, ++result)
		{
			*result = mystl::move(*first);
		}
		return result;
	}

	template <class InputIter, class OutputIter>
	OutputIter
	unchecked_move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move_cat(first, last, result, iterator_category(first));
	}

	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
	template <class Tp, class Up>
	typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
	std::is_trivially_move_assignable<Up>::value,
	Up*>::type
	unchecked_move(Tp* first, Tp* last, Up* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up));
		return result + n;
	}

	template <class InputIter, class OutputIter>
	OutputIter move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move(first, last, result);
	}

	//************************************************************************
	// move_backward
	// �� [first, last)�����ڵ�Ԫ���ƶ��� [result - (last - first), result)��

	// bidirectional_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result, mystl::bidirectional_iterator_tag)
	{
		while (first != last)
			*--result = mystl::move(*--last);
		return result;
	}

	// random_access_iterator_tag �汾
	template <class RandomIter1, class RandomIter2>
	RandomIter2
	unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
	RandomIter2 result, mystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
			*--result = mystl::move(*--last);
		return result;
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result)
	{
		return unchecked_move_backward_cat(first, last, result,iterator_category(first));
	}

	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
	template <class Tp, class Up>
	typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
	std::is_trivially_move_assignable<Up>::value,
	Up*>::type
	unchecked_move_backward(Tp* first, Tp* last, Up* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n != 0)
		{
			result -= n;
			std::memmove(result, first, n * sizeof(Up));
		}
		return result;
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
	move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_move_backward(first, last, result);
	}

	//************************************************************************
	// equal
	// �Ƚϵ�һ������ [first, last)�����ϵ�Ԫ��ֵ�Ƿ�͵ڶ��������
	template <class InputIter1, class InputIter2>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
				return false;
		}
		return true;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class InputIter1, class InputIter2, class Compared>
	bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!comp(*first1, *first2))
				return false;
		}
		return true;
	}

	//************************************************************************
	// fill_n
	// �� first λ�ÿ�ʼ��� n ��ֵ
	template <class OutputIter, class Size, class T>
	OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
		{
			*first = value;
		}
		return first;
	}

	// Ϊ one-byte �����ṩ�ػ��汾
	template <class Tp, class Size, class Up>
	typename std::enable_if<
	std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
	std::is_integral<Up>::value && sizeof(Up) == 1 &&
	!std::is_same<Tp, bool>::value,
	Tp*>::type
	unchecked_fill_n(Tp* first, Size n, Up value)
	{
		if (n > 0)
		{
			std::memset(first, (unsigned char)value, (size_t)(n));
		}
		return first + n;
	}

	template <class OutputIter, class Size, class T>
	OutputIter fill_n(OutputIter first, Size n, const T& value)
	{
		return unchecked_fill_n(first, n, value);
	}

	//************************************************************************
	// fill
	// Ϊ [first, last)�����ڵ�����Ԫ�������ֵ
	template <class ForwardIter, class T>
	void fill_cat(ForwardIter first, ForwardIter last, const T& value,
	mystl::forward_iterator_tag)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}

	template <class RandomIter, class T>
	void fill_cat(RandomIter first, RandomIter last, const T& value,
	mystl::random_access_iterator_tag)
	{
		fill_n(first, last - first, value);
	}

	template <class ForwardIter, class T>
	void fill(ForwardIter first, ForwardIter last, const T& value)
	{
		fill_cat(first, last, value, iterator_category(first));
	}

	//************************************************************************
	// mismatch
    // ƽ�бȽ��������У��ҵ���һ��ʧ���Ԫ�أ�����һ�Ե��������ֱ�ָ������������ʧ���Ԫ��
	template <class InputIter1, class InputIter2>
	mystl::pair<InputIter1, InputIter2>
		mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return mystl::pair<InputIter1, InputIter2>(first1, first2);
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class InputIter1, class InputIter2, class Compred>
	mystl::pair<InputIter1, InputIter2>
		mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp)
	{
		while (first1 != last1 && comp(*first1, *first2))
		{
			++first1;
			++first2;
		}
		return mystl::pair<InputIter1, InputIter2>(first1, first2);
	}























}



#endif // !MYTINYSTL_ALGOBASE_H
