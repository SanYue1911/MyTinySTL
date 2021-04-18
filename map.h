#ifndef MYTINYSTL_MAP_H_
#define MYTINYSTL_MAP_H_

// ���ͷ�ļ�����������ģ���� map �� multimap
// map      : ӳ�䣬Ԫ�ؾ��м�ֵ��ʵֵ������ݼ�ֵ��С�Զ����򣬼�ֵ�������ظ�
// multimap : ӳ�䣬Ԫ�ؾ��м�ֵ��ʵֵ������ݼ�ֵ��С�Զ����򣬼�ֵ�����ظ�

// notes:
//
// �쳣��֤��
// mystl::map<Key, T> / mystl::multimap<Key, T> ��������쳣��֤�������µȺ�����ǿ�쳣��ȫ��֤��
//   * emplace
//   * emplace_hint
//   * insert

#include "rb_tree.h"

namespace mystl
{

    // ģ���� map����ֵ�������ظ�
    // ����һ�����ֵ���ͣ�����������ʵֵ���ͣ������������ֵ�ıȽϷ�ʽ��ȱʡʹ�� mystl::less
    template <class Key, class T, class Compare = mystl::less<Key>>
    class map
    {
    public:
        // map ��Ƕ���ͱ���
        typedef Key                        key_type;
        typedef T                          mapped_type;
        typedef mystl::pair<const Key, T>  value_type;
        typedef Compare                    key_compare;

        // ����һ�� functor����������Ԫ�رȽ�
        class value_compare : public binary_function <value_type, value_type, bool>
        {
            friend class map<Key, T, Compare>;
        private:
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        public:
            bool operator()(const value_type& lhs, const value_type& rhs) const
            {
                return comp(lhs.first, rhs.first);  // �Ƚϼ�ֵ�Ĵ�С
            }
        };

    private:
        // �� mystl::rb_tree ��Ϊ�ײ����
        typedef mystl::rb_tree<value_type, key_compare>  base_type;
        base_type tree_;

    public:
        // ʹ�� rb_tree ���ͱ�
        typedef typename base_type::node_type              node_type;
        typedef typename base_type::pointer                pointer;
        typedef typename base_type::const_pointer          const_pointer;
        typedef typename base_type::reference              reference;
        typedef typename base_type::const_reference        const_reference;
        typedef typename base_type::iterator               iterator;
        typedef typename base_type::const_iterator         const_iterator;
        typedef typename base_type::reverse_iterator       reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        typedef typename base_type::size_type              size_type;
        typedef typename base_type::difference_type        difference_type;
        typedef typename base_type::allocator_type         allocator_type;

    public:
        // ���졢���ơ��ƶ�����ֵ����

        map() = default;

        template <class InputIterator>
        map(InputIterator first, InputIterator last)
            :tree_()
        {
            tree_.insert_unique(first, last);
        }

        map(std::initializer_list<value_type> ilist)
            :tree_()
        {
            tree_.insert_unique(ilist.begin(), ilist.end());
        }

        map(const map& rhs)
            :tree_(rhs.tree_)
        {
        }
        map(map&& rhs) noexcept
            :tree_(mystl::move(rhs.tree_))
        {
        }

        map& operator=(const map& rhs)
        {
            tree_ = rhs.tree_;
            return *this;
        }
        map& operator=(map&& rhs)
        {
            tree_ = mystl::move(rhs.tree_);
            return *this;
        }

        map& operator=(std::initializer_list<value_type> ilist)
        {
            tree_.clear();
            tree_.insert_unique(ilist.begin(), ilist.end());
            return *this;
        }

        // ��ؽӿ�

        key_compare            key_comp()      const { return tree_.key_comp(); }
        value_compare          value_comp()    const { return value_compare(tree_.key_comp()); }
        allocator_type         get_allocator() const { return tree_.get_allocator(); }

        // ���������

        iterator               begin()         noexcept
        {
            return tree_.begin();
        }
        const_iterator         begin()   const noexcept
        {
            return tree_.begin();
        }
        iterator               end()           noexcept
        {
            return tree_.end();
        }
        const_iterator         end()     const noexcept
        {
            return tree_.end();
        }

