// The MIT License (MIT)

// Copyright (c) 2013-2016 Rapptz, ThePhD and contributors

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This file was generated with a script.
// Generated 2016-05-03 21:20:08.031941 UTC
// This header was generated with sol v2.5.2 (revision 1c61773)
// https://github.com/ThePhD/sol2

#ifndef SOL_SINGLE_INCLUDE_HPP
#define SOL_SINGLE_INCLUDE_HPP

// beginning of sol\state.hpp

// beginning of sol\state_view.hpp

// beginning of sol\error.hpp

#include <stdexcept>
#include <string>

namespace sol {
namespace detail {
struct direct_error_tag {};
const auto direct_error = direct_error_tag{};
} // detail
class error : public std::runtime_error {
public:
    error(const std::string& str): std::runtime_error("lua: error: " + str) {}
    error(detail::direct_error_tag, const std::string& str) : std::runtime_error(str) {}
};
} // sol

// end of sol\error.hpp

// beginning of sol\table.hpp

// beginning of sol\table_core.hpp

// beginning of sol\proxy.hpp

// beginning of sol\traits.hpp

// beginning of sol\tuple.hpp

#include <tuple>
#include <cstddef>

namespace sol {
namespace detail {
using swallow = std::initializer_list<int>;
} // detail

template<typename... Args>
struct types { typedef std::make_index_sequence<sizeof...(Args)> indices; static constexpr std::size_t size() { return sizeof...(Args); } }; 
namespace meta {
namespace detail {
template<typename... Args>
struct tuple_types_ { typedef types<Args...> type; };

template<typename... Args>
struct tuple_types_<std::tuple<Args...>> { typedef types<Args...> type; };
} // detail

template<typename... Args>
using tuple_types = typename detail::tuple_types_<Args...>::type;

template<typename Arg>
struct pop_front_type;

template<typename Arg>
using pop_front_type_t = typename pop_front_type<Arg>::type;

template<typename Arg, typename... Args>
struct pop_front_type<types<Arg, Args...>> { typedef types<Args...> type; };
} // meta
} // sol

// end of sol\tuple.hpp

#include <type_traits>
#include <memory>
#include <functional>

namespace sol {
template<std::size_t I>
using index_value = std::integral_constant<std::size_t, I>;

namespace meta {
template<typename T>
struct identity { typedef T type; };

template<typename T>
using identity_t = typename identity<T>::type;

template<typename... Args>
struct is_tuple : std::false_type{ };

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type{ };

template<typename T>
struct unwrapped {
    typedef T type;
};

template<typename T>
struct unwrapped<std::reference_wrapper<T>> {
    typedef T type;
};

template<typename T>
struct remove_member_pointer;

template<typename R, typename T>
struct remove_member_pointer<R T::*> {
    typedef R type;
};

template<typename R, typename T>
struct remove_member_pointer<R T::* const> {
    typedef R type;
};

template<typename T>
using remove_member_pointer_t = remove_member_pointer<T>;

template<template<typename...> class Templ, typename T>
struct is_specialization_of : std::false_type { };
template<typename... T, template<typename...> class Templ>
struct is_specialization_of<Templ, Templ<T...>> : std::true_type { };

template<class T, class...>
struct are_same : std::true_type { };

template<class T, class U, class... Args>
struct are_same<T, U, Args...> : std::integral_constant <bool, std::is_same<T, U>::value && are_same<T, Args...>::value> { };

template<typename T>
using Type = typename T::type;

template<bool B>
using Bool = std::integral_constant<bool, B>;

template<typename T>
using Not = Bool<!T::value>;

template<typename Condition, typename Then, typename Else>
using If = typename std::conditional<Condition::value, Then, Else>::type;

template<typename Condition, typename Then, typename Else>
using TypeIf = typename std::conditional<Condition::value, Type<Then>, Type<Else>>::type;

template<typename... Args>
struct And : Bool<true> {};

template<typename T, typename... Args>
struct And<T, Args...> : If<T, And<Args...>, Bool<false>> {};

template<typename... Args>
struct Or : Bool<false> {};

template<typename T, typename... Args>
struct Or<T, Args...> : If<T, Bool<true>, Or<Args...>> {};

template<typename... Args>
using EnableIf = typename std::enable_if<And<Args...>::value, int>::type;

template<typename... Args>
using DisableIf = typename std::enable_if<Not<And<Args...>>::value, int>::type;

template<typename T>
using Unqualified = std::remove_cv_t<std::remove_reference_t<T>>;

template<typename T>
using Unwrapped = typename unwrapped<T>::type;

template <std::size_t N, typename Tuple>
using tuple_element = std::tuple_element<N, Unqualified<Tuple>>;

template <std::size_t N, typename Tuple>
using tuple_element_t = std::tuple_element_t<N, Unqualified<Tuple>>;

template<typename V, typename... Vs>
struct find_in_pack_v : Bool<false> { };

template<typename V, typename Vs1, typename... Vs>
struct find_in_pack_v<V, Vs1, Vs...> : Or<Bool<(V::value == Vs1::value)>, find_in_pack_v<V, Vs...>> { };

namespace meta_detail {
    template<std::size_t I, typename T, typename... Args>
    struct index_in_pack : std::integral_constant<std::size_t, SIZE_MAX> { };

    template<std::size_t I, typename T, typename T1, typename... Args>
    struct index_in_pack<I, T, T1, Args...> : std::conditional_t<std::is_same<T, T1>::value, std::integral_constant<std::ptrdiff_t, I>, index_in_pack<I + 1, T, Args...>> { };
}

template<typename T, typename... Args>
struct index_in_pack : meta_detail::index_in_pack<0, T, Args...> { };

template<typename T, typename List>
struct index_in : meta_detail::index_in_pack<0, T, List> { };

template<typename T, typename... Args>
struct index_in<T, types<Args...>> : meta_detail::index_in_pack<0, T, Args...> { };

template<std::size_t I, typename... Args>
struct at_in_pack {};

template<std::size_t I, typename... Args>
using at_in_pack_t = typename at_in_pack<I, Args...>::type;

template<std::size_t I, typename Arg, typename... Args>
struct at_in_pack<I, Arg, Args...> : std::conditional<I == 0, Arg, at_in_pack_t<I - 1, Args...>> {};

namespace meta_detail {
    template<std::size_t I, template<typename...> class Pred, typename... Ts>
    struct count_if_pack : std::integral_constant<std::size_t, 0> {};
    template<std::size_t I, template<typename...> class Pred, typename T, typename... Ts>
    struct count_if_pack<I, Pred, T, Ts...> : std::conditional_t<sizeof...(Ts) == 0, 
        std::integral_constant<std::size_t, I + static_cast<std::size_t>(Pred<T>::value)>,
        count_if_pack<I + static_cast<std::size_t>(Pred<T>::value), Pred, Ts...>
    > { };
    template<std::size_t I, template<typename...> class Pred, typename... Ts>
    struct count_if_2_pack : std::integral_constant<std::size_t, 0> {};
    template<std::size_t I, template<typename...> class Pred, typename T, typename U, typename... Ts>
    struct count_if_2_pack<I, Pred, T, U, Ts...> : std::conditional_t<sizeof...(Ts) == 0, 
        std::integral_constant<std::size_t, I + static_cast<std::size_t>(Pred<T>::value)>, 
        count_if_2_pack<I + static_cast<std::size_t>(Pred<T>::value), Pred, Ts...>
    > { };
} // meta_detail

template<template<typename...> class Pred, typename... Ts>
struct count_if_pack : meta_detail::count_if_pack<0, Pred, Ts...> { };

template<template<typename...> class Pred, typename... Ts>
struct count_if_2_pack : meta_detail::count_if_2_pack<0, Pred, Ts...> { };

template<typename... Args>
struct return_type {
    typedef std::tuple<Args...> type;
};

template<typename T>
struct return_type<T> {
    typedef T type;
};

template<>
struct return_type<> {
    typedef void type;
};

template <typename... Args>
using return_type_t = typename return_type<Args...>::type;

namespace meta_detail {
template <typename> struct always_true : std::true_type {};
struct is_callable_tester {
    template <typename Fun, typename... Args>
    always_true<decltype(std::declval<Fun>()(std::declval<Args>()...))> static test(int);
    template <typename...>
    std::false_type static test(...);
};
} // meta_detail

template <typename T>
struct is_callable;
template <typename Fun, typename... Args>
struct is_callable<Fun(Args...)> : decltype(meta_detail::is_callable_tester::test<Fun, Args...>(0)) {};

namespace meta_detail {

template<typename T, bool isclass = std::is_class<Unqualified<T>>::value>
struct is_function_impl : std::is_function<std::remove_pointer_t<T>> {};

template<typename T>
struct is_function_impl<T, true> {
    using yes = char;
    using no = struct { char s[2]; };

    struct F { void operator()(); };
    struct Derived : T, F {};
    template<typename U, U> struct Check;

    template<typename V>
    static no test(Check<void (F::*)(), &V::operator()>*);

    template<typename>
    static yes test(...);

    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
};

template<class F>
struct check_deducible_signature {
    struct nat {};
    template<class G>
    static auto test(int) -> decltype(&G::operator(), void());
    template<class>
    static auto test(...) -> nat;

    using type = std::is_void<decltype(test<F>(0))>;
};
} // meta_detail

template<class F>
struct has_deducible_signature : meta_detail::check_deducible_signature<F>::type { };

template<typename T>
struct Function : Bool<meta_detail::is_function_impl<T>::value> {};

namespace meta_detail {

template <std::size_t I, typename T>
struct void_tuple_element : meta::tuple_element<I, T> {};

template <std::size_t I>
struct void_tuple_element<I, std::tuple<>> { typedef void type; };

template <std::size_t I, typename T>
using void_tuple_element_t = typename void_tuple_element<I, T>::type;

template<typename Signature, bool b = has_deducible_signature<Signature>::value>
struct fx_traits {
    static const bool is_member_function = false;
    typedef std::tuple<> args_tuple_type;
    typedef types<> args_type;
    template<std::size_t i>
    using arg_at = void_tuple_element_t<i, args_tuple_type>;
};

template<typename Signature>
struct fx_traits<Signature, true> : fx_traits<decltype(&Signature::operator()), false> {};

template<typename T, typename R, typename... Args>
struct fx_traits<R(T::*)(Args...), false> {
    static const std::size_t arity = sizeof...(Args);
    static const bool is_member_function = true;
    typedef T object_type;
    typedef std::tuple<Args...> args_tuple_type;
    typedef types<Args...> args_type;
    typedef R(T::* function_pointer_type)(Args...);
    typedef std::remove_pointer_t<function_pointer_type> function_type;
    typedef R(*free_function_pointer_type)(Args...);
    typedef R return_type;
    typedef std::remove_pointer_t<free_function_pointer_type> signature_type;
    template<std::size_t i>
    using arg_at = void_tuple_element_t<i, args_tuple_type>;
};

template<typename T, typename R, typename... Args>
struct fx_traits<R(T::*)(Args...) const, false> {
    static const std::size_t arity = sizeof...(Args);
    static const bool is_member_function = true;
    typedef T object_type;
    typedef std::tuple<Args...> args_tuple_type;
    typedef types<Args...> args_type;
    typedef R(T::* function_pointer_type)(Args...);
    typedef std::remove_pointer_t<function_pointer_type> function_type;
    typedef R(*free_function_pointer_type)(Args...);
    typedef R return_type;
    typedef std::remove_pointer_t<free_function_pointer_type> signature_type;
    template<std::size_t i>
    using arg_at = void_tuple_element_t<i, args_tuple_type>;
};

template<typename R, typename... Args>
struct fx_traits<R(Args...), false> {
    static const std::size_t arity = sizeof...(Args);
    static const bool is_member_function = false;
    typedef std::tuple<Args...> args_tuple_type;
    typedef types<Args...> args_type;
    typedef R(function_type)(Args...);
    typedef R(*function_pointer_type)(Args...);
    typedef R(*free_function_pointer_type)(Args...);
    typedef R return_type;
    typedef std::remove_pointer_t<free_function_pointer_type> signature_type;
    template<std::size_t i>
    using arg_at = void_tuple_element_t<i, args_tuple_type>;
};

template<typename R, typename... Args>
struct fx_traits<R(*)(Args...), false> {
    static const std::size_t arity = sizeof...(Args);
    static const bool is_member_function = false;
    typedef std::tuple<Args...> args_tuple_type;
    typedef types<Args...> args_type;
    typedef R(function_type)(Args...);
    typedef R(*function_pointer_type)(Args...);
    typedef R(*free_function_pointer_type)(Args...);
    typedef R return_type;
    typedef std::remove_pointer_t<free_function_pointer_type> signature_type;
    template<std::size_t i>
    using arg_at = void_tuple_element_t<i, args_tuple_type>;
};

template<typename Signature, bool b = std::is_member_object_pointer<Signature>::value>
struct callable_traits : fx_traits<std::decay_t<Signature>> {

};

template<typename R, typename T>
struct callable_traits<R(T::*), true> {
    typedef R Arg;
    typedef T object_type;
    using signature_type = R(T::*);
    static const bool is_member_function = false;
    static const std::size_t arity = 1;
    typedef std::tuple<Arg> args_tuple_type;
    typedef types<Arg> args_type;
    typedef R return_type;
    typedef R(function_type)(Arg);
    typedef R(*function_pointer_type)(Arg);
    typedef R(*free_function_pointer_type)(Arg);
    template<std::size_t i>
    using arg_at = void_tuple_element_t<i, args_tuple_type>;
};
} // meta_detail

template<typename Signature>
struct bind_traits : meta_detail::callable_traits<std::remove_volatile_t<Signature>> {};

template<typename Signature>
using function_args_t = typename bind_traits<Signature>::args_type;

template<typename Signature>
using function_signature_t = typename bind_traits<Signature>::signature_type;

template<typename Signature>
using function_return_t = typename bind_traits<Signature>::return_type;

struct has_begin_end_impl {
    template<typename T, typename U = Unqualified<T>,
        typename B = decltype(std::declval<U&>().begin()),
        typename E = decltype(std::declval<U&>().end())>
    static std::true_type test(int);

    template<typename...>
    static std::false_type test(...);
};

template<typename T>
struct has_begin_end : decltype(has_begin_end_impl::test<T>(0)) {};

struct has_key_value_pair_impl {
    template<typename T, typename U = Unqualified<T>,
        typename V = typename U::value_type,
        typename F = decltype(std::declval<V&>().first),
        typename S = decltype(std::declval<V&>().second)>
    static std::true_type test(int);

    template<typename...>
    static std::false_type test(...);
};

template<typename T>
struct has_key_value_pair : decltype(has_key_value_pair_impl::test<T>(0)) {};

template <typename T>
using is_string_constructible = Or<std::is_same<Unqualified<T>, const char*>, std::is_same<Unqualified<T>, char>, std::is_same<Unqualified<T>, std::string>, std::is_same<Unqualified<T>, std::initializer_list<char>>>;

template <typename T>
using is_c_str = Or<
    std::is_same<std::decay_t<Unqualified<T>>, const char*>, 
    std::is_same<std::decay_t<Unqualified<T>>, char*>, 
    std::is_same<Unqualified<T>, std::string>
>;

namespace meta_detail {
template <typename T, meta::DisableIf<meta::is_specialization_of<std::tuple, meta::Unqualified<T>>> = 0>
decltype(auto) force_tuple(T&& x) {
    return std::forward_as_tuple(std::forward<T>(x));
}

template <typename T, meta::EnableIf<meta::is_specialization_of<std::tuple, meta::Unqualified<T>>> = 0>
decltype(auto) force_tuple(T&& x) {
    return std::forward<T>(x);
}
} // meta_detail

template <typename... X>
decltype(auto) tuplefy(X&&... x ) {
    return std::tuple_cat(meta_detail::force_tuple(std::forward<X>(x))...);
}
} // meta
namespace detail {
template <std::size_t I, typename Tuple>
decltype(auto) forward_get( Tuple&& tuple ) {
    return std::forward<meta::tuple_element_t<I, Tuple>>(std::get<I>(tuple));
}

template <std::size_t... I, typename Tuple>
auto forward_tuple_impl( std::index_sequence<I...>, Tuple&& tuple ) -> decltype(std::tuple<decltype(forward_get<I>(tuple))...>(forward_get<I>(tuple)...)) {
    return std::tuple<decltype(forward_get<I>(tuple))...>(std::move(std::get<I>(tuple))...);
}

template <typename Tuple>
auto forward_tuple( Tuple&& tuple ) {
    auto x = forward_tuple_impl(std::make_index_sequence<std::tuple_size<meta::Unqualified<Tuple>>::value>(), std::forward<Tuple>(tuple));
    return x;
}

template<typename T>
auto unwrap(T&& item) -> decltype(std::forward<T>(item)) {
    return std::forward<T>(item);
}

template<typename T>
T& unwrap(std::reference_wrapper<T> arg) {
    return arg.get();
}

template<typename T>
decltype(auto) deref(T&& item) {
    return std::forward<T>(item);
}

template<typename T>
T& deref(T* item) {
    return *item;
}

template<typename T, typename Dx>
decltype(auto) deref(std::unique_ptr<T, Dx>& item) {
    return *item;
}

template<typename T>
T& deref(std::shared_ptr<T>& item) {
    return *item;
}

template<typename T, typename Dx>
decltype(auto) deref(const std::unique_ptr<T, Dx>& item) {
    return *item;
}

template<typename T>
T& deref(const std::shared_ptr<T>& item) {
    return *item;
}

template<typename T>
inline T* ptr(T& val) {
    return std::addressof(val);
}

template<typename T>
inline T* ptr(std::reference_wrapper<T> val) {
    return std::addressof(val.get());
}

template<typename T>
inline T* ptr(T* val) {
    return val;
}
} // detail
} // sol

// end of sol\traits.hpp

// beginning of sol\object.hpp

// beginning of sol\reference.hpp

// beginning of sol\types.hpp

// beginning of sol\compatibility.hpp

// beginning of sol\compatibility\version.hpp

#include <lua.hpp>

#ifdef LUAJIT_VERSION
#ifndef SOL_LUAJIT
#define SOL_LUAJIT
#endif // sol luajit
#endif // luajit

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 502
#define SOL_LUA_VERSION LUA_VERSION_NUM
#elif defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501
#define SOL_LUA_VERSION LUA_VERSION_NUM
#elif !defined(LUA_VERSION_NUM)
#define SOL_LUA_VERSION 500
#else
#define SOL_LUA_VERSION 502
#endif // Lua Version 502, 501 || luajit, 500 

#ifdef _MSC_VER
#ifdef _DEBUG
#ifndef SOL_CHECK_ARGUMENTS
#endif // Check Arguments
#endif // Debug

#ifndef _CPPUNWIND
#ifndef SOL_NO_EXCEPTIONS
#define SOL_NO_EXCEPTIONS 1
#endif
#endif // Automatic Exceptions

#ifndef _CPPRTTI
#ifndef SOL_NO_RTTI
#define SOL_NO_RTTI 1
#endif
#endif // Automatic RTTI

#elif defined(__GNUC__) || defined(__clang__)

#ifndef NDEBUG
#ifndef __OPTIMIZE__
#ifndef SOL_CHECK_ARGUMENTS
#endif // Check Arguments
#endif // g++ optimizer flag
#endif // Not Debug

#ifndef __EXCEPTIONS
#ifndef SOL_NO_EXCEPTIONS
#define SOL_NO_EXCEPTIONS 1
#endif
#endif // No Exceptions

#ifndef __GXX_RTTI
#ifndef SOL_NO_RTII
#define SOL_NO_RTTI 1
#endif
#endif // No RTTI

#endif // vc++ || clang++/g++

// end of sol\compatibility\version.hpp

#ifdef __cplusplus
extern "C" {
#endif
// beginning of sol\compatibility\5.1.0.h

#ifndef SOL_5_1_0_H
#define SOL_5_1_0_H

#if SOL_LUA_VERSION == 501
/* Lua 5.1 */

#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* LuaJIT doesn't define these unofficial macros ... */
#if !defined(LUAI_INT32)
#include <limits.h>
#if INT_MAX-20 < 32760
#define LUAI_INT32  long
#define LUAI_UINT32 unsigned long
#elif INT_MAX > 2147483640L
#define LUAI_INT32  int
#define LUAI_UINT32 unsigned int
#else
#error "could not detect suitable lua_Unsigned datatype"
#endif
#endif

/* LuaJIT does not have the updated error codes for thread status/function returns */
#ifndef LUA_ERRGCMM
#define LUA_ERRGCMM (LUA_ERRERR + 1)
#endif // LUA_ERRGCMM

/* LuaJIT does not support continuation contexts / return error codes? */
#ifndef LUA_KCONTEXT
#define LUA_KCONTEXT std::ptrdiff_t
typedef LUA_KCONTEXT lua_KContext;
typedef int(*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);
#endif // LUA_KCONTEXT

#define LUA_OPADD 0
#define LUA_OPSUB 1
#define LUA_OPMUL 2
#define LUA_OPDIV 3
#define LUA_OPMOD 4
#define LUA_OPPOW 5
#define LUA_OPUNM 6
#define LUA_OPEQ 0
#define LUA_OPLT 1
#define LUA_OPLE 2

typedef LUAI_UINT32 lua_Unsigned;

typedef struct luaL_Buffer_52 {
    luaL_Buffer b; /* make incorrect code crash! */
    char *ptr;
    size_t nelems;
    size_t capacity;
    lua_State *L2;
} luaL_Buffer_52;
#define luaL_Buffer luaL_Buffer_52

#define lua_tounsigned(L, i) lua_tounsignedx(L, i, NULL)

#define lua_rawlen(L, i) lua_objlen(L, i)

inline void lua_callk(lua_State *L, int nargs, int nresults, lua_KContext, lua_KFunction) {
    // should probably warn the user of Lua 5.1 that continuation isn't supported...
    lua_call(L, nargs, nresults);
}
inline int lua_pcallk(lua_State *L, int nargs, int nresults, int errfunc, lua_KContext, lua_KFunction) {
    // should probably warn the user of Lua 5.1 that continuation isn't supported...
    return lua_pcall(L, nargs, nresults, errfunc);
}
void lua_arith(lua_State *L, int op);
int lua_compare(lua_State *L, int idx1, int idx2, int op);
void lua_pushunsigned(lua_State *L, lua_Unsigned n);
lua_Unsigned luaL_checkunsigned(lua_State *L, int i);
lua_Unsigned lua_tounsignedx(lua_State *L, int i, int *isnum);
lua_Unsigned luaL_optunsigned(lua_State *L, int i, lua_Unsigned def);
lua_Integer lua_tointegerx(lua_State *L, int i, int *isnum);
void lua_len(lua_State *L, int i);
int luaL_len(lua_State *L, int i);
const char *luaL_tolstring(lua_State *L, int idx, size_t *len);
void luaL_requiref(lua_State *L, char const* modname, lua_CFunction openf, int glb);

#define luaL_buffinit luaL_buffinit_52
void luaL_buffinit(lua_State *L, luaL_Buffer_52 *B);

#define luaL_prepbuffsize luaL_prepbuffsize_52
char *luaL_prepbuffsize(luaL_Buffer_52 *B, size_t s);

#define luaL_addlstring luaL_addlstring_52
void luaL_addlstring(luaL_Buffer_52 *B, const char *s, size_t l);

#define luaL_addvalue luaL_addvalue_52
void luaL_addvalue(luaL_Buffer_52 *B);

#define luaL_pushresult luaL_pushresult_52
void luaL_pushresult(luaL_Buffer_52 *B);

#undef luaL_buffinitsize
#define luaL_buffinitsize(L, B, s) \
  (luaL_buffinit(L, B), luaL_prepbuffsize(B, s))

#undef luaL_prepbuffer
#define luaL_prepbuffer(B) \
  luaL_prepbuffsize(B, LUAL_BUFFERSIZE)

#undef luaL_addchar
#define luaL_addchar(B, c) \
  ((void)((B)->nelems < (B)->capacity || luaL_prepbuffsize(B, 1)), \
   ((B)->ptr[(B)->nelems++] = (c)))

#undef luaL_addsize
#define luaL_addsize(B, s) \
  ((B)->nelems += (s))

#undef luaL_addstring
#define luaL_addstring(B, s) \
  luaL_addlstring(B, s, strlen(s))

#undef luaL_pushresultsize
#define luaL_pushresultsize(B, s) \
  (luaL_addsize(B, s), luaL_pushresult(B))

#endif /* Lua 5.1 */

#endif // SOL_5_1_0_H// end of sol\compatibility\5.1.0.h

// beginning of sol\compatibility\5.0.0.h

#ifndef SOL_5_0_0_H
#define SOL_5_0_0_H

#if SOL_LUA_VERSION < 501
/* Lua 5.0 */

#define LUA_QL(x) "'" x "'"
#define LUA_QS LUA_QL("%s")

#define luaL_Reg luaL_reg

#define luaL_opt(L, f, n, d) \
  (lua_isnoneornil(L, n) ? (d) : f(L, n))

#define luaL_addchar(B,c) \
  ((void)((B)->p < ((B)->buffer+LUAL_BUFFERSIZE) || luaL_prepbuffer(B)), \
   (*(B)->p++ = (char)(c)))

#endif // Lua 5.0

#endif // SOL_5_0_0_H
// end of sol\compatibility\5.0.0.h

// beginning of sol\compatibility\5.x.x.h

#ifndef SOL_5_X_X_H
#define SOL_5_X_X_H

#if SOL_LUA_VERSION < 502

#define LUA_RIDX_GLOBALS LUA_GLOBALSINDEX

#define LUA_OK 0

#define lua_pushglobaltable(L) \
  lua_pushvalue(L, LUA_GLOBALSINDEX)

#define luaL_newlib(L, l) \
  (lua_newtable((L)),luaL_setfuncs((L), (l), 0))

void luaL_checkversion(lua_State *L);

int lua_absindex(lua_State *L, int i);
void lua_copy(lua_State *L, int from, int to);
void lua_rawgetp(lua_State *L, int i, const void *p);
void lua_rawsetp(lua_State *L, int i, const void *p);
void *luaL_testudata(lua_State *L, int i, const char *tname);
lua_Number lua_tonumberx(lua_State *L, int i, int *isnum);
void lua_getuservalue(lua_State *L, int i);
void lua_setuservalue(lua_State *L, int i);
void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup);
void luaL_setmetatable(lua_State *L, const char *tname);
int luaL_getsubtable(lua_State *L, int i, const char *name);
void luaL_traceback(lua_State *L, lua_State *L1, const char *msg, int level);
int luaL_fileresult(lua_State *L, int stat, const char *fname);

#endif // Lua 5.1 and below

#endif // SOL_5_X_X_H
// end of sol\compatibility\5.x.x.h

// beginning of sol\compatibility\5.x.x.inl

#ifndef SOL_5_X_X_INL
#define SOL_5_X_X_INL

#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM == 501

#include <errno.h>

#define PACKAGE_KEY "_sol.package"

inline int lua_absindex(lua_State *L, int i) {
    if (i < 0 && i > LUA_REGISTRYINDEX)
        i += lua_gettop(L) + 1;
    return i;
}

inline void lua_copy(lua_State *L, int from, int to) {
    int abs_to = lua_absindex(L, to);
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_pushvalue(L, from);
    lua_replace(L, abs_to);
}

inline void lua_rawgetp(lua_State *L, int i, const void *p) {
    int abs_i = lua_absindex(L, i);
    lua_pushlightuserdata(L, (void*)p);
    lua_rawget(L, abs_i);
}

inline void lua_rawsetp(lua_State *L, int i, const void *p) {
    int abs_i = lua_absindex(L, i);
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_pushlightuserdata(L, (void*)p);
    lua_insert(L, -2);
    lua_rawset(L, abs_i);
}

inline void *luaL_testudata(lua_State *L, int i, const char *tname) {
    void *p = lua_touserdata(L, i);
    luaL_checkstack(L, 2, "not enough stack slots");
    if (p == NULL || !lua_getmetatable(L, i))
        return NULL;
    else {
        int res = 0;
        luaL_getmetatable(L, tname);
        res = lua_rawequal(L, -1, -2);
        lua_pop(L, 2);
        if (!res)
            p = NULL;
    }
    return p;
}

inline lua_Number lua_tonumberx(lua_State *L, int i, int *isnum) {
    lua_Number n = lua_tonumber(L, i);
    if (isnum != NULL) {
        *isnum = (n != 0 || lua_isnumber(L, i));
    }
    return n;
}

inline static void push_package_table(lua_State *L) {
    lua_pushliteral(L, PACKAGE_KEY);
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        /* try to get package table from globals */
        lua_pushliteral(L, "package");
        lua_rawget(L, LUA_GLOBALSINDEX);
        if (lua_istable(L, -1)) {
            lua_pushliteral(L, PACKAGE_KEY);
            lua_pushvalue(L, -2);
            lua_rawset(L, LUA_REGISTRYINDEX);
        }
    }
}

inline void lua_getuservalue(lua_State *L, int i) {
    luaL_checktype(L, i, LUA_TUSERDATA);
    luaL_checkstack(L, 2, "not enough stack slots");
    lua_getfenv(L, i);
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    if (lua_rawequal(L, -1, -2)) {
        lua_pop(L, 1);
        lua_pushnil(L);
        lua_replace(L, -2);
    }
    else {
        lua_pop(L, 1);
        push_package_table(L);
        if (lua_rawequal(L, -1, -2)) {
            lua_pop(L, 1);
            lua_pushnil(L);
            lua_replace(L, -2);
        }
        else
            lua_pop(L, 1);
    }
}

inline void lua_setuservalue(lua_State *L, int i) {
    luaL_checktype(L, i, LUA_TUSERDATA);
    if (lua_isnil(L, -1)) {
        luaL_checkstack(L, 1, "not enough stack slots");
        lua_pushvalue(L, LUA_GLOBALSINDEX);
        lua_replace(L, -2);
    }
    lua_setfenv(L, i);
}

/*
** Adapted from Lua 5.2.0
*/
inline void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
    luaL_checkstack(L, nup + 1, "too many upvalues");
    for (; l->name != NULL; l++) {  /* fill the table with given functions */
        int i;
        lua_pushstring(L, l->name);
        for (i = 0; i < nup; i++)  /* copy upvalues to the top */
            lua_pushvalue(L, -(nup + 1));
        lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
        lua_settable(L, -(nup + 3)); /* table must be below the upvalues, the name and the closure */
    }
    lua_pop(L, nup);  /* remove upvalues */
}

inline void luaL_setmetatable(lua_State *L, const char *tname) {
    luaL_checkstack(L, 1, "not enough stack slots");
    luaL_getmetatable(L, tname);
    lua_setmetatable(L, -2);
}

inline int luaL_getsubtable(lua_State *L, int i, const char *name) {
    int abs_i = lua_absindex(L, i);
    luaL_checkstack(L, 3, "not enough stack slots");
    lua_pushstring(L, name);
    lua_gettable(L, abs_i);
    if (lua_istable(L, -1))
        return 1;
    lua_pop(L, 1);
    lua_newtable(L);
    lua_pushstring(L, name);
    lua_pushvalue(L, -2);
    lua_settable(L, abs_i);
    return 0;
}

#ifndef SOL_LUAJIT
inline static int countlevels(lua_State *L) {
    lua_Debug ar;
    int li = 1, le = 1;
    /* find an upper bound */
    while (lua_getstack(L, le, &ar)) { li = le; le *= 2; }
    /* do a binary search */
    while (li < le) {
        int m = (li + le) / 2;
        if (lua_getstack(L, m, &ar)) li = m + 1;
        else le = m;
    }
    return le - 1;
}

inline static int findfield(lua_State *L, int objidx, int level) {
    if (level == 0 || !lua_istable(L, -1))
        return 0;  /* not found */
    lua_pushnil(L);  /* start 'next' loop */
    while (lua_next(L, -2)) {  /* for each pair in table */
        if (lua_type(L, -2) == LUA_TSTRING) {  /* ignore non-string keys */
            if (lua_rawequal(L, objidx, -1)) {  /* found object? */
                lua_pop(L, 1);  /* remove value (but keep name) */
                return 1;
            }
            else if (findfield(L, objidx, level - 1)) {  /* try recursively */
                lua_remove(L, -2);  /* remove table (but keep name) */
                lua_pushliteral(L, ".");
                lua_insert(L, -2);  /* place '.' between the two names */
                lua_concat(L, 3);
                return 1;
            }
        }
        lua_pop(L, 1);  /* remove value */
    }
    return 0;  /* not found */
}

inline static int pushglobalfuncname(lua_State *L, lua_Debug *ar) {
    int top = lua_gettop(L);
    lua_getinfo(L, "f", ar);  /* push function */
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    if (findfield(L, top + 1, 2)) {
        lua_copy(L, -1, top + 1);  /* move name to proper place */
        lua_pop(L, 2);  /* remove pushed values */
        return 1;
    }
    else {
        lua_settop(L, top);  /* remove function and global table */
        return 0;
    }
}

inline static void pushfuncname(lua_State *L, lua_Debug *ar) {
    if (*ar->namewhat != '\0')  /* is there a name? */
        lua_pushfstring(L, "function " LUA_QS, ar->name);
    else if (*ar->what == 'm')  /* main? */
        lua_pushliteral(L, "main chunk");
    else if (*ar->what == 'C') {
        if (pushglobalfuncname(L, ar)) {
            lua_pushfstring(L, "function " LUA_QS, lua_tostring(L, -1));
            lua_remove(L, -2);  /* remove name */
        }
        else
            lua_pushliteral(L, "?");
    }
    else
        lua_pushfstring(L, "function <%s:%d>", ar->short_src, ar->linedefined);
}

#define LEVELS1 12  /* size of the first part of the stack */
#define LEVELS2 10  /* size of the second part of the stack */

inline void luaL_traceback(lua_State *L, lua_State *L1,
    const char *msg, int level) {
    lua_Debug ar;
    int top = lua_gettop(L);
    int numlevels = countlevels(L1);
    int mark = (numlevels > LEVELS1 + LEVELS2) ? LEVELS1 : 0;
    if (msg) lua_pushfstring(L, "%s\n", msg);
    lua_pushliteral(L, "stack traceback:");
    while (lua_getstack(L1, level++, &ar)) {
        if (level == mark) {  /* too many levels? */
            lua_pushliteral(L, "\n\t...");  /* add a '...' */
            level = numlevels - LEVELS2;  /* and skip to last ones */
        }
        else {
            lua_getinfo(L1, "Slnt", &ar);
            lua_pushfstring(L, "\n\t%s:", ar.short_src);
            if (ar.currentline > 0)
                lua_pushfstring(L, "%d:", ar.currentline);
            lua_pushliteral(L, " in ");
            pushfuncname(L, &ar);
            lua_concat(L, lua_gettop(L) - top);
        }
    }
    lua_concat(L, lua_gettop(L) - top);
}
#endif

inline const lua_Number *lua_version(lua_State *L) {
    static const lua_Number version = LUA_VERSION_NUM;
    if (L == NULL) return &version;
    // TODO: wonky hacks to get at the inside of the incomplete type lua_State?
    //else return L->l_G->version;
    else return &version;
}

inline static void luaL_checkversion_(lua_State *L, lua_Number ver) {
    const lua_Number* v = lua_version(L);
    if (v != lua_version(NULL))
        luaL_error(L, "multiple Lua VMs detected");
    else if (*v != ver)
        luaL_error(L, "version mismatch: app. needs %f, Lua core provides %f",
            ver, *v);
    /* check conversions number -> integer types */
    lua_pushnumber(L, -(lua_Number)0x1234);
    if (lua_tointeger(L, -1) != -0x1234 ||
        lua_tounsigned(L, -1) != (lua_Unsigned)-0x1234)
        luaL_error(L, "bad conversion number->int;"
            " must recompile Lua with proper settings");
    lua_pop(L, 1);
}

inline void luaL_checkversion(lua_State* L) {
    luaL_checkversion_(L, LUA_VERSION_NUM);
}

#ifndef SOL_LUAJIT
inline int luaL_fileresult(lua_State *L, int stat, const char *fname) {
    int en = errno;  /* calls to Lua API may change this value */
    if (stat) {
        lua_pushboolean(L, 1);
        return 1;
    }
    else {
        char buf[1024];
#ifdef __GLIBC__
        strerror_r(en, buf, 1024);
#else
        strerror_s(buf, 1024, en);
#endif
        lua_pushnil(L);
        if (fname)
            lua_pushfstring(L, "%s: %s", fname, buf);
        else
            lua_pushstring(L, buf);
        lua_pushnumber(L, (lua_Number)en);
        return 3;
    }
}
#endif // luajit
#endif // Lua 5.0 or Lua 5.1

#if SOL_LUA_VERSION == 501

typedef LUAI_INT32 LUA_INT32;

/********************************************************************/
/*                    extract of 5.2's luaconf.h                    */
/*  detects proper defines for faster unsigned<->number conversion  */
/*           see copyright notice at the end of this file           */
/********************************************************************/

#if !defined(LUA_ANSI) && defined(_WIN32) && !defined(_WIN32_WCE)
#define LUA_WIN        /* enable goodies for regular Windows platforms */
#endif

#if defined(LUA_NUMBER_DOUBLE) && !defined(LUA_ANSI)    /* { */

/* Microsoft compiler on a Pentium (32 bit) ? */
#if defined(LUA_WIN) && defined(_MSC_VER) && defined(_M_IX86)    /* { */

