/*
-------------------------------------------------------------------
                this is for educational purposes only
-------------------------------------------------------------------
*/

#include "DataStore.h"
#include <iostream>

int main() {
    // Initialize with disk directory and cache capacity
    DataStore store("./data", 2);
    
    // Store data with different storage types
    store.store("memory_key", "Fast transient data", DataStore::StorageType::MEMORY);
    store.store("cache_key1", "Cached item 1", DataStore::StorageType::CACHE);
    store.store("cache_key2", "Cached item 2", DataStore::StorageType::CACHE);
    store.store("disk_key", "Persistent data", DataStore::StorageType::DISK);
    
    // Retrieve data
    std::cout << "Memory: " << store.retrieve("memory_key") << "\n";
    std::cout << "Cache1: " << store.retrieve("cache_key1") << "\n";
    std::cout << "Disk: " << store.retrieve("disk_key") << "\n";
    
    // Demonstrate cache eviction
    store.store("cache_key3", "New cached item", DataStore::StorageType::CACHE);
    
    // Try to retrieve evicted key
    try {
        std::cout << "Cache2: " << store.retrieve("cache_key2") << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
