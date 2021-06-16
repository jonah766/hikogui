
#include "main_window_controller.hpp"
#include "preferences_controller.hpp"
#include "application_controller.hpp"
#include "ttauri/logger.hpp"
#include "ttauri/widgets/widgets.hpp"
#include "ttauri/GUI/gui_system.hpp"
#include "ttauri/GUI/gui_window.hpp"

namespace demo {

void main_window_controller::init(tt::gui_window& self) noexcept
{
    using namespace tt;

    gui_window_delegate::init(self);

    // Add buttons to toolbar.
    auto preferences_button = self.make_toolbar_widget<toolbar_button_widget>(label{ elusive_icon::Wrench, l10n("Preferences") });
    preferences_button_callback = preferences_button->subscribe([&self]() {
        if (auto application_controller = application_controller::global.lock()) {
            self.system.make_window(
                application_controller->preferences_controller,
                label{ icon{URL{"resource:ttauri_demo.png"}}, l10n("TTauri Demo - Preferences") }
            );
        }
    });

    auto column = self.make_widget<column_layout_widget>("A1");
    auto button1 = column->make_widget<momentary_button_widget>(l10n("Hello \u4e16\u754c"));
    auto button2 = column->make_widget<momentary_button_widget>(l10n("Hello world"));
    auto button3 = column->make_widget<momentary_button_widget>(l10n("Hello earthlings"));
}

}
