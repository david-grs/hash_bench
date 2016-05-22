#include "futils.h"

#include <geiger/geiger.h>

#include <map>
#include <unordered_map>
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

template <typename _MapT, typename _KeyT>
_MapT prepare_map(const std::vector<_KeyT>& values)
{
    _MapT m;

    for (const auto& v : values)
        m.insert(std::make_pair(v, 0));
    return m;
}

template <typename _MapT, typename... Args>
void add_to_benchmark(geiger::suite<Args...>& s)
{
    const auto& values = get_dict_words();
    auto m = prepare_map<_MapT>(values);

    char* test_name = __cxxabiv1::__cxa_demangle(typeid(_MapT).name(), nullptr, nullptr, nullptr);
    s.add(test_name, [&values, m = std::move(m)]()
    {
        int sum = 0;
        for (const auto& v : values)
        {
            auto it = m.find(v);
            assert(it != m.end());
            sum += it->second;
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

    s.run();

	return 0;
}