#define LUA_MSASMTRICK
#define LUA_IEEEENDIAN        0
#define LUA_NANTRICK

/* pentium 32 bits? */
#elif defined(__i386__) || defined(__i386) || defined(__X86__) /* }{ */

#define LUA_IEEE754TRICK
#define LUA_IEEELL
#define LUA_IEEEENDIAN        0
#define LUA_NANTRICK

/* pentium 64 bits? */
#elif defined(__x86_64)                        /* }{ */

#define LUA_IEEE754TRICK
#define LUA_IEEEENDIAN        0

#elif defined(__POWERPC__) || defined(__ppc__)            /* }{ */

#define LUA_IEEE754TRICK
#define LUA_IEEEENDIAN        1

#else                                /* }{ */

/* assume IEEE754 and a 32-bit integer type */
#define LUA_IEEE754TRICK

#endif                                /* } */

#endif                            /* } */

/********************************************************************/
/*                    extract of 5.2's llimits.h                    */
/*       gives us lua_number2unsigned and lua_unsigned2number       */
/*           see copyright notice just below this one here          */
/********************************************************************/

/*********************************************************************
* This file contains parts of Lua 5.2's source code:
*
* Copyright (C) 1994-2013 Lua.org, PUC-Rio.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************/

#if defined(MS_ASMTRICK) || defined(LUA_MSASMTRICK)    /* { */
/* trick with Microsoft assembler for X86 */

#define lua_number2unsigned(i,n)  \
  {__int64 l; __asm {__asm fld n   __asm fistp l} i = (unsigned int)l;}

#elif defined(LUA_IEEE754TRICK)        /* }{ */
/* the next trick should work on any machine using IEEE754 with
a 32-bit int type */

union compat52_luai_Cast { double l_d; LUA_INT32 l_p[2]; };

#if !defined(LUA_IEEEENDIAN)    /* { */
#define LUAI_EXTRAIEEE    \
  static const union compat52_luai_Cast ieeeendian = {-(33.0 + 6755399441055744.0)};
#define LUA_IEEEENDIANLOC    (ieeeendian.l_p[1] == 33)
#else
#define LUA_IEEEENDIANLOC    LUA_IEEEENDIAN
#define LUAI_EXTRAIEEE        /* empty */
#endif                /* } */

#define lua_number2int32(i,n,t) \
  { LUAI_EXTRAIEEE \
    volatile union compat52_luai_Cast u; u.l_d = (n) + 6755399441055744.0; \
    (i) = (t)u.l_p[LUA_IEEEENDIANLOC]; }

#define lua_number2unsigned(i,n)    lua_number2int32(i, n, lua_Unsigned)

#endif                /* } */

/* the following definitions always work, but may be slow */

#if !defined(lua_number2unsigned)    /* { */
/* the following definition assures proper modulo behavior */
#if defined(LUA_NUMBER_DOUBLE) || defined(LUA_NUMBER_FLOAT)
#include <math.h>
#define SUPUNSIGNED    ((lua_Number)(~(lua_Unsigned)0) + 1)
#define lua_number2unsigned(i,n)  \
    ((i)=(lua_Unsigned)((n) - floor((n)/SUPUNSIGNED)*SUPUNSIGNED))
#else
#define lua_number2unsigned(i,n)    ((i)=(lua_Unsigned)(n))
#endif
#endif                /* } */

#if !defined(lua_unsigned2number)
/* on several machines, coercion from unsigned to double is slow,
so it may be worth to avoid */
#define lua_unsigned2number(u)  \
    (((u) <= (lua_Unsigned)INT_MAX) ? (lua_Number)(int)(u) : (lua_Number)(u))
#endif

/********************************************************************/

inline static void compat52_call_lua(lua_State *L, char const code[], size_t len,
    int nargs, int nret) {
    lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)code);
    if (lua_type(L, -1) != LUA_TFUNCTION) {
        lua_pop(L, 1);
        if (luaL_loadbuffer(L, code, len, "=none"))
            lua_error(L);
        lua_pushvalue(L, -1);
        lua_rawsetp(L, LUA_REGISTRYINDEX, (void*)code);
    }
    lua_insert(L, -nargs - 1);
    lua_call(L, nargs, nret);
}

static const char compat52_arith_code[] = {
    "local op,a,b=...\n"
    "if op==0 then return a+b\n"
    "elseif op==1 then return a-b\n"
    "elseif op==2 then return a*b\n"
    "elseif op==3 then return a/b\n"
    "elseif op==4 then return a%b\n"
    "elseif op==5 then return a^b\n"
    "elseif op==6 then return -a\n"
    "end\n"
};

inline void lua_arith(lua_State *L, int op) {
    if (op < LUA_OPADD || op > LUA_OPUNM)
        luaL_error(L, "invalid 'op' argument for lua_arith");
    luaL_checkstack(L, 5, "not enough stack slots");
    if (op == LUA_OPUNM)
        lua_pushvalue(L, -1);
    lua_pushnumber(L, op);
    lua_insert(L, -3);
    compat52_call_lua(L, compat52_arith_code,
        sizeof(compat52_arith_code) - 1, 3, 1);
}

static const char compat52_compare_code[] = {
    "local a,b=...\n"
    "return a<=b\n"
};

inline int lua_compare(lua_State *L, int idx1, int idx2, int op) {
    int result = 0;
    switch (op) {
    case LUA_OPEQ:
        return lua_equal(L, idx1, idx2);
    case LUA_OPLT:
        return lua_lessthan(L, idx1, idx2);
    case LUA_OPLE:
        luaL_checkstack(L, 5, "not enough stack slots");
        idx1 = lua_absindex(L, idx1);
        idx2 = lua_absindex(L, idx2);
        lua_pushvalue(L, idx1);
        lua_pushvalue(L, idx2);
        compat52_call_lua(L, compat52_compare_code,
            sizeof(compat52_compare_code) - 1, 2, 1);
        result = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return result;
    default:
        luaL_error(L, "invalid 'op' argument for lua_compare");
    }
    return 0;
}

inline void lua_pushunsigned(lua_State *L, lua_Unsigned n) {
    lua_pushnumber(L, lua_unsigned2number(n));
}

inline lua_Unsigned luaL_checkunsigned(lua_State *L, int i) {
    lua_Unsigned result;
    lua_Number n = lua_tonumber(L, i);
    if (n == 0 && !lua_isnumber(L, i))
        luaL_checktype(L, i, LUA_TNUMBER);
    lua_number2unsigned(result, n);
    return result;
}

inline lua_Unsigned lua_tounsignedx(lua_State *L, int i, int *isnum) {
    lua_Unsigned result;
    lua_Number n = lua_tonumberx(L, i, isnum);
    lua_number2unsigned(result, n);
    return result;
}

inline lua_Unsigned luaL_optunsigned(lua_State *L, int i, lua_Unsigned def) {
    return luaL_opt(L, luaL_checkunsigned, i, def);
}

inline lua_Integer lua_tointegerx(lua_State *L, int i, int *isnum) {
    lua_Integer n = lua_tointeger(L, i);
    if (isnum != NULL) {
        *isnum = (n != 0 || lua_isnumber(L, i));
    }
    return n;
}

inline void lua_len(lua_State *L, int i) {
    switch (lua_type(L, i)) {
    case LUA_TSTRING: /* fall through */
    case LUA_TTABLE:
        if (!luaL_callmeta(L, i, "__len"))
            lua_pushnumber(L, (int)lua_objlen(L, i));
        break;
    case LUA_TUSERDATA:
        if (luaL_callmeta(L, i, "__len"))
            break;
        /* maybe fall through */
    default:
        luaL_error(L, "attempt to get length of a %s value",
            lua_typename(L, lua_type(L, i)));
    }
}

inline int luaL_len(lua_State *L, int i) {
    int res = 0, isnum = 0;
    luaL_checkstack(L, 1, "not enough stack slots");
    lua_len(L, i);
    res = (int)lua_tointegerx(L, -1, &isnum);
    lua_pop(L, 1);
    if (!isnum)
        luaL_error(L, "object length is not a number");
    return res;
}

inline const char *luaL_tolstring(lua_State *L, int idx, size_t *len) {
    if (!luaL_callmeta(L, idx, "__tostring")) {
        int t = lua_type(L, idx);
        switch (t) {
        case LUA_TNIL:
            lua_pushliteral(L, "nil");
            break;
        case LUA_TSTRING:
        case LUA_TNUMBER:
            lua_pushvalue(L, idx);
            break;
        case LUA_TBOOLEAN:
            if (lua_toboolean(L, idx))
                lua_pushliteral(L, "true");
            else
                lua_pushliteral(L, "false");
            break;
        default:
            lua_pushfstring(L, "%s: %p", lua_typename(L, t),
                lua_topointer(L, idx));
            break;
        }
    }
    return lua_tolstring(L, -1, len);
}

inline void luaL_requiref(lua_State *L, char const* modname,
    lua_CFunction openf, int glb) {
    luaL_checkstack(L, 3, "not enough stack slots");
    lua_pushcfunction(L, openf);
    lua_pushstring(L, modname);
    lua_call(L, 1, 1);
    lua_getglobal(L, "package");
    if (lua_istable(L, -1) == 0) {
        lua_pop(L, 1);
        lua_createtable(L, 0, 16);
        lua_setglobal(L, "package");
        lua_getglobal(L, "package");
    }
    lua_getfield(L, -1, "loaded");
    if (lua_istable(L, -1) == 0) {
        lua_pop(L, 1);
        lua_createtable(L, 0, 1);
        lua_setfield(L, -2, "loaded");
        lua_getfield(L, -1, "loaded");
    }
    lua_replace(L, -2);
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, modname);
    lua_pop(L, 1);
    if (glb) {
        lua_pushvalue(L, -1);
        lua_setglobal(L, modname);
    }
}

inline void luaL_buffinit(lua_State *L, luaL_Buffer_52 *B) {
    /* make it crash if used via pointer to a 5.1-style luaL_Buffer */
    B->b.p = NULL;
    B->b.L = NULL;
    B->b.lvl = 0;
    /* reuse the buffer from the 5.1-style luaL_Buffer though! */
    B->ptr = B->b.buffer;
    B->capacity = LUAL_BUFFERSIZE;
    B->nelems = 0;
    B->L2 = L;
}

inline char *luaL_prepbuffsize(luaL_Buffer_52 *B, size_t s) {
    if (B->capacity - B->nelems < s) { /* needs to grow */
        char* newptr = NULL;
        size_t newcap = B->capacity * 2;
        if (newcap - B->nelems < s)
            newcap = B->nelems + s;
        if (newcap < B->capacity) /* overflow */
            luaL_error(B->L2, "buffer too large");
        newptr = (char*)lua_newuserdata(B->L2, newcap);
        memcpy(newptr, B->ptr, B->nelems);
        if (B->ptr != B->b.buffer)
            lua_replace(B->L2, -2); /* remove old buffer */
        B->ptr = newptr;
        B->capacity = newcap;
    }
    return B->ptr + B->nelems;
}

inline void luaL_addlstring(luaL_Buffer_52 *B, const char *s, size_t l) {
    memcpy(luaL_prepbuffsize(B, l), s, l);
    luaL_addsize(B, l);
}

inline void luaL_addvalue(luaL_Buffer_52 *B) {
    size_t len = 0;
    const char *s = lua_tolstring(B->L2, -1, &len);
    if (!s)
        luaL_error(B->L2, "cannot convert value to string");
    if (B->ptr != B->b.buffer)
        lua_insert(B->L2, -2); /* userdata buffer must be at stack top */
    luaL_addlstring(B, s, len);
    lua_remove(B->L2, B->ptr != B->b.buffer ? -2 : -1);
}

inline void luaL_pushresult(luaL_Buffer_52 *B) {
    lua_pushlstring(B->L2, B->ptr, B->nelems);
    if (B->ptr != B->b.buffer)
        lua_replace(B->L2, -2); /* remove userdata buffer */
}

#endif /* SOL_LUA_VERSION == 501 */

#endif // SOL_5_X_X_INL
// end of sol\compatibility\5.x.x.inl

#ifdef __cplusplus
}
#endif

// end of sol\compatibility.hpp

// beginning of sol\optional.hpp

#if __cplusplus > 201402L
#include <optional>
#else
// beginning of Optional\optional.hpp

# ifndef ___OPTIONAL_HPP___
# define ___OPTIONAL_HPP___

# include <utility>
# include <type_traits>
# include <initializer_list>
# include <cassert>
# include <functional>
# include <string>
# include <stdexcept>

# define TR2_OPTIONAL_REQUIRES(...) typename ::std::enable_if<__VA_ARGS__::value, bool>::type = false

# if defined __GNUC__ // NOTE: GNUC is also defined for Clang
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)
#     define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
#   endif
#
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)
#     define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_7_AND_HIGHER___
#   endif
#
#   if (__GNUC__ == 4) && (__GNUC_MINOR__ == 8) && (__GNUC_PATCHLEVEL__ >= 1)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   elif (__GNUC__ > 4)
#     define TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   endif
# endif
#
# if defined __clang_major__
#   if (__clang_major__ == 3 && __clang_minor__ >= 5)
#     define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   elif (__clang_major__ > 3)
#     define TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#   endif
#   if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_
#     define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   elif (__clang_major__ == 3 && __clang_minor__ == 4 && __clang_patchlevel__ >= 2)
#     define TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
#   endif
# endif
#
# if defined _MSC_VER
#   if (_MSC_VER >= 1900)
#     define TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   endif
# endif

# if defined __clang__
#   if (__clang_major__ > 2) || (__clang_major__ == 2) && (__clang_minor__ >= 9)
#     define OPTIONAL_HAS_THIS_RVALUE_REFS 1
#   else
#     define OPTIONAL_HAS_THIS_RVALUE_REFS 0
#   endif
# elif defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 1
# else
#   define OPTIONAL_HAS_THIS_RVALUE_REFS 0
# endif

# if defined TR2_OPTIONAL_GCC_4_8_1_AND_HIGHER___
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 1
#   define OPTIONAL_CONSTEXPR_INIT_LIST constexpr
# else
#   define OPTIONAL_HAS_CONSTEXPR_INIT_LIST 0
#   define OPTIONAL_CONSTEXPR_INIT_LIST
# endif

# if defined TR2_OPTIONAL_CLANG_3_5_AND_HIGHTER_ && (defined __cplusplus) && (__cplusplus != 201103L)
#   define OPTIONAL_HAS_MOVE_ACCESSORS 1
# else
#   define OPTIONAL_HAS_MOVE_ACCESSORS 0
# endif

# // In C++11 constexpr implies const, so we need to make non-const members also non-constexpr
# if (defined __cplusplus) && (__cplusplus == 201103L)
#   define OPTIONAL_MUTABLE_CONSTEXPR
# else
#   define OPTIONAL_MUTABLE_CONSTEXPR constexpr
# endif

namespace sol{

# if defined TR2_OPTIONAL_GCC_4_8_AND_HIGHER___
    // leave it: it is already there
# elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
    // leave it: it is already there
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
    // leave it: it is already there
# elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
    // leave it: the user doesn't want it
# else
    template <typename T>
    using is_trivially_destructible = std::has_trivial_destructor<T>;
# endif

# if (defined TR2_OPTIONAL_GCC_4_7_AND_HIGHER___)
    // leave it; our metafunctions are already defined.
# elif defined TR2_OPTIONAL_CLANG_3_4_2_AND_HIGHER_
    // leave it; our metafunctions are already defined.
# elif defined TR2_OPTIONAL_MSVC_2015_AND_HIGHER___
    // leave it: it is already there
# elif defined TR2_OPTIONAL_DISABLE_EMULATION_OF_TYPE_TRAITS
    // leave it: the user doesn't want it
# else

template <class T>
struct is_nothrow_move_constructible
{
  constexpr static bool value = std::is_nothrow_constructible<T, T&&>::value;
};

template <class T, class U>
struct is_assignable
{
  template <class X, class Y>
  constexpr static bool has_assign(...) { return false; }

  template <class X, class Y, size_t S = sizeof((std::declval<X>() = std::declval<Y>(), true)) >
  // the comma operator is necessary for the cases where operator= returns void
  constexpr static bool has_assign(bool) { return true; }

  constexpr static bool value = has_assign<T, U>(true);
};

template <class T>
struct is_nothrow_move_assignable
{
  template <class X, bool has_any_move_assign>
  struct has_nothrow_move_assign {
    constexpr static bool value = false;
  };

  template <class X>
  struct has_nothrow_move_assign<X, true> {
    constexpr static bool value = noexcept( std::declval<X&>() = std::declval<X&&>() );
  };

  constexpr static bool value = has_nothrow_move_assign<T, is_assignable<T&, T&&>::value>::value;
};

# endif

template <class T> class optional;

template <class T> class optional<T&>;

template <class T> inline constexpr T&& constexpr_forward(typename ::std::remove_reference<T>::type& t) noexcept
{
  return static_cast<T&&>(t);
}

template <class T> inline constexpr T&& constexpr_forward(typename ::std::remove_reference<T>::type&& t) noexcept
{
    static_assert(!::std::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T> inline constexpr typename ::std::remove_reference<T>::type&& constexpr_move(T&& t) noexcept
{
    return static_cast<typename ::std::remove_reference<T>::type&&>(t);
}

#if defined NDEBUG
# define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) (EXPR)
#else
# define TR2_OPTIONAL_ASSERTED_EXPRESSION(CHECK, EXPR) ((CHECK) ? (EXPR) : ([]{assert(!#CHECK);}(), (EXPR)))
#endif

namespace detail_
{

template <typename T>
struct has_overloaded_addressof
{
  template <class X>
  constexpr static bool has_overload(...) { return false; }
  
  template <class X, size_t S = sizeof(::std::declval<X&>().operator&()) >
  constexpr static bool has_overload(bool) { return true; }

  constexpr static bool value = has_overload<T>(true);
};

template <typename T, TR2_OPTIONAL_REQUIRES(!has_overloaded_addressof<T>)>
constexpr T* static_addressof(T& ref)
{
  return &ref;
}

template <typename T, TR2_OPTIONAL_REQUIRES(has_overloaded_addressof<T>)>
T* static_addressof(T& ref)
{
  return std::addressof(ref);
}

template <class U>
U convert(U v) { return v; }
  
} // namespace detail

constexpr struct trivial_init_t{} trivial_init{};

constexpr struct in_place_t{} in_place{};

struct nullopt_t
{
  struct init{};
  constexpr explicit nullopt_t(init){}
};
constexpr nullopt_t nullopt{nullopt_t::init()};

class bad_optional_access : public ::std::logic_error {
public:
  explicit bad_optional_access(const ::std::string& what_arg) : logic_error{what_arg} {}
  explicit bad_optional_access(const char* what_arg) : logic_error{what_arg} {}
};

template <class T>
union storage_t
{
  unsigned char dummy_;
  T value_;

  constexpr storage_t( trivial_init_t ) noexcept : dummy_() {};

  template <class... Args>
  constexpr storage_t( Args&&... args ) : value_(constexpr_forward<Args>(args)...) {}

  ~storage_t(){}
};

template <class T>
union constexpr_storage_t
{
    unsigned char dummy_;
    T value_;

    constexpr constexpr_storage_t( trivial_init_t ) noexcept : dummy_() {};

    template <class... Args>
    constexpr constexpr_storage_t( Args&&... args ) : value_(constexpr_forward<Args>(args)...) {}

    ~constexpr_storage_t() = default;
};

template <class T>
struct optional_base
{
    bool init_;
    storage_t<T> storage_;

    constexpr optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args> explicit optional_base(in_place_t, Args&&... args)
        : init_(true), storage_(constexpr_forward<Args>(args)...) {}

    template <class U, class... Args, TR2_OPTIONAL_REQUIRES(::std::is_constructible<T, std::initializer_list<U>>)>
    explicit optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
        : init_(true), storage_(il, std::forward<Args>(args)...) {}

    ~optional_base() { if (init_) storage_.value_.T::~T(); }
};

template <class T>
struct constexpr_optional_base
{
    bool init_;
    constexpr_storage_t<T> storage_;

    constexpr constexpr_optional_base() noexcept : init_(false), storage_(trivial_init) {};

    explicit constexpr constexpr_optional_base(const T& v) : init_(true), storage_(v) {}

    explicit constexpr constexpr_optional_base(T&& v) : init_(true), storage_(constexpr_move(v)) {}

    template <class... Args> explicit constexpr constexpr_optional_base(in_place_t, Args&&... args)
      : init_(true), storage_(constexpr_forward<Args>(args)...) {}

    template <class U, class... Args, TR2_OPTIONAL_REQUIRES(::std::is_constructible<T, std::initializer_list<U>>)>
    OPTIONAL_CONSTEXPR_INIT_LIST explicit constexpr_optional_base(in_place_t, std::initializer_list<U> il, Args&&... args)
      : init_(true), storage_(il, std::forward<Args>(args)...) {}

    ~constexpr_optional_base() = default;
};

template <class T>
using OptionalBase = typename std::conditional<
    ::std::is_trivially_destructible<T>::value,
    constexpr_optional_base<T>,
    optional_base<T>
>::type;

template <class T>
class optional : private OptionalBase<T>
{
  static_assert( !std::is_same<typename std::decay<T>::type, nullopt_t>::value, "bad T" );
  static_assert( !std::is_same<typename std::decay<T>::type, in_place_t>::value, "bad T" );
  
  constexpr bool initialized() const noexcept { return OptionalBase<T>::init_; }
  T* dataptr() {  return std::addressof(OptionalBase<T>::storage_.value_); }
  constexpr const T* dataptr() const { return detail_::static_addressof(OptionalBase<T>::storage_.value_); }
  
# if OPTIONAL_HAS_THIS_RVALUE_REFS == 1
  constexpr const T& contained_val() const& { return OptionalBase<T>::storage_.value_; }
#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1
  OPTIONAL_MUTABLE_CONSTEXPR T&& contained_val() && { return std::move(OptionalBase<T>::storage_.value_); }
  OPTIONAL_MUTABLE_CONSTEXPR T& contained_val() & { return OptionalBase<T>::storage_.value_; }
#   else
  T& contained_val() & { return OptionalBase<T>::storage_.value_; }
  T&& contained_val() && { return std::move(OptionalBase<T>::storage_.value_); }
#   endif
# else
  constexpr const T& contained_val() const { return OptionalBase<T>::storage_.value_; }
  T& contained_val() { return OptionalBase<T>::storage_.value_; }
# endif

  void clear() noexcept {
    if (initialized()) dataptr()->T::~T();
    OptionalBase<T>::init_ = false;
  }
  
  template <class... Args>
  void initialize(Args&&... args) noexcept(noexcept(T(::std::forward<Args>(args)...)))
  {
    assert(!OptionalBase<T>::init_);
    ::new (static_cast<void*>(dataptr())) T(::std::forward<Args>(args)...);
    OptionalBase<T>::init_ = true;
  }

  template <class U, class... Args>
  void initialize(::std::initializer_list<U> il, Args&&... args) noexcept(noexcept(T(il, ::std::forward<Args>(args)...)))
  {
    assert(!OptionalBase<T>::init_);
    ::new (static_cast<void*>(dataptr())) T(il, ::std::forward<Args>(args)...);
    OptionalBase<T>::init_ = true;
  }

public:
  typedef T value_type;

  // 20.5.5.1, constructors
  constexpr optional() noexcept : OptionalBase<T>()  {};
  constexpr optional(nullopt_t) noexcept : OptionalBase<T>() {};

  optional(const optional<T&>& rhs) : optional() {
    if (rhs) {
      ::new (static_cast<void*>(dataptr())) T(*rhs);
      OptionalBase<T>::init_ = true;
    }
  }

  optional(const optional& rhs)
  : OptionalBase<T>()
  {
    if (rhs.initialized()) {
        ::new (static_cast<void*>(dataptr())) T(*rhs);
        OptionalBase<T>::init_ = true;
    }
  }

  optional(optional&& rhs) noexcept(::std::is_nothrow_move_constructible<T>::value)
  : OptionalBase<T>()
  {
    if (rhs.initialized()) {
        ::new (static_cast<void*>(dataptr())) T(std::move(*rhs));
        OptionalBase<T>::init_ = true;
    }
  }

  constexpr optional(const T& v) : OptionalBase<T>(v) {}

  constexpr optional(T&& v) : OptionalBase<T>(constexpr_move(v)) {}

  template <class... Args>
  explicit constexpr optional(in_place_t, Args&&... args)
  : OptionalBase<T>(in_place_t{}, constexpr_forward<Args>(args)...) {}

  template <class U, class... Args, TR2_OPTIONAL_REQUIRES(::std::is_constructible<T, std::initializer_list<U>>)>
  OPTIONAL_CONSTEXPR_INIT_LIST explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
  : OptionalBase<T>(in_place_t{}, il, constexpr_forward<Args>(args)...) {}

  // 20.5.4.2, Destructor
  ~optional() = default;

  // 20.5.4.3, assignment
  optional& operator=(nullopt_t) noexcept
  {
    clear();
    return *this;
  }
  
  optional& operator=(const optional& rhs)
  {
    if      (initialized() == true  && rhs.initialized() == false) clear();
    else if (initialized() == false && rhs.initialized() == true)  initialize(*rhs);
    else if (initialized() == true  && rhs.initialized() == true)  contained_val() = *rhs;
    return *this;
  }
  
  optional& operator=(optional&& rhs)
  noexcept(::std::is_nothrow_move_assignable<T>::value && ::std::is_nothrow_move_constructible<T>::value)
  {
    if      (initialized() == true  && rhs.initialized() == false) clear();
    else if (initialized() == false && rhs.initialized() == true)  initialize(std::move(*rhs));
    else if (initialized() == true  && rhs.initialized() == true)  contained_val() = std::move(*rhs);
    return *this;
  }

  template <class U>
  auto operator=(U&& v)
  -> typename ::std::enable_if
  <
    ::std::is_same<typename ::std::decay<U>::type, T>::value,
    optional&
  >::type
  {
    if (initialized()) { contained_val() = ::std::forward<U>(v); }
    else               { initialize(::std::forward<U>(v));  }
    return *this;
  }
  
  template <class... Args>
  void emplace(Args&&... args)
  {
    clear();
    initialize(::std::forward<Args>(args)...);
  }
  
  template <class U, class... Args>
  void emplace(::std::initializer_list<U> il, Args&&... args)
  {
    clear();
    initialize<U, Args...>(il, std::forward<Args>(args)...);
  }
  
  // 20.5.4.4, Swap
  void swap(optional<T>& rhs) noexcept(::std::is_nothrow_move_constructible<T>::value && noexcept(::std::swap(::std::declval<T&>(), ::std::declval<T&>())))
  {
    if      (initialized() == true  && rhs.initialized() == false) { rhs.initialize(::std::move(**this)); clear(); }
    else if (initialized() == false && rhs.initialized() == true)  { initialize(::std::move(*rhs)); rhs.clear(); }
    else if (initialized() == true  && rhs.initialized() == true)  { using ::std::swap; swap(**this, *rhs); }
  }

  // 20.5.4.5, Observers
  
  explicit constexpr operator bool() const noexcept { return initialized(); }
  
  constexpr T const* operator ->() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), dataptr());
  }
  
# if OPTIONAL_HAS_MOVE_ACCESSORS == 1

  OPTIONAL_MUTABLE_CONSTEXPR T* operator ->() {
    assert (initialized());
    return dataptr();
  }
  
  constexpr T const& operator *() const& {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T& operator *() & {
    assert (initialized());
    return contained_val();
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T&& operator *() && {
    assert (initialized());
    return constexpr_move(contained_val());
  }

  constexpr T const& value() const& {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T& value() & {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  OPTIONAL_MUTABLE_CONSTEXPR T&& value() && {
    if (!initialized()) throw bad_optional_access("bad optional access");
    return ::std::move(contained_val());
  }
  
# else

  T* operator ->() {
    assert (initialized());
    return dataptr();
  }
  
  constexpr T const& operator *() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(initialized(), contained_val());
  }
  
  T& operator *() {
    assert (initialized());
    return contained_val();
  }
  
  constexpr T const& value() const {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
  T& value() {
    return initialized() ? contained_val() : (throw bad_optional_access("bad optional access"), contained_val());
  }
  
# endif
  
# if OPTIONAL_HAS_THIS_RVALUE_REFS == 1

  template <class V>
  constexpr T value_or(V&& v) const&
  {
    return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
  }
  
#   if OPTIONAL_HAS_MOVE_ACCESSORS == 1

  template <class V>
  OPTIONAL_MUTABLE_CONSTEXPR T value_or(V&& v) &&
  {
    return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
  }

#   else
 
  template <class V>
  T value_or(V&& v) &&
  {
    return *this ? constexpr_move(const_cast<optional<T>&>(*this).contained_val()) : detail_::convert<T>(constexpr_forward<V>(v));
  }
  
#   endif
  
# else
  
  template <class V>
  constexpr T value_or(V&& v) const
  {
    return *this ? **this : detail_::convert<T>(constexpr_forward<V>(v));
  }

# endif

};

template <class T>
class optional<T&>
{
  static_assert( !std::is_same<T, nullopt_t>::value, "bad T" );
  static_assert( !std::is_same<T, in_place_t>::value, "bad T" );
  T* ref;
  
public:
  typedef T& value_type;

  // 20.5.5.1, construction/destruction
  constexpr optional() noexcept : ref(nullptr) {}
  
  constexpr optional(nullopt_t) noexcept : ref(nullptr) {}
   
  constexpr optional(T& v) noexcept : ref(detail_::static_addressof(v)) {}
  
  optional(T&&) = delete;
  
  constexpr optional(const optional& rhs) noexcept : ref(rhs.ref) {}
  
  explicit constexpr optional(in_place_t, T& v) noexcept : ref(detail_::static_addressof(v)) {}
  
  explicit optional(in_place_t, T&&) = delete;
  
  ~optional() = default;
  
  // 20.5.5.2, mutation
  optional& operator=(nullopt_t) noexcept {
    ref = nullptr;
    return *this;
  }
  
  // optional& operator=(const optional& rhs) noexcept {
    // ref = rhs.ref;
    // return *this;
  // }
  
  // optional& operator=(optional&& rhs) noexcept {
    // ref = rhs.ref;
    // return *this;
  // }
  
  template <typename U>
  auto operator=(U&& rhs) noexcept
  -> typename ::std::enable_if
  <
    ::std::is_same<typename ::std::decay<U>::type, optional<T&>>::value,
    optional&
  >::type
  {
    ref = rhs.ref;
    return *this;
  }
  
  template <typename U>
  auto operator=(U&& rhs) noexcept
  -> typename ::std::enable_if
  <
    !::std::is_same<typename ::std::decay<U>::type, optional<T&>>::value,
    optional&
  >::type
  = delete;
  
  void emplace(T& v) noexcept {
    ref = detail_::static_addressof(v);
  }
  
  void emplace(T&&) = delete;
  
  void swap(optional<T&>& rhs) noexcept
  {
    ::std::swap(ref, rhs.ref);
  }
    
  // 20.5.5.3, observers
  constexpr T* operator->() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, ref);
  }
  
  constexpr T& operator*() const {
    return TR2_OPTIONAL_ASSERTED_EXPRESSION(ref, *ref);
  }
  
  constexpr T& value() const {
    return ref ? *ref : (throw bad_optional_access("bad optional access"), *ref);
  }
  
  explicit constexpr operator bool() const noexcept {
    return ref != nullptr;
  }
  
  template <class V>
  constexpr typename ::std::decay<T>::type value_or(V&& v) const
  {
    return *this ? **this : detail_::convert<typename ::std::decay<T>::type>(constexpr_forward<V>(v));
  }
};

template <class T>
class optional<T&&>
{
  static_assert( sizeof(T) == 0, "optional rvalue references disallowed" );
};

template <class T> constexpr bool operator==(const optional<T>& x, const optional<T>& y)
{
  return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <class T> constexpr bool operator!=(const optional<T>& x, const optional<T>& y)
{
  return !(x == y);
}

template <class T> constexpr bool operator<(const optional<T>& x, const optional<T>& y)
{
  return (!y) ? false : (!x) ? true : *x < *y;
}

template <class T> constexpr bool operator>(const optional<T>& x, const optional<T>& y)
{
  return (y < x);
}

template <class T> constexpr bool operator<=(const optional<T>& x, const optional<T>& y)
{
  return !(y < x);
}

template <class T> constexpr bool operator>=(const optional<T>& x, const optional<T>& y)
{
  return !(x < y);
}

template <class T> constexpr bool operator==(const optional<T>& x, nullopt_t) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator==(nullopt_t, const optional<T>& x) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator!=(const optional<T>& x, nullopt_t) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator!=(nullopt_t, const optional<T>& x) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator<(const optional<T>&, nullopt_t) noexcept
{
  return false;
}

template <class T> constexpr bool operator<(nullopt_t, const optional<T>& x) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator<=(const optional<T>& x, nullopt_t) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator<=(nullopt_t, const optional<T>&) noexcept
{
  return true;
}

template <class T> constexpr bool operator>(const optional<T>& x, nullopt_t) noexcept
{
  return bool(x);
}

template <class T> constexpr bool operator>(nullopt_t, const optional<T>&) noexcept
{
  return false;
}

template <class T> constexpr bool operator>=(const optional<T>&, nullopt_t) noexcept
{
  return true;
}

template <class T> constexpr bool operator>=(nullopt_t, const optional<T>& x) noexcept
{
  return (!x);
}

template <class T> constexpr bool operator==(const optional<T>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<T>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<T>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<T>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<T>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<T>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<T>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<T>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<T>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<T>& x)
{
  return bool(x) ? v >= *x : true;
}

template <class T> constexpr bool operator==(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<T&>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<T&>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<T&>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<T&>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<T&>& x)
{
  return bool(x) ? v >= *x : true;
}

template <class T> constexpr bool operator==(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x == v : false;
}

template <class T> constexpr bool operator==(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v == *x : false;
}

template <class T> constexpr bool operator!=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x != v : true;
}

template <class T> constexpr bool operator!=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v != *x : true;
}

template <class T> constexpr bool operator<(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x < v : true;
}

template <class T> constexpr bool operator>(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v > *x : true;
}

template <class T> constexpr bool operator>(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x > v : false;
}

template <class T> constexpr bool operator<(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v < *x : false;
}

template <class T> constexpr bool operator>=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x >= v : false;
}

template <class T> constexpr bool operator<=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v <= *x : false;
}

template <class T> constexpr bool operator<=(const optional<const T&>& x, const T& v)
{
  return bool(x) ? *x <= v : true;
}

template <class T> constexpr bool operator>=(const T& v, const optional<const T&>& x)
{
  return bool(x) ? v >= *x : true;
}

template <class T>
void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
{
  x.swap(y);
}

template <class T>
constexpr optional<typename ::std::decay<T>::type> make_optional(T&& v)
{
  return optional<typename ::std::decay<T>::type>(constexpr_forward<T>(v));
}

template <class X>
constexpr optional<X&> make_optional(::std::reference_wrapper<X> v)
{
  return optional<X&>(v.get());
}

} // namespace sol

namespace std
{
  template <typename T>
  struct hash<sol::optional<T>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef sol::optional<T> argument_type;
    
    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
  
  template <typename T>
  struct hash<sol::optional<T&>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef sol::optional<T&> argument_type;
    
    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
}

# undef TR2_OPTIONAL_REQUIRES
# undef TR2_OPTIONAL_ASSERTED_EXPRESSION

# endif //___OPTIONAL_HPP___
// end of Optional\optional.hpp

#endif // C++ 14

namespace sol {

#if __cplusplus > 201402L
template <typename T>
using optional = sol::optional<T>;
using nullopt_t = std::nullopt_t;
constexpr nullopt_t nullopt = std::nullopt;
#else
#endif // C++ 14
}

// end of sol\optional.hpp

#include <array>

namespace sol {
namespace detail {
#ifdef SOL_NO_EXCEPTIONS
template <lua_CFunction f>
inline int static_trampoline (lua_State* L) {
    return f(L);
}

template <typename Fx, typename... Args>
inline int trampoline(lua_State* L, Fx&& f, Args&&... args) {
    return f(L, std::forward<Args>(args)...);
}

inline int c_trampoline(lua_State* L, lua_CFunction f) {
    return trampoline(L, f);
}
#else
template <lua_CFunction f>
inline int static_trampoline (lua_State* L) {
    try {
        return f(L);
    }
    catch (const char *s) {
        lua_pushstring(L, s);
    }
    catch (const std::exception& e) {
        lua_pushstring(L, e.what());
    }
    catch (...) {
        lua_pushstring(L, "caught (...) exception");
    }
    return lua_error(L);
}

template <typename Fx, typename... Args>
inline int trampoline(lua_State* L, Fx&& f, Args&&... args) {
    try {
        return f(L, std::forward<Args>(args)...);
    }
    catch (const char *s) {
        lua_pushstring(L, s);
    }
    catch (const std::exception& e) {
        lua_pushstring(L, e.what());
    }
    catch (...) {
        lua_pushstring(L, "caught (...) exception");
    }
    return lua_error(L);
}

inline int c_trampoline(lua_State* L, lua_CFunction f) {
    return trampoline(L, f);
}
#endif // Exceptions vs. No Exceptions
struct empty { void operator()() {} };

template <typename T>
struct unique_usertype {};
} // detail
struct nil_t {};
const nil_t nil {};
struct metatable_key_t {};
const metatable_key_t metatable_key = {};
inline bool operator==(nil_t, nil_t) { return true; }
inline bool operator!=(nil_t, nil_t) { return false; }

typedef std::remove_pointer_t<lua_CFunction> lua_r_CFunction;

template <typename T>
struct unique_usertype_traits {
    typedef T type;
    typedef T actual_type;
    static const bool value = false;

