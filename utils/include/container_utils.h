
//
// Created by zlz on 2016/1/11.
//

#ifndef ZUTILS_CONTAINER_UTILS_H
#define ZUTILS_CONTAINER_UTILS_H

#include "common.h"

ZUTIL_NAMESPACE_BEGIN

class ContainerUtils {
public:
    template <typename Container1, typename Container2>
    static bool CompareContainer(const Container1& container1, const Container2& container2) {
        if (container1.size() > container2.size()) {
            return std::equal(container1.begin(), container1.end(), container2.begin());
        } else {
            return std::equal(container2.begin(), container2.end(), container1.begin());
        }
    }

    /*
     * first container can be any iteratable container
     * second container must be list/vector
     */
    template <typename Container1, typename Container2>
    static void CopyContainer(const Container1& c1, Container2& c2) {
        for (auto it = c1.begin(); it != c1.end(); it++) {
            c2.push_back(*it);
        }
    };

    /*
     * free all values in a sequence container (list, vector, set, etc.) and clear it
     */
    template <typename SeqContainer>
    static void ClearSequenceContainer(SeqContainer & container) {
        for (auto it = container.begin(); it != container.end(); it++) {
            delete *it;
        }
        container.clear();
    }

    /*
     * free all values in a map and clear it
     */
    template <typename MapContainer>
    static void ClearMapContainer(MapContainer& container) {
        for (auto it = container.begin(); it != container.end(); it++) {
            delete it->second;
        }
        container.clear();
    }

    /*
     * find the value in a map by key
     * return true if found and v should be the value
     * return false if not found and v is not changed
     */
    template <typename Key, typename Value>
    static bool Find(const std::map<Key, Value>& container, const Key& k, Value& v) {
        auto it = container.find(k);
        if (it != container.end()) {
            v = it->second;
            return true;
        }
        return false;
    };

    /*
     * test whether a sequence container (list, vector) contains a value
     */
    template <typename Container, typename T>
    static bool Contains(const Container& container, const T& x) {
        return std::find(container.begin(), container.end(), x) != container.end();
    };
};


ZUTIL_NAMESPACE_END



#endif //ZUTILS_CONTAINER_UTILS_H
