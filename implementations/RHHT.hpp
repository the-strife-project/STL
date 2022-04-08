#ifndef _STDLIB_RHHT_H
#define _STDLIB_RHHT_H

#include <types>
#include <utility>
#include <algorithm>

// Welcome to the Robin Hood Hash Table
// The generic version of kernel's HT64

namespace std {
	// Hashes of atomic types
	typedef size_t Hash;
	// This should include pretty much all ints
	inline Hash hash(size_t val) { return val; }
	// And this should include all pointers
	inline Hash hash(void* val) { return (size_t)val; }

	// MAX_USAGE is given in percentage (default is 80%)
	template<typename T, size_t MAX_USAGE=80> class RHHT {
	private:
		struct Bucket {
			bool filled = false;
			T val;
			size_t psl = 0;

			Bucket() = default;
			Bucket(const T& val_)
				: filled(true), val(val_)
			{}
		};

		std::vector<Bucket> data;
		size_t filledBuckets = 0;

	public:
		// --- ITERATORS ---

		class iterator {
		private:
			typename std::vector<Bucket>::iterator it;
			typename std::vector<Bucket>::iterator itend;
			iterator(typename std::vector<Bucket>::iterator it_,
					 typename std::vector<Bucket>::iterator itend_)
			{
				it = it_;
				itend = itend_;
				if(it != itend && !(*it).filled)
					++*this;
			}
			friend class RHHT;

		public:
			iterator() {}

			inline iterator& operator++() {
				// Keep going forward until filled or data.end()
				while(it != itend) {
					++it;
					if((*it).filled)
						break;
				}
				return *this;
			}

			inline iterator operator++(int) {
				iterator ret = *this;
				++*this;
				return ret;
			}

			// Be very careful to not change the hash while referencing
			// I think it can't mess it up, but it WILL make everything slower
			inline T& operator*() { return (*it).val; }
			inline bool operator==(const iterator& o) const { return it == o.it; }
			inline bool operator!=(const iterator& o) const { return it != o.it; }
		};

		class const_iterator {
		private:
			typename std::vector<Bucket>::const_iterator it;
			typename std::vector<Bucket>::const_iterator itend;
			const_iterator(typename std::vector<Bucket>::const_iterator it_,
						   typename std::vector<Bucket>::const_iterator itend_)
			{
				it = it_;
				itend = itend_;
				if(it != itend && !(*it).filled)
					++*this;
			}
			friend class RHHT;

		public:
			const_iterator() {}

			inline const_iterator& operator++() {
				while(it != itend) {
					++it;
					if((*it).filled)
						break;
				}
				return *this;
			}

			inline const_iterator operator++(int) {
				const_iterator ret = *this;
				++*this;
				return ret;
			}

			inline const T& operator*() const { return (*it).val; }
			inline bool operator==(const const_iterator& o) const { return it == o.it; }
			inline bool operator!=(const const_iterator& o) const { return it != o.it; }
		};

		inline iterator begin() { return iterator(data.begin(), data.end()); }
		inline iterator end() { return iterator(data.end(), data.end()); }
		inline const_iterator cbegin() const { return const_iterator(data.cbegin(), data.cend()); }
		inline const_iterator cend() const { return const_iterator(data.cend(), data.cend()); }
		inline const_iterator begin() const { return cbegin(); }
		inline const_iterator end() const { return cend(); }

	private:
		void more() {
			if(!data.size()) {
				// Perform first allocation
				data.resize(1);
				return;
			}

			// Allocate twice the number of buckets
			typename std::vector<Bucket> odata = std::move(data);
			data = typename std::vector<Bucket>();
			data.resize(odata.size() * 2);

			for(auto const& x : odata)
				add(x.val);
		}

		const Bucket* lookup(const T& val) const {
			// We don't want "% 0".
			if(!data.size())
				return nullptr;

			Hash h = _hash(val);
			Hash origh = h;

			if(data[h].val == val)
				return &data[h];
			++h;

			size_t psl = 1;
			while(true) {
				if(data[h].val == val)
					return &data[h];

				// Two cases: empty or higher PSL
				if(!data[h].filled || data[h].psl > psl)
					return nullptr;

				// Two passes, same as add()
				if(h == origh) {
					break;
				} else if(h >= data.size()-1) {
					// First pass complete
					h = 0;
				} else {
					++h;
				}

				++psl;
			}

			// In some very specific occasions, second pass might not find
			//   an empty bucket.
			return nullptr;
		}

		inline Hash _hash(const T& val) const {
			// Assuming this function exists
			return hash(val) % data.size();
		}

	public:
		void add(const T& val) {
			if(!data.size()) {
				more();
			} else {
				size_t used = (filledBuckets * 100) / data.size();
				if(used >= MAX_USAGE)
					more();
			}

			Hash h = _hash(val);
			Hash origh = h;

			// Is the optimal bucket free?
			if(!data[h].filled) {
				data[h] = Bucket(val);
				return;
			}

			// Nope. Here we go
			Bucket bucket(val);
			++h;
			++bucket.psl;
			while(true) {
				if(!data[h].filled) {
					data[h] = bucket;
					return;
				}

				// Used. Compare PSLs
				if(bucket.psl > data[h].psl)
					std::swap(bucket, data[h]); // Swap!

				// Two passes:
				//   First, from h to the end
				//   Then, from the beginning to h
				if(h == origh) {
					// Second pass complete
					break;
				} else if(h >= data.size()-1) {
					// First pass complete, start the second
					h = 0;
				} else {
					++h;
				}

				++bucket.psl;
			}

			// This can't happen
			*(uint64_t*)0x12345 = 0;
		}

		inline iterator find(const T& val) {
			// Some black magic due to std::vector implementation
			// This is very very ugly, I know
			Bucket* ptr = const_cast<Bucket*>(lookup(val));
			if(!ptr)
				return end();
			return iterator(ptr, data.end());
		}

		inline const_iterator find(const T& val) const {
			// This is just the black magic without the ugly
			const Bucket* ptr = lookup(val);
			if(!ptr)
				return cend();
			return const_iterator(ptr, data.cend());
		}

		inline bool has(const T& val) const { return find(val) != cend(); }

		//void erase(const K& key) {}

		inline size_t size() const { return filledBuckets; }
	};
};

#endif
