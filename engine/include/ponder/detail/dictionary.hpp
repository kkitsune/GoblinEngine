/****************************************************************************
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 ** Copyright (C) 2015-2016 Billy Quith.
 **
 ** This file is part of the Ponder library, formerly CAMP.
 **
 ** The MIT License (MIT)
 **
 ** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
 ** Copyright (C) 2015-2016 Billy Quith.
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in
 ** all copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 ** THE SOFTWARE.
 **
 ****************************************************************************/


#ifndef PONDER_DICTIONARY_HPP
#define PONDER_DICTIONARY_HPP

#include <ponder/config.hpp>
#include <utility>
#include <vector>
#include <algorithm> // std::lower_bound

namespace ponder
{
namespace detail
{

// Key-value pair dictionary.
// - Sorted on keys. Once only insertion cost gives better access times.
//
template <typename KEY, typename VALUE, class CMP>
class Dictionary
{
public:

    struct pair_t : public std::pair<KEY,VALUE> {
        pair_t() : std::pair<KEY,VALUE>() {}
        pair_t(const KEY& k, const VALUE& v) : std::pair<KEY,VALUE>(k, v) {}
        pair_t(const pair_t& p) = default;
        const KEY& name() const { return std::pair<KEY,VALUE>::first; }
        const VALUE& value() const { return std::pair<KEY,VALUE>::second; }
    };

private:
    
    struct KeyCmp {
        bool operator () (const pair_t& a, const KEY& b) const {
            return CMP() (a.first, b);
        }
    };

    typedef std::vector<pair_t> container_t;
    container_t m_contents;
    
public:
    
    typedef typename container_t::const_iterator const_iterator;
    
    const_iterator begin() const    { return m_contents.begin(); }
    const_iterator end() const      { return m_contents.end(); }
    
    class Iterator {
        const_iterator m_begin, m_end;
    public:
        Iterator(const_iterator b, const_iterator e) : m_begin(b), m_end(e) {}
        const_iterator begin() const    { return m_begin; }
        const_iterator end() const      { return m_end; }
    };
    
    Iterator getIterator() const { return Iterator(begin(), end()); }

    const_iterator findKey(const KEY& key) const
    {
        // binary search for key
        const_iterator it(std::lower_bound(m_contents.begin(), m_contents.end(), key, KeyCmp()));
        if (it != m_contents.end() && CMP()(key, it->first)) // it > it-1, check ==
            it = m_contents.end();
        return it;
    }

    const_iterator findValue(const VALUE& value) const
    {
        for (auto&& it = m_contents.begin(); it != m_contents.end(); ++it)
        {
            if (it->second == value)
                return it;
        }
        return m_contents.end();
    }

    bool tryFind(const KEY& key, const_iterator& returnValue) const
    {
        const_iterator it = findKey(key);
        if (it != m_contents.end())
        {
            returnValue = it;
            return true;
        }
        return false; // not found
    }
    
    bool containsKey(const KEY& key) const
    {
        return findKey(key) != m_contents.end();
    }
    
    bool containsValue(const VALUE& value) const
    {
        return findValue(value) != m_contents.end();
    }
    
    std::size_t size() const { return m_contents.size(); }

    void insert(const KEY &key, const VALUE &value)
    {
        erase(key);
        const pair_t p(key, value);
        auto it = std::lower_bound(m_contents.begin(), m_contents.end(), p);
        m_contents.insert(it, p);
    }
    
    void insert(const_iterator it)
    {
        insert(it->first, it->second);
    }
    
    void erase(const KEY& key)
    {
        const_iterator it = findKey(key);
        if (it != m_contents.end())
        {
            // Avoid std::vector.erase here due to bug in libstdc++ < v4.9
#if PONDER_WORKAROUND_GCC_N2350
            std::size_t pos = it - m_contents.begin();
            const std::size_t sz = m_contents.size() - 1;
            while (pos < sz)
                m_contents[pos] = m_contents[pos + 1], ++pos;
            m_contents.resize(sz);
#else
            m_contents.erase(it);
#endif
        }
    }
    
    const_iterator at(std::size_t index) const
    {
        const_iterator it(begin());
        std::advance(it, index);
        return it;
    }
};

} // detail
} // ponder

#endif // PONDER_DICTIONARY_HPP
