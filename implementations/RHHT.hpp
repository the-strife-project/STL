#ifndef _STDLIB_RHHT_H
#define _STDLIB_RHHT_H

#include <types>
#include <utility>
#include <algorithm>
#include <vector>
#include <functional>

// Welcome to the Robin Hood Hash Table
// The generic version of kernel's HT64

namespace std {
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
			typename std::vector<Bucket>::iterator it, itend;
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
				if(it == itend)
					return *this;
				while(++it != itend) {
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
			typename std::vector<Bucket>::const_iterator it, itend;
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
				if(it == itend)
					return *this;
				while(++it != itend) {
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

			filledBuckets = 0;
			for(auto const& x : odata)
				add(x.val);
		}

		const Bucket* lookup(const T& val) const {
			// We don't want "% 0".
			if(!data.size())
				return nullptr;

			Hash h = _hash(val);
			Hash origh = h;

			bool first = true;
			size_t psl = 0;
			while(first || h != origh) {
				if(!data[h].filled || data[h].psl > psl)
					return nullptr;

				if(data[h].val == val)
					return &data[h];

				if(first) first = false;
				if(++h == data.size())
					h = 0;

				++psl;
			}

			// In some very specific occasions, second pass might not find
			//   an empty bucket.
			return nullptr;
		}

		inline Hash _hash(const T& val) const {
			// Hashing is not handled here, go read <hash>
			return std::hash<T>()(val) % data.size();
		}

	public:
		iterator add(const T& val) {
			if(!data.size()) {
				more();
			} else {
				size_t used = (filledBuckets * 100) / data.size();
				if(used >= MAX_USAGE)
					more();
			}

			++filledBuckets;

			Hash h = _hash(val);
			Hash origh = h;

			// Nope. Here we go
			bool retset = false;
			iterator ret;
			Bucket bucket(val);

			bool first = true;
			while(first || h != origh) {
				if(!data[h].filled) {
					data[h] = bucket;
					if(!retset)
						ret = iterator(&data[h], data.end());
					return ret;
				}

				// No double insertions!
				if(data[h].val == bucket.val) {
					// This can only happen when bucket has not been swapped
					--filledBuckets; // Roll back!
					return iterator(&data[h], data.end());
				}

				if(first) first = false;

				// Used. Compare PSLs
				if(bucket.psl > data[h].psl) {
					if(!retset) {
						ret = iterator(&data[h], data.end());
						retset = true;
					}

					std::swap(bucket, data[h]);
				}

				if(++h == data.size())
					h = 0;

				++bucket.psl;
			}

			// This can't happen
			*(uint64_t*)0x12345 = 0;
			return end();
		}

		inline iterator insert(const T& val) { return add(val); }

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
