#ifndef MYTINYSTL_DEQUE_H
#define MYTINYSTL_DEQUE_H

// ���ͷ�ļ�������һ��ģ���� deque
// deque: ˫�˶���

// notes:
//
// �쳣��֤��
// mystl::deque<T> ��������쳣��֤�����ֺ������쳣��֤���������µȺ�����ǿ�쳣��ȫ��֤��
//   * emplace_front
//   * emplace_back
//   * emplace
//   * push_front
//   * push_back
//   * insert

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

// deque map ��ʼ���Ĵ�С
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

    template <class T>
    struct deque_buf_size
    {
        static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    };

    // deque �ĵ��������
    template <class T, class Ref, class Ptr>
    struct deque_iterator : public iterator<random_access_iterator_tag, T>
    {
        typedef deque_iterator<T, T&, T*>             iterator;
        typedef deque_iterator<T, const T&, const T*> const_iterator;
        typedef deque_iterator                        self;

        typedef T            value_type;
        typedef Ptr          pointer;
        typedef Ref          reference;
        typedef size_t       size_type;
        typedef ptrdiff_t    difference_type;
        typedef T* value_pointer;
        typedef T** map_pointer;

        static const size_type buffer_size = deque_buf_size<T>::value;

        // ������������Ա����
        value_pointer cur;    // ָ�����ڻ������ĵ�ǰԪ��
        value_pointer first;  // ָ�����ڻ�������ͷ��
        value_pointer last;   // ָ�����ڻ�������β��
        map_pointer   node;   // ���������ڽڵ�

        // ���졢���ơ��ƶ�����
        deque_iterator() noexcept
            :cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}

        deque_iterator(value_pointer v, map_pointer n)
            :cur(v), first(*n), last(*n + buffer_size), node(n) {}

        deque_iterator(const iterator& rhs)
            :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
        {
        }
        deque_iterator(iterator&& rhs) noexcept
            :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
        {
            rhs.cur = nullptr;
            rhs.first = nullptr;
            rhs.last = nullptr;
            rhs.node = nullptr;
        }

        deque_iterator(const const_iterator& rhs)
            :cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
        {
        }

        self& operator=(const iterator& rhs)
        {
            if (this != &rhs)
            {
                cur = rhs.cur;
                first = rhs.first;
                last = rhs.last;
                node = rhs.node;
            }
            return *this;
        }

        // ת����һ��������
        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *new_node;
            last = first + buffer_size;
        }

        // ���������
        reference operator*()  const { return *cur; }
        pointer   operator->() const { return cur; }

        difference_type operator-(const self& x) const
        {
            return static_cast<difference_type>(buffer_size) * (node - x.node)
                + (cur - first) - (x.cur - x.first);
        }

        self& operator++()
        {
            ++cur;
            if (cur == last)
            { // ������ﻺ������β
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            ++* this;
            return tmp;
        }

        self& operator--()
        {
            if (cur == first)
            { // ������ﻺ������ͷ
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            --* this;
            return tmp;
        }

        self& operator+=(difference_type n)
        {
            const auto offset = n + (cur - first);
            if (offset >= 0 && offset < static_cast<difference_type>(buffer_size))
            { // ���ڵ�ǰ������
                cur += n;
            }
            else
            { // Ҫ���������Ļ�����
                const auto node_offset = offset > 0
                    ? offset / static_cast<difference_type>(buffer_size)
                    : -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
            }
            return *this;
        }
        self operator+(difference_type n) const
        {
            self tmp = *this;
            return tmp += n;
        }
        self& operator-=(difference_type n)
        {
            return *this += -n;
        }
        self operator-(difference_type n) const
        {
            self tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        // ���رȽϲ�����
        bool operator==(const self& rhs) const { return cur == rhs.cur; }
        bool operator< (const self& rhs) const
        {
            return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
        }
        bool operator!=(const self& rhs) const { return !(*this == rhs); }
        bool operator> (const self& rhs) const { return rhs < *this; }
        bool operator<=(const self& rhs) const { return !(rhs < *this); }
        bool operator>=(const self& rhs) const { return !(*this < rhs); }
    };






}












#endif // !MYTINYSTL_DEQUE_H