    template <typename U>
    static bool is_null(U&&) {
        return false;
    }

    template <typename U>
    static auto get(U&& value) {
        return std::addressof(detail::deref(value));
    }
};

template <typename T>
struct unique_usertype_traits<std::shared_ptr<T>> {
    typedef T type;
    typedef std::shared_ptr<T> actual_type;
    static const bool value = true;

    static bool is_null(const actual_type& value) {
        return value == nullptr;
    }

    static type* get(const actual_type& p) {
        return p.get();
    }
};

template <typename T, typename D>
struct unique_usertype_traits<std::unique_ptr<T, D>> {
    typedef T type;
    typedef std::unique_ptr<T, D> actual_type;
    static const bool value = true;

    static bool is_null(const actual_type& value) {
        return value == nullptr;
    }

    static type* get(const actual_type& p) {
        return p.get();
    }
};

template <typename T>
struct non_null {};

template<typename... Args>
struct function_sig {};

struct up_value_index {
    int index;
    up_value_index(int idx) : index(lua_upvalueindex(idx)) {}
    operator int() const { return index; }
};

struct absolute_index {
    int index;
    absolute_index(lua_State* L, int idx) : index(lua_absindex(L, idx)) {}
    operator int() const { return index; }
};

struct light_userdata_value {
    void* value;
    light_userdata_value(void* data) : value(data) {}
    operator void*() const { return value; }
};

struct userdata_value {
    void* value;
    userdata_value(void* data) : value(data) {}
    operator void*() const { return value; }
};

struct c_closure {
    lua_CFunction c_function;
    int upvalues;
    c_closure(lua_CFunction f, int upvalues = 0) : c_function(f), upvalues(upvalues) {}
};

struct this_state {
    lua_State* L;
    operator lua_State* () const {
        return L;
    }
    lua_State* operator-> () const {
        return L;
    }
};

enum class call_syntax {
    dot = 0,
    colon = 1
};

enum class call_status : int {
    ok      = LUA_OK,
    yielded = LUA_YIELD,
    runtime = LUA_ERRRUN,
    memory  = LUA_ERRMEM,
    handler = LUA_ERRERR,
    gc      = LUA_ERRGCMM
};

enum class thread_status : int {
    ok      = LUA_OK,
    yielded = LUA_YIELD,
    runtime = LUA_ERRRUN,
    memory  = LUA_ERRMEM,
    gc      = LUA_ERRGCMM,
    handler = LUA_ERRERR,
    dead,
};

enum class load_status : int {
    ok        = LUA_OK,
    syntax    = LUA_ERRSYNTAX,
    memory    = LUA_ERRMEM,
    gc        = LUA_ERRGCMM,
    file      = LUA_ERRFILE,
};

enum class type : int {
    none          = LUA_TNONE,
    nil           = LUA_TNIL,
    string        = LUA_TSTRING,
    number        = LUA_TNUMBER,
    thread        = LUA_TTHREAD,
    boolean       = LUA_TBOOLEAN,
    function      = LUA_TFUNCTION,
    userdata      = LUA_TUSERDATA,
    lightuserdata = LUA_TLIGHTUSERDATA,
    table         = LUA_TTABLE,
    poly          = none   | nil     | string   | number   | thread          |
                    table  | boolean | function | userdata | lightuserdata
};

enum class meta_function {
    construct,
    index,
    new_index,
    mode,
    call,
    metatable,
    to_string,
    length,
    unary_minus,
    addition,
    subtraction,
    multiplication,
    division,
    modulus,
    power_of,
    involution = power_of,
    concatenation,
    equal_to,
    less_than,
    less_than_or_equal_to,
    garbage_collect,
    call_function = call,
};

const std::array<std::string, 2> meta_variable_names = { {
    "__index",
    "__newindex",
} };

const std::array<std::string, 21> meta_function_names = { {
    "new",
    "__index",
    "__newindex",
    "__mode",
    "__call",
    "__metatable",
    "__tostring",
    "__len",
    "__unm",
    "__add",
    "__sub",
    "__mul",
    "__div",
    "__mod",
    "__pow",
    "__concat",
    "__eq",
    "__lt",
    "__le",
    "__gc",
} };

inline const std::string& name_of( meta_function mf ) {
    return meta_function_names[static_cast<int>(mf)];
}

inline type type_of(lua_State* L, int index) {
    return static_cast<type>(lua_type(L, index));
}

inline int type_panic(lua_State* L, int index, type expected, type actual) {
    return luaL_error(L, "stack index %d, expected %s, received %s", index, 
        expected == type::poly ? "anything" : lua_typename(L, static_cast<int>(expected)),
        expected == type::poly ? "anything" : lua_typename(L, static_cast<int>(actual))
    );
}

inline int no_panic(lua_State*, int, type, type) noexcept {
    return 0;
}

inline void type_error(lua_State* L, int expected, int actual) {
    luaL_error(L, "expected %s, received %s", lua_typename(L, expected), lua_typename(L, actual));
}

inline void type_error(lua_State* L, type expected, type actual) {
    type_error(L, static_cast<int>(expected), static_cast<int>(actual));
}

inline void type_assert(lua_State* L, int index, type expected, type actual) {
    if (expected != type::poly && expected != actual) {
           type_panic(L, index, expected, actual);
    }
}

inline void type_assert(lua_State* L, int index, type expected) {
    type actual = type_of(L, index);
    type_assert(L, index, expected, actual);
}

inline std::string type_name(lua_State* L, type t) {
    return lua_typename(L, static_cast<int>(t));
}

class reference;
class stack_reference;
template<typename T>
class usertype;
template <bool, typename T>
class basic_table_core;
template <bool b>
using table_core = basic_table_core<b, reference>;
template <bool b>
using stack_table_core = basic_table_core<b, stack_reference>;
typedef table_core<false> table;
typedef table_core<true> global_table;
typedef stack_table_core<false> stack_table;
typedef stack_table_core<true> stack_global_table;
template <typename T>
class basic_function;
template <typename T>
class basic_protected_function;
using function = basic_function<reference>;
using protected_function = basic_protected_function<reference>;
using stack_function = basic_function<stack_reference>;
using stack_protected_function = basic_protected_function<stack_reference>;
template <typename base_t>
class basic_object;
template <typename base_t>
class basic_userdata;
template <typename base_t>
class basic_lightuserdata;
struct variadic_args;
using object = basic_object<reference>;
using stack_object = basic_object<stack_reference>;
using userdata = basic_userdata<reference>;
using stack_userdata = basic_userdata<stack_reference>;
using lightuserdata = basic_lightuserdata<reference>;
using stack_lightuserdata = basic_lightuserdata<stack_reference>;
class coroutine;
class thread;
struct variadic_args;
struct this_state;

template <typename T, typename = void>
struct lua_type_of : std::integral_constant<type, type::userdata> {};

template <>
struct lua_type_of<std::string> : std::integral_constant<type, type::string> {};

template <std::size_t N>
struct lua_type_of<char[N]> : std::integral_constant<type, type::string> {};

template <>
struct lua_type_of<const char*> : std::integral_constant<type, type::string> {};

template <>
struct lua_type_of<bool> : std::integral_constant<type, type::boolean> {};

template <>
struct lua_type_of<nil_t> : std::integral_constant<type, type::nil> { };

template <>
struct lua_type_of<nullopt_t> : std::integral_constant<type, type::nil> { };

template <>
struct lua_type_of<sol::error> : std::integral_constant<type, type::string> { };

template <bool b, typename Base>
struct lua_type_of<basic_table_core<b, Base>> : std::integral_constant<type, type::table> { };

template <>
struct lua_type_of<reference> : std::integral_constant<type, type::poly> {};

template <>
struct lua_type_of<stack_reference> : std::integral_constant<type, type::poly> {};

template <typename Base>
struct lua_type_of<basic_object<Base>> : std::integral_constant<type, type::poly> {};

template <typename... Args>
struct lua_type_of<std::tuple<Args...>> : std::integral_constant<type, type::poly> {};

template <typename A, typename B>
struct lua_type_of<std::pair<A, B>> : std::integral_constant<type, type::poly> {};

template <>
struct lua_type_of<light_userdata_value> : std::integral_constant<type, type::lightuserdata> {};

template <>
struct lua_type_of<userdata_value> : std::integral_constant<type, type::userdata> {};

template <typename Base>
struct lua_type_of<basic_lightuserdata<Base>> : std::integral_constant<type, type::lightuserdata> {};

template <typename Base>
struct lua_type_of<basic_userdata<Base>> : std::integral_constant<type, type::userdata> {};

template <>
struct lua_type_of<lua_CFunction> : std::integral_constant<type, type::function> {};

template <>
struct lua_type_of<std::remove_pointer_t<lua_CFunction>> : std::integral_constant<type, type::function> {};

template <typename Base>
struct lua_type_of<basic_function<Base>> : std::integral_constant<type, type::function> {};

template <typename Base>
struct lua_type_of<basic_protected_function<Base>> : std::integral_constant<type, type::function> {};

template <>
struct lua_type_of<coroutine> : std::integral_constant<type, type::function> {};

template <>
struct lua_type_of<thread> : std::integral_constant<type, type::thread> {};

template <typename Signature>
struct lua_type_of<std::function<Signature>> : std::integral_constant<type, type::function>{};

template <typename T>
struct lua_type_of<optional<T>> : std::integral_constant<type, type::poly> {};

template <>
struct lua_type_of<variadic_args> : std::integral_constant<type, type::poly> {};

template <typename T>
struct lua_type_of<T*> : std::integral_constant<type, type::userdata> {};

template <typename T>
struct lua_type_of<T, std::enable_if_t<std::is_arithmetic<T>::value>> : std::integral_constant<type, type::number> {};

template <typename T>
struct lua_type_of<T, std::enable_if_t<std::is_enum<T>::value>> : std::integral_constant<type, type::number> {};

template <>
struct lua_type_of<this_state> : std::integral_constant<type, type::none> {};

template <typename T>
struct is_lua_primitive : std::integral_constant<bool, 
    type::userdata != lua_type_of<meta::Unqualified<T>>::value
    || std::is_base_of<reference, meta::Unqualified<T>>::value
    || std::is_base_of<stack_reference, meta::Unqualified<T>>::value
    || meta::is_specialization_of<std::tuple, meta::Unqualified<T>>::value
    || meta::is_specialization_of<std::pair, meta::Unqualified<T>>::value
> { };

template <typename T>
struct is_lua_primitive<std::reference_wrapper<T>> : std::true_type { };
template <typename T>
struct is_lua_primitive<optional<T>> : std::true_type {};
template <typename T>
struct is_lua_primitive<T*> : std::true_type {};
template <>
struct is_lua_primitive<userdata_value> : std::true_type {};
template <>
struct is_lua_primitive<light_userdata_value> : std::true_type {};
template <typename T>
struct is_lua_primitive<non_null<T>> : is_lua_primitive<T*> {};

template <typename T>
struct is_proxy_primitive : is_lua_primitive<T> { };

template <typename T>
struct is_unique_usertype : std::integral_constant<bool, unique_usertype_traits<T>::value> {};

template <typename T>
struct is_transparent_argument : std::false_type {};

template <>
struct is_transparent_argument<this_state> : std::true_type {};

template <>
struct is_transparent_argument<variadic_args> : std::true_type {};

template<typename T>
inline type type_of() {
    return lua_type_of<meta::Unqualified<T>>::value;
}
} // sol

// end of sol\types.hpp

// beginning of sol\stack_reference.hpp

namespace sol {
class stack_reference {
private:
    lua_State* L = nullptr;
    int index = 0;

protected:
    int registry_index () const noexcept {
        return LUA_NOREF;
    }

public:
    stack_reference() noexcept = default;
    stack_reference(lua_State* L, int i) noexcept : L(L), index(lua_absindex(L, i)) {}
    stack_reference(stack_reference&& o) noexcept = default;
    stack_reference& operator=(stack_reference&&) noexcept = default;
    stack_reference(const stack_reference&) noexcept = default;
    stack_reference& operator=(const stack_reference&) noexcept = default;

    int push() const noexcept {
        lua_pushvalue(L, index);
        return 1;
    }

    void pop(int n = 1) const noexcept {
        lua_pop(lua_state( ), n);
    }

    int stack_index () const noexcept {
        return index;
    }

    type get_type() const noexcept {
        int result = lua_type(L, index);
        return static_cast<type>(result);
    }

    lua_State* lua_state() const noexcept {
        return L;
    }

    bool valid () const noexcept {
        type t = get_type();
        return t != type::nil && t != type::none;
    }
};

inline bool operator== (const stack_reference& l, const stack_reference& r) {
    return lua_compare(l.lua_state(), l.stack_index(), r.stack_index(), LUA_OPEQ) == 0;
}

inline bool operator!= (const stack_reference& l, const stack_reference& r) {
    return !operator==(l, r);
}
} // sol

// end of sol\stack_reference.hpp

namespace sol {
namespace stack {
template <bool top_level, typename T>
struct push_popper {
    T t;
    push_popper (T x) : t(x) { t.push(); }
    ~push_popper() { t.pop(); }
};
template <typename T>
struct push_popper<true, T> {
    push_popper (T) {}
    ~push_popper() {}
};
template <bool top_level = false, typename T>
push_popper<top_level, T> push_pop(T&& x) {
    return push_popper<top_level, T>(std::forward<T>(x));
}
} // stack

namespace detail {
struct global_tag { } const global_{};
} // detail

class reference {
private:
    lua_State* L = nullptr; // non-owning
    int ref = LUA_NOREF;

    int copy() const noexcept {
        if (ref == LUA_NOREF)
            return LUA_NOREF;
        push();
        return luaL_ref(L, LUA_REGISTRYINDEX);
    }

protected:
    reference(lua_State* L, detail::global_tag) noexcept : L(L) {
        lua_pushglobaltable(L);
        ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    int stack_index() const noexcept {
        return -1;
    }

public:
    reference() noexcept = default;
    reference(const stack_reference& r) noexcept : reference(r.lua_state(), r.stack_index()) {}
    reference(stack_reference&& r) noexcept : reference(r.lua_state(), r.stack_index()) {}
    reference(lua_State* L, int index = -1) noexcept : L(L) {
        lua_pushvalue(L, index);
        ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    virtual ~reference() noexcept {
        luaL_unref(L, LUA_REGISTRYINDEX, ref);
    }

    reference(reference&& o) noexcept {
        L = o.L;
        ref = o.ref;

        o.L = nullptr;
        o.ref = LUA_NOREF;
    }

    reference& operator=(reference&& o) noexcept {
        L = o.L;
        ref = o.ref;

        o.L = nullptr;
        o.ref = LUA_NOREF;

        return *this;
    }

    reference(const reference& o) noexcept {
        L = o.L;
        ref = o.copy();
    }

    reference& operator=(const reference& o) noexcept {
        L = o.L;
        ref = o.copy();
        return *this;
    }

    int push() const noexcept {
        lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
        return 1;
    }

    void pop(int n = 1) const noexcept {
        lua_pop(lua_state( ), n);
    }

    int registry_index() const noexcept {
        return ref;
    }

    bool valid () const noexcept {
        return !(ref == LUA_NOREF || ref == LUA_REFNIL);
    }

    explicit operator bool () const noexcept {
        return valid();
    }

    type get_type() const noexcept {
        push();
        int result = lua_type(L, -1);
        lua_pop(L, 1);
        return static_cast<type>(result);
    }

    lua_State* lua_state() const noexcept {
        return L;
    }
};

inline bool operator== (const reference& l, const reference& r) {
    auto ppl = stack::push_pop(l);
    auto ppr = stack::push_pop(r);
    return lua_compare(l.lua_state(), -1, -2, LUA_OPEQ) == 1;
}

inline bool operator!= (const reference& l, const reference& r) {
    return !operator==(l, r);
}
} // sol

// end of sol\reference.hpp

// beginning of sol\userdata.hpp

namespace sol {
template <typename base_t>
class basic_userdata : public base_t {
public:
    basic_userdata () noexcept = default;
    basic_userdata(const basic_userdata&) = default;
    basic_userdata(basic_userdata&&) = default;
    basic_userdata& operator=(const basic_userdata&) = default;
    basic_userdata& operator=(basic_userdata&&) = default;
    basic_userdata(const stack_reference& r) : basic_userdata(r.lua_state(), r.stack_index()) {}
    basic_userdata(stack_reference&& r) : basic_userdata(r.lua_state(), r.stack_index()) {}
    basic_userdata(lua_State* L, int index = -1) : base_t(L, index) {
#ifdef SOL_CHECK_ARGUMENTS
        type_assert(L, index, type::userdata);
#endif // Safety
    }
};

template <typename base_t>
class basic_lightuserdata : public base_t {
public:
    basic_lightuserdata () noexcept = default;
    basic_lightuserdata(const basic_lightuserdata&) = default;
    basic_lightuserdata(basic_lightuserdata&&) = default;
    basic_lightuserdata& operator=(const basic_lightuserdata&) = default;
    basic_lightuserdata& operator=(basic_lightuserdata&&) = default;
    basic_lightuserdata(const stack_reference& r) : basic_lightuserdata(r.lua_state(), r.stack_index()) {}
    basic_lightuserdata(stack_reference&& r) : basic_lightuserdata(r.lua_state(), r.stack_index()) {}
    basic_lightuserdata(lua_State* L, int index = -1) : base_t(L, index) {
#ifdef SOL_CHECK_ARGUMENTS
        type_assert(L, index, type::lightuserdata);
#endif // Safety
    }
};

} // sol

// end of sol\userdata.hpp

// beginning of sol\stack.hpp

// beginning of sol\stack_core.hpp

// beginning of sol\tie.hpp

namespace sol {

template <typename T>
struct tie_size : std::tuple_size<T> {};

template <typename T>
struct is_tieable : std::integral_constant<bool, (::sol::tie_size<T>::value > 0)> {};

template <typename... Tn>
struct tie_t : public std::tuple<std::add_lvalue_reference_t<Tn>...> {
private:
     typedef std::tuple<std::add_lvalue_reference_t<Tn>...> base_t;

     template <typename T>
     void set( std::false_type, T&& target ) {
          std::get<0>( *this ) = std::forward<T>( target );
     }

     template <typename T>
     void set( std::true_type, T&& target ) {
          typedef tie_size<meta::Unqualified<T>> value_size;
          typedef tie_size<std::tuple<Tn...>> tie_size;
          typedef std::conditional_t<(value_size::value < tie_size::value), value_size, tie_size> indices_size;
          typedef std::make_index_sequence<indices_size::value> indices;
          set( indices(), std::forward<T>( target ) );
     }

     template <std::size_t... I, typename T>
     void set( std::index_sequence<I...>, T&& target ) {
          using std::get;
          (void)detail::swallow{ 0, 
               ( get<I>(*this) = get<I>(target), 0 )...
          , 0 };
     }

public:
     using base_t::base_t;

     template <typename T>
     tie_t& operator= ( T&& value ) {
          typedef is_tieable<meta::Unqualified<T>> bondable;
          set( bondable(), std::forward<T>( value ) );
          return *this;
     }

};

template <typename... Tn>
struct tie_size<::sol::tie_t<Tn...>> : ::std::tuple_size<::std::tuple<Tn...>> { };

namespace adl_barrier_detail {
template <typename... Tn>
inline tie_t<std::remove_reference_t<Tn>...> tie(Tn&&... argn) {
    return tie_t<std::remove_reference_t<Tn>...>(std::forward<Tn>(argn)...);
}
}

using namespace adl_barrier_detail;

} // sol

// end of sol\tie.hpp

namespace sol {
namespace detail {
struct as_reference_tag{};

using special_destruct_func = void(*)(void*);

template <typename T, typename Real>
inline void special_destruct(void* memory) {
    T** pointerpointer = static_cast<T**>(memory);
    special_destruct_func* dx = static_cast<special_destruct_func*>(static_cast<void*>(pointerpointer + 1));
    Real* target = static_cast<Real*>(static_cast<void*>(dx + 1));
    target->~Real();
}

template <typename T>
inline int unique_destruct(lua_State* L) {
    void* memory = lua_touserdata(L, 1);
    T** pointerpointer = static_cast<T**>(memory);
    special_destruct_func& dx = *static_cast<special_destruct_func*>( static_cast<void*>( pointerpointer + 1 ) );
    (dx)(memory);
    return 0;
}
} // detail
namespace stack {

template<typename T, bool global = false, typename = void>
struct field_getter;
template <typename T, bool global = false, typename = void>
struct probe_field_getter;
template<typename T, bool global = false, typename = void>
struct field_setter;
template<typename T, typename = void>
struct getter;
template<typename T, typename = void>
struct popper;
template<typename T, typename = void>
struct pusher;
template<typename T, type = lua_type_of<T>::value, typename = void>
struct checker;
template<typename T, typename = void>
struct check_getter;

struct probe {
    bool success;
    int levels;

    probe(bool s, int l) : success(s), levels(l) {}

    operator bool() const { return success; };
};

namespace stack_detail {
template <typename T>
struct strip {
    typedef T type;
};
template <typename T>
struct strip<std::reference_wrapper<T>> {
    typedef T& type;
};
template <typename T>
struct strip<non_null<T>> {
    typedef T type;
};
template <typename T>
using strip_t = typename strip<T>::type;
const bool default_check_arguments = 
#ifdef SOL_CHECK_ARGUMENTS
true;
#else
false;
#endif
template<typename T>
inline decltype(auto) unchecked_get(lua_State* L, int index = -1) {
    return getter<meta::Unqualified<T>>{}.get(L, index);
}
} // stack_detail

inline bool maybe_indexable(lua_State* L, int index = -1) {
    type t = type_of(L, index);
    return t == type::userdata || t == type::table;
}

template<typename T, typename... Args>
inline int push(lua_State* L, T&& t, Args&&... args) {
    return pusher<meta::Unqualified<T>>{}.push(L, std::forward<T>(t), std::forward<Args>(args)...);
}

template<typename T, typename Arg, typename... Args>
inline int push(lua_State* L, Arg&& arg, Args&&... args) {
    return pusher<meta::Unqualified<T>>{}.push(L, std::forward<Arg>(arg), std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline int push_reference(lua_State* L, T&& t, Args&&... args) {
    typedef meta::And<
        std::is_lvalue_reference<T>, 
        meta::Not<std::is_const<T>>, 
        meta::Not<is_lua_primitive<T>>
    > use_reference_tag;
    return pusher<std::conditional_t<use_reference_tag::value, detail::as_reference_tag, meta::Unqualified<T>>>{}.push(L, std::forward<T>(t), std::forward<Args>(args)...);
}

inline int multi_push(lua_State*) {
    // do nothing
    return 0;
}

template<typename T, typename... Args>
inline int multi_push(lua_State* L, T&& t, Args&&... args) {
    int pushcount = push(L, std::forward<T>(t));
    void(sol::detail::swallow{(pushcount += sol::stack::push(L, std::forward<Args>(args)), 0)... });
    return pushcount;
}

inline int multi_push_reference(lua_State*) {
    // do nothing
    return 0;
}

template<typename T, typename... Args>
inline int multi_push_reference(lua_State* L, T&& t, Args&&... args) {
    int pushcount = push_reference(L, std::forward<T>(t));
    void(sol::detail::swallow{(pushcount += sol::stack::push_reference(L, std::forward<Args>(args)), 0)... });
    return pushcount;
}

template <typename T, typename Handler>
bool check(lua_State* L, int index, Handler&& handler) {
    typedef meta::Unqualified<T> Tu;
    checker<Tu> c;
    // VC++ has a bad warning here: shut it up
    (void)c;
    return c.check(L, index, std::forward<Handler>(handler));
}

template <typename T>
bool check(lua_State* L, int index = -1) {
    auto handler = no_panic;
    return check<T>(L, index, handler);
}

template<typename T, typename Handler>
inline decltype(auto) check_get(lua_State* L, int index, Handler&& handler) {
    return check_getter<meta::Unqualified<T>>{}.get(L, index, std::forward<Handler>(handler));
}

template<typename T>
inline decltype(auto) check_get(lua_State* L, int index = -1) {
    auto handler = no_panic;
    return check_get<T>(L, index, handler);
}

namespace stack_detail {

template <typename T>
inline decltype(auto) tagged_get(types<T>, lua_State* L, int index = -1) {
#ifdef SOL_CHECK_ARGUMENTS
    auto op = check_get<T>(L, index, type_panic);
    typedef typename meta::Unqualified<decltype(op)>::value_type U;
    return static_cast<U>(*op);
#else
    return stack_detail::unchecked_get<T>(L, index);
#endif
}

template <typename T>
inline decltype(auto) tagged_get(types<optional<T>>, lua_State* L, int index = -1) {
    return stack_detail::unchecked_get<optional<T>>(L, index);
}

} // stack_detail

template<typename T>
inline decltype(auto) get(lua_State* L, int index = -1) {
    return stack_detail::tagged_get(types<T>(), L, index);
}

template<typename T>
inline decltype(auto) pop(lua_State* L) {
    return popper<meta::Unqualified<T>>{}.pop(L);
}

template <bool global = false, typename Key>
void get_field(lua_State* L, Key&& key) {
    field_getter<meta::Unqualified<Key>, global>{}.get(L, std::forward<Key>(key));
}

template <bool global = false, typename Key>
void get_field(lua_State* L, Key&& key, int tableindex) {
    field_getter<meta::Unqualified<Key>, global>{}.get(L, std::forward<Key>(key), tableindex);
}

template <bool global = false, typename Key>
probe probe_get_field(lua_State* L, Key&& key) {
    return probe_field_getter<meta::Unqualified<Key>, global>{}.get(L, std::forward<Key>(key));
}

template <bool global = false, typename Key>
probe probe_get_field(lua_State* L, Key&& key, int tableindex) {
    return probe_field_getter<meta::Unqualified<Key>, global>{}.get(L, std::forward<Key>(key), tableindex);
}

template <bool global = false, typename Key, typename Value>
void set_field(lua_State* L, Key&& key, Value&& value) {
    field_setter<meta::Unqualified<Key>, global>{}.set(L, std::forward<Key>(key), std::forward<Value>(value));
}

template <bool global = false, typename Key, typename Value>
void set_field(lua_State* L, Key&& key, Value&& value, int tableindex) {
    field_setter<meta::Unqualified<Key>, global>{}.set(L, std::forward<Key>(key), std::forward<Value>(value), tableindex);
}
} // stack
} // sol

// end of sol\stack_core.hpp

// beginning of sol\stack_check.hpp

// beginning of sol\usertype_traits.hpp

// beginning of sol\demangle.hpp

#include <cstdlib>
#include <cctype>

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#endif

namespace sol {
namespace detail {
#ifdef _MSC_VER
#ifndef SOL_NO_RTTI
inline std::string get_type_name(const std::type_info& id) {
    std::string realname = id.name();
    const static std::array<std::string, 2> removals = { { "struct ", "class " } };
    for (std::size_t r = 0; r < removals.size(); ++r) {
        auto found = realname.find(removals[r]);
        while (found != std::string::npos) {
            realname.erase(found, removals[r].size());
            found = realname.find(removals[r]);
        }
    }
    return realname;
}
#endif // No RTII
template <typename T>
inline std::string ctti_get_type_name() {
    std::string name = __FUNCSIG__;
    std::size_t start = name.find("get_type_name");
    if (start == std::string::npos)
        start = 0;
    else
        start += 13;
    if (start < name.size() - 1)
        start += 1;
    std::size_t end = name.find_last_of('>');
    if (end == std::string::npos)
        end = name.size();
    name = name.substr(start, end - start);
    if (name.find("struct", 0) == 0)
        name.replace(0, 6, "", 0);
    if (name.find("class", 0) == 0)
        name.replace(0, 5, "", 0);
    while (!name.empty() && std::isblank(name.front())) name.erase(name.begin(), ++name.begin());
    while (!name.empty() && std::isblank(name.back())) name.erase(--name.end(), name.end());
    return name;
}
#elif defined(__GNUC__) || defined(__clang__)
template <typename T>
inline std::string ctti_get_type_name() {
    std::string name = __PRETTY_FUNCTION__;
    std::size_t start = name.find_last_of('[');
    start = name.find_first_of('=', start);
    std::size_t end = name.find_last_of(']');
    if (end == std::string::npos)
        end = name.size();
    if (start == std::string::npos)
        start = 0;
    if (start < name.size() - 1)
        start += 1;
    name = name.substr(start, end - start);
    start = name.find(";");
    if (start != std::string::npos) {
        name.erase(start, name.length());
    }
    while (!name.empty() && std::isblank(name.front())) name.erase(name.begin(), ++name.begin());
    while (!name.empty() && std::isblank(name.back())) name.erase(--name.end(), name.end());
    return name;
}
#ifndef SOL_NO_RTTI
#if defined(__clang__)
inline std::string get_type_name(const std::type_info& id) {
    int status;
    char* unmangled = abi::__cxa_demangle(id.name(), 0, 0, &status);
    std::string realname = unmangled;
    std::free(unmangled);
    return realname;
}
#elif defined(__GNUC__)
inline std::string get_type_name(const std::type_info& id) {
    int status;
    char* unmangled = abi::__cxa_demangle(id.name(), 0, 0, &status);
    std::string realname = unmangled;
    std::free(unmangled);
    return realname;
}
#endif // g++ || clang++
#endif // No RTII
#else
#error Compiler not supported for demangling
#endif // compilers

template <typename T>
inline std::string demangle_once() {
#ifndef SOL_NO_RTTI
    std::string realname = get_type_name(typeid(T));
#else
    std::string realname = ctti_get_type_name<T>();
#endif // No Runtime Type Information
    return realname;
}

template <typename T>
inline std::string short_demangle_once() {
    std::string realname = ctti_get_type_name<T>();
    std::size_t idx = realname.find_last_of(":`'\"{}[]|-)(*^&!@#$%`~", std::string::npos, 23);
    if (idx != std::string::npos) {
        realname.erase(0, realname.length() < idx ? realname.length() : idx + 1);
    }
    return realname;
}

template <typename T>
inline std::string demangle() {
    static const std::string d = demangle_once<T>();
    return d;
}

template <typename T>
inline std::string short_demangle() {
    static const std::string d = short_demangle_once<T>();
    return d;
}
} // detail
} // sol

// end of sol\demangle.hpp

namespace sol {

template<typename T>
struct usertype_traits {
    static const std::string name;
    static const std::string qualified_name;
    static const std::string metatable;
    static const std::string variable_metatable;
    static const std::string gc_table;
};

template<typename T>
const std::string usertype_traits<T>::name = detail::short_demangle<T>();

template<typename T>
const std::string usertype_traits<T>::qualified_name = detail::demangle<T>();

template<typename T>
const std::string usertype_traits<T>::metatable = std::string("sol.").append(detail::demangle<T>());

template<typename T>
const std::string usertype_traits<T>::variable_metatable = std::string("sol.").append(detail::demangle<T>()).append(".variables");

template<typename T>
const std::string usertype_traits<T>::gc_table = std::string("sol.").append(detail::demangle<T>().append(".\xE2\x99\xBB"));

}

// end of sol\usertype_traits.hpp

// beginning of sol\inheritance.hpp

#if defined(SOL_NO_RTTI) && defined(SOL_NO_EXCEPTIONS)
#include <atomic>
#endif // No Runtime Type Information and No Exceptions

namespace sol {
template <typename... Args>
struct base_list { };
template <typename... Args>
using bases = base_list<Args...>;

typedef bases<> base_classes_tag;
const auto base_classes = base_classes_tag();

namespace detail {
#if defined(SOL_NO_RTTI) && defined(SOL_NO_EXCEPTIONS)
inline std::size_t unique_id () {
    static std::atomic<std::size_t> x(0);
    return ++x;
}

template <typename T>
struct id_for {
    static const std::size_t value;
};

template <typename T>
const std::size_t id_for<T>::value = unique_id();
#endif // No Runtime Type Information / No Exceptions

inline decltype(auto) base_class_check_key() {
    static const auto& key = u8"♡o｡.(✿ฺ｡ ✿ฺ)";
    return key;
}

inline decltype(auto) base_class_cast_key() {
    static const auto& key = u8"(◕‿◕✿)";
    return key;
}

#ifndef SOL_NO_EXCEPTIONS

template <typename T>
void throw_as(void* p) {
    throw static_cast<T*>(p);
}

using throw_cast = decltype(&throw_as<void>);

template <typename T>
inline T* catch_cast(void* p, throw_cast f) {
    try {
        f(static_cast<void*>(p));
    }
    catch (T* ptr) {
        return ptr;
    }
    catch (...) {
        return static_cast<T*>(p);
    }
    return static_cast<T*>(p);
}

template <typename T>
inline bool catch_check(throw_cast f) {
    try {
        f( nullptr );
    }
    catch (T*) {
        return true;
    }
    catch (...) {
        return false;
    }
    return false;
}

#elif !defined(SOL_NO_RTTI)
template <typename T, typename... Bases>
struct inheritance {
    static bool type_check(types<>, const std::type_info&) {
        return false;
    }

    template <typename Base, typename... Args>
    static bool type_check(types<Base, Args...>, const std::type_info& ti) {
        return ti != typeid(Base) || type_check(types<Bases...>(), ti);
    }

    static bool check(const std::type_info& ti) {
        return ti != typeid(T) || type_check(types<Bases...>(), ti);
    }

    static void* type_cast(types<>, T*, const std::type_info& ti) {
        return nullptr;
    }

    template <typename Base, typename... Args>
    static void* type_cast(types<Base, Args...>, T* data, const std::type_info& ti) {
        // Make sure to convert to T first, and then dynamic cast to the proper type
        return ti != typeid(Base) ? type_cast(types<Bases...>(), data, ti) : static_cast<void*>(dynamic_cast<Base*>(static_cast<T*>(data)));
    }

    static void* cast(void* voiddata, const std::type_info& ti) {
        T* data = static_cast<T*>(voiddata);
        return static_cast<void*>(ti != typeid(T) ? type_cast(types<Bases...>(), data, ti) : data);
    }
};

using inheritance_check_function = decltype(&inheritance<void>::check);
using inheritance_cast_function = decltype(&inheritance<void>::cast);
#else
template <typename T, typename... Bases>
struct inheritance {
    static bool type_check(types<>, std::size_t) {
        return false;
    }

    template <typename Base, typename... Args>
    static bool type_check(types<Base, Args...>, std::size_t ti) {
        return ti != id_for<Base>::value || type_check(types<Bases...>(), ti);
    }

    static bool check(std::size_t ti) {
        return ti != id_for<T>::value || type_check(types<Bases...>(), ti);
    }

    static void* type_cast(types<>, T*, std::size_t) {
        return nullptr;
    }

    template <typename Base, typename... Args>
    static void* type_cast(types<Base, Args...>, T* data, std::size_t ti) {
        // Make sure to convert to T first, and then dynamic cast to the proper type
        return ti != id_for<Base>::value ? type_cast(types<Bases...>(), data, ti) : static_cast<void*>(static_cast<Base*>(data));
    }

    static void* cast(void* voiddata, std::size_t ti) {
        T* data = static_cast<T*>(voiddata);
        return static_cast<void*>(ti != id_for<T>::value ? type_cast(types<Bases...>(), data, ti) : data);
    }
};

using inheritance_check_function = decltype(&inheritance<void>::check);
using inheritance_cast_function = decltype(&inheritance<void>::cast);
#endif // No Exceptions and/or No Runtime Type Information

} // detail
} // sol

// end of sol\inheritance.hpp

#include <utility>

namespace sol {
namespace stack {
namespace stack_detail {
template <typename T>
inline bool check_metatable(lua_State* L, int index = -2) {
    luaL_getmetatable(L, &usertype_traits<T>::metatable[0]);
    const type expectedmetatabletype = static_cast<type>(lua_type(L, -1));
    if (expectedmetatabletype != type::nil) {
        if (lua_rawequal(L, -1, index) == 1) {
            lua_pop(L, 2);
            return true;
        }
    }
    lua_pop(L, 1);
    return false;
}

template <type expected, int(*check_func)(lua_State*, int)>
struct basic_check {
    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        bool success = check_func(L, index) == 1;
        if (!success) {
            // expected type, actual type
            handler(L, index, expected, type_of(L, index));
        }
        return success;
    }
};

template <bool b>
struct check_types {
    template <std::size_t I0, std::size_t... I, typename T, typename... Args, typename Handler>
    static bool check(types<T, Args...>, std::index_sequence<I0, I...>, lua_State* L, int firstargument, Handler&& handler) {
        if (!stack::check<T>(L, firstargument + I0, handler))
            return false;
        return check(types<Args...>(), std::index_sequence<I...>(), L, firstargument - static_cast<int>(is_transparent_argument<meta::Unqualified<T>>::value), std::forward<Handler>(handler));
    }

