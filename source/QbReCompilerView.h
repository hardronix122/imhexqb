#ifndef IMHEXQB_QBRECOMPILERVIEW_H
#define IMHEXQB_QBRECOMPILERVIEW_H

#include <vector>
#include <string>

#include "hex/ui/view.hpp"

using namespace hex;

class QbReCompilerView : public View {

private:
    std::string text;
public:
    QbReCompilerView();
    ~QbReCompilerView() override = default;

    void drawContent() override;

    void onRegionSelected(Region region);
};


#endif
