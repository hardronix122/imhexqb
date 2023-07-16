#include <hex/plugin.hpp>

#include <hex/api/content_registry.hpp>
#include <hex/ui/view.hpp>

class BulbSyntaxView : public hex::View {
public:
    BulbSyntaxView() : hex::View("BulbSyntax") {}
    ~BulbSyntaxView() override = default;

    void drawContent() override {
        if (ImGui::Begin("BulbSyntax")) {
            ImGui::Text("Decompiled qb code will be here UwU");
        }
        ImGui::End();
    }
};

IMHEX_PLUGIN_SETUP("imhexqb", "Hardronix", "An ImHex plugin for THPS script editing") {
    hex::ContentRegistry::Views::add<BulbSyntaxView>();
}


