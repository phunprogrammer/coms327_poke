#include <vector>
#include <map>

#ifndef MAPVECTOR_H
#define MAPVECTOR_H

template<typename K, typename V>
class MapVector {
    private:
        std::map<K, V> map;
        std::vector<V> vector;
    public:
        MapVector(std::initializer_list<std::pair<K, V>> initVector) {
            for (const auto& pair : initVector)
                insert(pair.first, pair.second);
        }
        void insert(const K& key, const V& value) {
            map[key] = value;
            vector.push_back(value);
        }

        const V& operator[](const K& key) const { return map.at(key); }

        const V& operator[](std::size_t index) const { return vector.at(index); }

        std::size_t size() const { return vector.size(); }
};

#endif