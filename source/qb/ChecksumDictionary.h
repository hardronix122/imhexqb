#ifndef CHECKSUMDICTIONARY_H
#define CHECKSUMDICTIONARY_H

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <map>

class ChecksumDictionary {
public:
    ChecksumDictionary();
    explicit ChecksumDictionary(const std::map<uint64_t, std::string> &dictionary);

    static std::string getDictionaryPath();
    static ChecksumDictionary load(const std::string& path);
    void save(const std::string& path);

    void populate(uint64_t value, const std::string& name);
    bool contains(uint64_t value);
    std::string resolve(uint64_t checksum);
private:
    std::map<uint64_t, std::string> dictionary;
};


#endif //CHECKSUMDICTIONARY_H
