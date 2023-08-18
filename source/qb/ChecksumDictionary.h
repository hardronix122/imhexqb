#ifndef CHECKSUMDICTIONARY_H
#define CHECKSUMDICTIONARY_H

#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <map>

class ChecksumDictionary {
public:
    ChecksumDictionary();
    explicit ChecksumDictionary(const std::map<uint32_t, std::string> &dictionary);

    static std::string getDictionaryPath();
    static ChecksumDictionary load(const std::string& path);
    void save(const std::string& path);

    void populate(uint32_t value, std::string name);
    bool contains(uint32_t value);
    std::string resolve(uint32_t checksum);
private:
    std::map<uint32_t, std::string> dictionary;
};


#endif //CHECKSUMDICTIONARY_H
