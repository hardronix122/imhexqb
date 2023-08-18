#include "QbRecompilerView.h"

QbRecompilerView::QbRecompilerView() : View("Qb Re-compiler") {
    text = std::string("#/ Decompiled instructions will appear here");
    oldgen = false;

    dictionary = ChecksumDictionary::load(ChecksumDictionary::getDictionaryPath());

    EventManager::subscribe<EventRegionSelected>(this, [this](Region region) {
        onRegionSelected(region);
    });
}

void QbRecompilerView::onRegionSelected(hex::Region region) {
    auto provider = ImHexApi::Provider::get();

    auto reader = prv::ProviderReader(provider);
    reader.setEndAddress(region.getEndAddress());

    text = QbRecompiler::decompile(reader.read(region.getStartAddress(), region.getSize()), dictionary,
                                   greedySymbolCapture, heuristicIndentation, oldgen ? 4 : 5);
    text.resize(text.size() * 2);
    selectedRegion = region;
}

void QbRecompilerView::drawContent() {
    if (ImGui::Begin("QB Re-compiler")) {
        if (ImGui::Button("Compile")) {
            std::vector<u8> bytes;
            auto provider = ImHexApi::Provider::get();

            errors.clear();

            try {
                bytes = QbRecompiler::compile(text, dictionary, oldgen ? 4 : 5);
            } catch (QbException &exception) {
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
            }
        }

        ImGui::SameLine();

        ImGui::Checkbox("Greedy symbol capture", &greedySymbolCapture);

        ImGui::SameLine();

        ImGui::Checkbox("Heuristic indentation", &heuristicIndentation);

        ImGui::SameLine();

        ImGui::Checkbox("Oldgen", &oldgen);

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