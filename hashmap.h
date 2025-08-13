//
//  hashmap.h
//  CS32 Project 4
//
//  Created by Jason Schacher on 3/12/24.
//

#ifndef hashmap_h
#define hashmap_h

#include <string>
#include <functional>
#include <cstddef>
using namespace std;

template <typename T>
class HashMap
{
public:
 HashMap(double max_load = 0.75); // constructor
 ~HashMap(); // destructor; deletes all of the items in the hashmap
 int size() const; // return the number of associations in the hashmap
 // The insert method associates one item (key) with another (value).
 // If no association currently exists with that key, this method inserts
 // a new association into the hashmap with that key/value pair. If there is
 // already an association with that key in the hashmap, then the item
 // associated with that key is replaced by the second parameter (value).
 // Thus, the hashmap must contain no duplicate keys.
 void insert(const std::string& key, const T& value);
 // Defines the bracket operator for HashMap, so you can use your map like this:
 // your_map["david"] = 2.99;
 // If the key does not exist in the hashmap, this will create a new entry in
 // the hashmap and map it to the default value of type T (0 for builtin types).
 // It returns a reference to the newly created value in the map.
 T& operator[](const std::string& key);
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value within the map.
 const T* find(const std::string& key) const;
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value or modify it directly within the map.
 T* find(const std::string& key) {
 const auto& hm = *this;
 return const_cast<T*>(hm.find(key));
 }

 HashMap(const HashMap&) = delete;
 HashMap& operator=(const HashMap&) = delete;
    
private:
    struct Node {
        string key;
        T value;
        Node* next;
        Node(const string& k, const T& v) : key(k), value(v), next(nullptr) {}
    };
    Node** buckets;
    int itemCount;
    int numBuckets;
    double maxLoadFactor;
    
    void rehash() {
        int newSize = numBuckets * 2;
        Node** newBuckets = new Node*[newSize]();
        for (int i = 0; i < numBuckets; i++) {
            Node* curr = buckets[i];
            while (curr != nullptr) {
                size_t hash = std::hash<string>()(curr->key);
                int newBucket = hash % newSize;
                Node* temp = curr;
                curr = curr->next;
                temp->next = newBuckets[newBucket];
                newBuckets[newBucket] = temp;
            }
        }
        delete [] buckets;
        numBuckets = newSize;
        buckets = newBuckets;
    }
};

template <typename T>
HashMap<T>::HashMap(double max_load) : itemCount(0), numBuckets(10), maxLoadFactor(max_load) {
    if (max_load <= 0) {
        maxLoadFactor = 0.75;
    }
    buckets = new Node*[numBuckets];
    for (int i = 0; i < numBuckets; i++) {
        buckets[i] = nullptr;
    }
}

template <typename T>
HashMap<T>::~HashMap() {
    for (int i = 0; i < numBuckets; i++) {
        Node* curr = buckets[i];
        while (curr != nullptr) {
            Node* kill = curr;
            curr = curr->next;
            delete kill;
        }
    }
    delete [] buckets;
}

template <typename T>
int HashMap<T>::size() const {
    return itemCount;
}

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value) {
    size_t hash = std::hash<string>()(key);
    int bucket = hash % numBuckets;
    Node* newNode = new Node(key, value);
    if (buckets[bucket] == nullptr) {
        buckets[bucket] = newNode;
    }
    else {
        Node* curr = buckets[bucket];
        while (curr->next != nullptr) {
            if (curr->key == key) {
                curr->value = value;
                delete newNode;
                return;
            }
            curr = curr->next;
        }
        curr->next = newNode;
    }
    itemCount++;
    int loadFactor = static_cast<double>(itemCount) / numBuckets;
    if (loadFactor > maxLoadFactor) {
        rehash();
    }
}

template <typename T>
T& HashMap<T>::operator[](const std::string& key) {
    size_t hash = std::hash<string>()(key);
    int bucket = hash % numBuckets;
    Node* curr = buckets[bucket];
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr->value;
        }
        curr = curr->next;
    }
    T defaultValue = T();
    insert(key, defaultValue);
    return buckets[bucket]->value;
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const {
    size_t hash = std::hash<string>()(key);
    int bucket = hash % numBuckets;
    Node* curr = buckets[bucket];
    while (curr != nullptr) {
        if (curr->key == key) {
            return &(curr->value);
        }
        curr = curr->next;
    }
    return nullptr;
}

#endif /* hashmap_h */
