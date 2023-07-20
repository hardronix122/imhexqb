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
                                    greedySymbolCapture);
    text.resize(text.size() * 2);
    selectedRegion = region;
}

void qb_recompiler_view::drawContent() {
    if (ImGui::Begin("QB Re-compiler")) {
        if (ImGui::Button("Compile")) {
            std::vector<u8> bytes = qb_recompiler::compile(text);

            auto provider = ImHexApi::Provider::get();

            if(selectedRegion.getSize() < bytes.size()) {
                provider->insert(selectedRegion.getStartAddress(), bytes.size() - selectedRegion.getSize());
            }

            provider->write(selectedRegion.getStartAddress(), bytes.data(), bytes.size());

            ImHexApi::HexEditor::setSelection(selectedRegion.getStartAddress(), bytes.size());
        }

        ImGui::SameLine();

        ImGui::Checkbox("Greedy symbol capture", &greedySymbolCapture);

        ImGui::SameLine();

        if (ImGui::Button("Clear symbol entries (checksum names list)")) {
            symbols.clear();
        }

        ImVec2 availableSize = ImGui::GetWindowSize();
        availableSize.y -= 58;
        availableSize.x -= 15;
        ImGui::InputTextMultiline("Code", &text, availableSize);
    }

    ImGui::End();
}