        reverse_iterator       rbegin()        noexcept
        {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin()  const noexcept
        {
            return const_reverse_iterator(end());
        }
        reverse_iterator       rend()          noexcept
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend()    const noexcept
        {
            return const_reverse_iterator(begin());
        }

        const_iterator         cbegin()  const noexcept
        {
            return begin();
        }
        const_iterator         cend()    const noexcept
        {
            return end();
        }
        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }
        const_reverse_iterator crend()   const noexcept
        {
            return rend();
        }

        // �������
        bool                   empty()    const noexcept { return tree_.empty(); }
        size_type              size()     const noexcept { return tree_.size(); }
        size_type              max_size() const noexcept { return tree_.max_size(); }

        // ����Ԫ�����

        // ����ֵ�����ڣ�at ���׳�һ���쳣
        mapped_type& at(const key_type& key)
        {
            iterator it = lower_bound(key);
            // it->first >= key
            THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key),
                "map<Key, T> no such element exists");
            return it->second;
        }
        const mapped_type& at(const key_type& key) const
        {
            const_iterator it = lower_bound(key);
            // it->first >= key
            THROW_OUT_OF_RANGE_IF(it == end() || key_comp()(it->first, key),
                "map<Key, T> no such element exists");
            return it->second;
        }

        mapped_type& operator[](const key_type& key)
        {
            iterator it = lower_bound(key);
            // it->first >= key
            if (it == end() || key_comp()(key, it->first))
                it = emplace_hint(it, key, T{});
            return it->second;
        }
        mapped_type& operator[](key_type&& key)
        {
            iterator it = lower_bound(key);
            // it->first >= key
            if (it == end() || key_comp()(key, it->first))
                it = emplace_hint(it, mystl::move(key), T{});
            return it->second;
        }

        // ����ɾ�����

        template <class ...Args>
        pair<iterator, bool> emplace(Args&& ...args)
        {
            return tree_.emplace_unique(mystl::forward<Args>(args)...);
        }

        template <class ...Args>
        iterator emplace_hint(iterator hint, Args&& ...args)
        {
            return tree_.emplace_unique_use_hint(hint, mystl::forward<Args>(args)...);
        }

        pair<iterator, bool> insert(const value_type& value)
        {
            return tree_.insert_unique(value);
        }
        pair<iterator, bool> insert(value_type&& value)
        {
            return tree_.insert_unique(mystl::move(value));
        }

        iterator insert(iterator hint, const value_type& value)
        {
            return tree_.insert_unique(hint, value);
        }
        iterator insert(iterator hint, value_type&& value)
        {
            return tree_.insert_unique(hint, mystl::move(value));
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            tree_.insert_unique(first, last);
        }

        void      erase(iterator position) { tree_.erase(position); }
        size_type erase(const key_type& key) { return tree_.erase_unique(key); }
        void      erase(iterator first, iterator last) { tree_.erase(first, last); }

        void      clear() { tree_.clear(); }

        // map ��ز���

        iterator       find(const key_type& key) { return tree_.find(key); }
        const_iterator find(const key_type& key)        const { return tree_.find(key); }

        size_type      count(const key_type& key)       const { return tree_.count_unique(key); }

        iterator       lower_bound(const key_type& key) { return tree_.lower_bound(key); }
        const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

        iterator       upper_bound(const key_type& key) { return tree_.upper_bound(key); }
        const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

        pair<iterator, iterator>
            equal_range(const key_type& key)
        {
            return tree_.equal_range_unique(key);
        }

        pair<const_iterator, const_iterator>
            equal_range(const key_type& key) const
        {
            return tree_.equal_range_unique(key);
        }

        void           swap(map& rhs) noexcept
        {
            tree_.swap(rhs.tree_);
        }

    public:
        friend bool operator==(const map& lhs, const map& rhs) { return lhs.tree_ == rhs.tree_; }
        friend bool operator< (const map& lhs, const map& rhs) { return lhs.tree_ < rhs.tree_; }
    };

    // ���رȽϲ�����
    template <class Key, class T, class Compare>
    bool operator==(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs)
    {
        return lhs == rhs;
    }

    template <class Key, class T, class Compare>
    bool operator<(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs)
    {
        return lhs < rhs;
    }

    template <class Key, class T, class Compare>
    bool operator!=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Key, class T, class Compare>
    bool operator>(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs)
    {
        return rhs < lhs;
    }

    template <class Key, class T, class Compare>
    bool operator<=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Key, class T, class Compare>
    bool operator>=(const map<Key, T, Compare>& lhs, const map<Key, T, Compare>& rhs)
    {
        return !(lhs < rhs);
    }

    // ���� mystl �� swap
    template <class Key, class T, class Compare>
    void swap(map<Key, T, Compare>& lhs, map<Key, T, Compare>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

    /*****************************************************************************************/

    // ģ���� multimap����ֵ�����ظ�
    // ����һ�����ֵ���ͣ�����������ʵֵ���ͣ������������ֵ�ıȽϷ�ʽ��ȱʡʹ�� mystl::less
    template <class Key, class T, class Compare = mystl::less<Key>>
    class multimap
    {
    public:
        // multimap ���ͱ���
        typedef Key                        key_type;
        typedef T                          mapped_type;
        typedef mystl::pair<const Key, T>  value_type;
        typedef Compare                    key_compare;

        // ����һ�� functor����������Ԫ�رȽ�
        class value_compare : public binary_function <value_type, value_type, bool>
        {
            friend class multimap<Key, T, Compare>;
        private:
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        public:
            bool operator()(const value_type& lhs, const value_type& rhs) const
            {
                return comp(lhs.first, rhs.first);
            }
        };

    private:
        // �� mystl::rb_tree ��Ϊ�ײ����
        typedef mystl::rb_tree<value_type, key_compare>  base_type;
        base_type tree_;

    public:
        // ʹ�� rb_tree ���ͱ�
        typedef typename base_type::node_type              node_type;
        typedef typename base_type::pointer                pointer;
        typedef typename base_type::const_pointer          const_pointer;
        typedef typename base_type::reference              reference;
        typedef typename base_type::const_reference        const_reference;
        typedef typename base_type::iterator               iterator;
        typedef typename base_type::const_iterator         const_iterator;
        typedef typename base_type::reverse_iterator       reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;
        typedef typename base_type::size_type              size_type;
        typedef typename base_type::difference_type        difference_type;
        typedef typename base_type::allocator_type         allocator_type;

    public:
        // ���졢���ơ��ƶ�����

        multimap() = default;

        template <class InputIterator>
        multimap(InputIterator first, InputIterator last)
            :tree_()
        {
            tree_.insert_multi(first, last);
        }
        multimap(std::initializer_list<value_type> ilist)
            :tree_()
        {
            tree_.insert_multi(ilist.begin(), ilist.end());
        }

        multimap(const multimap& rhs)
            :tree_(rhs.tree_)
        {
        }
        multimap(multimap&& rhs) noexcept
            :tree_(mystl::move(rhs.tree_))
        {
        }

        multimap& operator=(const multimap& rhs)
        {
            tree_ = rhs.tree_;
            return *this;
        }
        multimap& operator=(multimap&& rhs)
        {
            tree_ = mystl::move(rhs.tree_);
            return *this;
        }

        multimap& operator=(std::initializer_list<value_type> ilist)
        {
            tree_.clear();
            tree_.insert_multi(ilist.begin(), ilist.end());
            return *this;
        }

        // ��ؽӿ�

        key_compare            key_comp()      const { return tree_.key_comp(); }
        value_compare          value_comp()    const { return value_compare(tree_.key_comp()); }
        allocator_type         get_allocator() const { return tree_.get_allocator(); }

        // ���������

        iterator               begin()         noexcept
        {
            return tree_.begin();
        }
        const_iterator         begin()   const noexcept
        {
            return tree_.begin();
        }
        iterator               end()           noexcept
        {
            return tree_.end();
        }
        const_iterator         end()     const noexcept
        {
            return tree_.end();
        }

        reverse_iterator       rbegin()        noexcept
        {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin()  const noexcept
        {
            return const_reverse_iterator(end());
        }
        reverse_iterator       rend()          noexcept
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend()    const noexcept
        {
            return const_reverse_iterator(begin());
        }

        const_iterator         cbegin()  const noexcept
        {
            return begin();
        }
        const_iterator         cend()    const noexcept
        {
            return end();
        }
        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }
        const_reverse_iterator crend()   const noexcept
        {
            return rend();
        }

        // �������
        bool                   empty()    const noexcept { return tree_.empty(); }
        size_type              size()     const noexcept { return tree_.size(); }
        size_type              max_size() const noexcept { return tree_.max_size(); }

        // ����ɾ������

        template <class ...Args>
        iterator emplace(Args&& ...args)
        {
            return tree_.emplace_multi(mystl::forward<Args>(args)...);
        }

        template <class ...Args>
        iterator emplace_hint(iterator hint, Args&& ...args)
        {
            return tree_.emplace_multi_use_hint(hint, mystl::forward<Args>(args)...);
        }

        iterator insert(const value_type& value)
        {
            return tree_.insert_multi(value);
        }
        iterator insert(value_type&& value)
        {
            return tree_.insert_multi(mystl::move(value));
        }

        iterator insert(iterator hint, const value_type& value)
        {
            return tree_.insert_multi(hint, value);
        }
        iterator insert(iterator hint, value_type&& value)
        {
            return tree_.insert_multi(hint, mystl::move(value));
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            tree_.insert_multi(first, last);
        }

        void           erase(iterator position) { tree_.erase(position); }
        size_type      erase(const key_type& key) { return tree_.erase_multi(key); }
        void           erase(iterator first, iterator last) { tree_.erase(first, last); }

        void           clear() { tree_.clear(); }

        // multimap ��ز���

        iterator       find(const key_type& key) { return tree_.find(key); }
        const_iterator find(const key_type& key)        const { return tree_.find(key); }

        size_type      count(const key_type& key)       const { return tree_.count_multi(key); }

        iterator       lower_bound(const key_type& key) { return tree_.lower_bound(key); }
        const_iterator lower_bound(const key_type& key) const { return tree_.lower_bound(key); }

        iterator       upper_bound(const key_type& key) { return tree_.upper_bound(key); }
        const_iterator upper_bound(const key_type& key) const { return tree_.upper_bound(key); }

        pair<iterator, iterator>
            equal_range(const key_type& key)
        {
            return tree_.equal_range_multi(key);
        }

        pair<const_iterator, const_iterator>
            equal_range(const key_type& key) const
        {
            return tree_.equal_range_multi(key);
        }

        void swap(multimap& rhs) noexcept
        {
            tree_.swap(rhs.tree_);
        }

    public:
        friend bool operator==(const multimap& lhs, const multimap& rhs) { return lhs.tree_ == rhs.tree_; }
        friend bool operator< (const multimap& lhs, const multimap& rhs) { return lhs.tree_ < rhs.tree_; }
    };

    // ���رȽϲ�����
    template <class Key, class T, class Compare>
    bool operator==(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs)
    {
        return lhs == rhs;
    }

    template <class Key, class T, class Compare>
    bool operator<(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs)
    {
        return lhs < rhs;
    }

    template <class Key, class T, class Compare>
    bool operator!=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Key, class T, class Compare>
    bool operator>(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs)
    {
        return rhs < lhs;
    }

    template <class Key, class T, class Compare>
    bool operator<=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Key, class T, class Compare>
    bool operator>=(const multimap<Key, T, Compare>& lhs, const multimap<Key, T, Compare>& rhs)
    {
        return !(lhs < rhs);
    }

    // ���� mystl �� swap
    template <class Key, class T, class Compare>
    void swap(multimap<Key, T, Compare>& lhs, multimap<Key, T, Compare>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

} // namespace mystl
#endif // !MYTINYSTL_MAP_H_
