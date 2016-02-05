///
/// anax
/// An open source C++ entity system.
///
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///

#ifndef ANAX_DETAIL_CLASSTYPEID_HPP
#define ANAX_DETAIL_CLASSTYPEID_HPP

#include <cstddef>

namespace anax
{
    namespace detail
    {
        typedef std::size_t TypeId;

        template <typename TBase>
        class ClassTypeId
        {
        public:

            template <typename T>
            static TypeId GetTypeId()
            {
                static const TypeId id = m_nextTypeId++;
                return id;
            }

        private:

            static TypeId m_nextTypeId;
        };

        template <typename TBase>
        TypeId ClassTypeId<TBase>::m_nextTypeId = 0;
    }
}


#endif // ANAX_DETAIL_CLASSTYPEID_HPP
