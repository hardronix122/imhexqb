#include <hex/plugin.hpp>

#include <hex/api/content_registry.hpp>
#include <hex/ui/view.hpp>

#include "qb_recompiler_view.h"

IMHEX_PLUGIN_SETUP("imhexqb", "Hardronix", "An ImHex plugin for THPS script editing") {
    hex::ContentRegistry::Views::add<qb_recompiler_view>();
}


