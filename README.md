# Maked by kockerpro94
# Datastorecpp



DataStore - Configurable C++ Storage Library


DataStore is a flexible C++ storage library that allows developers to choose between different storage strategies on a per-item basis. With support for in-memory storage, LRU cache, and persistent disk storage, you can optimize your application's performance and persistence needs with fine-grained control.

Key Features
Multi-tier storage options:

🚀 Memory Storage: Blazing-fast volatile storage using RAM

⚡ Cache Storage: Configurable LRU cache with automatic eviction

💾 Disk Storage: Persistent file-based storage

Automatic key management across storage types

LRU cache eviction with callback notifications

Thread-safe design (when properly synchronized)

Simple API for storing and retrieving data

Getting Started
Prerequisites
C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)

Standard Library with filesystem support

Installation
Include the header and implementation files in your project:

DataStore.h

DataStore.cpp

Compile with C++17 support:

bash
g++ -std=c++17 -o your_app your_app.cpp DataStore.cpp -lstdc++fs
Usage
Basic Example
cpp
#include "DataStore.h"
#include <iostream>

int main() {
    // Create DataStore with disk directory and cache capacity
    DataStore store("./data", 3);  // 3 items in cache
    
    // Store items with different storage strategies
    store.store("user:123", "{'name':'John'}", DataStore::StorageType::MEMORY);
    store.store("config:theme", "dark", DataStore::StorageType::CACHE);
    store.store("backup:1", "important_data", DataStore::StorageType::DISK);
    
    // Retrieve items
    std::cout << "User: " << store.retrieve("user:123") << "\n";
    std::cout << "Theme: " << store.retrieve("config:theme") << "\n";
    std::cout << "Backup: " << store.retrieve("backup:1") << "\n";
    
    return 0;
}
API Reference
DataStore Class
cpp
// Create DataStore instance
DataStore(const std::string& diskPath = "./data", size_t cacheCapacity = 100);

// Store a value with specified storage strategy
void store(const std::string& key, 
           const std::string& value, 
           StorageType type);

// Retrieve a value
std::string retrieve(const std::string& key);
Storage Types
cpp
enum class StorageType {
    MEMORY,   // Volatile, super-fast RAM storage
    CACHE,    // In-memory LRU cache with eviction
    DISK      // Persistent storage on disk
};
Advanced Features
Cache Eviction Handling
The LRU cache automatically evicts least-recently-used items when capacity is reached. You can extend the class to handle eviction events:

cpp
class CustomDataStore : public DataStore {
public:
    using DataStore::DataStore;
    
    // Override cache eviction behavior
private:
    void handleEviction(const std::string& key) {
        std::cout << "Evicting key: " << key << "\n";
        // Add custom logic here
    }
};
Error Handling
Handle exceptions when accessing data:

cpp
try {
    auto data = store.retrieve("missing_key");
} catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << "\n";
}
Performance Considerations
Storage Type	Speed	Persistence	Best For
MEMORY	⚡⚡⚡	❌	Temporary data, session storage
CACHE	⚡⚡	❌	Frequently accessed data
DISK	⚡	✅	Important data, backups
Best Practices
Use MEMORY for short-lived, high-velocity data

Use CACHE for frequently accessed configuration data

Use DISK for critical data that must survive restarts

Monitor cache hit/miss ratios to optimize cache size

Sanitize keys if using user-provided input for disk storage

Contributing
Contributions are welcome! Please follow these steps:

Fork the repository

Create your feature branch (git checkout -b feature/AmazingFeature)

Commit your changes (git commit -m 'Add some AmazingFeature')

Push to the branch (git push origin feature/AmazingFeature)

Open a Pull Request

License
This project is licensed under the MIT License - see the LICENSE file for details.

Future Work
Add thread safety through mutexes

Implement TTL (Time-To-Live) for cached items

Add data serialization for complex objects

Implement asynchronous disk I/O operations

Add storage statistics and monitoring

DataStore - The right storage, where you need it, when you need it.
