#ifndef _STDLIB_UNORDERED_MAP_HPP
#define _STDLIB_UNORDERED_MAP_HPP

#include <RHHT>
#include <pair>

namespace std {
	template<typename K, typename V>
	class unordered_map {
	private:
		typedef typename std::pair<K, V> KV;
		typename std::RHHT<KV> data;

	public:
		typedef typename std::RHHT<KV>::iterator iterator;
		typedef typename std::RHHT<KV>::const_iterator const_iterator;

		inline iterator begin() { return data.begin(); }
		inline iterator end() { return data.end(); }
		inline const_iterator cbegin() const { return data.cbegin(); }
		inline const_iterator cend() const { return data.cend(); }
		inline const_iterator begin() const { return cbegin(); }
		inline const_iterator end() const { return cend(); }

		inline iterator find(const K& key) {
			return data.find(KV(key, V()));
		}
		inline const_iterator find(const K& key) const {
			return data.find(KV(key, V()));
		}

		inline V& operator[](const K& key) {
			iterator it = find(key);
			if(it != end())
				return (*it).s;

			it = data.add(KV(key, V()));
			return (*it).s;
		}

		inline bool has(const K& key) const {
			return data.has(KV(key, V()));
		}

		inline size_t size() const { return data.size(); }
	};
}

#endif
