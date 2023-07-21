#ifndef IMHEXQB_QBRECOMPILERVIEW_H
#define IMHEXQB_QBRECOMPILERVIEW_H

#include <vector>
#include <string>
#include "hex/ui/view.hpp"
#include "qb/qb_recompiler.h"
#include "hex/providers/buffered_reader.hpp"
#include "misc/cpp/imgui_stdlib.h"

using namespace hex;

class qb_recompiler_view : public View {

private:
    std::string text;
    std::vector<std::string> errors;
    std::map<int32_t, std::string> symbols;
    bool greedySymbolCapture = false;
    hex::Region selectedRegion{};
public:
    qb_recompiler_view();
    ~qb_recompiler_view() override = default;
    void drawContent() override;

    void onRegionSelected(Region region);
};


#endif
