#include "ChecksumDictionary.h"
#include <hex/helpers/fs.hpp>

ChecksumDictionary::ChecksumDictionary() {
    dictionary = std::map<uint32_t, std::string>();
}

std::string ChecksumDictionary::getDictionaryPath() {
    std::filesystem::path basePath = hex::fs::getDefaultPaths(hex::fs::ImHexPath::Config, false)[0];
    std::filesystem::path config = "imhexqb_dictionary.json";
    std::filesystem::path configPath = basePath / config;

    return configPath.string();
}

ChecksumDictionary ChecksumDictionary::load(const std::string& path) {
    std::ifstream data(path);

    if (!data.is_open()) {
        ChecksumDictionary dict;
        dict.save(getDictionaryPath());

        return dict;
    }

    std::stringstream buf;
    buf << data.rdbuf();

    data.close();

    nlohmann::basic_json json = nlohmann::json::parse(buf.str());

    if (!json.contains("checksums")) {
        json["checksums"] = std::map<uint32_t, std::string>();
    }

    std::map<uint32_t, std::string> dict = json["checksums"];

    return ChecksumDictionary(dict);
}

void ChecksumDictionary::save(const std::string& path) {
    nlohmann::basic_json json;

    json["checksums"] = dictionary;
    std::ofstream data(path);
    data << json;

    data.close();
}

void ChecksumDictionary::populate(uint32_t value, std::string name) {
    dictionary.insert({value, name});
}

bool ChecksumDictionary::contains(uint32_t value) {
    return dictionary.contains(value);
}

std::string ChecksumDictionary::resolve(uint32_t checksum) {
    return dictionary[checksum];
}

ChecksumDictionary::ChecksumDictionary(const std::map<uint32_t, std::string> &dictionary) : dictionary(dictionary) {}