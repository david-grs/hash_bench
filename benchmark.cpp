#include "futils.h"
#include "hot_set.h"
#include "ht_chained.h"
#include "x_hashmap/HashMap.h"
#include "stx/btree_set.h"

#include <geiger/geiger.h>

#include <set>
#include <unordered_set>
#include <google/dense_hash_set>
#include <google/sparse_hash_set>
#include <boost/container/flat_set.hpp>

#include <cxxabi.h>

const std::vector<std::string>& get_dict_words()
{
    static std::vector<std::string> v;
    if (!v.empty())
        return v;

    io::for_each_line(io::read_all("/etc/dictionaries-common/words"), [&v](const std::string& str)
    {
        v.emplace_back(str);
    });

    return v;
}

template <typename _MapT>
_MapT prepare_map()
{
    _MapT m;
    for (const auto& v : get_dict_words())
        m.insert(v);
    return m;
}

template <>
hov_set<std::string> prepare_map<hov_set<std::string>>()
{
    hov_set<std::string> m;
    for (const auto& v : get_dict_words())
        m.insert(v);
    return m;
}

template <>
rigtorp::HashMap<std::string, int> prepare_map<rigtorp::HashMap<std::string, int>>()
{
    rigtorp::HashMap<std::string, int> m(1, "");
    m.reserve(get_dict_words().size());

    for (const auto& v : get_dict_words())
        m.emplace(v, 0);
    return m;
}

template <>
google::dense_hash_set<std::string> prepare_map<google::dense_hash_set<std::string>>()
{
    google::dense_hash_set<std::string> m;
    m.set_empty_key("");
    m.set_deleted_key("-");

    for (const auto& v : get_dict_words())
        m.insert(v);
    return m;
}

template <typename _MapT>
std::string get_name()
{
    static std::string ret;
    if (!ret.empty())
        return ret;

    char* str = __cxxabiv1::__cxa_demangle(typeid(_MapT).name(), nullptr, nullptr, nullptr);
    ret = str;
    free(str);

    return ret;
}

template <typename _MapT, typename... Args>
void add_insert_test(geiger::suite<Args...>& s)
{
    auto m = prepare_map<_MapT>();
    std::string test_name = std::string("insert: ") + get_name<_MapT>();

    s.add(test_name, [m = std::move(m)]()
    {
        const auto& values = get_dict_words();
        int sum = 0;
        for (const auto& v : values)
        {
            auto it = m.find(v);
            assert(it != m.end());
            sum += m.size();
        }
    });
}

template <typename _MapT, typename... Args>
void add_erase_test(geiger::suite<Args...>& s)
{
    auto m = prepare_map<_MapT>();
    std::string test_name = std::string("delete: ") + get_name<_MapT>();

    s.add(test_name, [mn = std::move(m)]() mutable
    {
        const auto& values = get_dict_words();

        for (const auto& v : values)
            mn.erase(v);
    });
}

int main()
{
    geiger::init();
    geiger::suite<> s;
    s.set_printer<geiger::printer::console<>>();

    add_insert_test<std::set<std::string>>(s);
    add_insert_test<std::unordered_set<std::string>>(s);
    add_insert_test<google::dense_hash_set<std::string>>(s);
    add_insert_test<google::sparse_hash_set<std::string>>(s);
    add_insert_test<boost::container::flat_set<std::string>>(s);
    add_insert_test<stx::btree_set<std::string>>(s);
    add_insert_test<hov_set<std::string>>(s);
    add_insert_test<ht_chained<std::string>>(s);
    add_insert_test<rigtorp::HashMap<std::string, int>>(s);

    /*
    add_erase_test<std::set<std::string>>(s);
    add_erase_test<std::unordered_set<std::string>>(s);
    add_erase_test<google::dense_hash_set<std::string>>(s);
    add_erase_test<boost::container::flat_set<std::string>>(s);
    add_erase_test<hov_set<std::string>>(s);
    add_erase_test<rigtorp::HashMap<std::string, int>>(s);
    */

    s.run();

	return 0;
}

