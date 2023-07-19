#ifndef IMHEXQB_QBRECOMPILERVIEW_H
#define IMHEXQB_QBRECOMPILERVIEW_H

#include <vector>
#include <string>
#include "hex/ui/view.hpp"
#include "qb/QbReCompiler.h"
#include "hex/providers/buffered_reader.hpp"
#include "misc/cpp/imgui_stdlib.h"

using namespace hex;

class QbReCompilerView : public View {

private:
    std::string text;
    std::map<int32_t, std::string> symbols;
    bool greedySymbolCapture = false;
    hex::Region selectedRegion{};
public:
    QbReCompilerView();
    ~QbReCompilerView() override = default;
    void drawContent() override;

    void onRegionSelected(Region region);
};


#endif
