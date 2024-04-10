#include <vector>
#include <map>
#include <algorithm>
#include "AbstractTiles.h"

#ifndef MAPVECTOR_H
#define MAPVECTOR_H

static EntityTile* NULL_ENTITY_PTR;

template<typename K, typename V>
class MapVector {
    private:
        std::map<K, V> map;
        std::vector<V> vector;
    public:
        MapVector() {}

        MapVector(std::initializer_list<std::pair<K, V>> initVector) {
            for (const auto& pair : initVector)
                insert(pair.first, pair.second);
        }

        void remove(const K& key) {
            auto it = map.find(key);
            map.erase(it);
            auto vecIt = std::find(vector.begin(), vector.end(), it->second);
            vector.erase(vecIt);
        }

        void insert(const K& key, const V& value) {
            map[key] = value;
            vector.push_back(value);
        }

        void move(const K& oldKey, const K& newKey) {
            if (map.count(oldKey) == 0)
                return;

            auto nodeHandler = map.extract(oldKey);
            nodeHandler.key() = newKey;
            map.insert(std::move(nodeHandler));
        }

        const V& operator[](const K& key) const {
            return map.count(key) ? map.at(key) : NULL_ENTITY_PTR; 
        }

        const V& operator[](std::size_t index) const { return vector.at(index); }

        std::size_t size() const { return vector.size(); }
        std::map<K, V> getMap() const { return map; }
        std::vector<V> getVector() const { return vector; }
};

#endif