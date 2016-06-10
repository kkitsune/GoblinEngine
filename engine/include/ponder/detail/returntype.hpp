/****************************************************************************
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


#ifndef PONDER_DETAIL_RETURNTYPE_HPP
#define PONDER_DETAIL_RETURNTYPE_HPP


namespace ponder
{
namespace detail
{

/**
 * \class ReturnType
 *
 * \brief Utility class used at compile-time to which type to use to avoid copying the returned object
 *
 * \li If T is a reference or a pointer type, ReturnType<T>::Type is T
 * \li Otherwise, ReturnType<T>::Type is T&
 */

template <typename T> struct ReturnType     {typedef T& Type;};
template <typename T> struct ReturnType<T&> {typedef T& Type;};
template <typename T> struct ReturnType<T*> {typedef T* Type;};

} // namespace detail

} // namespace ponder


#endif // PONDER_DETAIL_RETURNTYPE_HPP