    template <typename Handler>
    static bool check(types<>, std::index_sequence<>, lua_State*, int, Handler&&) {
        return true;
    }
};

template <>
struct check_types<false> {
    template <std::size_t... I, typename... Args, typename Handler>
    static bool check(types<Args...>, std::index_sequence<I...>, lua_State*, int, Handler&&) {
        return true;
    }
};
} // stack_detail

template <typename T, type expected, typename>
struct checker {
    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        const type indextype = type_of(L, index);
        bool success = expected == indextype;
        if (!success) {
            // expected type, actual type
            handler(L, index, expected, indextype);
        }
        return success;
    }
};

template <type expected, typename C>
struct checker<type, expected, C> {
    template <typename Handler>
    static bool check (lua_State*, int, Handler&&) {
        return true;
    }
};

template <type expected, typename C>
struct checker<nil_t, expected, C> {
    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        bool success = lua_isnoneornil(L, index);
        if (!success) {
            // expected type, actual type
            handler(L, index, expected, type_of(L, index));
        }
        return success;
    }
};

template <type expected, typename C>
struct checker<nullopt_t, expected, C> : checker<nil_t> {};

template <typename C>
struct checker<this_state, type::none, C> {
    template <typename Handler>
    static bool check (lua_State*, int, Handler&&) {
        return true;
    }
};

template <typename C>
struct checker<variadic_args, type::poly, C> {
    template <typename Handler>
    static bool check (lua_State*, int, Handler&&) {
        return true;
    }
};

template <typename T, typename C>
struct checker<T, type::poly, C> {
    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        bool success = !lua_isnone(L, index);
        if (!success) {
            // expected type, actual type
            handler(L, index, type::none, type_of(L, index));
        }
        return success;
    }
};

template <typename T, typename C>
struct checker<T, type::lightuserdata, C> {
    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        type t = type_of(L, index);
        bool success = t == type::userdata || t == type::lightuserdata;
        if (!success) {
            // expected type, actual type
            handler(L, index, type::lightuserdata, t);
        }
        return success;
    }
};

template <typename T, typename C>
struct checker<non_null<T>, type::userdata, C> : checker<T, lua_type_of<T>::value, C> {};

template <typename C>
struct checker<lua_CFunction, type::function, C> : stack_detail::basic_check<type::function, lua_iscfunction> {};
template <typename C>
struct checker<std::remove_pointer_t<lua_CFunction>, type::function, C> : checker<lua_CFunction, type::function, C> {};
template <typename C>
struct checker<c_closure, type::function, C> : checker<lua_CFunction, type::function, C> {};

template <typename T, typename C>
struct checker<T, type::function, C> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        type t = type_of(L, index);
        if (t == type::function) {
            return true;
        }
        if (t != type::userdata && t != type::table) {
            handler(L, index, t, type::function);
            return false;
        }
        // Do advanced check for call-style userdata?
        static const auto& callkey = name_of(meta_function::call);
        lua_getmetatable(L, index);
        if (lua_isnoneornil(L, -1)) {
            handler(L, index, t, type::function);
            lua_pop(L, 1);
            return false;
        }
        lua_getfield(L, -1, &callkey[0]);
        if (lua_isnoneornil(L, -1)) {
            handler(L, index, t, type::function);
            lua_pop(L, 2);
            return false;
        }
        // has call, is definitely a function
        lua_pop(L, 2);
        return true;
    }
};

template <typename T, typename C>
struct checker<T, type::table, C> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        type t = type_of(L, index);
        if (t == type::table) {
            return true;
        }
        if (t != type::userdata) {
            handler(L, index, t, type::function);
            return false;
        }
        return true;
    }
};

template <typename T, typename C>
struct checker<T*, type::userdata, C> {
    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        const type indextype = type_of(L, index);
        // Allow nil to be transformed to nullptr
        if (indextype == type::nil) {
            return true;
        }
        return checker<T, type::userdata, C>{}.check(types<T>(), L, indextype, index, std::forward<Handler>(handler));
    }
};

template <typename C>
struct checker<userdata_value, type::userdata, C> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        type t = type_of(L, index);
        bool success = t == type::userdata || t == type::lightuserdata;
        if (!success) {
            // expected type, actual type
            handler(L, index, type::lightuserdata, t);
        }
        return success;
    }
};

template <typename T, typename C>
struct checker<T, type::userdata, C> {
    template <typename U, typename Handler>
    static bool check (types<U>, lua_State* L, type indextype, int index, Handler&& handler) {
        if (indextype != type::userdata) {
            handler(L, index, type::userdata, indextype);
            return false;
        }
        if (meta::Or<std::is_same<T, light_userdata_value>, std::is_same<T, userdata_value>>::value)
            return true;
        if (lua_getmetatable(L, index) == 0) {
             return true;
        }
        if (stack_detail::check_metatable<U>(L))
            return true;
        if (stack_detail::check_metatable<U*>(L))
            return true;
        if (stack_detail::check_metatable<detail::unique_usertype<U>>(L))
            return true;
#ifndef SOL_NO_EXCEPTIONS
        lua_getfield(L, -1, &detail::base_class_check_key()[0]);
        void* basecastdata = lua_touserdata(L, -1);
        detail::throw_cast basecast = (detail::throw_cast)basecastdata;
        bool success = detail::catch_check<T>(basecast);
#elif !defined(SOL_NO_RTTI)
        lua_getfield(L, -1, &detail::base_class_check_key()[0]);
        if (stack::get<type>(L) == type::nil) {
            lua_pop(L, 2);
            return false;
        }
        void* basecastdata = lua_touserdata(L, -1);
        detail::inheritance_check_function ic = (detail::inheritance_check_function)basecastdata;
        bool success = ic(typeid(T));
#else
        // Topkek
        lua_getfield(L, -1, &detail::base_class_check_key()[0]);
        if (stack::get<type>(L) == type::nil) {
            lua_pop(L, 2);
            return false;
        }
        void* basecastdata = lua_touserdata(L, -1);
        detail::inheritance_check_function ic = (detail::inheritance_check_function)basecastdata;
        bool success = ic(detail::id_for<T>::value);
#endif // No Runtime Type Information || Exceptions
        lua_pop(L, 2);
        if (!success) {
            handler(L, index, type::userdata, indextype);
            return false;
        }
        return true;
    }

    template <typename Handler>
    static bool check (lua_State* L, int index, Handler&& handler) {
        const type indextype = type_of(L, index);
        return check(types<T>(), L, indextype, index, std::forward<Handler>(handler));
    }
};

template<typename T>
struct checker<T, type::userdata, std::enable_if_t<is_unique_usertype<T>::value>> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        return checker<typename unique_usertype_traits<T>::type, type::userdata>{}.check(L, index, std::forward<Handler>(handler));
    }
};

template<typename T, typename C>
struct checker<std::reference_wrapper<T>, type::userdata, C> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        return checker<T, type::userdata, C>{}.check(L, index, std::forward<Handler>(handler));
    }
};

template<typename... Args, typename C>
struct checker<std::tuple<Args...>, type::poly, C> {
    template <std::size_t... I, typename Handler>
    static bool apply(std::index_sequence<I...> is, lua_State* L, int index, Handler&& handler) {
        index = index < 0 ? lua_absindex(L, index) - ( sizeof...(I) - 1 ) : index;
        return stack_detail::check_types<true>{}.check(types<Args...>(), is, L, index, handler);
    }

    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        return apply(std::make_index_sequence<sizeof...(Args)>(), L, index, std::forward<Handler>(handler));
    }
};

template<typename A, typename B, typename C>
struct checker<std::pair<A, B>, type::poly, C> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        index = index < 0 ? lua_absindex(L, index) - 1 : index;
        return stack::check<A>(L, index, handler) && stack::check<B>(L, index + 1, handler);
    }
};

template<typename T, typename C>
struct checker<optional<T>, type::poly, C> {
    template <typename Handler>
    static bool check(lua_State* L, int index, Handler&& handler) {
        return stack::check<nil_t>(L, index, no_panic) || stack::check<T>(L, index, std::forward<Handler>(handler));
    }
};
} // stack
} // sol

// end of sol\stack_check.hpp

// beginning of sol\stack_get.hpp

// beginning of sol\overload.hpp

namespace sol {
    template <typename... Functions>
    struct overload_set {
        std::tuple<Functions...> set;
        template <typename Arg, typename... Args, meta::DisableIf<std::is_same<overload_set, meta::Unqualified<Arg>>> = 0>
        overload_set (Arg&& arg, Args&&... args) : set(std::forward<Arg>(arg), std::forward<Args>(args)...) {}
        overload_set(const overload_set&) = default;
        overload_set(overload_set&&) = default;
        overload_set& operator=(const overload_set&) = default;
        overload_set& operator=(overload_set&&) = default;
    };

    template <typename... Args>
    decltype(auto) overload(Args&&... args) {
        return overload_set<Args...>(std::forward<Args>(args)...);
    }
}

// end of sol\overload.hpp

namespace sol {
namespace stack {

template<typename T, typename>
struct getter {
    static T& get(lua_State* L, int index = -1) {
        return getter<T&>{}.get(L, index);
    }
};

template<typename T>
struct getter<T, std::enable_if_t<std::is_floating_point<T>::value>> {
    static T get(lua_State* L, int index = -1) {
        return static_cast<T>(lua_tonumber(L, index));
    }
};

template<typename T>
struct getter<T, std::enable_if_t<meta::And<std::is_integral<T>, std::is_signed<T>>::value>> {
    static T get(lua_State* L, int index = -1) {
        return static_cast<T>(lua_tointeger(L, index));
    }
};

template<typename T>
struct getter<T, std::enable_if_t<meta::And<std::is_integral<T>, std::is_unsigned<T>>::value>> {
    static T get(lua_State* L, int index = -1) {
        return static_cast<T>(lua_tointeger(L, index));
    }
};

template<typename T>
struct getter<T, std::enable_if_t<std::is_base_of<reference, T>::value || std::is_base_of<stack_reference, T>::value>> {
    static T get(lua_State* L, int index = -1) {
        return T(L, index);
    }
};

template<>
struct getter<userdata_value> {
    static userdata_value get(lua_State* L, int index = -1) {
        return userdata_value( lua_touserdata(L, index) );
    }
};

template<>
struct getter<light_userdata_value> {
    static light_userdata_value get(lua_State* L, int index = -1) {
        return light_userdata_value( lua_touserdata(L, index) );
    }
};

template<>
struct getter<type> {
    static type get(lua_State *L, int index){
        return static_cast<type>(lua_type(L, index));
    }
};

template<>
struct getter<bool> {
    static bool get(lua_State* L, int index) {
        return lua_toboolean(L, index) != 0;
    }
};

template<>
struct getter<std::string> {
    static std::string get(lua_State* L, int index = -1) {
        std::size_t len;
        auto str = lua_tolstring(L, index, &len);
        return { str, len };
    }
};

template<>
struct getter<const char*> {
    static const char* get(lua_State* L, int index = -1) {
        return lua_tostring(L, index);
    }
};

template<>
struct getter<nil_t> {
    static nil_t get(lua_State*, int = -1) {
        return nil;
    }
};

template<>
struct pusher<std::nullptr_t> {
    static std::nullptr_t get(lua_State*, int = -1) {
        return nullptr;
    }
};

template<>
struct getter<nullopt_t> {
    static nullopt_t get(lua_State*, int = -1) {
        return nullopt;
    }
};

template<>
struct getter<this_state> {
    static this_state get(lua_State* L, int = -1) {
        return this_state{L};
    }
};

template<>
struct getter<lua_CFunction> {
    static lua_CFunction get(lua_State* L,  int index = -1) {
        return lua_tocfunction(L, index);
    }
};

template<>
struct getter<c_closure> {
    static c_closure get(lua_State* L, int index = -1) {
        return c_closure(lua_tocfunction(L, index), -1);
    }
};

template<>
struct getter<error> {
    static error get(lua_State* L,  int index = -1) {
        size_t sz = 0;
        const char* err = lua_tolstring(L, index, &sz);
        if (err == nullptr) {
            return error(detail::direct_error, "");
        }
        return error(detail::direct_error, std::string(err, sz));
    }
};

template<>
struct getter<void*> {
    static void* get(lua_State* L, int index = -1) {
        return lua_touserdata(L, index);
    }
};

template<typename T>
struct getter<T*> {
    static T* get_no_nil(lua_State* L, int index = -1) {
        void** pudata = static_cast<void**>(lua_touserdata(L, index));
        void* udata = *pudata;
        return get_no_nil_from(L, udata, index);
    }

    static T* get_no_nil_from(lua_State* L, void* udata, int index = -1) {
#ifndef SOL_NO_EXCEPTIONS
        if (luaL_getmetafield(L, index, &detail::base_class_check_key()[0]) != 0) {
            void* basecastdata = lua_touserdata(L, -1);
            detail::throw_cast basecast = (detail::throw_cast)basecastdata;
            // use the casting function to properly adjust the pointer for the desired T
            udata = detail::catch_cast<T>(udata, basecast);
            lua_pop(L, 1);
        }
#elif !defined(SOL_NO_RTTI)
        if (luaL_getmetafield(L, index, &detail::base_class_cast_key()[0]) != 0) {
            void* basecastdata = lua_touserdata(L, -1);
            detail::inheritance_cast_function ic = (detail::inheritance_cast_function)basecastdata;
            // use the casting function to properly adjust the pointer for the desired T
            udata = ic(udata, typeid(T));
            lua_pop(L, 1);
        }
#else
        // Lol, you motherfucker
        if (luaL_getmetafield(L, index, &detail::base_class_cast_key()[0]) != 0) {
            void* basecastdata = lua_touserdata(L, -1);
            detail::inheritance_cast_function ic = (detail::inheritance_cast_function)basecastdata;
            // use the casting function to properly adjust the pointer for the desired T
            udata = ic(udata, detail::id_for<T>::value);
            lua_pop(L, 1);
        }
#endif // No Runtime Type Information || Exceptions
        T* obj = static_cast<T*>(udata);
        return obj;
    }

    static T* get(lua_State* L, int index = -1) {
        type t = type_of(L, index);
        if (t == type::nil)
            return nullptr;
        return get_no_nil(L, index);
    }
};

template<typename T>
struct getter<non_null<T*>> {
    static T* get(lua_State* L, int index = -1) {
        return getter<T*>::get_no_nil(L, index);
    }
};

template<typename T>
struct getter<T&> {
    static T& get(lua_State* L, int index = -1) {
        return *getter<T*>::get_no_nil(L, index);
    }
};

template<typename T>
struct getter<T, std::enable_if_t<is_unique_usertype<T>::value>> {
    typedef typename unique_usertype_traits<T>::type P;
    typedef typename unique_usertype_traits<T>::actual_type Real;
    
    static Real& get(lua_State* L, int index = -1) {
        P** pref = static_cast<P**>(lua_touserdata(L, index));
        detail::special_destruct_func* fx = static_cast<detail::special_destruct_func*>(static_cast<void*>(pref + 1));
        Real* mem = static_cast<Real*>(static_cast<void*>(fx + 1));
        return *mem;
    }
};

template<typename T>
struct getter<std::reference_wrapper<T>> {
    static T& get(lua_State* L, int index = -1) {
        return getter<T&>{}.get(L, index);
    }
};

template<typename... Args>
struct getter<std::tuple<Args...>> {
    template <std::size_t... I>
    static decltype(auto) apply(std::index_sequence<I...>, lua_State* L, int index = -1) {
        index = index < 0 ? lua_absindex(L, index) - ( sizeof...(I) - 1 ) : index;
        return std::tuple<decltype(stack::get<Args>(L, index + I))...>(stack::get<Args>(L, index + I)...);
    }

    static decltype(auto) get(lua_State* L, int index = -1) {
        return apply(std::make_index_sequence<sizeof...(Args)>(), L, index);
    }
};

template<typename A, typename B>
struct getter<std::pair<A, B>> {
    static decltype(auto) get(lua_State* L, int index = -1) {
        index = index < 0 ? lua_absindex(L, index) - 1 : index;
        return std::pair<decltype(stack::get<A>(L, index)), decltype(stack::get<B>(L, index))>(stack::get<A>(L, index), stack::get<B>(L, index + 1));
    }
};

} // stack
} // sol

// end of sol\stack_get.hpp

// beginning of sol\stack_check_get.hpp

namespace sol {
namespace stack {
template <typename T, typename>
struct check_getter {
    typedef stack_detail::strip_t<T> U;
    typedef std::conditional_t<is_proxy_primitive<T>::value, U, U&> R;

    template <typename Handler>
    static optional<R> get( lua_State* L, int index, Handler&& handler) {
        if (!check<T>(L, index, std::forward<Handler>(handler)))
            return nullopt;
        return stack_detail::unchecked_get<T>(L, index);
    }
};

template <typename T>
struct check_getter<optional<T>> {
    template <typename Handler>
    static decltype(auto) get(lua_State* L, int index, Handler&&) {
        return check_get<T>(L, index, no_panic);
    }
};

template <typename T>
struct check_getter<T, std::enable_if_t<std::is_integral<T>::value && !std::is_same<T, bool>::value>> {
    template <typename Handler>
    static optional<T> get( lua_State* L, int index, Handler&& handler) {
        int isnum = 0;
        lua_Integer value = lua_tointegerx(L, index, &isnum);
        if (isnum == 0) {
            handler(L, index, type::number, type_of(L, index));
            return nullopt;
        }
        return static_cast<T>(value);
    }
};

template <typename T>
struct check_getter<T, std::enable_if_t<std::is_floating_point<T>::value>> {
    template <typename Handler>
    static optional<T> get( lua_State* L, int index, Handler&& handler) {
        int isnum = 0;
        lua_Number value = lua_tonumberx(L, index, &isnum);
        if (isnum == 0) {
            handler(L, index, type::number, type_of(L, index));
            return nullopt;
        }
        return static_cast<T>(value);
    }
};

template <typename T>
struct getter<optional<T>> {
    static decltype(auto) get(lua_State* L, int index) {
        return check_get<T>(L, index);
    }
};
} // stack
} // sol

// end of sol\stack_check_get.hpp

// beginning of sol\stack_push.hpp

// beginning of sol\raii.hpp

