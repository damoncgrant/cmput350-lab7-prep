#include <type_traits>

#ifndef PACK_H
#define PACK_H

namespace pack_detail {
    // Size
    // base case: packSize<> = 0
    template <typename... Args>
    constexpr int packSize = 0;

    template <typename Arg, typename... Args>
    constexpr int packSize<Arg, Args...> = 1 + packSize<Args...>;

    // Index to type
    template <int QueryIndex, typename... Args>
    struct _packIndexToType {
        static_assert(QueryIndex >= 0, "QueryIndex must be non-negative");
        static_assert(QueryIndex < packSize<Args...>, "QueryIndex must be within the type pack");
    };

    // Decrement index until 0
    template <int QueryIndex, typename Arg, typename... Args>
    struct _packIndexToType<QueryIndex, Arg, Args...> {
        static_assert(QueryIndex >= 0, "QueryIndex must be non-negative");
        static_assert(QueryIndex < packSize<Arg, Args...>, "QueryIndex must be within the type pack");
        using type = typename _packIndexToType<QueryIndex - 1, Args...>::type;
    };

    // Index is 0, return first arg
    template <typename Arg, typename... Args>
    struct _packIndexToType<0, Arg, Args...> {
        using type = Arg;
    };

    template <int QueryIndex, typename... Args>
    using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

    // Type to index
    template <int Accumulated, typename QueryType, typename... Args>
    constexpr int _packTypeToIndex = -1; 

    template <int Accumulated, typename QueryType, typename Arg, typename... Args>
    constexpr int _packTypeToIndex<Accumulated, QueryType, Arg, Args...> = std::is_same_v<QueryType, Arg> ? Accumulated :
        _packTypeToIndex<Accumulated + 1, QueryType, Args...>;


    template <typename QueryType, typename... Args> 
    constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;
}

template <typename... Args>
struct Pack {
    constexpr static int size = pack_detail::packSize<Args...>;

    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    template <typename QueryType>
    constexpr static int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};

#endif  // PACK_H
