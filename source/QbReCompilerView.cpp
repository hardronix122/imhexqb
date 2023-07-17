#include "QbReCompilerView.h"
#include "qb/QbReCompiler.h"
#include "hex/providers/buffered_reader.hpp"

QbReCompilerView::QbReCompilerView() : View("Qb Re-compiler") {
    symbols = std::map<int32_t, std::string>();

    EventManager::subscribe<EventRegionSelected>(this, [this](Region region) {
        onRegionSelected(region);
    });
}

void QbReCompilerView::onRegionSelected(hex::Region region) {
    auto provider = ImHexApi::Provider::get();

    auto reader = prv::ProviderReader(provider);
    reader.setEndAddress(region.getEndAddress());

    text = QbReCompiler::decompile(reader.read(region.getStartAddress(), region.getSize()), symbols,
                                   greedySymbolCapture);
    text.resize(text.size() * 2);
}

void QbReCompilerView::drawContent() {
    if (ImGui::Begin("QB Re-compiler")) {

        if (ImGui::Button("Compile")) {
            printf("clicked!\n");
        }

        ImGui::SameLine();

        ImGui::Checkbox("Greedy symbol capture", &greedySymbolCapture);

        ImGui::SameLine();

        if (ImGui::Button("Clear symbol entries (checksum names list)")) {
            symbols.clear();
        }

        ImVec2 availableSize = ImGui::GetWindowSize();
        ImGui::InputTextMultiline("Code", text.data(), text.size(), availableSize);
    }

    ImGui::End();
}