namespace sol {
namespace detail {
struct default_construct {
    template<typename T, typename... Args>
    static void construct(T&& obj, Args&&... args) {
        std::allocator<meta::Unqualified<T>> alloc{};
        alloc.construct(obj, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    void operator()(T&& obj, Args&&... args) const {
        construct(std::forward<T>(obj), std::forward<Args>(args)...);
    }
};

struct default_destruct {
    template<typename T>
    static void destroy(T&& obj) {
        std::allocator<meta::Unqualified<T>> alloc{};
        alloc.destroy(obj);
    }

    template<typename T>
    void operator()(T&& obj) const {
        destroy(std::forward<T>(obj));
    }
};
} // detail

template <typename... Args>
struct constructor_list {};

template<typename... Args>
using constructors = constructor_list<Args...>;

const auto default_constructor = constructors<types<>>{};

struct no_construction {};
const auto no_constructor = no_construction{};

struct call_construction {};
const auto call_constructor = call_construction{};

template <typename... Functions>
struct constructor_wrapper {
    std::tuple<Functions...> set;
    template <typename... Args>
    constructor_wrapper(Args&&... args) : set(std::forward<Args>(args)...) {}
};

template <typename... Functions>
constructor_wrapper<Functions...> initializers(Functions&&... functions) {
    return constructor_wrapper<Functions...>(std::forward<Functions>(functions)...);
}

template <typename Function>
struct destructor_wrapper {
    Function fx;
    template <typename... Args>
    destructor_wrapper(Args&&... args) : fx(std::forward<Args>(args)...) {}
};

template <>
struct destructor_wrapper<void> {};

const destructor_wrapper<void> default_destructor{};

template <typename Fx>
inline destructor_wrapper<Fx> destructor(Fx&& fx) {
    return destructor_wrapper<Fx>(std::forward<Fx>(fx));
}

} // sol

// end of sol\raii.hpp

namespace sol {
namespace stack {
template<typename T, typename>
struct pusher {
    template <typename... Args>
    static int push(lua_State* L, Args&&... args) {
        // Basically, we store all user-data like this:
        // If it's a movable/copyable value (no std::ref(x)), then we store the pointer to the new
        // data in the first sizeof(T*) bytes, and then however many bytes it takes to
        // do the actual object. Things that are std::ref or plain T* are stored as 
        // just the sizeof(T*), and nothing else.
        T** pointerpointer = static_cast<T**>(lua_newuserdata(L, sizeof(T*) + sizeof(T)));
        T*& referencereference = *pointerpointer;
        T* allocationtarget = reinterpret_cast<T*>(pointerpointer + 1);
        referencereference = allocationtarget;
        std::allocator<T> alloc{};
        alloc.construct(allocationtarget, std::forward<Args>(args)...);
        luaL_newmetatable(L, &usertype_traits<T>::metatable[0]);
        lua_setmetatable(L, -2);
        return 1;
    }
};

template <>
struct pusher<detail::as_reference_tag> {
    template <typename T>
    static int push(lua_State* L, T&& obj) {
        return stack::push(L, detail::ptr(obj));
    }
};

template<typename T>
struct pusher<T*> {
    static int push(lua_State* L, T* obj) {
        if (obj == nullptr)
            return stack::push(L, nil);
        T** pref = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
        *pref = obj;
        luaL_getmetatable(L, &usertype_traits<T*>::metatable[0]);
        lua_setmetatable(L, -2);
        return 1;
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<is_unique_usertype<T>::value>> {
    typedef typename unique_usertype_traits<T>::type P;
    typedef typename unique_usertype_traits<T>::actual_type Real;

    template <typename Arg, meta::EnableIf<std::is_base_of<Real, meta::Unqualified<Arg>>> = 0>
    static int push(lua_State* L, Arg&& arg) {
        if (unique_usertype_traits<T>::is_null(arg))
            return stack::push(L, nil);
        return push_deep(L, std::forward<Arg>(arg));
    }

    template <typename Arg0, typename Arg1, typename... Args>
    static int push(lua_State* L, Arg0&& arg0, Arg0&& arg1, Args&&... args) {
        return push_deep(L, std::forward<Arg0>(arg0), std::forward<Arg1>(arg1), std::forward<Args>(args)...);
    }

    template <typename... Args>
    static int push_deep(lua_State* L, Args&&... args) {
        P** pref = static_cast<P**>(lua_newuserdata(L, sizeof(P*) + sizeof(detail::special_destruct_func) + sizeof(Real)));
        detail::special_destruct_func* fx = static_cast<detail::special_destruct_func*>(static_cast<void*>(pref + 1));
        Real* mem = static_cast<Real*>(static_cast<void*>(fx + 1));
        *fx = detail::special_destruct<P, Real>;
        detail::default_construct::construct(mem, std::forward<Args>(args)...);
        *pref = unique_usertype_traits<T>::get(*mem);
        if (luaL_newmetatable(L, &usertype_traits<detail::unique_usertype<P>>::metatable[0]) == 1) {
            set_field(L, "__gc", detail::unique_destruct<P>);
        }
        lua_setmetatable(L, -2);
        return 1;
    }
};

template<typename T>
struct pusher<std::reference_wrapper<T>> {
    static int push(lua_State* L, const std::reference_wrapper<T>& t) {
        return stack::push(L, std::addressof(detail::deref(t.get())));
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<std::is_floating_point<T>::value>> {
    static int push(lua_State* L, const T& value) {
        lua_pushnumber(L, value);
        return 1;
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<meta::And<std::is_integral<T>, std::is_signed<T>>::value>> {
    static int push(lua_State* L, const T& value) {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
        return 1;
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<meta::And<std::is_integral<T>, std::is_unsigned<T>>::value>> {
    static int push(lua_State* L, const T& value) {
         typedef std::make_signed_t<T> signed_int;
         return stack::push(L, static_cast<signed_int>(value));
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<meta::And<meta::has_begin_end<T>, meta::Not<meta::has_key_value_pair<T>>, meta::Not<std::is_base_of<reference, T>>>::value>> {
    static int push(lua_State* L, const T& cont) {
        lua_createtable(L, static_cast<int>(cont.size()), 0);
        int tableindex = lua_gettop(L);
        unsigned index = 1;
        for(auto&& i : cont) {
            set_field(L, index++, i, tableindex);
        }
        return 1;
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<meta::And<meta::has_begin_end<T>, meta::has_key_value_pair<T>, meta::Not<std::is_base_of<reference, T>>>::value>> {
    static int push(lua_State* L, const T& cont) {
        lua_createtable(L, static_cast<int>(cont.size()), 0);
        int tableindex = lua_gettop(L);
        for(auto&& pair : cont) {
            set_field(L, pair.first, pair.second, tableindex);
        }
        return 1;
    }
};

template<typename T>
struct pusher<T, std::enable_if_t<std::is_base_of<reference, T>::value || std::is_base_of<stack_reference, T>::value>> {
    static int push(lua_State*, T& ref) {
        return ref.push();
    }

    static int push(lua_State*, T&& ref) {
        return ref.push();
    }
};

template<>
struct pusher<bool> {
    static int push(lua_State* L, bool b) {
        lua_pushboolean(L, b);
        return 1;
    }
};

template<>
struct pusher<nil_t> {
    static int push(lua_State* L, nil_t) {
        lua_pushnil(L);
        return 1;
    }
};

template<>
struct pusher<metatable_key_t> {
    static int push(lua_State* L, metatable_key_t) {
        lua_pushlstring(L, "__mt", 4);
        return 1;
    }
};

template<>
struct pusher<std::remove_pointer_t<lua_CFunction>> {
    static int push(lua_State* L, lua_CFunction func, int n = 0) {
        lua_pushcclosure(L, func, n);
        return 1;
    }
};

template<>
struct pusher<lua_CFunction> {
    static int push(lua_State* L, lua_CFunction func, int n = 0) {
        lua_pushcclosure(L, func, n);
        return 1;
    }
};

template<>
struct pusher<c_closure> {
    static int push(lua_State* L, c_closure closure) {
        lua_pushcclosure(L, closure.c_function, closure.upvalues);
        return 1;
    }
};

template<>
struct pusher<void*> {
    static int push(lua_State* L, void* userdata) {
        lua_pushlightuserdata(L, userdata);
        return 1;
    }
};

template<>
struct pusher<light_userdata_value> {
    static int push(lua_State* L, light_userdata_value userdata) {
        lua_pushlightuserdata(L, userdata);
        return 1;
    }
};

template<>
struct pusher<userdata_value> {
    static int push(lua_State* L, userdata_value data) {
        void** ud = static_cast<void**>(lua_newuserdata(L, sizeof(void*)));
        *ud = data.value;
        return 1;
    }
};

template<>
struct pusher<const char*> {
    static int push(lua_State* L, const char* str) {
        lua_pushlstring(L, str, std::char_traits<char>::length(str));
        return 1;
    }
};

template<size_t N>
struct pusher<char[N]> {
    static int push(lua_State* L, const char (&str)[N]) {
        lua_pushlstring(L, str, N - 1);
        return 1;
    }
};

template<>
struct pusher<std::string> {
    static int push(lua_State* L, const std::string& str) {
        lua_pushlstring(L, str.c_str(), str.size());
        return 1;
    }
};

template<typename... Args>
struct pusher<std::tuple<Args...>> {
    template <std::size_t... I, typename T>
    static int push(std::index_sequence<I...>, lua_State* L, T&& t) {
        int pushcount = 0;
        (void)detail::swallow{ 0, (pushcount += stack::push(L, 
              detail::forward_get<I>(t)
        ), 0)... };
        return pushcount;
    }

    template <typename T>
    static int push(lua_State* L, T&& t) {
        return push(std::index_sequence_for<Args...>(), L, std::forward<T>(t));
    }
};

template<typename A, typename B>
struct pusher<std::pair<A, B>> {
    template <typename T>
    static int push(lua_State* L, T&& t) {
        int pushcount = stack::push(L, detail::forward_get<0>(t));
        pushcount += stack::push(L, detail::forward_get<1>(t));
        return pushcount;
    }
};

template<typename O>
struct pusher<optional<O>> {
    template <typename T>
    static int push(lua_State* L, T&& t) {
        if (t == nullopt) {
            return stack::push(L, nullopt);
        }
        return stack::push(L, t.value());
    }
};

template<>
struct pusher<nullopt_t> {
    static int push(lua_State* L, nullopt_t) {
        return stack::push(L, nil);
    }
};

template<>
struct pusher<this_state> {
    static int push(lua_State*, const this_state&) {
        return 0;
    }
};
} // stack
} // sol

// end of sol\stack_push.hpp

// beginning of sol\stack_pop.hpp

namespace sol {
namespace stack {
template <typename T, typename>
struct popper {
    inline static decltype(auto) pop(lua_State* L) {
        decltype(auto) r = get<T>(L);
        lua_pop(L, 1);
        return r;
    }
};

template <typename... Args>
struct popper<std::tuple<Args...>> {
    inline static decltype(auto) pop(lua_State* L) {
        decltype(auto) r = get<std::tuple<Args...>>(L);
        lua_pop(L, static_cast<int>(sizeof...(Args)));
        return r;
    }
};

template <typename A, typename B>
struct popper<std::pair<A, B>> {
    inline static decltype(auto) pop(lua_State* L) {
        decltype(auto) r = get<std::pair<A, B>>(L);
        lua_pop(L, 2);
        return r;
    }
};

template <typename T>
struct popper<T, std::enable_if_t<std::is_base_of<stack_reference, meta::Unqualified<T>>::value>> {
    static_assert(meta::Not<std::is_base_of<stack_reference, meta::Unqualified<T>>>::value, "You cannot pop something that derives from stack_reference: it will not remain on the stack and thusly will go out of scope!");
};
} // stack
} // sol

// end of sol\stack_pop.hpp

// beginning of sol\stack_field.hpp

namespace sol {
namespace stack {
template <typename T, bool, typename>
struct field_getter {
    template <typename Key>
    void get(lua_State* L, Key&& key, int tableindex = -2) {
        push( L, std::forward<Key>( key ) );
        lua_gettable( L, tableindex );
    }
};

template <bool b, typename C>
struct field_getter<metatable_key_t, b, C> {
    void get(lua_State* L, metatable_key_t, int tableindex = -1) {
        if (lua_getmetatable(L, tableindex) == 0)
            push(L, nil);
    }
};

template <typename... Args, bool b, typename C>
struct field_getter<std::tuple<Args...>, b, C> {
    template <std::size_t... I, typename Keys>
    void apply(std::index_sequence<0, I...>, lua_State* L, Keys&& keys, int tableindex) {
        get_field<b>(L, detail::forward_get<0>(keys), tableindex);
        void(detail::swallow{ (get_field<false>(L, detail::forward_get<I>(keys)), 0)... });
        reference saved(L, -1);
        lua_pop(L, static_cast<int>(sizeof...(I)));
        saved.push();
    }

    template <typename Keys>
    void get(lua_State* L, Keys&& keys) {
        apply(std::make_index_sequence<sizeof...(Args)>(), L, std::forward<Keys>(keys), lua_absindex(L, -1));
    }

    template <typename Keys>
    void get(lua_State* L, Keys&& keys, int tableindex) {
        apply(std::make_index_sequence<sizeof...(Args)>(), L, std::forward<Keys>(keys), tableindex);
    }
};

template <typename A, typename B, bool b, typename C>
struct field_getter<std::pair<A, B>, b, C> {
    template <typename Keys>
    void get(lua_State* L, Keys&& keys, int tableindex) {
        get_field<b>(L, detail::forward_get<0>(keys), tableindex);
        get_field<false>(L, detail::forward_get<1>(keys));
        reference saved(L, -1);
        lua_pop(L, static_cast<int>(2));
        saved.push();
    }

    template <typename Keys>
    void get(lua_State* L, Keys&& keys) {
        get_field<b>(L, detail::forward_get<0>(keys));
        get_field<false>(L, detail::forward_get<1>(keys));
        reference saved(L, -1);
        lua_pop(L, static_cast<int>(2));
        saved.push();
    }
};

template <typename T>
struct field_getter<T, true, std::enable_if_t<meta::is_c_str<T>::value>> {
    template <typename Key>
    void get(lua_State* L, Key&& key, int = -1) {
        lua_getglobal(L, &key[0]);
    }
};

template <typename T>
struct field_getter<T, false, std::enable_if_t<meta::is_c_str<T>::value>> {
    template <typename Key>
    void get(lua_State* L, Key&& key, int tableindex = -1) {
        lua_getfield(L, tableindex, &key[0]);
    }
};

#if SOL_LUA_VERSION >= 503
template <typename T>
struct field_getter<T, false, std::enable_if_t<std::is_integral<T>::value>> {
    template <typename Key>
    void get(lua_State* L, Key&& key, int tableindex = -1) {
        lua_geti(L, tableindex, static_cast<lua_Integer>(key));
    }
};
#endif // Lua 5.3.x

template <typename T, bool, typename>
struct field_setter {
    template <typename Key, typename Value>
    void set(lua_State* L, Key&& key, Value&& value, int tableindex = -3) {
        push(L, std::forward<Key>(key));
        push(L, std::forward<Value>(value));
        lua_settable(L, tableindex);
    }
};

template <bool b, typename C>
struct field_setter<metatable_key_t, b, C> {
    template <typename Value>
    void set(lua_State* L, metatable_key_t, Value&& value, int tableindex = -2) {
        push(L, std::forward<Value>(value));
        lua_setmetatable(L, tableindex);
    }
};

template <typename T>
struct field_setter<T, true, std::enable_if_t<meta::is_c_str<T>::value>> {
    template <typename Key, typename Value>
    void set(lua_State* L, Key&& key, Value&& value, int = -2) {
        push(L, std::forward<Value>(value));
        lua_setglobal(L, &key[0]);
    }
};

template <typename T>
struct field_setter<T, false, std::enable_if_t<meta::is_c_str<T>::value>> {
    template <typename Key, typename Value>
    void set(lua_State* L, Key&& key, Value&& value, int tableindex = -2) {
        push(L, std::forward<Value>(value));
        lua_setfield(L, tableindex, &key[0]);
    }
};

#if SOL_LUA_VERSION >= 503
template <typename T>
struct field_setter<T, false, std::enable_if_t<std::is_integral<T>::value>> {
    template <typename Key, typename Value>
    void set(lua_State* L, Key&& key, Value&& value, int tableindex = -2) {
        push(L, std::forward<Value>(value));
        lua_seti(L, tableindex, static_cast<lua_Integer>(key));
    }
};
#endif // Lua 5.3.x

template <typename... Args, bool b, typename C>
struct field_setter<std::tuple<Args...>, b, C> {
    template <bool g, std::size_t I, typename Key, typename Value>
    void apply(std::index_sequence<I>, lua_State* L, Key&& keys, Value&& value, int tableindex) {
        I < 1 ? 
            set_field<g>(L, detail::forward_get<I>(keys), std::forward<Value>(value), tableindex) :
            set_field<g>(L, detail::forward_get<I>(keys), std::forward<Value>(value));
    }

    template <bool g, std::size_t I0, std::size_t I1, std::size_t... I, typename Keys, typename Value>
    void apply(std::index_sequence<I0, I1, I...>, lua_State* L, Keys&& keys, Value&& value, int tableindex) {
        I0 < 1 ? get_field<g>(L, detail::forward_get<I0>(keys), tableindex) : get_field<g>(L, detail::forward_get<I0>(keys), -1);
        apply<false>(std::index_sequence<I1, I...>(), L, std::forward<Keys>(keys), std::forward<Value>(value), -1);
    }

    template <typename Keys, typename Value>
    void set(lua_State* L, Keys&& keys, Value&& value, int tableindex = -3) {
        apply<b>(std::make_index_sequence<sizeof...(Args)>(), L, std::forward<Keys>(keys), std::forward<Value>(value), tableindex);
    }
};

template <typename A, typename B, bool b, typename C>
struct field_setter<std::pair<A, B>, b, C> {
    template <typename Keys, typename Value>
    void set(lua_State* L, Keys&& keys, Value&& value, int tableindex = -1) {
        get_field<b>(L, detail::forward_get<0>(keys), tableindex);
        set_field<false>(L, detail::forward_get<1>(keys), std::forward<Value>(value));
    }
};
} // stack
} // sol

// end of sol\stack_field.hpp

// beginning of sol\stack_probe.hpp

namespace sol {
namespace stack {
template <typename T, bool b, typename>
struct probe_field_getter {
    template <typename Key>
    probe get(lua_State* L, Key&& key, int tableindex = -2) {
        if (!b && !maybe_indexable(L, tableindex)) {
            return probe(false, 0);
        }
        get_field<b>(L, std::forward<Key>(key), tableindex);
        return probe(!check<nil_t>(L), 1);
    }
};

template <typename A, typename B, bool b, typename C>
struct probe_field_getter<std::pair<A, B>, b, C> {
    template <typename Keys>
    probe get(lua_State* L, Keys&& keys, int tableindex = -2) {
        if (!b && !maybe_indexable(L, tableindex)) {
            return probe(false, 0);
        }
        get_field<b>(L, std::get<0>(keys), tableindex);
        if (!maybe_indexable(L)) {
            return probe(false, 1);
        }
        get_field<false>(L, std::get<1>(keys), tableindex);
        return probe(!check<nil_t>(L), 2);
    }
};

template <typename... Args, bool b, typename C>
struct probe_field_getter<std::tuple<Args...>, b, C> {
    template <std::size_t I, typename Keys>
    probe apply(std::index_sequence<I>, int sofar, lua_State* L, Keys&& keys, int tableindex) {
        get_field<I < 1 && b>(L, std::get<I>(keys), tableindex);
        return probe(!check<nil_t>(L), sofar);
    }

    template <std::size_t I, std::size_t I1, std::size_t... In, typename Keys>
    probe apply(std::index_sequence<I, I1, In...>, int sofar, lua_State* L, Keys&& keys, int tableindex) {
        get_field<I < 1 && b>(L, std::get<I>(keys), tableindex);
        if (!maybe_indexable(L)) {
            return probe(false, sofar);
        }
        return apply(std::index_sequence<I1, In...>(), sofar + 1, L, std::forward<Keys>(keys), -1);
    }

    template <typename Keys>
    probe get(lua_State* L, Keys&& keys, int tableindex = -2) {
        if (!b && !maybe_indexable(L, tableindex)) {
            return probe(false, 0);
        }
        return apply(std::index_sequence_for<Args...>(), 1, L, std::forward<Keys>(keys), tableindex);
    }
};
} // stack
} // sol

// end of sol\stack_probe.hpp

#include <cstring>

namespace sol {
namespace stack {
namespace stack_detail {
template <typename T>
struct is_this_state_raw : std::false_type {};
template <>
struct is_this_state_raw<this_state> : std::true_type {};
template <typename T>
using is_this_state = is_this_state_raw<meta::Unqualified<T>>;

template<typename T>
inline int push_as_upvalues(lua_State* L, T& item) {
    typedef std::decay_t<T> TValue;
    const static std::size_t itemsize = sizeof(TValue);
    const static std::size_t voidsize = sizeof(void*);
    const static std::size_t voidsizem1 = voidsize - 1;
    const static std::size_t data_t_count = (sizeof(TValue) + voidsizem1) / voidsize;
    typedef std::array<void*, data_t_count> data_t;

    data_t data{{}};
    std::memcpy(&data[0], std::addressof(item), itemsize);
    int pushcount = 0;
    for(auto&& v : data) {
        pushcount += push(L, light_userdata_value(v));
    }
    return pushcount;
}

template<typename T>
inline std::pair<T, int> get_as_upvalues(lua_State* L, int index = 1) {
    const static std::size_t data_t_count = (sizeof(T)+(sizeof(void*)-1)) / sizeof(void*);
    typedef std::array<void*, data_t_count> data_t;
    data_t voiddata{ {} };
    for(std::size_t i = 0, d = 0; d < sizeof(T); ++i, d += sizeof(void*)) {
        voiddata[i] = get<light_userdata_value>(L, up_value_index(index++));
    }
    return std::pair<T, int>(*reinterpret_cast<T*>(static_cast<void*>(voiddata.data())), index);
}

template <bool checkargs = default_check_arguments, std::size_t... I, typename R, typename... Args, typename Fx, typename... FxArgs, typename = std::enable_if_t<!std::is_void<R>::value>>
inline decltype(auto) call(types<R>, types<Args...> ta, std::index_sequence<I...> tai, lua_State* L, int start, Fx&& fx, FxArgs&&... args) {
    typedef meta::index_in_pack<this_state, Args...> state_index;
    typedef meta::index_in_pack<variadic_args, Args...> va_pack_index;
    check_types<checkargs>{}.check(ta, tai, L, start, type_panic);
    return fx(std::forward<FxArgs>(args)..., stack_detail::unchecked_get<Args>(L, start + I - static_cast<int>(state_index::value < I) - static_cast<int>(va_pack_index::value < I))...);
}

template <bool checkargs = default_check_arguments, std::size_t... I, typename... Args, typename Fx, typename... FxArgs>
inline void call(types<void>, types<Args...> ta, std::index_sequence<I...> tai, lua_State* L, int start, Fx&& fx, FxArgs&&... args) {
    typedef meta::index_in_pack<this_state, Args...> state_index;
    typedef meta::index_in_pack<variadic_args, Args...> va_pack_index;
    check_types<checkargs>{}.check(ta, tai, L, start, type_panic);
    fx(std::forward<FxArgs>(args)..., stack_detail::unchecked_get<Args>(L, start + I - static_cast<int>(state_index::value < I) - static_cast<int>(va_pack_index::value < I))...);
}
} // stack_detail

inline void remove( lua_State* L, int index, int count ) {
    if ( count < 1 )
        return;
    int top = lua_gettop(L);
    if ( index == -1 || top == index ) {
        // Slice them right off the top
        lua_pop( L, static_cast<int>(count) );
        return;
    }

    // Remove each item one at a time using stack operations
    // Probably slower, maybe, haven't benchmarked,
    // but necessary
    if ( index < 0 ) {
        index = lua_gettop(L) + (index + 1);
    }
    int last = index + count;
    for ( int i = index; i < last; ++i ) {
        lua_remove(L, i);
    }
}

template <bool check_args = stack_detail::default_check_arguments, typename R, typename... Args, typename Fx, typename... FxArgs, typename = std::enable_if_t<!std::is_void<R>::value>>
inline decltype(auto) call(types<R> tr, types<Args...> ta, lua_State* L, int start, Fx&& fx, FxArgs&&... args) {
    typedef std::make_index_sequence<sizeof...(Args)> args_indices;
    return stack_detail::call<check_args>(tr, ta, args_indices(), L, start, std::forward<Fx>(fx), std::forward<FxArgs>(args)...);
}

template <bool check_args = stack_detail::default_check_arguments, typename R, typename... Args, typename Fx, typename... FxArgs, typename = std::enable_if_t<!std::is_void<R>::value>>
inline decltype(auto) call(types<R> tr, types<Args...> ta, lua_State* L, Fx&& fx, FxArgs&&... args) {
    return call<check_args>(tr, ta, L, 1, std::forward<Fx>(fx), std::forward<FxArgs>(args)...);
}

template <bool check_args = stack_detail::default_check_arguments, typename... Args, typename Fx, typename... FxArgs>
inline void call(types<void> tr, types<Args...> ta, lua_State* L, int start, Fx&& fx, FxArgs&&... args) {
    typedef std::make_index_sequence<sizeof...(Args)> args_indices;
    stack_detail::call<check_args>(tr, ta, args_indices(), L, start, std::forward<Fx>(fx), std::forward<FxArgs>(args)...);
}

template <bool check_args = stack_detail::default_check_arguments, typename... Args, typename Fx, typename... FxArgs>
inline void call(types<void> tr, types<Args...> ta, lua_State* L, Fx&& fx, FxArgs&&... args) {
    call<check_args>(tr, ta, L, 1, std::forward<Fx>(fx), std::forward<FxArgs>(args)...);
}

template <bool check_args = stack_detail::default_check_arguments, typename R, typename... Args, typename Fx, typename... FxArgs, typename = std::enable_if_t<!std::is_void<R>::value>>
inline decltype(auto) call_from_top(types<R> tr, types<Args...> ta, lua_State* L, Fx&& fx, FxArgs&&... args) {
    return call<check_args>(tr, ta, L, static_cast<int>(lua_gettop(L) - sizeof...(Args)), std::forward<Fx>(fx), std::forward<FxArgs>(args)...);
}

template <bool check_args = stack_detail::default_check_arguments, typename... Args, typename Fx, typename... FxArgs>
inline void call_from_top(types<void> tr, types<Args...> ta, lua_State* L, Fx&& fx, FxArgs&&... args) {
    call<check_args>(tr, ta, L, static_cast<int>(lua_gettop(L) - sizeof...(Args)), std::forward<Fx>(fx), std::forward<FxArgs>(args)...);
}

template<int additionalpop = 0, bool check_args = stack_detail::default_check_arguments, typename... Args, typename Fx, typename... FxArgs>
inline int call_into_lua(types<void> tr, types<Args...> ta, lua_State* L, int start, Fx&& fx, FxArgs&&... fxargs) {
    call<check_args>(tr, ta, L, start, std::forward<Fx>(fx), std::forward<FxArgs>(fxargs)...);
    int nargs = static_cast<int>(sizeof...(Args)) + additionalpop - meta::count_if_pack<stack_detail::is_this_state, Args...>::value;
    lua_pop(L, nargs);
    return 0;
}

template<int additionalpop = 0, bool check_args = stack_detail::default_check_arguments, typename Ret0, typename... Ret, typename... Args, typename Fx, typename... FxArgs, typename = std::enable_if_t<meta::Not<std::is_void<Ret0>>::value>>
inline int call_into_lua(types<Ret0, Ret...>, types<Args...> ta, lua_State* L, int start, Fx&& fx, FxArgs&&... fxargs) {
    decltype(auto) r = call<check_args>(types<meta::return_type_t<Ret0, Ret...>>(), ta, L, start, std::forward<Fx>(fx), std::forward<FxArgs>(fxargs)...);
    int nargs = static_cast<int>(sizeof...(Args)) + additionalpop - meta::count_if_pack<stack_detail::is_this_state, Args...>::value;
    lua_pop(L, nargs);
    return push_reference(L, std::forward<decltype(r)>(r));
}

inline call_syntax get_call_syntax(lua_State* L, const std::string& key, int index = -2) {
    luaL_getmetatable(L, key.c_str());
    if (lua_compare(L, -1, index, LUA_OPEQ) == 1) {
        lua_pop(L, 1);
        return call_syntax::colon;
    }
    lua_pop(L, 1);
    return call_syntax::dot;
}

inline void luajit_exception_handler(lua_State* L, int(*handler)(lua_State*, lua_CFunction) = detail::c_trampoline) {
#ifdef SOL_LUAJIT
    lua_pushlightuserdata(L, (void*)handler);
    luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
    lua_pop(L, 1);
#else
    (void)L;
    (void)handler;
#endif
}

inline void luajit_exception_off(lua_State* L) {
#ifdef SOL_LUAJIT
    luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_OFF);
#else
    (void)L;
#endif
}
} // stack
} // sol

// end of sol\stack.hpp

// beginning of sol\variadic_args.hpp

// beginning of sol\stack_proxy.hpp

// beginning of sol\function.hpp

// beginning of sol\function_result.hpp

// beginning of sol\proxy_base.hpp

namespace sol {
template <typename Super>
struct proxy_base {
    operator std::string() const {
        const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
        return super.template get<std::string>();
    }

    template<typename T, meta::EnableIf<meta::Not<meta::is_string_constructible<T>>, is_proxy_primitive<meta::Unqualified<T>>> = 0>
    operator T ( ) const {
        const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
        return super.template get<T>( );
    }

    template<typename T, meta::EnableIf<meta::Not<meta::is_string_constructible<T>>, meta::Not<is_proxy_primitive<meta::Unqualified<T>>>> = 0>
    operator T& ( ) const {
        const Super& super = *static_cast<const Super*>(static_cast<const void*>(this));
        return super.template get<T&>( );
    }
};
} // sol

// end of sol\proxy_base.hpp

#include <cstdint>

namespace sol {
struct function_result : public proxy_base<function_result> {
private:
    lua_State* L;
    int index;
    int returncount;

public:
    function_result() = default;
    function_result(lua_State* L, int index = -1, int returncount = 0): L(L), index(index), returncount(returncount) {
        
    }
    function_result(const function_result&) = default;
    function_result& operator=(const function_result&) = default;
    function_result(function_result&& o) : L(o.L), index(o.index), returncount(o.returncount) {
        // Must be manual, otherwise destructor will screw us
        // return count being 0 is enough to keep things clean
        // but will be thorough
        o.L = nullptr;
        o.index = 0;
        o.returncount = 0;
    }
    function_result& operator=(function_result&& o) {
        L = o.L;
        index = o.index;
        returncount = o.returncount;
        // Must be manual, otherwise destructor will screw us
        // return count being 0 is enough to keep things clean
        // but will be thorough
        o.L = nullptr;
        o.index = 0;
        o.returncount = 0;
        return *this;
    }

    template<typename T>
    decltype(auto) get() const {
        return stack::get<T>(L, index);
    }

    lua_State* lua_state() const { return L; };
    int stack_index() const { return index; };

    ~function_result() {
        lua_pop(L, returncount);
    }
};
} // sol

// end of sol\function_result.hpp

// beginning of sol\function_types.hpp

// beginning of sol\function_types_core.hpp

namespace sol {
template <typename RSig = void, typename WSig = void>
struct member_property {
    typedef std::conditional_t<std::is_void<RSig>::value, detail::empty, RSig> R;
    typedef std::conditional_t<std::is_void<WSig>::value, detail::empty, WSig> W;

    R read;
    W write;

    member_property(R read, W write) : read(std::move(read)), write(std::move(write)) {}

    template <typename T, typename Arg>
    void write_if(std::true_type, T& mem, Arg&& arg) {
        write(mem, arg);
    }

    template <typename T, typename Arg>
    void write_if(std::false_type, T&, Arg&&) {
        // This is a fatal error if we get here...
        // Should never happen but...
        // Crash horrifically, for safety?
        std::abort();
    }

    template <typename T, typename Arg>
    void operator()(T& mem, Arg&& arg) {
        write_if(meta::Not<std::is_void<WSig>>(), mem, arg);
    }

    template <typename T>
    decltype(auto) read_if(std::true_type, T& mem) {
        return read(mem);
    }

    template <typename T>
    decltype(auto) read_if(std::false_type, T&) {
        typedef typename meta::bind_traits<WSig>::template arg_at<1> Arg;
       typedef std::add_pointer_t<std::remove_reference_t<Arg>> pret;
        // This is a fatal error if we get here...
        // Should never happen but...
        // Crash horrifically, for safety?
        std::abort();
        return *pret();
    }

    template <typename T>
    decltype(auto) operator()(T& mem) {
        return read_if(meta::Not<std::is_void<RSig>>(), mem);
    }
};

template <typename R, typename T>
inline decltype(auto) property( R(T::* readfunc )() const) {
    auto rf = [readfunc](T& mem) -> R {return (mem.*readfunc)();};
    return member_property<decltype(rf)>(std::move(rf), detail::empty());
}

template <typename R, typename T>
inline decltype(auto) property( R(T::* readfunc )()) {
    auto rf = [readfunc](T& mem) -> R {return (mem.*readfunc)();};
    return member_property<decltype(rf)>(std::move(rf), detail::empty());
}

template <typename R, typename T, typename Arg>
inline decltype(auto) property(R(T::* writefunc)(Arg)) {
    auto wf = [writefunc](T& mem, Arg arg) {(mem.*writefunc)(std::forward<Arg>(arg));};
    return member_property<void, decltype(wf)>(detail::empty(), std::move(wf));
}

template <typename R, typename T, typename Arg>
inline decltype(auto) property(R(T::* writefunc)(Arg) const) {
    auto wf = [writefunc](T& mem, Arg arg) {(mem.*writefunc)(std::forward<Arg>(arg));};
    return member_property<void, decltype(wf)>(detail::empty(), std::move(wf));
}

template <typename RR, typename RT, typename WR, typename WT, typename Arg>
inline decltype(auto) property(RR(RT::* readfunc)(), WR(WT::* writefunc)(Arg)) {
    auto rf = [readfunc](RT& mem) -> RR {return (mem.*readfunc)();};
    auto wf = [writefunc](WT& mem, Arg arg) {(mem.*writefunc)(std::forward<Arg>(arg));};
    return member_property<decltype(rf), decltype(wf)>(std::move(rf), std::move(wf));
}

template <typename RR, typename RT, typename WR, typename WT, typename Arg>
inline decltype(auto) property(RR(RT::* readfunc)() const, WR(WT::* writefunc)(Arg)) {
    auto rf = [readfunc](RT& mem) -> RR {return (mem.*readfunc)();};
    auto wf = [writefunc](WT& mem, Arg arg) {(mem.*writefunc)(std::forward<Arg>(arg));};
    return member_property<decltype(rf), decltype(wf)>(std::move(rf), std::move(wf));
}

template <typename RR, typename RT, typename WR, typename WT, typename Arg>
inline decltype(auto) property(RR(RT::* readfunc)(), WR(WT::* writefunc)(Arg) const) {
    auto rf = [readfunc](RT& mem) -> RR {return (mem.*readfunc)();};
    auto wf = [writefunc](WT& mem, Arg arg) {(mem.*writefunc)(std::forward<Arg>(arg));};
    return member_property<decltype(rf), decltype(wf)>(std::move(rf), std::move(wf));
}

template <typename RR, typename RT, typename WR, typename WT, typename Arg>
inline decltype(auto) property(RR(RT::* readfunc)() const, WR(WT::* writefunc)(Arg) const) {
    auto rf = [readfunc](RT& mem) -> RR {return (mem.*readfunc)();};
    auto wf = [writefunc](WT& mem, Arg arg) {(mem.*writefunc)(std::forward<Arg>(arg));};
    return member_property<decltype(rf), decltype(wf)>(std::move(rf), std::move(wf));
}

namespace function_detail {
template <typename T>
struct implicit_wrapper {
    T& item;
    implicit_wrapper(T* item) : item(*item) {}
    implicit_wrapper(T& item) : item(item) {}
    operator T& () {
        return item;
    }
    operator T* () {
        return std::addressof(item);
    }
};

inline decltype(auto) cleanup_key() {
    const auto& name = u8"sol.ƒ.♲.🗑.(/¯◡ ‿ ◡)/¯ ~ ┻━┻ (ﾉ◕ヮ◕)ﾉ*:･ﾟ✧";
    return name;
}

template<typename T, typename Func, typename = void>
struct functor {
    typedef meta::bind_traits<Func> traits_type;
    typedef typename traits_type::args_type args_type;
    typedef typename traits_type::return_type return_type;
    typedef std::conditional_t<std::is_pointer<Func>::value || std::is_class<Func>::value, Func, std::add_pointer_t<Func>> function_type;
    static const std::size_t arity = traits_type::arity;
    static const bool is_free = true;
    function_type invocation;

    template<typename... Args>
    functor(Args&&... args): invocation(std::forward<Args>(args)...) {}

    template<typename... Args>
    void call(types<void>, Args&&... args) {
        invocation(std::forward<Args>(args)...);
    }

    template<typename Ret, typename... Args>
    Ret call(types<Ret>, Args&&... args) {
        return invocation(std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(std::declval<functor>().call(types<return_type>{}, std::forward<Args>(args)...)) {
        return this->call(types<return_type>(), std::forward<Args>(args)...);
    }
};

template<typename T, typename Func>
struct functor<T, Func, std::enable_if_t<!std::is_member_pointer<Func>::value && std::is_base_of<T, meta::Unqualified<typename meta::bind_traits<Func>::template arg_at<0>>>::value>> {
    typedef meta::bind_traits<Func> traits_type;
    typedef meta::pop_front_type_t<typename traits_type::args_type> args_type;
    typedef typename traits_type::return_type return_type;
    typedef std::conditional_t<std::is_pointer<Func>::value || std::is_class<Func>::value, Func, std::add_pointer_t<Func>> function_type;
    static const std::size_t arity = traits_type::arity - 1;
    static const bool is_free = false;
    T* item;
    function_type invocation;

    template<typename... Args>
    functor(Args&&... args): item(nullptr), invocation(std::forward<Args>(args)...) {}

    template<typename... Args>
    void call(types<void>, Args&&... args) {
        T& member = *item;
        invocation(implicit_wrapper<T>(member), std::forward<Args>(args)...);
    }

    template<typename Ret, typename... Args>
    Ret call(types<Ret>, Args&&... args) {
        T& member = *item;
        return invocation(implicit_wrapper<T>(member), std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(std::declval<functor>().call(types<return_type>{}, std::forward<Args>(args)...)) {
        return this->call(types<return_type>(), std::forward<Args>(args)...);
    }
};

template<typename T, typename RSig, typename WSig, typename C>
struct functor<T, member_property<RSig, WSig>, C> {
    typedef meta::bind_traits<std::conditional_t<std::is_void<WSig>::value, RSig, WSig>> traits_type;
    typedef meta::pop_front_type_t<typename traits_type::args_type> args_type;
    typedef std::conditional_t<std::is_void<typename traits_type::return_type>::value, typename traits_type::template arg_at<0>, typename traits_type::return_type> return_type;
    typedef member_property<RSig, WSig> function_type;
    typedef meta::Not<std::is_void<RSig>> can_read;
    typedef meta::Not<std::is_void<WSig>> can_write;
    static const bool is_free = false;

    T* item;
    function_type invocation;

    template<typename... Args>
    functor(Args&&... args): item(nullptr), invocation(std::forward<Args>(args)...) {}

    template<typename Arg>
    void call(Arg&& arg) {
        T& member = *item;
        invocation(member, std::forward<Arg>(arg));
    }

    decltype(auto) call() {
        T& member = *item;
        return invocation(member);
    }

    template<typename... Args>
    decltype(auto) operator()(Args&&... args) {
        return this->call(std::forward<Args>(args)...);
    }
};

template<typename T, typename Func>
struct functor<T, Func, std::enable_if_t<std::is_member_object_pointer<Func>::value>> {
    typedef meta::bind_traits<Func> traits_type;
    typedef typename traits_type::args_type args_type;
    typedef typename traits_type::return_type return_type;
    static const std::size_t arity = traits_type::arity;
    typedef std::true_type can_read;
    typedef std::true_type can_write;
    static const bool is_free = false;

    T* item;
    Func invocation;

    template<typename... Args>
    functor(Args&&... args): item(nullptr), invocation(std::forward<Args>(args)...) {}

    template<typename Arg>
    void call(Arg&& arg) {
        T& member = *item;
        (member.*invocation) = std::forward<Arg>(arg);
    }

    return_type& call() {
         T& member = *item;
         return (member.*invocation);
    }

    template<typename... Args>
    decltype(auto) operator()(Args&&... args) {
        return this->call(std::forward<Args>(args)...);
    }
};

template<typename T, typename Func>
struct functor<T, Func, std::enable_if_t<std::is_member_function_pointer<Func>::value>> {
    typedef meta::bind_traits<Func> traits_type;
    typedef typename traits_type::args_type args_type;
    typedef typename traits_type::return_type return_type;
    static const std::size_t arity = traits_type::arity;
    static const bool is_free = false;

    T* item;
    Func invocation;

    template<typename... Args>
    functor(Args&&... args): item(nullptr), invocation(std::forward<Args>(args)...) {}

    template<typename... Args>
    void call(types<void>, Args&&... args) {
        T& member = *item;
        (member.*invocation)(std::forward<Args>(args)...);
    }

    template<typename Ret, typename... Args>
    Ret call(types<Ret>, Args&&... args) {
        T& member = *item;
        return (member.*invocation)(std::forward<Args>(args)...);
    }

    template<typename... Args>
    decltype(auto) operator()(Args&&... args) {
        return this->call(types<return_type>{}, std::forward<Args>(args)...);
    }
};

struct base_function {
    virtual int operator()(lua_State* L) {
        return luaL_error(L, "sol: failure to call specialized wrapped C++ function from Lua");
    }

    virtual ~base_function() {}
};

static int base_call(lua_State* L, void* inheritancedata) {
    if (inheritancedata == nullptr) {
        return luaL_error(L, "sol: call from Lua to C++ function has null data");
    }

    base_function* pfx = static_cast<base_function*>(inheritancedata);
    base_function& fx = *pfx;
    return detail::trampoline(L, fx);
}

static int base_gc(lua_State* L, void* udata) {
    if (udata == nullptr) {
        return luaL_error(L, "sol: call from lua to C++ gc function with null data");
    }

    base_function* ptr = static_cast<base_function*>(udata);
    std::default_delete<base_function> dx{};
    dx(ptr);
    return 0;
}

template <std::size_t limit>
static void func_gc(std::true_type, lua_State*) {

}

template <std::size_t limit>
static void func_gc(std::false_type, lua_State* L) {
    for (std::size_t i = 0; i < limit; ++i) {
        void* value = stack::get<light_userdata_value>(L, up_value_index(static_cast<int>(i + 1)));
        if (value == nullptr)
            continue;
        base_function* obj = static_cast<base_function*>(value);
        std::allocator<base_function> alloc{};
        alloc.destroy(obj);
        alloc.deallocate(obj, 1);
    }
}

inline int call(lua_State* L) {
    void* ludata = stack::get<light_userdata_value>(L, up_value_index(1));
    void** pinheritancedata = static_cast<void**>(ludata);
    return base_call(L, *pinheritancedata);
}

inline int gc(lua_State* L) {
    void* udata = stack::get<userdata_value>(L, 1);
    void** pudata = static_cast<void**>(udata);
    return base_gc(L, *pudata);
}

template<std::size_t I>
inline int usertype_call(lua_State* L) {
    // Zero-based template parameter, but upvalues start at 1
    return base_call(L, stack::get<light_userdata_value>(L, up_value_index(static_cast<int>(I + 1))));
}

template<std::size_t I>
inline int usertype_gc(lua_State* L) {
    func_gc<I>(meta::Bool<(I < 1)>(), L);
    return 0;
}

inline void free_function_cleanup(lua_State* L) {
    const static char* metatablename = &cleanup_key()[0];
    int metapushed = luaL_newmetatable(L, metatablename);
    if (metapushed == 1) {
        stack::set_field(L, "__gc", function_detail::gc);
    }
}
} // function_detail
} // sol

// end of sol\function_types_core.hpp

// beginning of sol\function_types_templated.hpp

namespace sol {
namespace function_detail {
    template <typename F, F fx>
    inline int call_wrapper_variable(std::false_type, lua_State* L) {
        typedef meta::bind_traits<meta::Unqualified<F>> traits_type;
        typedef typename traits_type::args_type args_type;
        typedef meta::tuple_types<typename traits_type::return_type> return_type;
        return stack::call_into_lua(return_type(), args_type(), L, 1, fx);
    }

    template <typename R, typename V, V, typename T>
    inline int call_set_assignable(std::false_type, T&&, lua_State* L) {
        lua_pop(L, 2);
        return luaL_error(L, "cannot write to this type: copy assignment/constructor not available");
    }

    template <typename R, typename V, V variable, typename T>
    inline int call_set_assignable(std::true_type, lua_State* L, T&& mem) {
        (mem.*variable) = stack::get<R>(L, 2);
        lua_pop(L, 2);
        return 0;
    }

    template <typename R, typename V, V, typename T>
    inline int call_set_variable(std::false_type, lua_State* L, T&&) {
        lua_pop(L, 2);
        return luaL_error(L, "cannot write to a const variable");
    }

    template <typename R, typename V, V variable, typename T>
    inline int call_set_variable(std::true_type, lua_State* L, T&& mem) {
        return call_set_assignable<R, V, variable>(std::is_assignable<std::add_lvalue_reference_t<R>, R>(), L, std::forward<T>(mem));
    }

    template <typename V, V variable>
    inline int call_wrapper_variable(std::true_type, lua_State* L) {
        typedef meta::bind_traits<meta::Unqualified<V>> traits_type;
        typedef typename traits_type::object_type T;
        typedef typename traits_type::return_type R;
        auto& mem = stack::get<T>(L, 1);
        switch (lua_gettop(L)) {
        case 1: {
            decltype(auto) r = (mem.*variable);
            lua_pop(L, 1);
            stack::push_reference(L, std::forward<decltype(r)>(r));
            return 1; }
        case 2:
            return call_set_variable<R, V, variable>(meta::Not<std::is_const<R>>(), L, mem);
        default:
            return luaL_error(L, "incorrect number of arguments to member variable function call");
        }
    }

    template <typename F, F fx>
    inline int call_wrapper_function(std::false_type, lua_State* L) {
        return call_wrapper_variable<F, fx>(std::is_member_object_pointer<F>(), L);
    }

    template <typename F, F fx>
    inline int call_wrapper_function(std::true_type, lua_State* L) {
        typedef meta::bind_traits<meta::Unqualified<F>> traits_type;
        typedef typename traits_type::object_type T;
        typedef typename traits_type::args_type args_type;
        typedef typename traits_type::return_type return_type;
        typedef meta::tuple_types<return_type> return_type_list;
        auto mfx = [&](auto&&... args) -> typename traits_type::return_type {
            auto& member = stack::get<T>(L, 1);
            return (member.*fx)(std::forward<decltype(args)>(args)...);
        };
        int n = stack::call_into_lua<1>(return_type_list(), args_type(), L, 2, mfx);
        return n;
    }

    template <typename F, F fx>
    inline int call_wrapper_entry(lua_State* L) {
        return call_wrapper_function<F, fx>(std::is_member_function_pointer<meta::Unqualified<F>>(), L);
    }
} // function_detail

template <typename F, F fx>
inline int c_call(lua_State* L) {
    return detail::c_trampoline(L, function_detail::call_wrapper_entry<F, fx>);
}
} // sol

// end of sol\function_types_templated.hpp

// beginning of sol\function_types_basic.hpp

namespace sol {
namespace function_detail {
template<typename Function>
struct upvalue_free_function {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef meta::bind_traits<function_type> traits_type;

    static int real_call(lua_State* L) {
        auto udata = stack::stack_detail::get_as_upvalues<function_type*>(L);
        function_type* fx = udata.first;
        int r = stack::call_into_lua(meta::tuple_types<typename traits_type::return_type>(), typename traits_type::args_type(), L, 1, fx);
        return r;
    }

    static int call (lua_State* L) {
        return detail::static_trampoline<(&real_call)>(L);
    }

    int operator()(lua_State* L) {
        return call(L);
    }
};

template<typename T, typename Function>
struct upvalue_member_function {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef meta::bind_traits<function_type> traits_type;

    static int real_call(lua_State* L) {
        // Layout:
        // idx 1...n: verbatim data of member function pointer
        // idx n + 1: is the object's void pointer
        // We don't need to store the size, because the other side is templated
        // with the same member function pointer type
        auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L, 1);
        auto objdata = stack::stack_detail::get_as_upvalues<T*>(L, memberdata.second);
        function_type& memfx = memberdata.first;
        auto& item = *objdata.first;
        auto fx = [&item, &memfx](auto&&... args) -> typename traits_type::return_type { 
            return (item.*memfx)(std::forward<decltype(args)>(args)...);
        };
        return stack::call_into_lua(meta::tuple_types<typename traits_type::return_type>(), typename traits_type::args_type(), L, 1, fx);
    }

    static int call (lua_State* L) {
        return detail::static_trampoline<(&real_call)>(L);
    }

    int operator()(lua_State* L) {
        return call(L);
    }
};

template <int N, typename R, typename M, typename V>
int set_assignable(std::false_type, lua_State* L, M&, V&) {
    lua_pop(L, N);
    return luaL_error(L, "sol: cannot write to this type: copy assignment/constructor not available");
}

template <int N, typename R, typename M, typename V>
int set_assignable(std::true_type, lua_State* L, M& mem, V& var) {
    (mem.*var) = stack::get<R>(L, N);
    lua_pop(L, N);
    return 0;
}

template <int N, typename R, typename M, typename V>
int set_variable(std::true_type, lua_State* L, M& mem, V& var) {
    return set_assignable<N, R>(std::is_assignable<std::add_lvalue_reference_t<R>, R>(), L, mem, var);
}

template <int N, typename R, typename M, typename V>
int set_variable(std::false_type, lua_State* L, M&, V&) {
    lua_pop(L, N);
    return luaL_error(L, "sol: cannot write to a const variable");
}

template<typename T, typename Function>
struct upvalue_member_variable {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef meta::bind_traits<function_type> traits_type;

    static int real_call(lua_State* L) {
        // Layout:
        // idx 1...n: verbatim data of member variable pointer
        // idx n + 1: is the object's void pointer
        // We don't need to store the size, because the other side is templated
        // with the same member function pointer type
        auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L, 1);
        auto objdata = stack::stack_detail::get_as_upvalues<T*>(L, memberdata.second);
        auto& mem = *objdata.first;
        function_type& var = memberdata.first;
        switch (lua_gettop(L)) {
        case 0:
            stack::push(L, (mem.*var));
            return 1;
        case 1:
            set_variable<1, typename traits_type::return_type>(meta::Not<std::is_const<typename traits_type::return_type>>(), L, mem, var);
            return 0;
        default:
            return luaL_error(L, "sol: incorrect number of arguments to member variable function");
        }
    }

    static int call (lua_State* L) {
        return detail::static_trampoline<(&real_call)>(L);
    }

    int operator()(lua_State* L) {
        return call(L);
    }
};

template<typename T, typename Function>
struct upvalue_this_member_function {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef meta::bind_traits<function_type> traits_type;

    static int real_call(lua_State* L) {
        // Layout:
        // idx 1...n: verbatim data of member variable pointer
        auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L, 1);
        function_type& memfx = memberdata.first;
        auto fx = [&L, &memfx](auto&&... args) -> typename traits_type::return_type { 
            auto& item = stack::get<T>(L, 1);
            return (item.*memfx)(std::forward<decltype(args)>(args)...);
        };
        int n = stack::call_into_lua<1>(meta::tuple_types<typename traits_type::return_type>(), typename traits_type::args_type(), L, 2, fx);
        return n;
    }

    static int call (lua_State* L) {
        return detail::static_trampoline<(&real_call)>(L);
    }

    int operator()(lua_State* L) {
        return call(L);
    }
};

template<typename T, typename Function>
struct upvalue_this_member_variable {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef meta::bind_traits<function_type> traits_type;

    static int real_call(lua_State* L) {
        // Layout:
        // idx 1...n: verbatim data of member variable pointer
        auto memberdata = stack::stack_detail::get_as_upvalues<function_type>(L, 1);
        auto& mem = stack::get<T>(L, 1);
        function_type& var = memberdata.first;
        switch (lua_gettop(L)) {
        case 1:
            lua_pop(L, 1);
            stack::push(L, (mem.*var));
            return 1;
        case 2:
            set_variable<2, typename traits_type::return_type>(meta::Not<std::is_const<typename traits_type::return_type>>(), L, mem, var);
            return 0;
        default:
            return luaL_error(L, "sol: incorrect number of arguments to member variable function");
        }
    }

    static int call (lua_State* L) {
        return detail::static_trampoline<(&real_call)>(L);
    }

    int operator()(lua_State* L) {
        return call(L);
    }
};
} // function_detail
} // sol

// end of sol\function_types_basic.hpp

// beginning of sol\function_types_allocator.hpp

// beginning of sol\function_types_overload.hpp

// beginning of sol\function_types_usertype.hpp

#include <map>

namespace sol {
namespace function_detail {
template<typename Tp, typename Function>
struct usertype_function_core : public base_function {
    typedef std::remove_pointer_t<Tp> T;
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef functor<T, function_type> fx_t;
    typedef typename fx_t::traits_type traits_type;
    typedef typename fx_t::args_type args_type;
    typedef typename fx_t::return_type return_type;

    fx_t fx;

    template<typename... Args>
    usertype_function_core(Args&&... args): fx(std::forward<Args>(args)...) {}

    template<typename... Ret, typename... Args, std::size_t Start>
    int operator()(types<Ret...> tr, types<Args...> ta, index_value<Start>, lua_State* L) {
        return stack::call_into_lua<1>(tr, ta, L, static_cast<int>(Start), fx);
    }
};

template<typename Tp, typename Function>
struct usertype_function : public usertype_function_core<Tp, Function> {
    typedef usertype_function_core<Tp, Function> base_t;
    typedef std::remove_pointer_t<Tp> T;
    typedef typename base_t::traits_type traits_type;
    typedef typename base_t::args_type args_type;
    typedef typename base_t::return_type return_type;

    template<typename... Args>
    usertype_function(Args&&... args): base_t(std::forward<Args>(args)...) {}

    int prelude(lua_State* L) {
        this->fx.item = detail::ptr(stack::get<T>(L, 1));
        return static_cast<base_t&>(*this)(meta::tuple_types<return_type>(), args_type(), index_value<2>(), L);
    }

    virtual int operator()(lua_State* L) override {
        return prelude(L);
    }
};

template<typename Tp, typename Function>
struct usertype_variable_function : public usertype_function_core<Tp, Function> {
    typedef usertype_function_core<Tp, Function> base_t;
    typedef std::remove_pointer_t<Tp> T;
    typedef typename base_t::fx_t fx_t;
    typedef typename base_t::traits_type traits_type;
    typedef typename base_t::args_type args_type;
    typedef typename base_t::return_type return_type;
    typedef typename fx_t::can_read can_read;
    typedef typename fx_t::can_write can_write;

    template<typename... Args>
    usertype_variable_function(Args&&... args): base_t(std::forward<Args>(args)...) {}

    int set_assignable(std::false_type, lua_State* L) {
        lua_pop(L, 3);
        return luaL_error(L, "sol: cannot write to this type: copy assignment/constructor not available");
    }

    int set_assignable(std::true_type, lua_State* L) {
        return set_writable(can_write(), L);
    }

    int set_writable(std::false_type, lua_State* L) {
        lua_pop(L, 3);
        return luaL_error(L, "sol: cannot write to readonly variable");
    }

    int set_writable(std::true_type, lua_State* L) {
        return static_cast<base_t&>(*this)(meta::tuple_types<void>(), args_type(), index_value<3>(), L);
    }

    int set_variable(std::false_type, lua_State* L) {
        lua_pop(L, 3);
        return luaL_error(L, "sol: cannot write to a const variable");
    }

    int set_variable(std::true_type, lua_State* L) {
        return set_assignable(std::is_assignable<std::add_lvalue_reference_t<return_type>, return_type>(), L);
    }

    int get_variable(std::false_type, lua_State* L) {
        lua_pop(L, 2);
        return luaL_error(L, "sol: cannot read from a readonly property");
    }

    int get_variable(std::true_type, lua_State* L) {
        return static_cast<base_t&>(*this)(meta::tuple_types<return_type>(), types<>(), index_value<2>(), L);
    }

    int prelude(lua_State* L) {
        int argcount = lua_gettop(L);
        this->fx.item = stack::get<T*>(L, 1);
        switch(argcount) {
        case 2:
            return get_variable(can_read(), L);
        case 3:
            return set_variable(meta::Not<std::is_const<return_type>>(), L);
        default:
            return luaL_error(L, "sol: cannot get/set userdata member variable with inappropriate number of arguments");
        }
    }

    virtual int operator()(lua_State* L) override {
        return prelude(L);
    }
};

struct usertype_indexing_function : base_function {
    std::string name;
    base_function* original;
    std::map<std::string, std::pair<bool, base_function*>> functions;

    template<typename... Args>
    usertype_indexing_function(std::string name, base_function* original, Args&&... args): name(std::move(name)), original(original), functions(std::forward<Args>(args)...) {}

    int prelude(lua_State* L) {
        const char* accessor = stack::get<const char*>(L, 1 - lua_gettop(L));
        auto functionpair = functions.find(accessor);
        if (functionpair != functions.end()) {
            std::pair<bool, base_function*>& target = functionpair->second;
            if (target.first) {
                stack::push<light_userdata_value>(L, target.second);
                stack::push(L, c_closure(usertype_call<0>, 1));
                return 1;
            }
            return (*target.second)(L);
       }
       base_function& core = *original;
       return core(L);
    }

    virtual int operator()(lua_State* L) override {
        return prelude(L);
    }
};
} // function_detail
} // sol

// end of sol\function_types_usertype.hpp

namespace sol {
namespace function_detail {
namespace internals {
template <typename T>
struct overload_traits : meta::bind_traits<T> {
    static const std::size_t boost = 0;
};

template <typename T, typename Func, typename X>
struct overload_traits<functor<T, Func, X>> {
    typedef typename functor<T, Func, X>::args_type args_type;
    typedef typename functor<T, Func, X>::return_type return_type;
    static const std::size_t arity = functor<T, Func, X>::arity;
    static const std::size_t boost = static_cast<std::size_t>(functor<T, Func, X>::is_free);
};

template <std::size_t... M, typename Match, typename... Args>
inline int overload_match_arity(types<>, std::index_sequence<>, std::index_sequence<M...>, Match&&, lua_State* L, int, int, Args&&...) {
    return luaL_error(L, "sol: no matching function call takes this number of arguments and the specified types");
}

template <typename Fx, typename... Fxs, std::size_t I, std::size_t... In, std::size_t... M, typename Match, typename... Args>
inline int overload_match_arity(types<Fx, Fxs...>, std::index_sequence<I, In...>, std::index_sequence<M...>, Match&& matchfx, lua_State* L, int nfxarity, int start, Args&&... args) {
    typedef overload_traits<meta::Unqualified<Fx>> traits;
    typedef meta::tuple_types<typename traits::return_type> return_types;
    typedef typename traits::args_type args_type;
    typedef typename args_type::indices args_indices;
    typedef meta::index_in<this_state, args_type> state_index;
    typedef meta::index_in<variadic_args, args_type> va_pack_index;
    static const std::size_t arity = traits::arity - static_cast<std::size_t>(state_index::value != SIZE_MAX) - static_cast<std::size_t>(va_pack_index::value != SIZE_MAX);
    int fxarity = traits::boost + nfxarity;
    // compile-time eliminate any functions that we know ahead of time are of improper arity
    if (meta::find_in_pack_v<index_value<arity>, index_value<M>...>::value) {
        return overload_match_arity(types<Fxs...>(), std::index_sequence<In...>(), std::index_sequence<M...>(), std::forward<Match>(matchfx), L, nfxarity, start, std::forward<Args>(args)...);
    }
    if (arity != fxarity) {
        return overload_match_arity(types<Fxs...>(), std::index_sequence<In...>(), std::index_sequence<arity, M...>(), std::forward<Match>(matchfx), L, nfxarity, start, std::forward<Args>(args)...);
    }
    if (!stack::stack_detail::check_types<true>().check(args_type(), args_indices(), L, start - traits::boost, no_panic)) {
        return overload_match_arity(types<Fxs...>(), std::index_sequence<In...>(), std::index_sequence<M...>(), std::forward<Match>(matchfx), L, nfxarity, start, std::forward<Args>(args)...);
    }
    return matchfx(types<Fx>(), index_value<I>(), return_types(), args_type(), L, fxarity, start, std::forward<Args>(args)...);
}
} // internals

template <typename... Functions, typename Match, typename... Args>
inline int overload_match_arity(Match&& matchfx, lua_State* L, int fxarity, int start, Args&&... args) {
    return internals::overload_match_arity(types<Functions...>(), std::make_index_sequence<sizeof...(Functions)>(), std::index_sequence<>(), std::forward<Match>(matchfx), L, fxarity,  start, std::forward<Args>(args)...);
}

template <typename... Functions, typename Match, typename... Args>
inline int overload_match(Match&& matchfx, lua_State* L, int start, Args&&... args) {
     int fxarity = lua_gettop(L) - (start - 1);
     return overload_match_arity<Functions...>(std::forward<Match>(matchfx), L, fxarity, start, std::forward<Args>(args)...);
}

template <typename... Functions>
struct overloaded_function : base_function {
    typedef std::tuple<Functions...> overload_list;
    typedef std::make_index_sequence<sizeof...(Functions)> indices;
    overload_list overloads;

    overloaded_function(overload_list set)
    : overloads(std::move(set)) {}

    overloaded_function(Functions... fxs)
    : overloads(fxs...) {

    }

    template <typename Fx, std::size_t I, typename... R, typename... Args>
    int call(types<Fx>, index_value<I>, types<R...> r, types<Args...> a, lua_State* L, int, int start) {
        auto& func = std::get<I>(overloads);
        return stack::call_into_lua<0, false>(r, a, L, start, func);
    }

    virtual int operator()(lua_State* L) override {
        auto mfx = [&](auto&&... args){ return this->call(std::forward<decltype(args)>(args)...); };
        return overload_match<Functions...>(mfx, L, 1);
    }
};

template <typename T, typename... Functions>
struct usertype_overloaded_function : base_function {
    typedef std::tuple<functor<T, std::remove_pointer_t<std::decay_t<Functions>>>...> overload_list;
    typedef std::make_index_sequence<sizeof...(Functions)> indices;
    overload_list overloads;
    
    usertype_overloaded_function(std::tuple<Functions...> set) : overloads(std::move(set)) {}

    template <typename Fx, std::size_t I, typename... R, typename... Args, meta::DisableIf<meta::Bool<Fx::is_free>> = 0>
    int call(types<Fx>, index_value<I>, types<R...> r, types<Args...> a, lua_State* L, int, int start) {
        auto& func = std::get<I>(overloads);
        func.item = detail::ptr(stack::get<T>(L, 1));
        return stack::call_into_lua<0, false>(r, a, L, start, func);
    }

    template <typename Fx, std::size_t I, typename... R, typename... Args, meta::EnableIf<meta::Bool<Fx::is_free>> = 0>
    int call(types<Fx>, index_value<I>, types<R...> r, types<Args...> a, lua_State* L, int, int start) {
        auto& func = std::get<I>(overloads);
        return stack::call_into_lua<0, false>(r, a, L, start - 1, func);
    }

    virtual int operator()(lua_State* L) override {
        auto mfx = [&](auto&&... args){ return this->call(std::forward<decltype(args)>(args)...); };
        return overload_match<functor<T, std::remove_pointer_t<std::decay_t<Functions>>>...>(mfx, L, 2);
    }
};
} // function_detail
} // sol

// end of sol\function_types_overload.hpp

namespace sol {
namespace detail {
template <typename T, typename List>
struct void_call;

template <typename T, typename... Args>
struct void_call<T, types<Args...>> {
    static void call(Args...) {}
};

template <typename T>
struct constructor_match {
    T* obj;

    constructor_match(T* obj) : obj(obj) {}

    template <typename Fx, std::size_t I, typename... R, typename... Args>
    int operator()(types<Fx>, index_value<I>, types<R...> r, types<Args...> a, lua_State* L, int, int start) const {
        default_construct func{};
        return stack::call_into_lua<0, false>(r, a, L, start, func, obj);
    }
};
} // detail

namespace function_detail {
template <typename T, typename... TypeLists, typename Match>
inline int construct(Match&& matchfx, lua_State* L, int fxarity, int start) {
    // use same overload resolution matching as all other parts of the framework
    return overload_match_arity<decltype(detail::void_call<T, TypeLists>::call)...>(std::forward<Match>(matchfx), L, fxarity, start);
}

template <typename T, typename... TypeLists>
inline int construct(lua_State* L) {
    static const auto& meta = usertype_traits<T>::metatable;
    int argcount = lua_gettop(L);
    call_syntax syntax = argcount > 0 ? stack::get_call_syntax(L, meta, 1) : call_syntax::dot;
    argcount -= static_cast<int>(syntax);

    T** pointerpointer = reinterpret_cast<T**>(lua_newuserdata(L, sizeof(T*) + sizeof(T)));
    T*& referencepointer = *pointerpointer;
    T* obj = reinterpret_cast<T*>(pointerpointer + 1);
    referencepointer = obj;
    reference userdataref(L, -1);
    userdataref.pop();
        
    construct<T, TypeLists...>(detail::constructor_match<T>(obj), L, argcount, 1 + static_cast<int>(syntax));

    userdataref.push();
    luaL_getmetatable(L, &meta[0]);
    if (stack::get<type>(L) == type::nil) {
        lua_pop(L, 1);
        return luaL_error(L, "sol: unable to get usertype metatable");
    }

    lua_setmetatable(L, -2);
    return 1;
}

template <typename T>
inline int destruct(lua_State* L) {
    T* obj = stack::get<non_null<T*>>(L, 1);
    std::allocator<T> alloc{};
    alloc.destroy(obj);
    return 0;
}

template <typename T, typename... Functions>
struct usertype_constructor_function : base_function {
    typedef std::tuple<Functions...> overload_list;
    typedef std::make_index_sequence<sizeof...(Functions)> indices;
    overload_list overloads;

    usertype_constructor_function(overload_list set) : overloads(std::move(set)) {}

    template <std::size_t... I>
    usertype_constructor_function(std::index_sequence<I...>, constructor_wrapper<Functions...> set) : usertype_constructor_function(detail::forward_get<I>(set.set)...) {}

    usertype_constructor_function(Functions... fxs) : overloads(fxs...) {}

    template <typename Fx, std::size_t I, typename... R, typename... Args>
    int call(types<Fx>, index_value<I>, types<R...> r, types<Args...> a, lua_State* L, int, int start) {
        static const auto& meta = usertype_traits<T>::metatable;
        T** pointerpointer = reinterpret_cast<T**>(lua_newuserdata(L, sizeof(T*) + sizeof(T)));
        T*& referencepointer = *pointerpointer;
        T* obj = reinterpret_cast<T*>(pointerpointer + 1);
        referencepointer = obj;
        reference userdataref(L, -1);
        userdataref.pop();

        auto& func = std::get<I>(overloads);
        stack::call_into_lua<false>(r, a, L, start, func, function_detail::implicit_wrapper<T>(obj));

        userdataref.push();
        luaL_getmetatable(L, &meta[0]);
        if (stack::get<type>(L) == type::nil) {
            lua_pop(L, 1);
            std::string err = "sol: unable to get usertype metatable for ";
            err += usertype_traits<T>::name;
            return luaL_error(L, err.c_str());
        }
        lua_setmetatable(L, -2);
            
        return 1;
    }

    virtual int operator()(lua_State* L) override {
        static const auto& meta = usertype_traits<T>::metatable;
        call_syntax syntax = stack::get_call_syntax(L, meta);
        int argcount = lua_gettop(L) - static_cast<int>(syntax);
        auto mfx = [&](auto&&... args) { return this->call(std::forward<decltype(args)>(args)...); };
        return construct<T, meta::pop_front_type_t<meta::function_args_t<Functions>>...>(mfx, L, argcount, 1 + static_cast<int>(syntax));
    }
};
} // function_detail
} // sol

// end of sol\function_types_allocator.hpp

// beginning of sol\function_types_member.hpp

namespace sol {
namespace function_detail {
template<typename Func>
struct free_function : public base_function {
    typedef meta::Unwrapped<meta::Unqualified<Func>> Function;
    typedef meta::function_return_t<Function> return_type;
    typedef meta::function_args_t<Function> args_types;
    Function fx;

    template<typename... Args>
    free_function(Args&&... args): fx(std::forward<Args>(args)...) {}

    int call(lua_State* L) {
        return stack::call_into_lua(meta::tuple_types<return_type>(), args_types(), L, 1, fx);
    }

    virtual int operator()(lua_State* L) override {
        auto f = [&](lua_State* L) -> int { return this->call(L);};
        return detail::trampoline(L, f);
    }
};

template<typename Func>
struct functor_function : public base_function {
    typedef meta::Unwrapped<meta::Unqualified<Func>> Function;
    typedef decltype(&Function::operator()) function_type;
    typedef meta::function_return_t<function_type> return_type;
    typedef meta::function_args_t<function_type> args_types;
    Function fx;

    template<typename... Args>
    functor_function(Args&&... args): fx(std::forward<Args>(args)...) {}

    int call(lua_State* L) {
        return stack::call_into_lua(meta::tuple_types<return_type>(), args_types(), L, 1, fx);
    }

    virtual int operator()(lua_State* L) override {
        auto f = [&](lua_State* L) -> int { return this->call(L);};
        return detail::trampoline(L, f);
    }
};

template<typename T, typename Function>
struct member_function : public base_function {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef meta::function_return_t<function_type> return_type;
    typedef meta::function_args_t<function_type> args_types;
    struct functor {
        function_type invocation;
        T member;
        
        template<typename F, typename... Args>
        functor(F&& f, Args&&... args): invocation(std::forward<F>(f)), member(std::forward<Args>(args)...) {}

        template<typename... Args>
        return_type operator()(Args&&... args) {
            auto& mem = detail::unwrap(detail::deref(member));
            return (mem.*invocation)(std::forward<Args>(args)...);
        }
    } fx;

    template<typename F, typename... Args>
    member_function(F&& f, Args&&... args) : fx(std::forward<F>(f), std::forward<Args>(args)...) {}

    int call(lua_State* L) {
        return stack::call_into_lua(meta::tuple_types<return_type>(), args_types(), L, 1, fx);
    }

    virtual int operator()(lua_State* L) override {
        auto f = [&](lua_State* L) -> int { return this->call(L);};
        return detail::trampoline(L, f);
    }
};

template<typename T, typename Function>
struct member_variable : public base_function {
    typedef std::remove_pointer_t<std::decay_t<Function>> function_type;
    typedef typename meta::bind_traits<function_type>::return_type return_type;
    typedef typename meta::bind_traits<function_type>::args_type args_types;
    function_type var;
    T member;
    typedef std::add_lvalue_reference_t<meta::Unwrapped<std::remove_reference_t<decltype(detail::deref(member))>>> M;

    template<typename V, typename... Args>
    member_variable(V&& v, Args&&... args): var(std::forward<V>(v)), member(std::forward<Args>(args)...) {}

    int set_assignable(std::false_type, lua_State* L, M) {
        lua_pop(L, 1);
        return luaL_error(L, "sol: cannot write to this type: copy assignment/constructor not available");
    }

    int set_assignable(std::true_type, lua_State* L, M mem) {
        (mem.*var) = stack::get<return_type>(L, 1);
        lua_pop(L, 1);
        return 0;
    }

    int set_variable(std::true_type, lua_State* L, M mem) {
        return set_assignable(std::is_assignable<std::add_lvalue_reference_t<return_type>, return_type>(), L, mem);
    }

    int set_variable(std::false_type, lua_State* L, M) {
        lua_pop(L, 1);
        return luaL_error(L, "sol: cannot write to a const variable");
    }

    int call(lua_State* L) {
        M mem = detail::unwrap(detail::deref(member));
        switch (lua_gettop(L)) {
        case 0:
            stack::push(L, (mem.*var));
            return 1;
        case 1:
            return set_variable(meta::Not<std::is_const<return_type>>(), L, mem);
        default:
            return luaL_error(L, "sol: incorrect number of arguments to member variable function");
        }
    }

    virtual int operator()(lua_State* L) override {
        auto f = [&](lua_State* L) -> int { return this->call(L);};
        return detail::trampoline(L, f);
    }
};
} // function_detail
} // sol

// end of sol\function_types_member.hpp

// beginning of sol\resolve.hpp

namespace sol {
namespace detail {
template<typename R, typename... Args, typename F, typename = std::result_of_t<meta::Unqualified<F>(Args...)>>
inline auto resolve_i(types<R(Args...)>, F&&) -> R(meta::Unqualified<F>::*)(Args...) {
    using Sig = R(Args...);
    typedef meta::Unqualified<F> Fu;
    return static_cast<Sig Fu::*>(&Fu::operator());
}

template<typename F, typename U = meta::Unqualified<F>>
inline auto resolve_f(std::true_type, F&& f)
-> decltype(resolve_i(types<meta::function_signature_t<decltype(&U::operator())>>(), std::forward<F>(f))) {
    return resolve_i(types<meta::function_signature_t<decltype(&U::operator())>>(), std::forward<F>(f));
}

template<typename F>
inline void resolve_f(std::false_type, F&&) {
    static_assert(meta::has_deducible_signature<F>::value,
                    "Cannot use no-template-parameter call with an overloaded functor: specify the signature");
}

template<typename F, typename U = meta::Unqualified<F>>
inline auto resolve_i(types<>, F&& f) -> decltype(resolve_f(meta::has_deducible_signature<U>(), std::forward<F>(f))) {
    return resolve_f(meta::has_deducible_signature<U> {}, std::forward<F>(f));
}

template<typename... Args, typename F, typename R = std::result_of_t<F&(Args...)>>
inline auto resolve_i(types<Args...>, F&& f) -> decltype( resolve_i(types<R(Args...)>(), std::forward<F>(f))) {
    return resolve_i(types<R(Args...)>(), std::forward<F>(f));
}

template<typename Sig, typename C>
inline Sig C::* resolve_v(std::false_type, Sig C::* mem_func_ptr) {
    return mem_func_ptr;
}

template<typename Sig, typename C>
inline Sig C::* resolve_v(std::true_type, Sig C::* mem_variable_ptr) {
    return mem_variable_ptr;
}
} // detail

template<typename... Args, typename R>
inline auto resolve(R fun_ptr(Args...)) -> R(*)(Args...) {
    return fun_ptr;
}

template<typename Sig>
inline Sig* resolve(Sig* fun_ptr) {
    return fun_ptr;
}

template<typename... Args, typename R, typename C>
inline auto resolve(R(C::*mem_ptr)(Args...)) -> R(C::*)(Args...) {
    return mem_ptr;
}

template<typename Sig, typename C>
inline Sig C::* resolve(Sig C::* mem_ptr) {
    return detail::resolve_v(std::is_member_object_pointer<Sig C::*>(), mem_ptr);
}

template<typename... Sig, typename F>
inline auto resolve(F&& f) -> decltype(detail::resolve_i(types<Sig...>(), std::forward<F>(f))) {
    return detail::resolve_i(types<Sig...>(), std::forward<F>(f));
}
} // sol

// end of sol\resolve.hpp

namespace sol {
template <typename Sig, typename... Ps>
struct function_arguments {
    std::tuple<Ps...> params;
    template <typename... Args>
    function_arguments(Args&&... args) : params(std::forward<Args>(args)...) {}
};

template <typename Sig = function_sig<>, typename... Args>
function_arguments<Sig, Args...> function_args( Args&&... args ) { 
    return function_arguments<Sig, Args...>(std::forward<Args>(args)...);
}

template <typename R, typename T>
auto readonly( R T::* v ) {
    typedef const R C;
    return static_cast<C T::*>( v );
}

namespace stack {
template<typename... Sigs>
struct pusher<function_sig<Sigs...>> {
    template <typename... Sig, typename Fx, typename... Args>
    static void select_convertible(std::false_type, types<Sig...>, lua_State* L, Fx&& fx, Args&&... args) {
        typedef std::remove_pointer_t<std::decay_t<Fx>> clean_fx;
        std::unique_ptr<function_detail::base_function> sptr = std::make_unique<function_detail::functor_function<clean_fx>>(std::forward<Fx>(fx), std::forward<Args>(args)...);
        set_fx(L, std::move(sptr));
    }

    template <typename R, typename... A, typename Fx, typename... Args>
    static void select_convertible(std::true_type, types<R(A...)>, lua_State* L, Fx&& fx, Args&&... args) {
        using fx_ptr_t = R(*)(A...);
        fx_ptr_t fxptr = detail::unwrap(std::forward<Fx>(fx));
        select_function(std::true_type(), L, fxptr, std::forward<Args>(args)...);
    }

    template <typename R, typename... A, typename Fx, typename... Args>
    static void select_convertible(types<R(A...)> t, lua_State* L, Fx&& fx, Args&&... args) {
        typedef std::decay_t<meta::Unwrapped<meta::Unqualified<Fx>>> raw_fx_t;
        typedef R(* fx_ptr_t)(A...);
        typedef std::is_convertible<raw_fx_t, fx_ptr_t> is_convertible;
        select_convertible(is_convertible(), t, L, std::forward<Fx>(fx), std::forward<Args>(args)...);
    }

    template <typename Fx, typename... Args>
    static void select_convertible(types<>, lua_State* L, Fx&& fx, Args&&... args) {
        typedef meta::function_signature_t<meta::Unwrapped<meta::Unqualified<Fx>>> Sig;
        select_convertible(types<Sig>(), L, std::forward<Fx>(fx), std::forward<Args>(args)...);
    }

    template <typename Fx, typename T, typename... Args>
    static void select_reference_member_variable(std::false_type, lua_State* L, Fx&& fx, T&& obj, Args&&... args) {
        typedef std::remove_pointer_t<std::decay_t<Fx>> clean_fx;
        std::unique_ptr<function_detail::base_function> sptr = std::make_unique<function_detail::member_variable<meta::Unqualified<T>, clean_fx>>(std::forward<Fx>(fx), std::forward<T>(obj), std::forward<Args>(args)... );
        set_fx(L, std::move(sptr));
    }

    template <typename Fx, typename T, typename... Args>
    static void select_reference_member_variable(std::true_type, lua_State* L, Fx&& fx, T&& obj, Args&&... args) {
        typedef std::decay_t<Fx> dFx;
        dFx memfxptr(std::forward<Fx>(fx));
        auto userptr = detail::ptr(std::forward<T>(obj), std::forward<Args>(args)...);
        lua_CFunction freefunc = &function_detail::upvalue_member_variable<std::decay_t<decltype(*userptr)>, meta::Unqualified<Fx>>::call;

        int upvalues = stack::stack_detail::push_as_upvalues(L, memfxptr);
        upvalues += stack::push(L, light_userdata_value(static_cast<void*>(userptr)));        
        stack::push(L, c_closure(freefunc, upvalues));
    }

    template <typename Fx, typename... Args>
    static void select_member_variable(std::false_type, lua_State* L, Fx&& fx, Args&&... args) {
        select_convertible(types<Sigs...>(), L, std::forward<Fx>(fx), std::forward<Args>(args)...);
    }

    template <typename Fx, typename T, typename... Args>
    static void select_member_variable(std::true_type, lua_State* L, Fx&& fx, T&& obj, Args&&... args) {
        typedef meta::Bool<meta::is_specialization_of<std::reference_wrapper, meta::Unqualified<T>>::value || std::is_pointer<T>::value> is_reference;
        select_reference_member_variable(is_reference(), L, std::forward<Fx>(fx), std::forward<T>(obj), std::forward<Args>(args)...);
    }

    template <typename Fx>
    static void select_member_variable(std::true_type, lua_State* L, Fx&& fx) {
        typedef typename meta::bind_traits<meta::Unqualified<Fx>>::object_type C;
        lua_CFunction freefunc = &function_detail::upvalue_this_member_variable<C, Fx>::call;
        int upvalues = stack::stack_detail::push_as_upvalues(L, fx);
        stack::push(L, c_closure(freefunc, upvalues));
    }

    template <typename Fx, typename T, typename... Args>
    static void select_reference_member_function(std::false_type, lua_State* L, Fx&& fx, T&& obj, Args&&... args) {
        typedef std::remove_pointer_t<std::decay_t<Fx>> clean_fx;
        std::unique_ptr<function_detail::base_function> sptr = std::make_unique<function_detail::member_function<meta::Unwrapped<meta::Unqualified<T>>, clean_fx>>(std::forward<Fx>(fx), std::forward<T>(obj), std::forward<Args>(args)... );
        set_fx(L, std::move(sptr));
    }

    template <typename Fx, typename T, typename... Args>
    static void select_reference_member_function(std::true_type, lua_State* L, Fx&& fx, T&& obj, Args&&... args) {
        typedef std::decay_t<Fx> dFx;
        dFx memfxptr(std::forward<Fx>(fx));
        auto userptr = detail::ptr(std::forward<T>(obj), std::forward<Args>(args)...);
        lua_CFunction freefunc = &function_detail::upvalue_member_function<std::decay_t<decltype(*userptr)>, meta::Unqualified<Fx>>::call;

        int upvalues = stack::stack_detail::push_as_upvalues(L, memfxptr);
        upvalues += stack::push(L, light_userdata_value(static_cast<void*>(userptr)));        
        stack::push(L, c_closure(freefunc, upvalues));
    }

    template <typename Fx, typename... Args>
    static void select_member_function(std::false_type, lua_State* L, Fx&& fx, Args&&... args) {
        select_member_variable(std::is_member_object_pointer<meta::Unqualified<Fx>>(), L, std::forward<Fx>(fx), std::forward<Args>(args)...);
    }

    template <typename Fx, typename T, typename... Args>
    static void select_member_function(std::true_type, lua_State* L, Fx&& fx, T&& obj, Args&&... args) {
        typedef meta::Bool<meta::is_specialization_of<std::reference_wrapper, meta::Unqualified<T>>::value || std::is_pointer<T>::value> is_reference;
        select_reference_member_function(is_reference(), L, std::forward<Fx>(fx), std::forward<T>(obj), std::forward<Args>(args)...);
    }

    template <typename Fx>
    static void select_member_function(std::true_type, lua_State* L, Fx&& fx) {
        typedef typename meta::bind_traits<meta::Unqualified<Fx>>::object_type C;
        lua_CFunction freefunc = &function_detail::upvalue_this_member_function<C, Fx>::call;
        int upvalues = stack::stack_detail::push_as_upvalues(L, fx);
        stack::push(L, c_closure(freefunc, upvalues));
    }

    template <typename Fx, typename... Args>
    static void select_function(std::false_type, lua_State* L, Fx&& fx, Args&&... args) {
        select_member_function(std::is_member_function_pointer<meta::Unqualified<Fx>>(), L, std::forward<Fx>(fx), std::forward<Args>(args)...);
    }

    template <typename Fx, typename... Args>
    static void select_function(std::true_type, lua_State* L, Fx&& fx, Args&&... args) {
        std::decay_t<Fx> target(std::forward<Fx>(fx), std::forward<Args>(args)...);
        lua_CFunction freefunc = &function_detail::upvalue_free_function<Fx>::call;

        int upvalues = stack::stack_detail::push_as_upvalues(L, target);
        stack::push(L, freefunc, upvalues);
    }

    static void select_function(std::true_type, lua_State* L, lua_CFunction f) {
        stack::push(L, f);
    }

    template <typename Fx, typename... Args>
    static void select(lua_State* L, Fx&& fx, Args&&... args) {
        select_function(std::is_function<meta::Unqualified<Fx>>(), L, std::forward<Fx>(fx), std::forward<Args>(args)...);
    }

    static void set_fx(lua_State* L, std::unique_ptr<function_detail::base_function> luafunc) {
        function_detail::base_function* target = luafunc.release();
        void* targetdata = static_cast<void*>(target);
        lua_CFunction freefunc = function_detail::call;

        stack::push(L, userdata_value(targetdata));
        function_detail::free_function_cleanup(L);
        lua_setmetatable(L, -2);
        stack::push(L, freefunc, 1);
    }

    template<typename... Args>
    static int push(lua_State* L, Args&&... args) {
        // Set will always place one thing (function) on the stack
        select(L, std::forward<Args>(args)...);
        return 1;
    }
};

template<typename T, typename... Args>
struct pusher<function_arguments<T, Args...>> {
    template <std::size_t... I, typename FP>
    static int push_func(std::index_sequence<I...>, lua_State* L, FP&& fp) {
        return stack::push<T>(L, detail::forward_get<I>(fp.params)...);
    }

    template <typename FP>
    static int push(lua_State* L, FP&& fp) {
        return push_func(std::make_index_sequence<sizeof...(Args)>(), L, std::forward<FP>(fp));
    }
};

template<typename Signature>
struct pusher<std::function<Signature>> {
    static int push(lua_State* L, std::function<Signature> fx) {
        return pusher<function_sig<Signature>>{}.push(L, std::move(fx));
    }
};

template<typename Signature>
struct pusher<Signature, std::enable_if_t<std::is_member_pointer<Signature>::value>> {
    template <typename F>
    static int push(lua_State* L, F&& f) {
        return pusher<function_sig<>>{}.push(L, std::forward<F>(f));
    }
};

template<typename Signature>
struct pusher<Signature, std::enable_if_t<meta::And<std::is_function<Signature>, meta::Not<std::is_same<Signature, lua_CFunction>>, meta::Not<std::is_same<Signature, std::remove_pointer_t<lua_CFunction>>>>::value>> {
    template <typename F>
    static int push(lua_State* L, F&& f) {
        return pusher<function_sig<>>{}.push(L, std::forward<F>(f));
    }
};

template<typename... Functions>
struct pusher<overload_set<Functions...>> {
    static int push(lua_State* L, overload_set<Functions...>&& set) {
        pusher<function_sig<>>{}.set_fx(L, std::make_unique<function_detail::overloaded_function<Functions...>>(std::move(set.set)));
        return 1;
    }

    static int push(lua_State* L, const overload_set<Functions...>& set) {
        pusher<function_sig<>>{}.set_fx(L, std::make_unique<function_detail::overloaded_function<Functions...>>(set.set));
        return 1;
    }
};

} // stack
} // sol

// end of sol\function_types.hpp

namespace sol {
template <typename base_t>
class basic_function : public base_t {
private:
    void luacall( std::ptrdiff_t argcount, std::ptrdiff_t resultcount ) const {
        lua_callk( base_t::lua_state( ), static_cast<int>( argcount ), static_cast<int>( resultcount ), 0, nullptr );
    }

    template<std::size_t... I, typename... Ret>
    auto invoke( types<Ret...>, std::index_sequence<I...>, std::ptrdiff_t n ) const {
        luacall( n, sizeof...( Ret ) );
        return stack::pop<std::tuple<Ret...>>( base_t::lua_state( ) );
    }

    template<std::size_t I, typename Ret>
    Ret invoke(types<Ret>, std::index_sequence<I>, std::ptrdiff_t n ) const {
        luacall( n, 1 );
        return stack::pop<Ret>( base_t::lua_state( ) );
    }

    template <std::size_t I>
    void invoke(types<void>, std::index_sequence<I>, std::ptrdiff_t n) const {
        luacall( n, 0 );
    }

    function_result invoke(types<>, std::index_sequence<>, std::ptrdiff_t n ) const {
        int stacksize = lua_gettop( base_t::lua_state( ) );
        int firstreturn = std::max( 1, stacksize - static_cast<int>( n ) );
        luacall(n, LUA_MULTRET);
        int poststacksize = lua_gettop( base_t::lua_state( ) );
        int returncount = poststacksize - (firstreturn - 1);
        return function_result( base_t::lua_state( ), firstreturn, returncount );
    }

public:
    basic_function() = default;
    basic_function(const basic_function&) = default;
    basic_function& operator=(const basic_function&) = default;
    basic_function(basic_function&& ) = default;
    basic_function& operator=(basic_function&& ) = default;
    basic_function(lua_State* L, int index = -1): base_t(L, index) {
#ifdef SOL_CHECK_ARGUMENTS
        stack::check<basic_function>(L, index, type_panic);
#endif // Safety
    }
    
    template<typename... Args>
    function_result operator()( Args&&... args ) const {
        return call<>( std::forward<Args>( args )... );
    }

    template<typename... Ret, typename... Args>
    decltype(auto) operator()( types<Ret...>, Args&&... args ) const {
        return call<Ret...>( std::forward<Args>( args )... );
    }

    template<typename... Ret, typename... Args>
    decltype(auto) call( Args&&... args ) const {
        base_t::push( );
        int pushcount = stack::multi_push_reference( base_t::lua_state( ), std::forward<Args>( args )... );
        return invoke( types<Ret...>( ), std::make_index_sequence<sizeof...(Ret)>(), pushcount );
    }
};

namespace stack {
template<typename Signature>
struct getter<std::function<Signature>> {
    typedef meta::bind_traits<Signature> fx_t;
    typedef typename fx_t::args_type args_types;
    typedef meta::tuple_types<typename fx_t::return_type> return_types;

    template<typename... Args, typename... Ret>
    static std::function<Signature> get_std_func(types<Ret...>, types<Args...>, lua_State* L, int index = -1) {
        sol::function f(L, index);
        auto fx = [f, L, index](Args&&... args) -> meta::return_type_t<Ret...> {
            return f.call<Ret...>(std::forward<Args>(args)...);
        };
        return std::move(fx);
    }

    template<typename... FxArgs>
    static std::function<Signature> get_std_func(types<void>, types<FxArgs...>, lua_State* L, int index = -1) {
        sol::function f(L, index);
        auto fx = [f, L, index](FxArgs&&... args) -> void {
            f(std::forward<FxArgs>(args)...);
        };
        return std::move(fx);
    }

    template<typename... FxArgs>
    static std::function<Signature> get_std_func(types<>, types<FxArgs...> t, lua_State* L, int index = -1) {
        return get_std_func(types<void>(), t, L, index);
    }

    static std::function<Signature> get(lua_State* L, int index) {
        return get_std_func(return_types(), args_types(), L, index);
    }
};
} // stack
} // sol

// end of sol\function.hpp

// beginning of sol\protected_function.hpp

// beginning of sol\protected_function_result.hpp

namespace sol {
struct protected_function_result : public proxy_base<protected_function_result> {
private:
    lua_State* L;
    int index;
    int returncount;
    int popcount;
    call_status err;

    template <typename T>
    decltype(auto) tagged_get( types<sol::optional<T>> ) const {
        if (!valid()) {
            return sol::optional<T>(nullopt);
        }
        return stack::get<sol::optional<T>>(L, index);
    }

    template <typename T>
    decltype(auto) tagged_get( types<T> ) const {
#ifdef SOL_CHECK_ARGUMENTS
        if (!valid()) {
            type_panic(L, index, type_of(L, index), type::none);
        }
#endif // Check Argument Safety
        return stack::get<T>(L, index);
    }

    sol::optional<sol::error> tagged_get( types<sol::optional<sol::error>> ) const {
        if (valid()) {
            return nullopt;
        }
        return sol::error(detail::direct_error, stack::get<std::string>(L, index));
    }

    sol::error tagged_get( types<sol::error> ) const {
#ifdef SOL_CHECK_ARGUMENTS
        if (valid()) {
            type_panic(L, index, type_of(L, index), type::none);
        }
#endif // Check Argument Safety
        return sol::error(detail::direct_error, stack::get<std::string>(L, index));
    }

public:
    protected_function_result() = default;
    protected_function_result(lua_State* L, int index = -1, int returncount = 0, int popcount = 0, call_status err = call_status::ok): L(L), index(index), returncount(returncount), popcount(popcount), err(err) {
        
    }
    protected_function_result(const protected_function_result&) = default;
    protected_function_result& operator=(const protected_function_result&) = default;
    protected_function_result(protected_function_result&& o) : L(o.L), index(o.index), returncount(o.returncount), popcount(o.popcount), err(o.err) {
        // Must be manual, otherwise destructor will screw us
        // return count being 0 is enough to keep things clean
        // but will be thorough
        o.L = nullptr;
        o.index = 0;
        o.returncount = 0;
        o.popcount = 0;
        o.err = call_status::runtime;
    }
    protected_function_result& operator=(protected_function_result&& o) {
        L = o.L;
        index = o.index;
        returncount = o.returncount;
        popcount = o.popcount;
        err = o.err;
        // Must be manual, otherwise destructor will screw us
        // return count being 0 is enough to keep things clean
        // but will be thorough
        o.L = nullptr;
        o.index = 0;
        o.returncount = 0;
        o.popcount = 0;
        o.err = call_status::runtime;
        return *this;
    }

    call_status error() const {
        return err;
    }

    bool valid() const {
        return error() == call_status::ok || error() == call_status::yielded;
    }

    template<typename T>
    T get() const {
        return tagged_get(types<meta::Unqualified<T>>());
    }

    lua_State* lua_state() const { return L; };
    int stack_index() const { return index; };

    ~protected_function_result() {
        stack::remove(L, index, popcount);
    }
};
} // sol

// end of sol\protected_function_result.hpp

namespace sol {
template <typename base_t>
class basic_protected_function : public base_t {
private:
    static reference& handler_storage() {
        static sol::reference h;
        return h;
    }

public:
    static const reference& get_default_handler () {
        return handler_storage();
    }

    static void set_default_handler( reference& ref ) {
        handler_storage() = ref;
    }

private:
    struct handler {
        const reference& target;
        int stackindex;
        handler(const reference& target) : target(target), stackindex(0) {
            if (target.valid()) {
                stackindex = lua_gettop(target.lua_state()) + 1;
                target.push();
            }
        }
        ~handler() {
            if (stackindex > 0) {
                lua_remove(target.lua_state(), stackindex);
            }
        }
    };

    int luacall(std::ptrdiff_t argcount, std::ptrdiff_t resultcount, handler& h) const {
        return lua_pcallk(base_t::lua_state(), static_cast<int>(argcount), static_cast<int>(resultcount), h.stackindex, 0, nullptr);
    }

    template<std::size_t... I, typename... Ret>
    auto invoke(types<Ret...>, std::index_sequence<I...>, std::ptrdiff_t n, handler& h) const {
        luacall(n, sizeof...(Ret), h);
        return stack::pop<std::tuple<Ret...>>(base_t::lua_state());
    }

    template<std::size_t I, typename Ret>
    Ret invoke(types<Ret>, std::index_sequence<I>, std::ptrdiff_t n, handler& h) const {
        luacall(n, 1, h);
        return stack::pop<Ret>(base_t::lua_state());
    }

    template <std::size_t I>
    void invoke(types<void>, std::index_sequence<I>, std::ptrdiff_t n, handler& h) const {
        luacall(n, 0, h);
    }

    protected_function_result invoke(types<>, std::index_sequence<>, std::ptrdiff_t n, handler& h) const {
        int stacksize = lua_gettop(base_t::lua_state());
        int firstreturn = std::max(1, stacksize - static_cast<int>(n) - 1);
        int returncount = 0;
        call_status code = call_status::ok;
#ifndef SOL_NO_EXCEPTIONS
        auto onexcept = [&](const char* error) {
            h.stackindex = 0;
            if (h.target.valid()) {
                h.target.push();
                stack::push(base_t::lua_state(), error);
                lua_call(base_t::lua_state(), 1, 1);
            }
            else {
                stack::push(base_t::lua_state(), error);
            }
        };
        try {
#endif // No Exceptions
            code = static_cast<call_status>(luacall(n, LUA_MULTRET, h));
            int poststacksize = lua_gettop(base_t::lua_state());
            returncount = poststacksize - (stacksize - 1);
#ifndef SOL_NO_EXCEPTIONS
        }
        // Handle C++ errors thrown from C++ functions bound inside of lua
        catch (const char* error) {
            onexcept(error);
            firstreturn = lua_gettop(base_t::lua_state());
            return protected_function_result(base_t::lua_state(), firstreturn, 0, 1, call_status::runtime);
        }
        catch (const std::exception& error) {
            onexcept(error.what());
            firstreturn = lua_gettop(base_t::lua_state());
            return protected_function_result(base_t::lua_state(), firstreturn, 0, 1, call_status::runtime);
        }
        catch (...) {
            onexcept("caught (...) unknown error during protected_function call");
            firstreturn = lua_gettop(base_t::lua_state());
            return protected_function_result(base_t::lua_state(), firstreturn, 0, 1, call_status::runtime);
        }
#endif // No Exceptions
        return protected_function_result(base_t::lua_state(), firstreturn, returncount, returncount, code);
    }

public:
    reference error_handler;

    basic_protected_function() = default;
    basic_protected_function(const basic_protected_function&) = default;
    basic_protected_function& operator=(const basic_protected_function&) = default;
    basic_protected_function(basic_protected_function&& ) = default;
    basic_protected_function& operator=(basic_protected_function&& ) = default;
    basic_protected_function(const basic_function<base_t>& b) : base_t(b) {}
    basic_protected_function(basic_function<base_t>&& b) : base_t(std::move(b)) {}
    basic_protected_function(lua_State* L, int index = -1) : base_t(L, index), error_handler(get_default_handler()) {
#ifdef SOL_CHECK_ARGUMENTS
        stack::check<basic_protected_function>(L, index, type_panic);
#endif // Safety
    }

    template<typename... Args>
    protected_function_result operator()(Args&&... args) const {
        return call<>(std::forward<Args>(args)...);
    }

    template<typename... Ret, typename... Args>
    decltype(auto) operator()(types<Ret...>, Args&&... args) const {
        return call<Ret...>(std::forward<Args>(args)...);
    }

    template<typename... Ret, typename... Args>
    decltype(auto) call(Args&&... args) const {
        handler h(error_handler);
        base_t::push();
        int pushcount = stack::multi_push_reference(base_t::lua_state(), std::forward<Args>(args)...);
        return invoke(types<Ret...>(), std::make_index_sequence<sizeof...(Ret)>(), pushcount, h);
    }
};
} // sol

// end of sol\protected_function.hpp

namespace sol {
struct stack_proxy : public proxy_base<stack_proxy> {
private:
    lua_State* L;
    int index;

public:
    stack_proxy() : L(nullptr), index(0){}
    stack_proxy(lua_State* L, int index) : L(L), index(index) {}

    template<typename T>
    decltype(auto) get() const {
        return stack::get<T>(L, stack_index());
    }

    int push () const {
        lua_pushvalue(L, index);
        return 1;
    }

    lua_State* lua_state() const { return L; }
    int stack_index() const { return index; }

    template<typename... Ret, typename... Args>
    decltype(auto) call(Args&&... args) {
        return get<function>().template call<Ret...>(std::forward<Args>(args)...);
    }

    template<typename... Args>
    decltype(auto) operator()(Args&&... args) {
        return call<>(std::forward<Args>(args)...);
    }
};

namespace stack {
template <>
struct getter<stack_proxy> {
    static stack_proxy get(lua_State* L, int index = -1) {
        return stack_proxy(L, index);
    }
};

template <>
struct pusher<stack_proxy> {
    static int push(lua_State*, const stack_proxy& ref) {
        return ref.push();
    }
};
} // stack

template <>
struct tie_size<function_result> : std::integral_constant<std::size_t, SIZE_MAX> {};

template <std::size_t I>
stack_proxy get(const function_result& fr) {
    return stack_proxy(fr.lua_state(), static_cast<int>(fr.stack_index() + I));
}

template <>
struct tie_size<protected_function_result> : std::integral_constant<std::size_t, SIZE_MAX> {};

template <std::size_t I>
stack_proxy get(const protected_function_result& fr) {
    return stack_proxy(fr.lua_state(), static_cast<int>(fr.stack_index() + I));
}
} // sol

// end of sol\stack_proxy.hpp

#include <limits>
#include <iterator>

namespace sol {
    template <bool is_const>
    struct va_iterator : std::iterator<std::random_access_iterator_tag, std::conditional_t<is_const, const stack_proxy, stack_proxy>, std::ptrdiff_t, std::conditional_t<is_const, const stack_proxy*, stack_proxy*>, std::conditional_t<is_const, const stack_proxy, stack_proxy>> {
        typedef std::iterator<std::random_access_iterator_tag, std::conditional_t<is_const, const stack_proxy, stack_proxy>, std::ptrdiff_t, std::conditional_t<is_const, const stack_proxy*, stack_proxy*>, std::conditional_t<is_const, const stack_proxy, stack_proxy>> base_t;
        typedef typename base_t::reference reference;
        typedef typename base_t::pointer pointer;
        typedef typename base_t::value_type value_type;
        typedef typename base_t::difference_type difference_type;
        typedef typename base_t::iterator_category iterator_category;
        lua_State* L;
        int index;
        int stacktop;
        stack_proxy sp;

        va_iterator() : L(nullptr), index(std::numeric_limits<int>::max()), stacktop(std::numeric_limits<int>::max()) {}
        va_iterator(lua_State* L, int index, int stacktop) : L(L), index(index), stacktop(stacktop), sp(L, index) {}

        reference operator*() {
            return stack_proxy(L, index);
        }

        pointer operator->() {
            sp = stack_proxy(L, index);
            return &sp;
        }

        va_iterator& operator++ () {
            ++index;
            return *this;
        }

        va_iterator operator++ (int) {
            auto r = *this;
            this->operator ++();
            return r;
        }

        va_iterator& operator-- () {
            --index;
            return *this;
        }

        va_iterator operator-- (int) {
            auto r = *this;
            this->operator --();
            return r;
        }

        va_iterator& operator+= (difference_type idx) {
            index += static_cast<int>(idx);
            return *this;
        }

        va_iterator& operator-= (difference_type idx) {
            index -= static_cast<int>(idx);
            return *this;
        }

        difference_type operator- (const va_iterator& r) const {
            return index - r.index;
        }

        va_iterator operator+ (difference_type idx) const {
            va_iterator r = *this;
            r += idx;
            return r;
        }

        reference operator[](difference_type idx) {
            return stack_proxy(L, index + static_cast<int>(idx));
        }

        bool operator==(const va_iterator& r) const {
            if (stacktop == std::numeric_limits<int>::max()) {
                return r.index == r.stacktop;
            }
            else if (r.stacktop == std::numeric_limits<int>::max()) {
                return index == stacktop;
            }
            return index == r.index;
        }

        bool operator != (const va_iterator& r) const {
            return !(this->operator==(r));
        }

        bool operator < (const va_iterator& r) const {
            return index < r.index;
        }

        bool operator > (const va_iterator& r) const {
            return index > r.index;
        }

        bool operator <= (const va_iterator& r) const {
            return index <= r.index;
        }

        bool operator >= (const va_iterator& r) const {
            return index >= r.index;
        }
    };
    
    template <bool is_const>
    inline va_iterator<is_const> operator+(typename va_iterator<is_const>::difference_type n, const va_iterator<is_const>& r) {
        return r + n;
    }

struct variadic_args {
private:
    lua_State* L;
    int index;
    int stacktop;

public:
    typedef stack_proxy reference_type;
    typedef stack_proxy value_type;
    typedef stack_proxy* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;
    typedef va_iterator<false> iterator;
    typedef va_iterator<true> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    variadic_args() = default;
    variadic_args(lua_State* L, int index = -1): L(L), index(lua_absindex(L, index)), stacktop(lua_gettop(L)) {}
    variadic_args(const variadic_args&) = default;
    variadic_args& operator=(const variadic_args&) = default;
    variadic_args(variadic_args&& o) : L(o.L), index(o.index), stacktop(o.stacktop) {
        // Must be manual, otherwise destructor will screw us
        // return count being 0 is enough to keep things clean
        // but will be thorough
        o.L = nullptr;
        o.index = 0;
        o.stacktop = 0;
    }
    variadic_args& operator=(variadic_args&& o) {
        L = o.L;
        index = o.index;
        stacktop = o.stacktop;
        // Must be manual, otherwise destructor will screw us
        // return count being 0 is enough to keep things clean
        // but will be thorough
        o.L = nullptr;
        o.index = 0;
        o.stacktop = 0;
        return *this;
    }

    iterator begin() { return iterator(L, index, stacktop + 1); }
    iterator end() { return iterator(L, stacktop + 1, stacktop + 1); }
    const_iterator begin() const { return const_iterator(L, index, stacktop + 1); }
    const_iterator end() const { return const_iterator(L, stacktop + 1, stacktop + 1); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    reverse_iterator rbegin() { return std::reverse_iterator<iterator>(begin()); }
    reverse_iterator rend() { return std::reverse_iterator<iterator>(end()); }
    const_reverse_iterator rbegin() const { return std::reverse_iterator<const_iterator>(begin()); }
    const_reverse_iterator rend() const { return std::reverse_iterator<const_iterator>(end()); }
    const_reverse_iterator crbegin() const { return std::reverse_iterator<const_iterator>(cbegin()); }
    const_reverse_iterator crend() const { return std::reverse_iterator<const_iterator>(cend()); }

    int push () const {
        int pushcount = 0;
        for (int i = index; i <= stacktop; ++i) {
            lua_pushvalue(L, i);
            pushcount += 1;
        }
        return pushcount;
    }

    template<typename T>
    decltype(auto) get(difference_type start = 0) const {
        return stack::get<T>(L, index + static_cast<int>(start));
    }

    stack_proxy operator[](difference_type start) const {
        return stack_proxy(L, index + static_cast<int>(start));
    }

    lua_State* lua_state() const { return L; };
    int stack_index() const { return index; };
    int leftover_count() const { return stacktop - (index - 1); }
    int top() const { return stacktop; }
};

namespace stack {
template <>
struct getter<variadic_args> {
    static variadic_args get(lua_State* L, int index = -1) {
        return variadic_args(L, index);
    }
};

template <>
struct pusher<variadic_args> {
    static int push(lua_State*, const variadic_args& ref) {
        return ref.push();
    }
};
} // stack
} // sol

// end of sol\variadic_args.hpp

namespace sol {
template <typename base_t>
class basic_object : public base_t {
private:
    template<typename T>
    decltype(auto) as_stack(std::true_type) const {
        return stack::get<T>(lua_state(), base_t::stack_index());
    }

    template<typename T>
    decltype(auto) as_stack(std::false_type) const {
        base_t::push();
        return stack::pop<T>(lua_state());
    }

    template<typename T>
    bool is_stack(std::true_type) const {
        return stack::check<T>(lua_state(), base_t::stack_index(), no_panic);
    }

    template<typename T>
    bool is_stack(std::false_type) const {
        auto pp = stack::push_pop(*this);
        return stack::check<T>(lua_state(), -1, no_panic);
    }

public:
    using base_t::base_t;
    using base_t::lua_state;

    template<typename T>
    decltype(auto) as() const {
        return as_stack<T>( std::is_same<base_t, stack_reference>() );
    }

    template<typename T>
    bool is() const {
        if (!base_t::valid())
            return false;
        return is_stack<T>(std::is_same<base_t, stack_reference>());
    }
};

template <typename T>
object make_object(lua_State* L, T&& value) {
    int backpedal = stack::push(L, std::forward<T>(value));
    object r = stack::get<object>(L, -backpedal);
    lua_pop(L, backpedal);
    return r;
}

template <typename T, typename... Args>
object make_object(lua_State* L, Args&&... args) {
    int backpedal = stack::push<T>(L, std::forward<Args>(args)...);
    object r = stack::get<sol::object>(L, -backpedal);
    lua_pop(L, backpedal);
    return r;
}

inline bool operator==(const object& lhs, const nil_t&) {
    return !lhs.valid();
}

inline bool operator==(const nil_t&, const object& rhs) {
    return !rhs.valid();
}

inline bool operator!=(const object& lhs, const nil_t&) {
    return lhs.valid();
}

inline bool operator!=(const nil_t&, const object& rhs) {
    return rhs.valid();
}
} // sol

// end of sol\object.hpp

namespace sol {
template<typename Table, typename Key>
struct proxy : public proxy_base<proxy<Table, Key>> {
private:
    typedef meta::If<meta::is_specialization_of<std::tuple, Key>, Key, std::tuple<meta::If<std::is_array<meta::Unqualified<Key>>, Key&, meta::Unqualified<Key>>>> key_type;

    template<typename T, std::size_t... I>
    decltype(auto) tuple_get(std::index_sequence<I...>) const {
        return tbl.template traverse_get<T>(std::get<I>(key)...);
    }

    template<std::size_t... I, typename T>
    void tuple_set(std::index_sequence<I...>, T&& value) const {
        tbl.traverse_set(std::get<I>(key)..., std::forward<T>(value));
    }

public:
    Table tbl;
    key_type key;

    template<typename T>
    proxy(Table table, T&& key) : tbl(table), key(std::forward<T>(key)) {}

    template<typename T>
    proxy& set(T&& item) {
        tuple_set( std::make_index_sequence<std::tuple_size<meta::Unqualified<key_type>>::value>(), std::forward<T>(item) );
        return *this;
    }

    template<typename... Args>
    proxy& set_function(Args&&... args) {
        tbl.set_function(key, std::forward<Args>(args)...);
        return *this;
    }

    template<typename U, meta::EnableIf<meta::Function<meta::Unqualified<U>>> = 0>
    proxy& operator=(U&& other) {
        return set_function(std::forward<U>(other));
    }

    template<typename U, meta::DisableIf<meta::Function<meta::Unqualified<U>>> = 0>
    proxy& operator=(U&& other) {
        return set(std::forward<U>(other));
    }

    template<typename T>
    decltype(auto) get() const {
        return tuple_get<T>( std::make_index_sequence<std::tuple_size<meta::Unqualified<key_type>>::value>() );
    }

    template<typename T>
    decltype(auto) get_or(T&& otherwise) const {
        typedef decltype(get<T>()) U;
        sol::optional<U> option = get<sol::optional<U>>();
        if (option) {
            return static_cast<U>(option.value());
        }
        return static_cast<U>(std::forward<T>(otherwise));
    }

    template<typename T, typename D>
    decltype(auto) get_or(D&& otherwise) const {
        sol::optional<T> option = get<sol::optional<T>>();
        if (option) {
            return static_cast<T>(option.value());
        }
        return static_cast<T>(std::forward<D>(otherwise));
    }

    template <typename K>
    decltype(auto) operator[](K&& k) const {
        auto keys = meta::tuplefy(key, std::forward<K>(k));
        return proxy<Table, decltype(keys)>(tbl, std::move(keys));
    }

    template<typename... Ret, typename... Args>
    decltype(auto) call(Args&&... args) {
        return get<function>().template call<Ret...>(std::forward<Args>(args)...);
    }

    template<typename... Args>
    decltype(auto) operator()(Args&&... args) {
        return call<>(std::forward<Args>(args)...);
    }

    bool valid () const {
        stack::push_pop(tbl);
        auto p = stack::probe_get_field<std::is_same<meta::Unqualified<Table>, global_table>::value>(tbl.lua_state(), key, lua_gettop(tbl.lua_state()));
        lua_pop(tbl.lua_state(), p.levels);
        return p;
    }
};

template<typename Table, typename Key, typename T>
inline bool operator==(T&& left, const proxy<Table, Key>& right) {
    return left == right.template get<std::decay_t<T>>();
}

template<typename Table, typename Key, typename T>
inline bool operator==(const proxy<Table, Key>& right, T&& left) {
    return right.template get<std::decay_t<T>>() == left;
}

template<typename Table, typename Key, typename T>
inline bool operator!=(T&& left, const proxy<Table, Key>& right) {
    return right.template get<std::decay_t<T>>() != left;
}

template<typename Table, typename Key, typename T>
inline bool operator!=(const proxy<Table, Key>& right, T&& left) {
    return right.template get<std::decay_t<T>>() != left;
}

template<typename Table, typename Key>
inline bool operator==(nil_t, const proxy<Table, Key>& right) {
    return !right.valid();
}

template<typename Table, typename Key>
inline bool operator==(const proxy<Table, Key>& right, nil_t) {
    return !right.valid();
}

template<typename Table, typename Key>
inline bool operator!=(nil_t, const proxy<Table, Key>& right) {
    return right.valid();
}

template<typename Table, typename Key>
inline bool operator!=(const proxy<Table, Key>& right, nil_t) {
    return right.valid();
}

namespace stack {
template <typename Table, typename Key>
struct pusher<proxy<Table, Key>> {
    static int push (lua_State*, const proxy<Table, Key>& p) {
        sol::reference r = p;
        return r.push();
    }
};
} // stack
} // sol

// end of sol\proxy.hpp

// beginning of sol\usertype.hpp

// beginning of sol\deprecate.hpp

#ifndef SOL_DEPRECATED
    #ifdef _MSC_VER
        #define SOL_DEPRECATED __declspec(deprecated)
    #elif __GNUC__
        #define SOL_DEPRECATED __attribute__((deprecated)) 
    #else
        #define SOL_DEPRECATED [[deprecated]]
    #endif // compilers
#endif // SOL_DEPRECATED

namespace sol {
namespace detail {
   template <typename T>
   struct SOL_DEPRECATED deprecate_type {
       using type = T;
   };
} // detail
} // sol

// end of sol\deprecate.hpp

#include <vector>
#include <algorithm>

namespace sol {
namespace usertype_detail {
struct add_destructor_tag {};
struct check_destructor_tag {};
struct verified_tag {} const verified {};

template <typename T>
struct is_constructor : std::false_type {};

template <typename... Args>
struct is_constructor<constructors<Args...>> : std::true_type {};

template <typename... Args>
struct is_constructor<constructor_wrapper<Args...>> : std::true_type {};

template <>
struct is_constructor<no_construction> : std::true_type {};

template <typename... Args>
using has_constructor = meta::Or<is_constructor<meta::Unqualified<Args>>...>;

template <typename T>
struct is_destructor : std::false_type {};

template <typename Fx>
struct is_destructor<destructor_wrapper<Fx>> : std::true_type {};

template <typename... Args>
using has_destructor = meta::Or<is_destructor<meta::Unqualified<Args>>...>;

enum class stage {
    normalmeta,
    refmeta,
    uniquemeta,
};

template<bool releasemem = false, typename TCont>
inline int push_upvalues(lua_State* L, TCont&& cont) {
    int n = 0;
    for(auto& c : cont) {
        if(releasemem) {
            stack::push<light_userdata_value>(L, c.release());
        }
        else {
            stack::push<light_userdata_value>(L, c.get());
        }
        ++n;
    }
    return n;
}

template<typename T, stage metastage>
inline void push_metatable(lua_State* L, bool needsindexfunction, std::vector<std::unique_ptr<function_detail::base_function>>& funcs, std::vector<luaL_Reg>& functable, std::vector<luaL_Reg>& metafunctable, void* baseclasscheck, void* baseclasscast) {
    static const auto& gcname = meta_function_names[static_cast<int>(meta_function::garbage_collect)];
    luaL_newmetatable(L, &usertype_traits<T>::metatable[0]);
    int metatableindex = lua_gettop(L);
    if (baseclasscheck != nullptr) {
        stack::push(L, light_userdata_value(baseclasscheck));
        lua_setfield(L, metatableindex, &detail::base_class_check_key()[0]);
    }
    if (baseclasscast != nullptr) {
        stack::push(L, light_userdata_value(baseclasscast));
        lua_setfield(L, metatableindex, &detail::base_class_cast_key()[0]);
    }
    if (funcs.size() < 1 && metafunctable.size() < 2) {
        return;
    }
  
    // Functions should be placed on the metatable so that they can be called "statically" if the user wants
    int up = push_upvalues(L, funcs);
    functable.push_back({ nullptr, nullptr });
    luaL_setfuncs(L, functable.data(), up);
    functable.pop_back();

    // Metamethods directly on the metatable itself
    int metaup = push_upvalues(L, funcs);
    switch (metastage) {
        case stage::uniquemeta: {
            if (gcname != metafunctable.back().name) {
                metafunctable.push_back({ "__gc", nullptr });
            }
            luaL_Reg& target = metafunctable.back();
            luaL_Reg old = target;
            target.func = detail::unique_destruct<T>;
            metafunctable.push_back({nullptr, nullptr});
            luaL_setfuncs(L, metafunctable.data(), metaup);
            metafunctable.pop_back();
            target = old;
            break; }
        case stage::refmeta:
            if (gcname == metafunctable.back().name) {
                // We can just "clip" out the __gc function,
                // which we always put as the last entry in the meta function table.
                luaL_Reg& target = metafunctable.back();
                luaL_Reg old = target;
                target = { nullptr, nullptr };
                luaL_setfuncs(L, metafunctable.data(), metaup);
                target = old;
            }
            break;
        case stage::normalmeta:
        default:
            metafunctable.push_back({nullptr, nullptr});
            luaL_setfuncs(L, metafunctable.data(), metaup);
            metafunctable.pop_back();
            break;
    }
    if (needsindexfunction) {
        // We don't need to do anything more
        // since we've already bound the __index field using
        // setfuncs above...
        return;
    }
    // Otherwise, we use quick, fast table indexing for methods
    // gives us performance boost in calling them
    lua_createtable(L, 0, static_cast<int>(functable.size()));
    up = push_upvalues(L, funcs);
    functable.push_back({nullptr, nullptr});
    luaL_setfuncs(L, functable.data(), up);
    functable.pop_back();
    lua_setfield(L, metatableindex, "__index");
    return;
}

template <typename T, typename Functions>
inline void set_global_deleter(lua_State* L, lua_CFunction cleanup, Functions&& functions) {
    // Automatic deleter table -- stays alive until lua VM dies
    // even if the user calls collectgarbage(), weirdly enough
    lua_createtable(L, 0, 0); // global table that sits at toplevel
    lua_createtable(L, 0, 1); // metatable for the global table
    int up = push_upvalues<true>(L, functions);
    stack::set_field(L, "__gc", c_closure(cleanup, up));
    lua_setmetatable(L, -2);
    // gctable name by default has ♻ part of it
    lua_setglobal(L, &usertype_traits<T>::gc_table[0]);
}
} // usertype_detail

template<typename T>
class usertype {
private:
    typedef std::map<std::string, std::pair<bool, function_detail::base_function*>> function_map_t;
    std::vector<std::string> functionnames;
    std::vector<std::unique_ptr<function_detail::base_function>> functions;
    std::vector<luaL_Reg> functiontable;
    std::vector<luaL_Reg> metafunctiontable;
    function_detail::base_function* indexfunc;
    function_detail::base_function* newindexfunc;
    function_map_t indexwrapper, newindexwrapper;
    const char* constructfuncname;
    lua_CFunction constructfunc;
    const char* destructfuncname;
    lua_CFunction destructfunc;
    lua_CFunction functiongcfunc;
    bool needsindexfunction;
    void* baseclasscheck;
    void* baseclasscast;

    template<typename... Functions>
    std::unique_ptr<function_detail::base_function> make_function(const std::string&, overload_set<Functions...> func) {
        return std::make_unique<function_detail::usertype_overloaded_function<T, Functions...>>(std::move(func.set));
    }

    template<typename... Functions>
    std::unique_ptr<function_detail::base_function> make_function(const std::string&, constructor_wrapper<Functions...> func) {
        return std::make_unique<function_detail::usertype_constructor_function<T, Functions...>>(std::move(func.set));
    }

    template<typename RSig, typename WSig>
    std::unique_ptr<function_detail::base_function> make_function(const std::string&, member_property<RSig, WSig> func) {
        return std::make_unique<function_detail::usertype_variable_function<T, member_property<RSig, WSig>>>(std::move(func));
    }

    template<typename Fx>
    std::unique_ptr<function_detail::base_function> make_free_function(std::true_type, const std::string&, Fx&& func) {
        typedef std::decay_t<meta::Unqualified<Fx>> function_type;
        return std::make_unique<function_detail::usertype_function<T, function_type>>(func);
    }

    template<typename Fx>
    std::unique_ptr<function_detail::base_function> make_free_function(std::false_type, const std::string&, Fx&& func) {
        typedef std::decay_t<meta::Unqualified<Fx>> function_type;
        return std::make_unique<function_detail::free_function<function_type>>(func);
    }

    template<typename... Args, typename Ret>
    std::unique_ptr<function_detail::base_function> make_function(const std::string& name, Ret(*func)(Args...)) {
        typedef meta::bind_traits<Ret(Args...)> btraits;
        typedef typename btraits::template arg_at<0> Argu;
        typedef meta::Unqualified<std::remove_pointer_t<Argu>> Arg0;
        return make_free_function(std::is_base_of<Arg0, T>(), name, func);
    }

    template<typename Base, typename Ret>
    std::unique_ptr<function_detail::base_function> make_variable_function(std::true_type, const std::string&, Ret Base::* func) {
        static_assert(std::is_base_of<Base, T>::value, "Any registered member variable must be part of the class");
        typedef std::decay_t<decltype(func)> function_type;
        return std::make_unique<function_detail::usertype_variable_function<T, function_type>>(func);
    }

    template<typename Base, typename Ret>
    std::unique_ptr<function_detail::base_function> make_variable_function(std::false_type, const std::string&, Ret Base::* func) {
        static_assert(std::is_base_of<Base, T>::value, "Any registered member function must be part of the class");
        typedef std::decay_t<decltype(func)> function_type;
        return std::make_unique<function_detail::usertype_function<T, function_type>>(func);
    }

    template<typename Base, typename Ret>
    std::unique_ptr<function_detail::base_function> make_function(const std::string& name, Ret Base::* func) {
        typedef std::decay_t<decltype(func)> function_type;
        return make_variable_function(std::is_member_object_pointer<function_type>(), name, func);
    }

    template<typename Fx>
    std::unique_ptr<function_detail::base_function> make_functor_function(std::true_type, const std::string&, Fx&& func) {
        typedef std::decay_t<meta::Unqualified<Fx>> function_type;
        return std::make_unique<function_detail::usertype_function<T, function_type>>(func);
    }

    template<typename Fx>
    std::unique_ptr<function_detail::base_function> make_functor_function(std::false_type, const std::string&, Fx&& func) {
        typedef std::decay_t<meta::Unqualified<Fx>> function_type;
        return std::make_unique<function_detail::functor_function<function_type>>(func);
    }

    template<typename Fx>
    std::unique_ptr<function_detail::base_function> make_function(const std::string& name, Fx&& func) {
        typedef meta::Unqualified<Fx> Fxu;
        typedef meta::bind_traits<Fxu> btraits;
        typedef typename btraits::template arg_at<0> Argu;
        typedef meta::Unqualified<std::remove_pointer_t<Argu>> Arg0;
        return make_functor_function(std::is_base_of<Arg0, T>(), name, std::forward<Fx>(func));
    }

    template<std::size_t N>
    void build_function(std::string, no_construction) {}

    template<std::size_t N, typename... Args>
    void build_function(std::string funcname, constructors<Args...>) {
        functionnames.push_back(std::move(funcname));
        std::string& name = functionnames.back();
        // Insert bubble to keep with compile-time argument count (simpler and cheaper to do)
        functions.push_back(nullptr);
        constructfuncname = name.c_str();
        constructfunc = function_detail::construct<T, Args...>;
        metafunctiontable.push_back({ name.c_str(), constructfunc });
    }

    template<std::size_t N, typename... Args>
    void build_function(std::string funcname, lua_CFunction direct) {
        functionnames.push_back(std::move(funcname));
        std::string& name = functionnames.back();
        
        // Insert bubble to keep with compile-time argument count (simpler and cheaper to do)
        functions.push_back(nullptr);
        
        auto metamethodfind = std::find(meta_function_names.begin(), meta_function_names.end(), name);
        if (metamethodfind != meta_function_names.end()) {
            metafunctiontable.push_back({ name.c_str(), function_detail::usertype_call<N> });
            meta_function metafunction = static_cast<meta_function>(metamethodfind - meta_function_names.begin());
            switch (metafunction) {
            case meta_function::garbage_collect:
                destructfuncname = name.c_str();
                destructfunc = function_detail::usertype_call<N>;
                return;
            case meta_function::index:
                indexfunc = functions.back().get();
                needsindexfunction = true;
                break;
            case meta_function::new_index:
                newindexfunc = functions.back().get();
                break;
            case meta_function::construct:
                constructfuncname = name.c_str();
                constructfunc = function_detail::usertype_call<N>;
                break;
             default:
                  break;
            }
            return;
        }
        functiontable.push_back({ name.c_str(), direct });
    }

    template<std::size_t N>
    void build_function(std::string funcname, destructor_wrapper<void>) {
        auto metamethodfind = std::find(meta_function_names.begin(), meta_function_names.end(), funcname);
        if (metamethodfind == meta_function_names.end())
            throw error("cannot set destructor to anything but the metamethod \"__gc\"");
        meta_function metafunction = static_cast<meta_function>(metamethodfind - meta_function_names.begin());
        if (metafunction != meta_function::garbage_collect)
            throw error("cannot set destructor to anything but the metamethod \"__gc\"");
            
        functionnames.push_back(std::move(funcname));
        std::string& name = functionnames.back();
        destructfunc = function_detail::destruct<T>;
        destructfuncname = name.c_str();
        // Insert bubble to stay with the compile-time count
        functions.push_back(nullptr);    
    }

    template<std::size_t N, typename Fx>
    void build_function(std::string funcname, destructor_wrapper<Fx> dx) {
        auto metamethodfind = std::find(meta_function_names.begin(), meta_function_names.end(), funcname);
        if (metamethodfind == meta_function_names.end())
            throw error("cannot set destructor to anything but the metamethod \"__gc\"");
        meta_function metafunction = static_cast<meta_function>(metamethodfind - meta_function_names.begin());
        if (metafunction != meta_function::garbage_collect)
            throw error("cannot set destructor to anything but the metamethod \"__gc\"");

        functionnames.push_back(std::move(funcname));
        std::string& name = functionnames.back();
        auto baseptr = make_function(name, std::move(dx.fx));
        functions.emplace_back(std::move(baseptr));
        destructfunc = function_detail::usertype_call<N>;
        destructfuncname = name.c_str();
    }

    template<std::size_t N, typename Fx>
    void build_function(std::string funcname, Fx&& func) {
        typedef meta::Or<std::is_member_object_pointer<meta::Unqualified<Fx>>, meta::is_specialization_of<member_property, meta::Unqualified<Fx>>> is_variable;
        functionnames.push_back(std::move(funcname));
        std::string& name = functionnames.back();
        auto baseptr = make_function(name, std::forward<Fx>(func));
        functions.emplace_back(std::move(baseptr));
        auto metamethodfind = std::find(meta_function_names.begin(), meta_function_names.end(), name);
        if (metamethodfind != meta_function_names.end()) {
            metafunctiontable.push_back({ name.c_str(), function_detail::usertype_call<N> });
            meta_function metafunction = static_cast<meta_function>(metamethodfind - meta_function_names.begin());
            switch (metafunction) {
            case meta_function::garbage_collect:
                destructfuncname = name.c_str();
                destructfunc = function_detail::usertype_call<N>;
                return;
            case meta_function::index:
                indexfunc = functions.back().get();
                needsindexfunction = true;
                break;
            case meta_function::new_index:
                newindexfunc = functions.back().get();
                break;
            case meta_function::construct:
                constructfunc = function_detail::usertype_call<N>;
                break;
            default:
                break;
            }
            return;
        }
        if (is_variable::value) {
            needsindexfunction = true;
            indexwrapper.insert({ name, { false, functions.back().get() } });
            newindexwrapper.insert({ name, { false, functions.back().get() } });
            return;
        }
        indexwrapper.insert({ name, { true, functions.back().get() } });
        functiontable.push_back({ name.c_str(), function_detail::usertype_call<N> });
    }

    template<std::size_t N, typename Fx, typename... Args>
    void build_function_tables(std::string funcname, Fx&& func, Args&&... args) {
        build_function<N>(std::move(funcname), std::forward<Fx>(func));
        build_function_tables<N + 1>(std::forward<Args>(args)...);
    }

    template<std::size_t N, typename Fx, typename... Args>
    void build_function_tables(meta_function metafunc, Fx&& func, Args&&... args) {
        std::size_t idx = static_cast<std::size_t>(metafunc);
        const std::string& funcname = meta_function_names[idx];
        build_function_tables<N>(funcname, std::forward<Fx>(func), std::forward<Args>(args)...);
    }

    template<std::size_t N, typename Fx, typename... Args>
    void build_function_tables(call_construction, Fx&& func, Args&&... args) {
        build_function_tables<N>("__call", std::forward<Fx>(func), std::forward<Args>(args)...);
    }

    template<std::size_t N, typename... Bases, typename... Args>
    void build_function_tables(base_classes_tag, bases<Bases...>, Args&&... args) {
        build_function_tables<N>(std::forward<Args>(args)...);
        if (sizeof...(Bases) < 1)
            return;
#ifndef SOL_NO_EXCEPTIONS
        static_assert(sizeof(void*) <= sizeof(detail::throw_cast), "The size of this data pointer is too small to fit the inheritance checking function: file a bug report.");
        baseclasscast = (void*)&detail::throw_as<T>;
#elif !defined(SOL_NO_RTTI)
        static_assert(sizeof(void*) <= sizeof(detail::inheritance_check_function), "The size of this data pointer is too small to fit the inheritance checking function: file a bug report.");
        static_assert(sizeof(void*) <= sizeof(detail::inheritance_cast_function), "The size of this data pointer is too small to fit the inheritance checking function: file a bug report.");
        baseclasscheck = (void*)&detail::inheritance<T, Bases...>::check;
        baseclasscast = (void*)&detail::inheritance<T, Bases...>::cast;
#else
        static_assert(sizeof(void*) <= sizeof(detail::inheritance_check_function), "The size of this data pointer is too small to fit the inheritance checking function: file a bug report.");
        static_assert(sizeof(void*) <= sizeof(detail::inheritance_cast_function), "The size of this data pointer is too small to fit the inheritance checking function: file a bug report.");
        baseclasscheck = (void*)&detail::inheritance<T, Bases...>::check;
        baseclasscast = (void*)&detail::inheritance<T, Bases...>::cast;
#endif // No Runtime Type Information vs. Throw-Style Inheritance
    }

    template<std::size_t N>
    void build_function_tables() {
        int variableend = 0;
        if (!indexwrapper.empty()) {
            functions.push_back(std::make_unique<function_detail::usertype_indexing_function>("__index", indexfunc, std::move(indexwrapper)));
            metafunctiontable.push_back({ "__index", function_detail::usertype_call<N> });
            ++variableend;
        }
        if (!newindexwrapper.empty()) {
            functions.push_back(std::make_unique<function_detail::usertype_indexing_function>("__newindex", newindexfunc, std::move(newindexwrapper)));
            metafunctiontable.push_back({ "__newindex", indexwrapper.empty() ? function_detail::usertype_call<N> : function_detail::usertype_call<N + 1> });
            ++variableend;
        }
        if (destructfunc != nullptr) {
            metafunctiontable.push_back({ destructfuncname, destructfunc });
        }
        switch (variableend) {
        case 2:
            functiongcfunc = function_detail::usertype_gc<N + 2>;
            break;
        case 1:
            functiongcfunc = function_detail::usertype_gc<N + 1>;
            break;
        case 0:
            functiongcfunc = function_detail::usertype_gc<N + 0>;
            break;
        }
    }

    template<typename... Args>
    usertype(usertype_detail::verified_tag, Args&&... args) : indexfunc(nullptr), newindexfunc(nullptr), constructfuncname(""), constructfunc(nullptr),
    destructfuncname(""), destructfunc(nullptr), functiongcfunc(nullptr), needsindexfunction(false), baseclasscheck(nullptr), baseclasscast(nullptr) {
        static_assert((sizeof...(Args) % 2) == 0, "Incorrect argument count to usertype creation: not in pairs. Might be missing name, function/property/variable, comma");
        functionnames.reserve(sizeof...(args)+3);
        functiontable.reserve(sizeof...(args)+3);
        metafunctiontable.reserve(sizeof...(args)+3);

        build_function_tables<0>(std::forward<Args>(args)...);
    }

    template<typename... Args>
    usertype(usertype_detail::add_destructor_tag, Args&&... args) : usertype(usertype_detail::verified, "__gc", default_destructor, std::forward<Args>(args)...) {}

    template<typename... Args>
    usertype(usertype_detail::check_destructor_tag, Args&&... args) : usertype(meta::If<meta::And<std::is_destructible<T>, meta::Not<usertype_detail::has_destructor<Args...>>>, usertype_detail::add_destructor_tag, usertype_detail::verified_tag>(), std::forward<Args>(args)...) {}

public:

    template<typename... Args>
    usertype(Args&&... args) : usertype(meta::If<meta::And<std::is_default_constructible<T>, meta::Not<usertype_detail::has_constructor<Args...>>>, decltype(default_constructor), usertype_detail::check_destructor_tag>(), std::forward<Args>(args)...) {}

    template<typename... Args, typename... CArgs>
    usertype(constructors<CArgs...> constructorlist, Args&&... args) : usertype(usertype_detail::verified, "new", constructorlist, "__gc", default_destructor, std::forward<Args>(args)...) {
            
    }

    int push(lua_State* L) {
        // push pointer tables first,
        usertype_detail::push_metatable<T*, usertype_detail::stage::refmeta>(L, needsindexfunction, functions, functiontable, metafunctiontable, baseclasscheck, baseclasscast);
        lua_pop(L, 1);
        usertype_detail::push_metatable<detail::unique_usertype<T>, usertype_detail::stage::uniquemeta>(L, needsindexfunction, functions, functiontable, metafunctiontable, baseclasscheck, baseclasscast);
        lua_pop(L, 1);
        // but leave the regular T table on last
        // so it can be linked to a key for usage with `.new(...)` or `:new(...)`
        usertype_detail::push_metatable<T, usertype_detail::stage::normalmeta>(L, needsindexfunction, functions, functiontable, metafunctiontable, baseclasscheck, baseclasscast);
        // be sure to link the construction function to allow for people to do the whole lua_bind thing
        if (constructfunc != nullptr && constructfuncname != nullptr && std::find(meta_function_names.cbegin(), meta_function_names.cend(), constructfuncname) != meta_function_names.cend()) {
            lua_createtable(L, 0, 0);
		  lua_pushcclosure(L, constructfunc, 0);
            lua_setfield(L, -2, constructfuncname);
            lua_setmetatable(L, -2);
        }
        // Make sure to drop a table in the global namespace to properly destroy the pushed functions
        // at some later point in life
        usertype_detail::set_global_deleter<T>(L, functiongcfunc, functions);
        return 1;
    }
};

namespace stack {
template<typename T>
struct pusher<usertype<T>> {
    static int push(lua_State* L, usertype<T>& user) {
        return user.push(L);
    }
};
} // stack
} // sol

// end of sol\usertype.hpp

// beginning of sol\table_iterator.hpp

namespace sol {

template <typename reference_type>
class basic_table_iterator : public std::iterator<std::input_iterator_tag, std::pair<object, object>> {
private:
    typedef std::iterator<std::input_iterator_tag, std::pair<object, object>> base_t;
public:
    typedef object key_type;
    typedef object mapped_type;
    typedef base_t::value_type value_type;
    typedef base_t::iterator_category iterator_category;
    typedef base_t::difference_type difference_type;
    typedef base_t::pointer pointer;
    typedef base_t::reference reference;
    typedef const value_type& const_reference;

private:
    std::pair<object, object> kvp;
    reference_type ref;
    int tableidx = 0;
    std::ptrdiff_t idx = 0;

public:

    basic_table_iterator () : idx(-1) {

    }

    basic_table_iterator(reference_type x) : ref(std::move(x)) {
        ref.push();
        tableidx = lua_gettop(ref.lua_state());
        stack::push(ref.lua_state(), nil);
        this->operator++();
        if (idx == -1) {
            return;
        }
        --idx;
    }

    basic_table_iterator& operator++() {
        if (idx == -1)
            return *this;

        if (lua_next(ref.lua_state(), tableidx) == 0) {
            idx = -1;
            return *this;
        }
        ++idx;
        kvp.first = object(ref.lua_state(), -2);
        kvp.second = object(ref.lua_state(), -1);
        lua_pop(ref.lua_state(), 1);
        // leave key on the stack
        return *this;
    }

    basic_table_iterator operator++(int) {
        auto saved = *this;
        this->operator++();
        return saved;
    }

    reference operator*() {
        return kvp;
    }

    const_reference operator*() const {
        return kvp;
    }

    bool operator== (const basic_table_iterator& right) const {
        return idx == right.idx;
    }

    bool operator!= (const basic_table_iterator& right) const {
        return idx != right.idx;
    }

    ~basic_table_iterator() {
        if (ref.valid()) {
            stack::remove(ref.lua_state(), tableidx, 1);
        }
    }
};

} // sol

// end of sol\table_iterator.hpp

namespace sol {
namespace detail {
template <std::size_t n>
struct clean { lua_State* L; clean(lua_State* L) : L(L) {} ~clean() { lua_pop(L, static_cast<int>(n)); } };
struct ref_clean { lua_State* L; int& n; ref_clean(lua_State* L, int& n) : L(L), n(n) {} ~ref_clean() { lua_pop(L, static_cast<int>(n)); } };
}

template <bool top_level, typename base_t>
class basic_table_core : public base_t {
    friend class state;
    friend class state_view;

    template <typename... Args>
    using is_global = meta::And<meta::Bool<top_level>, meta::is_c_str<Args>...>;

    template<typename Fx>
    void for_each(std::true_type, Fx&& fx) const {
        auto pp = stack::push_pop( *this );
        stack::push( base_t::lua_state( ), nil );
        while ( lua_next( base_t::lua_state( ), -2 ) ) {
            sol::object key( base_t::lua_state( ), -2 );
            sol::object value( base_t::lua_state( ), -1 );
            std::pair<sol::object&, sol::object&> keyvalue(key, value);
            fx( keyvalue );
            lua_pop( base_t::lua_state( ), 1 );
        }
    }

    template<typename Fx>
    void for_each(std::false_type, Fx&& fx) const {
        auto pp = stack::push_pop( *this );
        stack::push( base_t::lua_state( ), nil );
        while ( lua_next( base_t::lua_state( ), -2 ) ) {
            sol::object key( base_t::lua_state( ), -2 );
            sol::object value( base_t::lua_state( ), -1 );
            fx( key, value );
            lua_pop( base_t::lua_state( ), 1 );
        }
    }

    template<typename Ret0, typename Ret1, typename... Ret, std::size_t... I, typename Keys>
    auto tuple_get( types<Ret0, Ret1, Ret...>, std::index_sequence<0, 1, I...>, Keys&& keys ) const
    -> decltype(stack::pop<std::tuple<Ret0, Ret1, Ret...>>(nullptr)){
        typedef decltype(stack::pop<std::tuple<Ret0, Ret1, Ret...>>(nullptr)) Tup;
        return Tup(
            traverse_get_optional<top_level, Ret0>(meta::is_specialization_of<sol::optional, meta::Unqualified<Ret0>>(), detail::forward_get<0>(keys)),
            traverse_get_optional<top_level, Ret1>(meta::is_specialization_of<sol::optional, meta::Unqualified<Ret1>>(), detail::forward_get<1>(keys)),
            traverse_get_optional<top_level, Ret>(meta::is_specialization_of<sol::optional, meta::Unqualified<Ret>>(), detail::forward_get<I>(keys))...
        );
    }

    template<typename Ret, std::size_t I, typename Keys>
    decltype(auto) tuple_get( types<Ret>, std::index_sequence<I>, Keys&& keys ) const {
        return traverse_get_optional<top_level, Ret>( meta::is_specialization_of<sol::optional, meta::Unqualified<Ret>>(), detail::forward_get<I>(keys) );
    }

    template<typename Pairs, std::size_t... I>
    void tuple_set( std::index_sequence<I...>, Pairs&& pairs ) {
        auto pp = stack::push_pop<top_level && (is_global<decltype(detail::forward_get<I * 2>(pairs))...>::value)>(*this);
        void(detail::swallow{ (stack::set_field<top_level>(base_t::lua_state(), 
            detail::forward_get<I * 2>(pairs),
            detail::forward_get<I * 2 + 1>(pairs),
            lua_gettop(base_t::lua_state())
        ), 0)... });
    }

    template <bool global, typename T, typename Key>
    decltype(auto) traverse_get_deep( Key&& key ) const {
        stack::get_field<global>( base_t::lua_state( ), std::forward<Key>( key ) );
        return stack::get<T>( base_t::lua_state( ) );
    }

    template <bool global, typename T, typename Key, typename... Keys>
    decltype(auto) traverse_get_deep( Key&& key, Keys&&... keys ) const {
        stack::get_field<global>( base_t::lua_state( ), std::forward<Key>( key ) );
        return traverse_get_deep<false, T>(std::forward<Keys>(keys)...);
    }

    template <bool global, typename T, std::size_t I, typename Key>
    decltype(auto) traverse_get_deep_optional( int& popcount, Key&& key ) const {
        typedef decltype(stack::get<T>(base_t::lua_state())) R;
        auto p = stack::probe_get_field<global>(base_t::lua_state(), std::forward<Key>(key), lua_gettop(base_t::lua_state()));
        popcount += p.levels;
        if (!p.success)
            return R(nullopt);
        return stack::get<T>( base_t::lua_state( ) );
    }

    template <bool global, typename T, std::size_t I, typename Key, typename... Keys>
    decltype(auto) traverse_get_deep_optional( int& popcount, Key&& key, Keys&&... keys ) const {
        auto p = I > 0 ? stack::probe_get_field<global>(base_t::lua_state(), std::forward<Key>(key), -1) : stack::probe_get_field<global>( base_t::lua_state( ), std::forward<Key>( key ), lua_gettop(base_t::lua_state()));
        popcount += p.levels;
        if (!p.success)
            return T(nullopt);
        return traverse_get_deep_optional<false, T, I + 1>(popcount, std::forward<Keys>(keys)...);
    }

    template <bool global, typename T, typename... Keys>
    decltype(auto) traverse_get_optional( std::false_type, Keys&&... keys ) const {
        detail::clean<sizeof...(Keys)> c(base_t::lua_state());
        return traverse_get_deep<top_level, T>(std::forward<Keys>(keys)...);
    }

    template <bool global, typename T, typename... Keys>
    decltype(auto) traverse_get_optional( std::true_type, Keys&&... keys ) const {
        int popcount = 0;
        detail::ref_clean c(base_t::lua_state(), popcount);
        return traverse_get_deep_optional<top_level, T, 0>(popcount, std::forward<Keys>(keys)...);
    }

    template <bool global, typename Key, typename Value>
    void traverse_set_deep( Key&& key, Value&& value ) const {
        stack::set_field<global>( base_t::lua_state( ), std::forward<Key>( key ), std::forward<Value>(value) );
    }

    template <bool global, typename Key, typename... Keys>
    void traverse_set_deep( Key&& key, Keys&&... keys ) const {
        stack::get_field<global>( base_t::lua_state( ), std::forward<Key>( key ) );
        traverse_set_deep<false>(std::forward<Keys>(keys)...);
    }

    basic_table_core(lua_State* L, detail::global_tag t) noexcept : reference(L, t) { }

public:
    typedef basic_table_iterator<base_t> iterator;
    typedef iterator const_iterator;

    basic_table_core( ) noexcept : base_t( ) { }
    basic_table_core( const table_core<true>& global ) noexcept : base_t( global ) { }
    basic_table_core(const stack_reference& r) : basic_table_core(r.lua_state(), r.stack_index()) {}
    basic_table_core(stack_reference&& r) : basic_table_core(r.lua_state(), r.stack_index()) {}
    basic_table_core( lua_State* L, int index = -1 ) : base_t( L, index ) {
#ifdef SOL_CHECK_ARGUMENTS
        stack::check<basic_table_core>(L, index, type_panic);
#endif // Safety
    }

    iterator begin () const {
        return iterator(*this);
    }

    iterator end() const {
        return iterator();
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }

    template<typename... Ret, typename... Keys>
    decltype(auto) get( Keys&&... keys ) const {
        static_assert(sizeof...(Keys) == sizeof...(Ret), "number of keys and number of return types do not match");
        auto pp = stack::push_pop<is_global<Keys...>::value>(*this);
        return tuple_get( types<Ret...>( ), std::make_index_sequence<sizeof...(Ret)>( ), std::forward_as_tuple(std::forward<Keys>(keys)...));
    }

    template<typename T, typename Key>
    decltype(auto) get_or(Key&& key, T&& otherwise) const {
        typedef decltype(get<T>("")) U;
        sol::optional<U> option = get<sol::optional<U>>(std::forward<Key>(key));
        if (option) {
            return static_cast<U>(option.value());
        }
        return static_cast<U>(std::forward<T>(otherwise));
    }

    template<typename T, typename Key, typename D>
    decltype(auto) get_or(Key&& key, D&& otherwise) const {
        sol::optional<T> option = get<sol::optional<T>>(std::forward<Key>(key));
        if (option) {
            return static_cast<T>(option.value());
        }
        return static_cast<T>(std::forward<D>(otherwise));
    }

    template <typename T, typename... Keys>
    decltype(auto) traverse_get( Keys&&... keys ) const {
        auto pp = stack::push_pop<is_global<Keys...>::value>(*this);
        return traverse_get_optional<top_level, T>(meta::is_specialization_of<sol::optional, meta::Unqualified<T>>(), std::forward<Keys>(keys)...);
    }

    template <typename... Keys>
    basic_table_core& traverse_set( Keys&&... keys ) {
        auto pp = stack::push_pop<is_global<Keys...>::value>(*this);
        traverse_set_deep<top_level>(std::forward<Keys>(keys)...);
        lua_pop(base_t::lua_state(), static_cast<int>(sizeof...(Keys)-2));
        return *this;
    }

    template<typename... Args>
    basic_table_core& set( Args&&... args ) {
        tuple_set(std::make_index_sequence<sizeof...(Args) / 2>(), std::forward_as_tuple(std::forward<Args>(args)...));
        return *this;
    }

    template<typename T>
    basic_table_core& set_usertype( usertype<T>& user ) {
        return set_usertype(usertype_traits<T>::name, user);
    }

    template<typename Key, typename T>
    basic_table_core& set_usertype( Key&& key, usertype<T>& user ) {
        return set(std::forward<Key>(key), user);
    }

    template<typename Class, typename... Args>
    basic_table_core& new_usertype(const std::string& name, Args&&... args) {
        usertype<Class> utype(std::forward<Args>(args)...);
        set_usertype(name, utype);
        return *this;
    }

    template<typename Class, typename CTor0, typename... CTor, typename... Args>
    basic_table_core& new_usertype(const std::string& name, Args&&... args) {
        constructors<types<CTor0, CTor...>> ctor{};
        return new_usertype<Class>(name, ctor, std::forward<Args>(args)...);
    }

    template<typename Class, typename... CArgs, typename... Args>
    basic_table_core& new_usertype(const std::string& name, constructors<CArgs...> ctor, Args&&... args) {
        usertype<Class> utype(ctor, std::forward<Args>(args)...);
        set_usertype(name, utype);
        return *this;
    }

    template<typename Fx>
    void for_each( Fx&& fx ) const {
        typedef meta::is_callable<Fx( std::pair<sol::object, sol::object> )> is_paired;
        for_each(is_paired(), std::forward<Fx>(fx));
    }

    size_t size( ) const {
        auto pp = stack::push_pop( *this );
        return lua_rawlen(base_t::lua_state(), -1);
    }

    bool empty() const {
        return cbegin() == cend();
    }

    template<typename T>
    proxy<basic_table_core&, T> operator[]( T&& key ) & {
        return proxy<basic_table_core&, T>( *this, std::forward<T>( key ) );
    }

    template<typename T>
    proxy<const basic_table_core&, T> operator[]( T&& key ) const & {
        return proxy<const basic_table_core&, T>( *this, std::forward<T>( key ) );
    }

    template<typename T>
    proxy<basic_table_core, T> operator[]( T&& key ) && {
        return proxy<basic_table_core, T>( *this, std::forward<T>( key ) );
    }

    template<typename Sig, typename Key, typename... Args>
    basic_table_core& set_function( Key&& key, Args&&... args ) {
        set_fx( types<Sig>( ), std::forward<Key>( key ), std::forward<Args>( args )... );
        return *this;
    }

    template<typename Key, typename... Args>
    basic_table_core& set_function( Key&& key, Args&&... args ) {
        set_fx( types<>( ), std::forward<Key>( key ), std::forward<Args>( args )... );
        return *this;
    }

private:
    template<typename R, typename... Args, typename Fx, typename Key, typename = std::result_of_t<Fx( Args... )>>
    void set_fx( types<R( Args... )>, Key&& key, Fx&& fx ) {
        set_resolved_function<R( Args... )>( std::forward<Key>( key ), std::forward<Fx>( fx ) );
    }

    template<typename Fx, typename Key, meta::EnableIf<meta::is_specialization_of<overload_set, meta::Unqualified<Fx>>> = 0>
    void set_fx( types<>, Key&& key, Fx&& fx ) {
        set(std::forward<Key>(key), std::forward<Fx>(fx));
    }

    template<typename Fx, typename Key, typename... Args, meta::DisableIf<meta::is_specialization_of<overload_set, meta::Unqualified<Fx>>> = 0>
    void set_fx( types<>, Key&& key, Fx&& fx, Args&&... args ) {
        set(std::forward<Key>(key), function_args(std::forward<Fx>(fx), std::forward<Args>(args)...));
    }

    template<typename... Sig, typename... Args, typename Key>
    void set_resolved_function( Key&& key, Args&&... args ) {
        set(std::forward<Key>(key), function_args<function_sig<Sig...>>(std::forward<Args>(args)...));
    }

public:
    static inline table create(lua_State* L, int narr = 0, int nrec = 0) {
        lua_createtable(L, narr, nrec);
        table result(L);
        lua_pop(L, 1);
        return result;
    }

    template <typename Key, typename Value, typename... Args>
    static inline table create(lua_State* L, int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
        lua_createtable(L, narr, nrec);
        table result(L);
        result.set(std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
        lua_pop(L, 1);
        return result;
    }

    template <typename... Args>
    static inline table create_with(lua_State* L, Args&&... args) {
        static const int narr = static_cast<int>(meta::count_if_2_pack<std::is_integral, Args...>::value);
        return create(L, narr, static_cast<int>((sizeof...(Args) / 2) - narr), std::forward<Args>(args)...);
    }

    table create(int narr = 0, int nrec = 0) {
        return create(base_t::lua_state(), narr, nrec);
    }

    template <typename Key, typename Value, typename... Args>
    table create(int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
        return create(base_t::lua_state(), narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
    }
    
    template <typename Name>
    table create(Name&& name, int narr = 0, int nrec = 0) {
        table x = create(base_t::lua_state(), narr, nrec);
        this->set(std::forward<Name>(name), x);
        return x;
    }

    template <typename Name, typename Key, typename Value, typename... Args>
    table create(Name&& name, int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
        table x = create(base_t::lua_state(), narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
        this->set(std::forward<Name>(name), x);
        return x;
    }

    template <typename... Args>
    table create_with(Args&&... args) {
        return create_with(base_t::lua_state(), std::forward<Args>(args)...);
    }

    template <typename Name, typename... Args>
    table create_named(Name&& name, Args&&... args) {
        static const int narr = static_cast<int>(meta::count_if_2_pack<std::is_integral, Args...>::value);
        return create(std::forward<Name>(name), narr, sizeof...(Args) / 2 - narr, std::forward<Args>(args)...);
    }
};
} // sol

// end of sol\table_core.hpp

namespace sol {
    typedef table_core<false> table;
} // sol

// end of sol\table.hpp

// beginning of sol\load_result.hpp

namespace sol {
    struct load_result : public proxy_base<load_result> {
    private:
        lua_State* L;
        int index;
        int returncount;
        int popcount;
        load_status err;

        template <typename T>
        decltype(auto) tagged_get(types<sol::optional<T>>) const {
            if (!valid()) {
                return sol::optional<T>(nullopt);
            }
            return stack::get<sol::optional<T>>(L, index);
        }

        template <typename T>
        decltype(auto) tagged_get(types<T>) const {
#ifdef SOL_CHECK_ARGUMENTS
            if (!valid()) {
                type_panic(L, index, type_of(L, index), type::none);
            }
#endif // Check Argument Safety
            return stack::get<T>(L, index);
        }

        sol::optional<sol::error> tagged_get(types<sol::optional<sol::error>>) const {
            if (valid()) {
                return nullopt;
            }
            return sol::error(detail::direct_error, stack::get<std::string>(L, index));
        }

        sol::error tagged_get(types<sol::error>) const {
#ifdef SOL_CHECK_ARGUMENTS
            if (valid()) {
                type_panic(L, index, type_of(L, index), type::none);
            }
#endif // Check Argument Safety
            return sol::error(detail::direct_error, stack::get<std::string>(L, index));
        }

    public:
        load_result() = default;
        load_result(lua_State* L, int index = -1, int returncount = 0, int popcount = 0, load_status err = load_status::ok) : L(L), index(index), returncount(returncount), popcount(popcount), err(err) {

        }
        load_result(const load_result&) = default;
        load_result& operator=(const load_result&) = default;
        load_result(load_result&& o) : L(o.L), index(o.index), returncount(o.returncount), popcount(o.popcount), err(o.err) {
            // Must be manual, otherwise destructor will screw us
            // return count being 0 is enough to keep things clean
            // but will be thorough
            o.L = nullptr;
            o.index = 0;
            o.returncount = 0;
            o.popcount = 0;
            o.err = load_status::syntax;
        }
        load_result& operator=(load_result&& o) {
            L = o.L;
            index = o.index;
            returncount = o.returncount;
            popcount = o.popcount;
            err = o.err;
            // Must be manual, otherwise destructor will screw us
            // return count being 0 is enough to keep things clean
            // but will be thorough
            o.L = nullptr;
            o.index = 0;
            o.returncount = 0;
            o.popcount = 0;
            o.err = load_status::syntax;
            return *this;
        }

        load_status error() const {
            return err;
        }

        bool valid() const {
            return error() == load_status::ok;
        }

        template<typename T>
        T get() const {
            return tagged_get(types<meta::Unqualified<T>>());
        }

        template<typename... Ret, typename... Args>
        decltype(auto) call(Args&&... args) {
            return get<function>().template call<Ret...>(std::forward<Args>(args)...);
        }

        template<typename... Args>
        decltype(auto) operator()(Args&&... args) {
            return call<>(std::forward<Args>(args)...);
        }

        lua_State* lua_state() const { return L; };
        int stack_index() const { return index; };

        ~load_result() {
            stack::remove(L, index, popcount);
        }
    };
} // sol

// end of sol\load_result.hpp

namespace sol {
enum class lib : char {
    base,
    package,
    coroutine,
    string,
    os,
    math,
    table,
    debug,
    bit32,
    io,
    ffi,
    jit,
    count
};

class state_view {
private:
    lua_State* L;
    table reg;
    global_table global;
public:
    typedef typename global_table::iterator iterator;
    typedef typename global_table::const_iterator const_iterator;

    state_view(lua_State* L):
    L(L),
    reg(L, LUA_REGISTRYINDEX),
    global(L, detail::global_) {

    }

    lua_State* lua_state() const {
        return L;
    }

    template<typename... Args>
    void open_libraries(Args&&... args) {
        static_assert(meta::are_same<lib, Args...>::value, "all types must be libraries");
        if(sizeof...(args) == 0) {
            luaL_openlibs(L);
            return;
        }

        lib libraries[1 + sizeof...(args)] = { lib::count, std::forward<Args>(args)... };

        for(auto&& library : libraries) {
            switch(library) {
#if SOL_LUA_VERSION <= 501 && defined(SOL_LUAJIT)
            case lib::coroutine:
#endif // luajit opens coroutine base stuff
            case lib::base:
                luaL_requiref(L, "base", luaopen_base, 1);
                lua_pop(L, 1);
                break;
            case lib::package:
                luaL_requiref(L, "package", luaopen_package, 1);
                lua_pop(L, 1);
                break;
#if !defined(SOL_LUAJIT)
            case lib::coroutine:
#if SOL_LUA_VERSION > 501
                luaL_requiref(L, "coroutine", luaopen_coroutine, 1);
                lua_pop(L, 1);
#endif // Lua 5.2+ only
                break;
#endif // Not LuaJIT
            case lib::string:
                luaL_requiref(L, "string", luaopen_string, 1);
                lua_pop(L, 1);
                break;
            case lib::table:
                luaL_requiref(L, "table", luaopen_table, 1);
                lua_pop(L, 1);
                break;
            case lib::math:
                luaL_requiref(L, "math", luaopen_math, 1);
                lua_pop(L, 1);
                break;
            case lib::bit32:
#ifdef SOL_LUAJIT
                luaL_requiref(L, "bit32", luaopen_bit, 1);
                lua_pop(L, 1);
#elif SOL_LUA_VERSION == 502
                luaL_requiref(L, "bit32", luaopen_bit32, 1);
                lua_pop(L, 1);
#else
#endif // Lua 5.2 only (deprecated in 5.3 (503))
                break;
            case lib::io:
                luaL_requiref(L, "io", luaopen_io, 1);
                lua_pop(L, 1);
                break;
            case lib::os:
                luaL_requiref(L, "os", luaopen_os, 1);
                lua_pop(L, 1);
                break;
            case lib::debug:
                luaL_requiref(L, "debug", luaopen_debug, 1);
                lua_pop(L, 1);
                break;
            case lib::ffi:
#ifdef SOL_LUAJIT
                luaL_requiref(L, "ffi", luaopen_ffi, 1);
#endif
                break;
            case lib::jit:
#ifdef SOL_LUAJIT
                luaL_requiref(L, "jit", luaopen_jit, 1);
#endif
                break;
            case lib::count:
            default:
                break;
            }
        }
    }

    void script(const std::string& code) {
        if(luaL_dostring(L, code.c_str())) {
            lua_error(L);
        }
    }

    void script_file(const std::string& filename) {
        if(luaL_dofile(L, filename.c_str())) {
            lua_error(L);
        }
    }

    load_result load(const std::string& code) {
        load_status x = static_cast<load_status>(luaL_loadstring(L, code.c_str()));
        return load_result(L, -1, 1, 1, x);
    }

    load_result load_file(const std::string& filename) {
        load_status x = static_cast<load_status>(luaL_loadfile(L, filename.c_str()));
        return load_result(L, -1, 1, 1, x);
    }

    iterator begin () const {
        return global.begin();
    }

    iterator end() const {
        return global.end();
    }

    const_iterator cbegin() const {
        return global.cbegin();
    }

    const_iterator cend() const {
        return global.cend();
    }

    global_table globals() const {
        return global;
    }

    table registry() const {
        return reg;
    }

    operator lua_State* () const {
        return lua_state();
    }

    void set_panic(lua_CFunction panic){
        lua_atpanic(L, panic);
    }

    template<typename... Args, typename... Keys>
    decltype(auto) get(Keys&&... keys) const {
        return global.get<Args...>(std::forward<Keys>(keys)...);
    }

    template<typename T, typename Key>
    decltype(auto) get_or(Key&& key, T&& otherwise) const {
        return global.get_or(std::forward<Key>(key), std::forward<T>(otherwise));
    }

    template<typename T, typename Key, typename D>
    decltype(auto) get_or(Key&& key, D&& otherwise) const {
        return global.get_or<T>(std::forward<Key>(key), std::forward<D>(otherwise));
    }

    template<typename... Args>
    state_view& set(Args&&... args) {
        global.set(std::forward<Args>(args)...);
        return *this;
    }

    template<typename T, typename... Keys>
    decltype(auto) traverse_get(Keys&&... keys) const {
        return global.traverse_get<T>(std::forward<Keys>(keys)...);
    }

    template<typename... Args>
    state_view& traverse_set(Args&&... args) {
        global.traverse_set(std::forward<Args>(args)...);
        return *this;
    }

    template<typename T>
    state_view& set_usertype(usertype<T>& user) {
        return set_usertype(usertype_traits<T>::name, user);
    }

    template<typename Key, typename T>
    state_view& set_usertype(Key&& key, usertype<T>& user) {
        global.set_usertype(std::forward<Key>(key), user);
        return *this;
    }

    template<typename Class, typename... Args>
    state_view& new_usertype(const std::string& name, Args&&... args) {
        global.new_usertype<Class>(name, std::forward<Args>(args)...);
        return *this;
    }

    template<typename Class, typename CTor0, typename... CTor, typename... Args>
    state_view& new_usertype(const std::string& name, Args&&... args) {
        global.new_usertype<Class, CTor0, CTor...>(name, std::forward<Args>(args)...);
        return *this;
    }

    template<typename Class, typename... CArgs, typename... Args>
    state_view& new_usertype(const std::string& name, constructors<CArgs...> ctor, Args&&... args) {
        global.new_usertype<Class>(name, ctor, std::forward<Args>(args)...);
        return *this;
    }

    template <typename Fx>
    void for_each(Fx&& fx) {
        global.for_each(std::forward<Fx>(fx));
    }

    template<typename T>
    proxy<global_table&, T> operator[](T&& key) {
        return global[std::forward<T>(key)];
    }

    template<typename T>
    proxy<const global_table&, T> operator[](T&& key) const {
        return global[std::forward<T>(key)];
    }

    template<typename Sig, typename... Args, typename Key>
    state_view& set_function(Key&& key, Args&&... args) {
        global.set_function<Sig>(std::forward<Key>(key), std::forward<Args>(args)...);
        return *this;
    }

    template<typename... Args, typename Key>
    state_view& set_function(Key&& key, Args&&... args) {
        global.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        return *this;
    }

    template <typename Name>
    table create_table(Name&& name, int narr = 0, int nrec = 0) {
        return global.create(std::forward<Name>(name), narr, nrec);
    }

    template <typename Name, typename Key, typename Value, typename... Args>
    table create_table(Name&& name, int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
        return global.create(std::forward<Name>(name), narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
    }

    template <typename Name, typename... Args>
    table create_named_table(Name&& name, Args&&... args) {
        table x = global.create_with(std::forward<Args>(args)...);
        global.set(std::forward<Name>(name), x);
        return x;
    }

    table create_table(int narr = 0, int nrec = 0) {
        return create_table(lua_state(), narr, nrec);
    }

    template <typename Key, typename Value, typename... Args>
    table create_table(int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
        return create_table(lua_state(), narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
    }

    template <typename... Args>
    table create_table_with(Args&&... args) {
        return create_table_with(lua_state(), std::forward<Args>(args)...);
    }

    static inline table create_table(lua_State* L, int narr = 0, int nrec = 0) {
        return global_table::create(L, narr, nrec);
    }

    template <typename Key, typename Value, typename... Args>
    static inline table create_table(lua_State* L, int narr, int nrec, Key&& key, Value&& value, Args&&... args) {
        return global_table::create(L, narr, nrec, std::forward<Key>(key), std::forward<Value>(value), std::forward<Args>(args)...);
    }

    template <typename... Args>
    static inline table create_table_with(lua_State* L, Args&&... args) {
        return global_table::create_with(L, std::forward<Args>(args)...);
    }
};
} // sol

// end of sol\state_view.hpp

namespace sol {
inline int default_at_panic(lua_State* L) {
#ifdef SOL_NO_EXCEPTIONS
    (void)L;
    return -1;
#else
    const char* message = lua_tostring(L, -1);
    std::string err = message ? message : "An unexpected error occurred and forced the lua state to call atpanic";
    throw error(err);
#endif
}

class state : private std::unique_ptr<lua_State, void(*)(lua_State*)>, public state_view {
private:
    typedef std::unique_ptr<lua_State, void(*)(lua_State*)> unique_base;
public:
    state(lua_CFunction panic = default_at_panic) : unique_base(luaL_newstate(), lua_close),
    state_view(unique_base::get()) {
        set_panic(panic);
        stack::luajit_exception_handler(unique_base::get());
    }

    state(lua_CFunction panic, lua_Alloc alfunc, void* alpointer = nullptr) : unique_base(lua_newstate(alfunc, alpointer), lua_close),
    state_view(unique_base::get()) {
        set_panic(panic);
        stack::luajit_exception_handler(unique_base::get());
    }

    using state_view::get;
};
} // sol

// end of sol\state.hpp

// beginning of sol\coroutine.hpp

// beginning of sol\thread.hpp

namespace sol {
class thread : public reference {
public:
    thread () noexcept = default;
    thread(const thread&) = default;
    thread(thread&&) = default;
    thread& operator=(const thread&) = default;
    thread& operator=(thread&&) = default;
    thread(lua_State* L, int index = -1) : reference(L, index) {
#ifdef SOL_CHECK_ARGUMENTS
        type_assert(L, index, type::thread);
#endif // Safety
    }

    state_view state() const {
        return state_view(this->thread_state());
    }

    lua_State* thread_state () const {
        auto pp = stack::push_pop(*this);
        lua_State* lthread = lua_tothread(lua_state(), -1);
        return lthread;
    }

    thread_status status () const {
        lua_State* lthread = thread_state();
        thread_status lstat = static_cast<thread_status>(lua_status(lthread));
        if (lstat != thread_status::ok && lua_gettop(lthread) == 0) {
            // No thing on the thread's stack means its dead
            return thread_status::dead;
        }
        return lstat;
    }

    thread create () {
        return create(lua_state());
    }

    static thread create (lua_State* L) {
        lua_newthread(L);
        thread result(L);
        lua_pop(L, 1);
        return result;
    }
};
} // sol

// end of sol\thread.hpp

namespace sol {
class coroutine : public reference {
private:
    call_status stats = call_status::yielded;

    void luacall(std::ptrdiff_t argcount, std::ptrdiff_t) {
#if SOL_LUA_VERSION < 502
        stats = static_cast<call_status>(lua_resume(lua_state(), static_cast<int>(argcount)));
#else
        stats = static_cast<call_status>(lua_resume(lua_state(), nullptr, static_cast<int>(argcount)));
#endif // Lua 5.1 compat
    }

    template<std::size_t... I, typename... Ret>
    auto invoke( types<Ret...>, std::index_sequence<I...>, std::ptrdiff_t n ) {
        luacall(n, sizeof...(Ret));
        return stack::pop<std::tuple<Ret...>>(lua_state());
    }

    template<std::size_t I, typename Ret>
    Ret invoke(types<Ret>, std::index_sequence<I>, std::ptrdiff_t n) {
        luacall(n, 1);
        return stack::pop<Ret>(lua_state());
    }

    template <std::size_t I>
    void invoke(types<void>, std::index_sequence<I>, std::ptrdiff_t n) {
        luacall(n, 0);
    }

    protected_function_result invoke(types<>, std::index_sequence<>, std::ptrdiff_t n) {
        int stacksize = lua_gettop( lua_state() );
        int firstreturn = std::max( 1, stacksize - static_cast<int>( n ) );
        luacall(n, LUA_MULTRET);
        int poststacksize = lua_gettop(lua_state());
        int returncount = poststacksize - (firstreturn - 1);
        return protected_function_result( lua_state( ), firstreturn, returncount, returncount, status() );
    }

public:
    coroutine() noexcept = default;
    coroutine(const coroutine&) noexcept = default;
    coroutine& operator=(const coroutine&) noexcept = default;
    coroutine(lua_State* L, int index = -1) : reference(L, index) {
#ifdef SOL_CHECK_ARGUMENTS
        stack::check<coroutine>(L, index, type_panic);
#endif // Safety
    }

    call_status status() const noexcept {
        return stats;
    }

    bool error() const noexcept {
        call_status cs = status();
        return cs != call_status::ok && cs != call_status::yielded;
    }

    bool runnable () const noexcept {
        return valid() 
            && (status() == call_status::yielded);
    }

    explicit operator bool() const noexcept {
        return runnable();
    }

    template<typename... Args>
    protected_function_result operator()( Args&&... args ) {
        return call<>( std::forward<Args>( args )... );
    }

    template<typename... Ret, typename... Args>
    decltype(auto) operator()( types<Ret...>, Args&&... args ) {
        return call<Ret...>( std::forward<Args>( args )... );
    }

    template<typename... Ret, typename... Args>
    decltype(auto) call( Args&&... args ) {
        push();
        int pushcount = stack::multi_push( lua_state(), std::forward<Args>( args )... );
        return invoke( types<Ret...>( ), std::make_index_sequence<sizeof...(Ret)>(), pushcount );
    }
};
}
// end of sol\coroutine.hpp

#endif // SOL_SINGLE_INCLUDE_HPP
