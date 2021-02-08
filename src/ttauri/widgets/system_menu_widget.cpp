// Copyright 2020 Pokitec
// All rights reserved.

#include "system_menu_widget.hpp"
#include "../GUI/utils.hpp"
#include "../text/ttauri_icon.hpp"
#include "../utils.hpp"
#include <Windows.h>
#include <WinUser.h>
#include <cmath>
#include <typeinfo>

namespace tt {

system_menu_widget::system_menu_widget(
    gui_window &window,
    std::shared_ptr<abstract_container_widget> parent,
    icon const &icon) noexcept :
    super(window, parent), _icon_stencil(stencil::make_unique(alignment::middle_center, icon))
{
    // Toolbar buttons hug the toolbar and neighbour widgets.
    _margin = 0.0f;
}

[[nodiscard]] bool
system_menu_widget::update_constraints(hires_utc_clock::time_point display_time_point, bool need_reconstrain) noexcept
{
    tt_axiom(gui_system_mutex.recurse_lock_count());

    if (super::update_constraints(display_time_point, need_reconstrain)) {
        ttlet width = theme::global->toolbarDecorationButtonWidth;
        ttlet height = theme::global->toolbarHeight;
        _preferred_size = {f32x4{width, height}, f32x4{width, std::numeric_limits<float>::infinity()}};
        return true;
    } else {
        return false;
    }
}

[[nodiscard]] void system_menu_widget::update_layout(hires_utc_clock::time_point display_time_point, bool need_layout) noexcept
{
    tt_axiom(gui_system_mutex.recurse_lock_count());

    need_layout |= std::exchange(_request_relayout, false);
    if (need_layout) {
        ttlet icon_height = rectangle().height() < theme::global->toolbarHeight * 1.2f ? rectangle().height() : theme::global->toolbarHeight;
        ttlet icon_rectangle = aarect{rectangle().x(), rectangle().top() - icon_height, rectangle().width(), icon_height};

        _icon_stencil->set_layout_parameters(icon_rectangle);

        // Leave space for window resize handles on the left and top.
        system_menu_rectangle = aarect{
            rectangle().x() + theme::global->margin,
            rectangle().y(),
            rectangle().width() - theme::global->margin,
            rectangle().height() - theme::global->margin};
    }

    super::update_layout(display_time_point, need_layout);
}

void system_menu_widget::draw(draw_context context, hires_utc_clock::time_point display_time_point) noexcept
{
    tt_axiom(gui_system_mutex.recurse_lock_count());

    if (overlaps(context, this->window_clipping_rectangle())) {
        _icon_stencil->draw(context);
    }

    super::draw(std::move(context), display_time_point);
}

HitBox system_menu_widget::hitbox_test(f32x4 window_position) const noexcept
{
    ttlet lock = std::scoped_lock(gui_system_mutex);

    if (window_clipping_rectangle().contains(window_position)) {
        // Only the top-left square should return ApplicationIcon, leave
        // the reset to the toolbar implementation.
        return HitBox{weak_from_this(), _draw_layer, HitBox::Type::ApplicationIcon};
    } else {
        return {};
    }
}

} // namespace tt
