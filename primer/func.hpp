//
//  func.hpp
//  primer
//
//  Created by liweijian on 2017/2/7.
//  Copyright © 2017年 liweijian. All rights reserved.
//

#ifndef func_hpp
#define func_hpp

//将一个函数转换为函数对象
#define define_functor_type(func_name)                                                          \
class tnf_##func_name {                                                                         \
public:                                                                                         \
    template<typename... Args>                                                                  \
    auto operator()(Args&&... args) const -> decltype(func_name(std::forward<Args>(args)...))   \
    {                                                                                           \
        return func_name(std::forward<Args>(args)...);                                          \
    }                                                                                           \
}                                                                                               \

#define make_global_functor(NAME, F)  define_functor_type(F); const auto NAME = F;

auto add(int a, int b)
{
    return a+b;
}

auto add_one(int a)
{
    return 1+a;
}


//惰性求值
template <typename F, size_t... I, typename ...Args>
inline auto  tuple_apply_impl(const F& f, const std::index_sequence<I...>&, const std::tuple<Args...>& tp)
{
    
}

#endif /* func_hpp */
