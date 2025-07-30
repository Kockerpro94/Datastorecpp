/*
-------------------------------------------------------------------
                this is for educational purposes only
-------------------------------------------------------------------
*/


#include "DataStore.h"
#include <fstream>
#include <iostream>

// LRUCache Implementation
DataStore::LRUCache::LRUCache(size_t capacity, 
                              std::function<void(const std::string&)> onEvict)
    : capacity_(capacity), onEvict_(std::move(onEvict)) {}

void DataStore::LRUCache::put(const std::string& key, const std::string& value) {
    if (auto it = map_.find(key); it != map_.end()) {
        items_.erase(it->second.second);
        items_.push_front({key, value});
        it->second = {value, items_.begin()};
    } else {
        if (items_.size() >= capacity_) {
            evict();
        }
        items_.push_front({key, value});
        map_[key] = {value, items_.begin()};
    }
}

std::string DataStore::LRUCache::get(const std::string& key) {
    if (auto it = map_.find(key); it != map_.end()) {
        items_.splice(items_.begin(), items_, it->second.second);
        return it->second.first;
    }
    throw std::runtime_error("Key not in cache: " + key);
}

void DataStore::LRUCache::remove(const std::string& key) {
    if (auto it = map_.find(key); it != map_.end()) {
        items_.erase(it->second.second);
        map_.erase(it);
    }
}

void DataStore::LRUCache::evict() {
    const auto& [key, _] = items_.back();
    map_.erase(key);
    items_.pop_back();
    if (onEvict_) onEvict_(key);
}

// DataStore Implementation
DataStore::DataStore(const std::string& diskPath, size_t cacheCapacity)
    : diskDir_(diskPath),
      cache_(cacheCapacity, [this](const std::string& key) {
          keyLocation_.erase(key);
      }) {
    if (!fs::exists(diskDir_)) {
        fs::create_directories(diskDir_);
    }
}

void DataStore::store(const std::string& key, 
                     const std::string& value, 
                     StorageType type) {
    if (auto it = keyLocation_.find(key); it != keyLocation_.end()) {
        removeFromStorage(key, it->second);
    }

    switch (type) {
        case StorageType::MEMORY:
            memoryStore_[key] = value;
            break;
        case StorageType::CACHE:
            cache_.put(key, value);
            break;
        case StorageType::DISK:
            storeOnDisk(key, value);
            break;
    }
    keyLocation_[key] = type;
}

std::string DataStore::retrieve(const std::string& key) {
    if (auto it = keyLocation_.find(key); it != keyLocation_.end()) {
        switch (it->second) {
            case StorageType::MEMORY:
                return memoryStore_.at(key);
            case StorageType::CACHE:
                return cache_.get(key);
            case StorageType::DISK:
                return retrieveFromDisk(key);
        }
    }
    throw std::runtime_error("Key not found: " + key);
}

void DataStore::removeFromStorage(const std::string& key, StorageType type) {
    switch (type) {
        case StorageType::MEMORY:
            memoryStore_.erase(key);
            break;
        case StorageType::CACHE:
            cache_.remove(key);
            break;
        case StorageType::DISK:
            fs::remove(getDiskPath(key));
            break;
    }
}

void DataStore::storeOnDisk(const std::string& key, const std::string& value) {
    std::ofstream file(getDiskPath(key), std::ios::binary);
    file.write(value.data(), value.size());
}

std::string DataStore::retrieveFromDisk(const std::string& key) {
    std::ifstream file(getDiskPath(key), std::ios::binary);
    if (!file) throw std::runtime_error("Disk read failed: " + key);
    return {std::istreambuf_iterator<char>(file), {}};
}

std::string DataStore::getDiskPath(const std::string& key) const {
    return (diskDir_ / key).string();
}
