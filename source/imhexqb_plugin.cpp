#include <hex/plugin.hpp>

#include <hex/api/content_registry.hpp>
#include <hex/ui/view.hpp>

#include "QbReCompilerView.h"

IMHEX_PLUGIN_SETUP("imhexqb", "Hardronix", "An ImHex plugin for THPS script editing") {
    hex::ContentRegistry::Views::add<QbReCompilerView>();
}


