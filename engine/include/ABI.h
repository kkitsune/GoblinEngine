// Copyright (c) 2016 Roland Jaeger
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
/**
 * The main macro defined by this header is `E_ABI(s)` and it is used to export
 * and import symbols of dynamic libraries.
 *
 * The normal solution is to create a header for each library, copy & paste the
 * contents of an existing library header and then change the macros that are
 * being defined by search and replace. If your project only has a few libraries
 * this header will not proove to be of much convenience. (Lazzy people might
 * want to disagree with that after the first copy without further modifications
 * ;))
 *
 * Usage:
 *
 *      void E_ABI(my_lib) foo();
 *
 * Will evaluate to different statements depending on compile-time definitions.
 *
 * The default behaviour is to import symbols. To export `foo` you'll have to
 * pass `E_ABI_my_lib` (eg. `gcc -DE_ABI_my_lib ...`). If the library is build as
 * a static version you can pass `E_ABI_STATIC_my_lib` since the linker will
 * handle all symbols.
 *
 * |      import      |      export      |         static          |
 * |                  |  E_ABI_my_lib    |  E_ABI_STATIC_my_lib    |
 * |  E_ABI_my_lib 0  |  E_ABI_my_lib 1  |  E_ABI_STATIC_my_lib 1  |
 * |  E_ABI_my_lib 2  |  --------------  |  ---------------------  |
 * |  E_ABI_my_lib a  |  --------------  |  ---------------------  |
 * |       ...        |  --------------  |  ---------------------  |
 *
 * + Import means either `__declspec(dllimport)`,
 *   `__attribute__((dllimport))` or `__attribute__((visibility("default")))`
 *   depending on the platform / compiler.
 * + Export means either `__declspec(dllexport)`,
 *   `__attribute__((dllexport))` or `__attribute__((visibility("default")))`
 *   depending on the platform / compiler.
 * + Static means that the macro will be evaluated to nothing -- the linker
 *   will take care of everything.
 *
 * To gain benefits from exporting / importing with gcc / clang the compile
 * option `-fvisibility=hidden` has to be passed to the compiler. But if you
 * didn't know that already you should head over to this page and read up on
 * this subject. https://gcc.gnu.org/wiki/Visibility
 *
 *
 * Credits:
 * pfultz2 for his Cloak  library. http://pfultz2.com/blog/
 *                                 https://github.com/pfultz2/Cloak
 */


//////////////////////////////////////////
// Internal macros -- subjects to change
//////////////////////////////////////////
/**
 * Check which compiler is used
 */
#if defined _WIN32 || defined __CYGWIN__
#ifndef __GNUC__
#define EYENSEO_ABI_COMPILER_MSVC 1
#else
#define EYENSEO_ABI_COMPILER_GWIN 1
#endif
#elif defined __GNUC__
// Clang is compatible to gcc so we don't need a special case
#if __GNUC__ >= 4
#define EYENSEO_ABI_COMPILER_GCC 1
#else
#define EYENSEO_ABI_COMPILER_VOID 1
#endif
#else
#error E_ABI - Unknown compiler, please implement the macros
#endif

/**
 * Magic ...
 *
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 */
#define EYENSEO_ABI_PASS(...) __VA_ARGS__

#define EYENSEO_ABI_CAT(a, ...) a##__VA_ARGS__

#define EYENSEO_ABI_IF_ELSE_IMPL_0(t, f) f
#define EYENSEO_ABI_IF_ELSE_IMPL_1(t, f) t
#define EYENSEO_ABI_IF_ELSE_IMPL(c) \
  EYENSEO_ABI_CAT(EYENSEO_ABI_IF_ELSE_IMPL_, c)

#ifdef EYENSEO_ABI_COMPILER_MSVC
// You screwed up msvc
#define EYENSEO_ABI_SECOND_IMPL(a, b, ...) b
#define EYENSEO_ABI_SECOND(...) \
  EYENSEO_ABI_PASS(EYENSEO_ABI_SECOND_IMPL(__VA_ARGS__))
#else
#define EYENSEO_ABI_SECOND(a, b, ...) b
#endif

#define EYENSEO_ABI_CHECK(...) EYENSEO_ABI_SECOND(__VA_ARGS__, 0, ~)

#define EYENSEO_ABI_IS_1() ~, 1
#define EYENSEO_ABI_IS_() ~, 1
#define EYENSEO_ABI_IS(...) \
  EYENSEO_ABI_CHECK(EYENSEO_ABI_CAT(EYENSEO_ABI_IS_, __VA_ARGS__)())

#ifdef EYENSEO_ABI_COMPILER_MSVC
// You screwed up msvc ... again
#define EYENSEO_ABI_IF_ELSE(c) \
  EYENSEO_ABI_IF_ELSE_IMPL(EYENSEO_ABI_PASS(EYENSEO_ABI_IS(c)))
#else
#define EYENSEO_ABI_IF_ELSE(c) EYENSEO_ABI_IF_ELSE_IMPL(EYENSEO_ABI_IS(c))
#endif

//////////////////////////////////////////
// Main macro
//////////////////////////////////////////
/**
 * Compiler switch for the main macro `E_ABI(s)`
 */
#ifdef EYENSEO_ABI_COMPILER_MSVC
#define E_ABI(s)                                                     \
  EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_STATIC_, s))             \
  (/* If static: Static libraries will be handled by the linker */,  \
   /* Else:      */ EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_, s))( \
       /* If defined:   We export */ __declspec(dllexport),          \
       /* Else:         We import */ __declspec(dllimport)))
#elif defined(EYENSEO_ABI_COMPILER_GWIN)
#define E_ABI(s)                                                     \
  EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_STATIC_, s))             \
  (/* If static: Static libraries will be handled by the linker */,  \
   /* Else:      */ EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_, s))( \
       /* If defined:   We export */ __attribute__((dllexport)),     \
       /* Else:         We import */ __attribute__((dllimport))))
#elif defined(EYENSEO_ABI_COMPILER_GCC)
#define E_ABI(s)                                                             \
  EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_STATIC_, s))                     \
  (/* If static: Static libraries will be handled by the linker */,          \
   /* Else:      */ EYENSEO_ABI_IF_ELSE(EYENSEO_ABI_CAT(E_ABI_, s))(         \
       /* If defined:   We export */ __attribute__((visibility("default"))), \
       /* Else:         We import */ __attribute__((visibility("default")))))
#elif defined(EYENSEO_ABI_COMPILER_VOID)
#define E_ABI(s)
#endif
