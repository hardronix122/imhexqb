#include <hex/plugin.hpp>

#include <hex/api/content_registry.hpp>

#include "view/CStructRecompilerView.h"
#include "view/QbRecompilerView.h"

IMHEX_PLUGIN_SETUP("imhexqb", "Hardronix", "An ImHex plugin for THPS script editing") {
    hex::ContentRegistry::Views::add<QbRecompilerView>();
    //hex::ContentRegistry::Views::add<CStructRecompilerView>();
}


