
#pragma once

#include "abstract_button_widget.hpp"
#include "audio_device_widget.hpp"
#include "button_delegate.hpp"
#include "checkbox_widget.hpp"
#include "grid_widget.hpp"
#include "icon_widget.hpp"
#include "label_widget.hpp"
#include "menu_button_widget.hpp"
#include "momentary_button_widget.hpp"
#include "overlay_widget.hpp"
#include "radio_button_widget.hpp"
#include "row_column_widget.hpp"
#include "scroll_aperture_widget.hpp"
#include "scroll_bar_widget.hpp"
#include "scroll_widget.hpp"
#include "selection_delegate.hpp"
#include "selection_widget.hpp"
#include "spacer_widget.hpp"
#include "system_menu_widget.hpp"
#include "tab_delegate.hpp"
#include "tab_widget.hpp"
#include "text_delegate.hpp"
#include "text_field_delegate.hpp"
#include "text_field_widget.hpp"
#include "text_widget.hpp"
#include "toggle_widget.hpp"
#include "toolbar_button_widget.hpp"
#include "toolbar_tab_button_widget.hpp"
#include "toolbar_widget.hpp"
#include "widget_mode.hpp"
#include "widget.hpp"
#include "window_controls_macos_widget.hpp"
#include "window_controls_win32_widget.hpp"
#include "window_widget.hpp"


namespace hi {
inline namespace v1 {
/**
\defgroup widgets Widgets
\ingroup GUI
\brief Graphical elements of interaction.

Widget are graphical elements which are shown inside a window and often can be
interacted with using the mouse and keyboard, such as various kinds of buttons,
text fields and selection boxes.

Many widgets such as the `grid_widget`, `tab_widget`, `row_column_widget` or
the `scroll_widget` are containers for other widgets.
The top level `window_widget` of a window for example contains two containers
a `grid_widget` for the content area and a `toolbar_widget` for the toolbar
at the top of the window.

### Example
In the example below we are adding 4 widgets to the content area of the window.

The `gui_window::content()` function returns a reference to the `grid_widget`,
and we use its `make_widget<>()` function to add new widgets. The template
argument is the type of widget to instantiate and the first argument is the
spreadsheet-like coordinate within the `grid_widget`. The rest of the arguments
are passed to the constructor of the new widget.

```
int hi_main(int argc, char *argv[])
{
    observer<int> value = 0;

    auto gui = gui_system::make_unique();
    auto &window = gui->make_window(txt("Radio button example"));

    window.content().make_widget<label_widget>("A1", txt("radio buttons:"));
    window.content().make_widget<radio_button_widget>("B1", value, 1, txt("one"));
    window.content().make_widget<radio_button_widget>("B2", value, 2, txt("two"));
    window.content().make_widget<radio_button_widget>("B3", value, 3, txt("three"));

    return gui->loop();
}
```

There are often two different ways to construct a widget: with a delegate or
with an observer. In the example above we use an `observer<int>` for the
radio buttons to monitor and update. Sharing the same observer allows the
radio buttons to act as a set.

### Delegates
Many widgets are controlled by a delegate, such as all the button widgets,
the `text_widget`, `text_field_widget`, `selection_widget` and `tab_widget`. 

For these widgets you may use their delegate's base class to implement your own
delegate and pass a `std::shared_ptr` to the delegate to the constructor of
that widget. For more information about delegates see: \ref widget_delegates.

### Observer
If instead you pass an observer to a widget's constructor, it will instantiate
a default-delegate which uses the observer to control the widget.

An observer is a type that observes a value, it will use a callback to notify listeners when the
observed value changes. For more information see: \ref observer.

### Attributes

Observers are used for many member variables of a widget, including the
`hi::widget::enabled`, `hi::widget::visible` members and various labels.


*/
}}
