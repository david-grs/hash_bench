#include "futils.h"
#include "hot_set.h"

#include <geiger/geiger.h>

#include <map>
#include <unordered_map>
#include <google/dense_hash_map>

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
        m.insert(std::make_pair(v, 0));
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

template <typename _MapT, typename... Args>
void add_to_benchmark(geiger::suite<Args...>& s)
{
    auto m = prepare_map<_MapT>();

    char* test_name = __cxxabiv1::__cxa_demangle(typeid(_MapT).name(), nullptr, nullptr, nullptr);
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
    free(test_name);
}

int main()
{
    geiger::init();
    geiger::suite<> s;
    s.set_printer<geiger::printer::console<>>();

    add_to_benchmark<std::map<std::string, int>>(s);
    add_to_benchmark<std::unordered_map<std::string, int>>(s);
    //add_to_benchmark<google::dense_hash_map<std::string, int>>(s);
    add_to_benchmark<hov_set<std::string>>(s);

    s.run();

	return 0;
}

