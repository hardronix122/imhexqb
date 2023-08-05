#include "CStructRecompilerView.h"

CStructRecompilerView::CStructRecompilerView() : View("CStruct Re-compiler") {
    EventManager::subscribe<EventRegionSelected>(this, [this](Region region) {
        onRegionSelected(region);
    });
}

void CStructRecompilerView::drawContent() {
    if (ImGui::Begin("CStruct Re-compiler")) {
        if (!structure.value.empty()) {
            if (ImGui::TreeNode("Data")) {
                createRecursiveTreeView(structure);

                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
}

void CStructRecompilerView::onRegionSelected(Region region) {
    auto provider = ImHexApi::Provider::get();
    auto reader = prv::ProviderReader(provider);

    reader.setEndAddress(region.getEndAddress());

    int offset = 0;

    structure = CStructHelper::deserializeStructure(offset, reader.read(region.getStartAddress(), region.size));
}

void CStructRecompilerView::createRecursiveTreeView(CStructStructure &c_structure) {
    for (CStructBase base: c_structure.value) {
        switch (base.type) {
            case INTEGER:
            case ZERO_INTEGER: {
                auto integer = (CStructInteger) base;
                ImGui::Text("Integer: %08x - %d", integer.name, integer.value);
                break;
            }
            case BYTE: {
                auto byte = (CStructByte) base;
                ImGui::Text("Byte: %08x - %d", byte.name, byte.value);
                break;
            }
            case UNSIGNED_BYTE: {
                auto byte = (CStructUnsignedByte) base;
                ImGui::Text("Unsigned Byte: %08x - %d", byte.name, byte.value);
                break;
            }
            case SHORT: {
                auto value = (CStructShort) base;
                ImGui::Text("Short: %08x - %d", value.name, value.value);
                break;
            }
            case UNSIGNED_SHORT: {
                auto value = (CStructUnsignedShort) base;
                ImGui::Text("Unsigned Short: %08x - %d", value.name, value.value);
                break;
            }
            case FLOAT:
            case ZERO_FLOAT: {
                auto value = (CStructFloat) base;
                ImGui::Text("Float: %08x - %f", value.name, value.value);
                break;
            }
            case STRING: {
                auto value = (CStructString) base;
                ImGui::Text("String: %08x - %s", value.name, value.value.c_str());
                break;
            }
            case LOCAL_STRING: {
                auto value = (CStructLocalString) base;
                ImGui::Text("Local String: %08x - %s", value.name, value.value.c_str());
                break;
            }
            case PAIR: {
                auto value = (CStructPair) base;
                ImGui::Text("Pair: %08x - %f, %f", value.name, value.value.x, value.value.y);
                break;
            }
            case VECTOR: {
                auto value = (CStructVector) base;
                ImGui::Text("Vector: %08x - %f, %f, %f", value.name, value.value.x, value.value.y, value.value.z);
                break;
            }
            case STRUCTURE: {
                auto value = (CStructStructure) base;

                if (ImGui::TreeNode(std::to_string(value.name).c_str())) {
                    createRecursiveTreeView(value);
                    ImGui::TreePop();
                }
                break;
            }
            case ARRAY: {
                auto value = (CStructArray) base;

                if (ImGui::TreeNode(std::to_string(value.name).c_str())) {
                    createRecursiveArrayTreeView(value);
                    ImGui::TreePop();
                }
                break;
            }
            case NAME: {
                auto value = (CStructName) base;
                ImGui::Text("Name: %08x - %d", value.name, value.value);
                break;
            }
            default:
                ImGui::Text("Unhandled: %02x", base.name);
                break;
        }
    }
}

void CStructRecompilerView::createRecursiveArrayTreeView(CStructArray &c_structure) {
    ImGui::Text("Array size: %zu", c_structure.value.size());

    for (CStructBase base: c_structure.value) {
        switch (c_structure.type) {
            case INTEGER:
            case ZERO_INTEGER: {
                auto value = (CStructInteger) base;
                ImGui::Text("Integer - %d", value.value);

                break;
            }
            case BYTE: {
                auto value = (CStructByte) base;
                ImGui::Text("Byte - %d", value.value);

                break;
            }
            case UNSIGNED_BYTE: {
                auto value = (CStructUnsignedByte) base;
                ImGui::Text("Unsigned byte - %d", value.value);

                break;
            }
            case SHORT: {
                auto value = (CStructShort) base;
                ImGui::Text("Short - %d", value.value);

                break;
            }
            case UNSIGNED_SHORT: {
                auto value = (CStructShort) base;
                ImGui::Text("Unsigned short - %d", value.value);

                break;
            }
            case FLOAT:
            case ZERO_FLOAT: {
                auto value = (CStructFloat) base;
                ImGui::Text("Float - %f", value.value);

                break;
            }
            case STRING: {
                auto value = (CStructString) base;
                ImGui::Text("String - %s", value.value.c_str());

                break;
            }
            case LOCAL_STRING: {
                auto value = (CStructLocalString) base;
                ImGui::Text("Local string - %s", value.value.c_str());

                break;
            }
            case PAIR: {
                auto value = (CStructPair) base;
                ImGui::Text("Pair - %f, %f", value.value.x, value.value.y);

                break;
            }
            case VECTOR: {
                auto value = (CStructVector) base;
                ImGui::Text("Vector - %f, %f, %f", value.value.x, value.value.y, value.value.z);

                break;
            }
            case STRUCTURE: {
                auto value = (CStructStructure) base;
                createRecursiveTreeView(value);

                break;
            }
            case ARRAY: {
                auto value = (CStructArray) base;
                createRecursiveArrayTreeView(value);

                break;
            }
            case NAME: {
                auto value = (CStructName) base;
                ImGui::Text("Name: %d", value.value);
                break;
            }
            default:
                ImGui::Text("Unhandled: %02x", base.name);
                break;
        }
    }
}
