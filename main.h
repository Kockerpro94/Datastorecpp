/*
-------------------------------------------------------------------
                this is for educational purposes only
-------------------------------------------------------------------
*/


#ifndef DATASTORE_H
#define DATASTORE_H

#include <filesystem>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <stdexcept>

namespace fs = std::filesystem;

class DataStore {
public:
    enum class StorageType {
        MEMORY,   // Volatile, super-fast RAM storage
        CACHE,    // In-memory LRU cache with eviction
        DISK      // Persistent storage on disk
    };

    DataStore(const std::string& diskPath = "./data", size_t cacheCapacity = 100);
    
    void store(const std::string& key, const std::string& value, StorageType type);
    std::string retrieve(const std::string& key);

private:
    // Nested LRU Cache Class
    class LRUCache {
    public:
        LRUCache(size_t capacity, std::function<void(const std::string&)> onEvict);
        
        void put(const std::string& key, const std::string& value);
        std::string get(const std::string& key);
        void remove(const std::string& key);

    private:
        void evict();
        
        size_t capacity_;
        std::list<std::pair<std::string, std::string>> items_;
        using ListIt = typename decltype(items_)::iterator;
        std::unordered_map<std::string, std::pair<std::string, ListIt>> map_;
        std::function<void(const std::string&)> onEvict_;
    };

    // Helper methods
    void removeFromStorage(const std::string& key, StorageType type);
    void storeOnDisk(const std::string& key, const std::string& value);
    std::string retrieveFromDisk(const std::string& key);
    std::string getDiskPath(const std::string& key) const;

    // Data members
    fs::path diskDir_;
    std::unordered_map<std::string, std::string> memoryStore_;
    LRUCache cache_;
    std::unordered_map<std::string, StorageType> keyLocation_;
};

#endif // DATASTORE_H
