#ifndef CSTRUCTRECOMPILERVIEW_H
#define CSTRUCTRECOMPILERVIEW_H

#include "hex/ui/view.hpp"
#include "hex/providers/buffered_reader.hpp"
#include "../qb/cstruct/CStructHelper.h"
#include "../qb/cstruct/CStructElements.h"

using namespace hex;

class CStructRecompilerView : public View {
private:
    CStructStructure structure;
public:
    CStructRecompilerView();

    ~CStructRecompilerView() override = default;

    void drawContent() override;

    void onRegionSelected(Region region);

    static void createRecursiveTreeView(CStructStructure &c_structure);

    static void createRecursiveArrayTreeView(CStructArray &c_structure);
};


#endif //CSTRUCTRECOMPILERVIEW_H
