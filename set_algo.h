#ifndef MYTINYSTL_SET_ALGO_H_
#define MYTINYSTL_SET_ALGO_H_

// ���ͷ�ļ����� set �������㷨: union, intersection, difference, symmetric_difference
// ���к�����Ҫ����������

#include "algobase.h"
#include "iterator.h"

namespace mystl
{

    /*****************************************************************************************/
    // set_union
    // ���� S1��S2 �Ľ�������浽 result �У�����һ��������ָ����������β��
    /*****************************************************************************************/
    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_union(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                *result = *first1;
                ++first1;
            }
            else if (*first2 < *first1)
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        // ��ʣ��Ԫ�ؿ����� result
        return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_union(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result, Compared comp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first1, *first2))
            {
                *result = *first1;
                ++first1;
            }
            else if (comp(*first2, *first1))
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
            }
            ++result;
        }
        // ��ʣ��Ԫ�ؿ����� result
        return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
    }

    /*****************************************************************************************/
    // set_intersection
    // ���� S1��S2 �Ľ�������浽 result �У�����һ��������ָ����������β��
    /*****************************************************************************************/
    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                ++first1;
            }
            else if (*first2 < *first1)
            {
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_intersection(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result, Compared comp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first1, *first2))
            {
                ++first1;
            }
            else if (comp(*first2, *first1))
            {
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
                ++first2;
                ++result;
            }
        }
        return result;
    }

    /*****************************************************************************************/
    // set_difference
    // ���� S1-S2 �Ľ�������浽 result �У�����һ��������ָ����������β��
    /*****************************************************************************************/
    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (*first2 < *first1)
            {
                ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mystl::copy(first1, last1, result);
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_difference(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result, Compared comp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first1, *first2))
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (comp(*first2, *first1))
            {
                ++first2;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mystl::copy(first1, last1, result);
    }

    /*****************************************************************************************/
    // set_symmetric_difference
    // ���� (S1-S2)��(S2-S1) �Ľ�������浽 result �У�����һ��������ָ����������β��
    /*****************************************************************************************/
    template <class InputIter1, class InputIter2, class OutputIter>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (*first2 < *first1)
            {
                *result = *first2;
                ++first2;
                ++result;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
    }

    // ���ذ汾ʹ�ú������� comp ����Ƚϲ���
    template <class InputIter1, class InputIter2, class OutputIter, class Compared>
    OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1,
        InputIter2 first2, InputIter2 last2,
        OutputIter result, Compared comp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first1, *first2))
            {
                *result = *first1;
                ++first1;
                ++result;
            }
            else if (comp(*first2, *first1))
            {
                *result = *first2;
                ++first2;
                ++result;
            }
            else
            {
                ++first1;
                ++first2;
            }
        }
        return mystl::copy(first2, last2, mystl::copy(first1, last1, result));
    }

} // namespace mystl
#endif // !MYTINYSTL_SET_ALGO_H_