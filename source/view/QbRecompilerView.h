#ifndef IMHEXQB_QBRECOMPILERVIEW_H
#define IMHEXQB_QBRECOMPILERVIEW_H

#include <vector>
#include <string>
#include "hex/ui/view.hpp"
#include "../qb/QbRecompiler.h"
#include "hex/providers/buffered_reader.hpp"
#include "misc/cpp/imgui_stdlib.h"

using namespace hex;

class QbRecompilerView : public View {

private:
    std::string text;
    std::vector<std::string> errors;
    bool greedySymbolCapture = false;
    bool heuristicIndentation = false;
    bool oldgen;
    hex::Region selectedRegion{};
    ChecksumDictionary dictionary;
public:
    QbRecompilerView();
    ~QbRecompilerView() override = default;
    void drawContent() override;

    void onRegionSelected(Region region);
};


#endif
