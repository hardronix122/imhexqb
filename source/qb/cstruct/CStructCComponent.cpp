#include "CStructCComponent.h"

#include <utility>

void CStructCComponent::setName(uint32_t componentName) {
    name = componentName;
}

void CStructCComponent::setType(CStructType componentType) {
    type = componentType;
}

void CStructCComponent::setData(std::vector<uint8_t> componentData) {
    rawData = std::move(componentData);
}

uint32_t CStructCComponent::getName() const {
    return name;
}

CStructType CStructCComponent::getType() {
    return type;
}

std::vector<uint8_t> CStructCComponent::getRawData() {
    return rawData;
}
