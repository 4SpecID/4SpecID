#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <iterator>
#include <memory>

#define PRINT(X) std::cout << X << std::endl;
#define PRINTV(X) for(auto x : X){ std::cout << x << " "; }  std::cout << std::endl;
#define STRICT_MODE_OFF                                                                 \
    _Pragma("GCC diagnostic push")                                            \
    _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")             \
    _Pragma("GCC diagnostic ignored \"-Wdelete-non-virtual-dtor\"") \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")        \
    _Pragma("GCC diagnostic ignored \"-Wshadow\"")                  \
    _Pragma("GCC diagnostic ignored \"-Wold-style-cast\"")          \
    _Pragma("GCC diagnostic ignored \"-Wundef\"")          \
    _Pragma("GCC diagnostic ignored \"-Wswitch-default\"")

/* Addition options that can be enabled 
    _Pragma("GCC diagnostic ignored \"-pedantic\"")                 \
    _Pragma("GCC diagnostic ignored \"-Wpedantic\"")                \
    _Pragma("GCC diagnostic ignored \"-Wformat=\"")                 \
    _Pragma("GCC diagnostic ignored \"-Werror\"")                   \
    _Pragma("GCC diagnostic ignored \"-Werror=\"")                  \
    _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")         \
*/
              
#define STRICT_MODE_ON                                                                  \
    _Pragma("GCC diagnostic pop")  




    
namespace utils{
    enum class Grade{
        A, B, C, D, E1, E2, U
    };


    template <class T>
    T argParse(int argc, char *argv[], std::string prefix, T defaultValue)
    {
        for (int i = 0; i < argc; ++i){
            std::string argument(argv[i]);
            if(argument.substr(0, prefix.size()) == prefix) {
                std::string argument_value = argument.substr(prefix.size());
                std::istringstream ss(argument_value);
                T value;
                ss >> value;
                return value;
            }
        }
        return defaultValue;
    }

    template<class K, class V>
    bool hasIntersection(std::unordered_map<K,V>& o1, std::unordered_map<K,V>& o2){
        size_t o1s = o1.size();
        size_t o2s = o2.size();
        if(o1s <= o2s){
            auto o2e = o2.end();
            for (auto item : o1){ //= o1.begin(); i != o1.end(); i++) {
                if(o2.find(item.first) != o2e) return true;
            }
        }else{
            auto o1e = o1.end();
            for (auto item : o2){ //= o1.begin(); i != o1.end(); i++) {
                if(o1.find(item.first) != o1e) return true;
            }
        }
        return false;
    }

    template<class K>
    bool hasIntersection(const std::unordered_set<K>& o1, const std::unordered_set<K>& o2){
        size_t o1s = o1.size();
        size_t o2s = o2.size();
        if(o1s <= o2s){
            auto o2e = o2.end();
            for (auto item : o1){ //= o1.begin(); i != o1.end(); i++) {
                if(o2.find(item) != o2e) return true;
            }
        }else{
            auto o1e = o1.end();
            for (auto item : o2){ //= o1.begin(); i != o1.end(); i++) {
                if(o1.find(item) != o1e) return true;
            }
        }
        return false;
    }

    template<class Key, class Value>
    using Aggregation  = std::unordered_map<Key, Value>;

    template<class Item>
    using Table = std::vector<Item>;


    template<class Item, class Predicate>
    Table<Item> filter(Table<Item>& tbl, Predicate pred){
        Table<Item> accept;
        Table<Item> filtered;
        for(auto& item : tbl){
            if(!pred(item)){
                accept.push_back(std::move(item));
            }else{
                filtered.push_back(std::move(item));
            }
        }
        return accept;
    }

    template<class Item, class Predicate>
    Table<Item> filter(Table<Item>& tbl, Predicate pred, Table<Item>& filtered){
        Table<Item> accept;
        for(auto& item : tbl){
            if(!pred(item)){
                accept.push_back(std::move(item));
            }else{
                filtered.push_back(std::move(item));
            }
        }
        return accept;
    }

    template<class Item, class Key, class Value>
    Aggregation<Key, Value> group(Table<Item>& tbl, Key get_key(Item&), void join_op(Value&, Item&), Value from_item(Item&)){
        Aggregation<Key, Value> result;
        for (auto& item : tbl)
        {
            Key item_key = get_key(item);
            auto value = result.find(item_key);
            if(value != result.end()){
                Value& current_value = (*value).second;
                join_op(current_value, item);
            }else{
                Value new_value = from_item(item);
                result.insert({item_key, new_value});
            }
        }
        return result;
    }
}
#endif
