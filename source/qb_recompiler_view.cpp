#include "qb_recompiler_view.h"

qb_recompiler_view::qb_recompiler_view() : View("Qb Re-compiler") {
    symbols = std::map<int32_t, std::string>();
    text = std::string("#/ Decompiled instructions will appear here");

    EventManager::subscribe<EventRegionSelected>(this, [this](Region region) {
        onRegionSelected(region);
    });
}

void qb_recompiler_view::onRegionSelected(hex::Region region) {
    auto provider = ImHexApi::Provider::get();

    auto reader = prv::ProviderReader(provider);
    reader.setEndAddress(region.getEndAddress());

    text = qb_recompiler::decompile(reader.read(region.getStartAddress(), region.getSize()), symbols,
                                    greedySymbolCapture, heuristicIndentation);
    text.resize(text.size() * 2);
    selectedRegion = region;
}

void qb_recompiler_view::drawContent() {
    if (ImGui::Begin("QB Re-compiler")) {
        if (ImGui::Button("Compile")) {
            std::vector<u8> bytes;
            auto provider = ImHexApi::Provider::get();

            errors.clear();

            try {
                bytes = qb_recompiler::compile(text);
            } catch (qb_exception &exception) {
                errors.emplace_back(exception.what());
            } catch (std::exception &exception) {
                errors.emplace_back(exception.what());
            }

            if (!bytes.empty()) {
                if (selectedRegion.getSize() < bytes.size()) {
                    provider->insert(selectedRegion.getStartAddress(), bytes.size() - selectedRegion.getSize());
                } else if (bytes.size() < selectedRegion.getSize()) {
                    unsigned long excessiveSize = selectedRegion.getSize() - bytes.size();
                    provider->remove(selectedRegion.getEndAddress() - excessiveSize, excessiveSize);
                }

                provider->write(selectedRegion.getStartAddress(), bytes.data(), bytes.size());

                ImHexApi::HexEditor::setSelection(selectedRegion.getStartAddress(), bytes.size());

                std::optional<ImHexApi::HexEditor::ProviderRegion> newRegion = ImHexApi::HexEditor::getSelection();

                if (newRegion.has_value()) {
                    selectedRegion = newRegion->getRegion();
                }
            }
        }

        ImGui::SameLine();

        ImGui::Checkbox("Greedy symbol capture", &greedySymbolCapture);

        ImGui::SameLine();

        if (ImGui::Button("Clear checksum names list")) {
            symbols.clear();
        }

        ImGui::SameLine();

        ImGui::Checkbox("Heuristic indentation", &heuristicIndentation);

        ImVec2 errorFieldSize = ImGui::GetWindowSize();
        errorFieldSize.x -= 15;
        errorFieldSize.y = errorFieldSize.y / 4;

        ImVec2 codeFieldSize = ImGui::GetWindowSize();
        codeFieldSize.y -= 65 + errorFieldSize.y;
        codeFieldSize.x -= 15;

        ImGui::InputTextMultiline("Code", &text, codeFieldSize);

        if (ImGui::BeginListBox("Errors", errorFieldSize)) {
            for (std::string &error: errors) {
                ImGui::Selectable(error.c_str(), false, ImGuiSelectableFlags_Disabled);
            }

            ImGui::EndListBox();
        }
    }

    ImGui::End();
}