#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename K>//, typename V>
struct ht_chained
{
	typedef K key_type;
	//typedef V mapped_type;
	//typedef std::pair<K, V> value_type;
	typedef int size_type;

	struct bucket
	{
		//typedef std::pair<K, V> value_type;
        typedef K value_type;
        typedef typename std::vector<value_type>::size_type size_type;

		//V& insert(const K& k)
        void insert(const K& k)
        {
 //           auto it = std::find_if(m_values.begin(), m_values.end(), [&k](value_type& p) { return p.first == k; });
            auto it = std::find_if(m_values.begin(), m_values.end(), [&k](const value_type& p) { return p == k; });
            if (it == m_values.end())
            {
                //m_values.emplace_back(k, V());
                m_values.emplace_back(k);
                it = m_values.end() - 1;
            }

            //return it->second;
        }

        bool find(const K& k) const
        {
            auto it = std::find_if(m_values.begin(), m_values.end(), [&k](const value_type& p) { return p == k; });
            return it != m_values.end();
        }

		bool full() const { return m_values.size() == 8; }
		size_type size() const { return m_values.size(); }

		std::vector<value_type> m_values;
	};

	std::vector<bucket> m_buckets;
	double m_growing_factor;

public:
	ht_chained(double growing_factor = 2.0)
	: m_buckets(32),
	  m_growing_factor(growing_factor)
	{}

    void operator[](const K& k)
    {
        insert(m_buckets, k);
    }

    void insert(const K& k)
    {
        insert(m_buckets, k);
    }

    bool find(const K& k) const
    {
        std::size_t s = std::hash<K>()(k);
        const bucket& b = m_buckets[s % m_buckets.size()];
        return b.find(k);
    }

	size_type size() const { return std::accumulate(m_buckets.begin(), m_buckets.end(), 0, [](size_type s, const bucket& b) { return s + b.size(); }); }
	size_type buckets() const { return m_buckets.size(); }

	template <typename F>
	void visit(F&& f)
	{
		std::size_t bck_idx = 0;
		for (bucket& b : m_buckets)
		{
			for (auto& p : b.m_values)
				f(bck_idx, p);//, p.second);

			++bck_idx;
		}
	}

private:
	//V& insert(std::vector<bucket>& buckets, const K& k)
    void insert(std::vector<bucket>& buckets, const K& k)
    {
        std::size_t s = std::hash<K>()(k);
        bucket& b = buckets[s % buckets.size()];

        if (b.full())
        {
            expand(buckets);
            return insert(buckets, k);
        }

        return b.insert(k);
    }

	void expand(std::vector<bucket>& buckets)
	{
		std::vector<bucket> newbuckets(buckets.size() * m_growing_factor);

		for (bucket& b : buckets)
			for (auto& p : b.m_values)
				insert(newbuckets, p);// = p.second;

		buckets.swap(newbuckets);
	}
};
