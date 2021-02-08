// Copyright 2020 Pokitec
// All rights reserved.

#include "icon.hpp"
#include "stencils/pixel_map_stencil.hpp"
#include "stencils/glyph_stencil.hpp"
#include "codec/png.hpp"

namespace tt {

icon::icon() noexcept : image(std::monostate{}) {}

icon::icon(pixel_map<R16G16B16A16SFloat> &&image) noexcept : image(std::move(image)) {}

icon::icon(font_glyph_ids const &image) noexcept : image(image) {}

icon::icon(URL const &url) : icon(png::load(url)) {}

icon::icon(elusive_icon const &icon) noexcept : icon(to_font_glyph_ids(icon)) {}

icon::icon(ttauri_icon const &icon) noexcept : icon(to_font_glyph_ids(icon)) {}

icon::icon(icon const &other) noexcept
{
    if (auto font_glyph_id = std::get_if<font_glyph_ids>(&other.image)) {
        image = *font_glyph_id;

    } else if (auto pixmap = std::get_if<pixel_map<R16G16B16A16SFloat>>(&other.image)) {
        image = pixmap->copy();

    } else if (std::holds_alternative<std::monostate>(other.image)) {
        image = std::monostate{};

    } else {
        tt_no_default();
    }
}

icon &icon::operator=(icon const &other) noexcept
{
    if (auto font_glyph_id = std::get_if<font_glyph_ids>(&other.image)) {
        image = *font_glyph_id;

    } else if (auto pixmap = std::get_if<pixel_map<R16G16B16A16SFloat>>(&other.image)) {
        image = pixmap->copy();

    } else if (std::holds_alternative<std::monostate>(other.image)) {
        image = std::monostate{};

    } else {
        tt_no_default();
    }
    return *this;
}

} // namespace tt
