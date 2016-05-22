#pragma once

#include <fstream>
#include <string>
#include <algorithm>

namespace io
{

std::string read_all(const std::string& filename)
{
	enum { BufferSize = 1024 };
	char buff[BufferSize];

	std::string str;
	std::ifstream ifs(filename);
	while (std::streamsize bytes = ifs.readsome(buff, BufferSize))
		str.append(buff, bytes);

	return str;
}

template <typename F>
void for_each_line(const std::string& str, F&& f)
{
    auto bol = str.begin();

    for (auto eol = std::find(bol, str.end(), '\n');
            eol != str.end();
            bol = eol + 1, eol = std::find(bol, str.end(), '\n'))
    {
        f (std::string(bol, eol));
    }
};

}

