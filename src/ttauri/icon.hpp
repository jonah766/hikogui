// Copyright 2020 Pokitec
// All rights reserved.

#pragma once

#include "URL.hpp"
#include "pixel_map.hpp"
#include "R16G16B16A16SFloat.hpp"
#include "text/font_glyph_ids.hpp"
#include "text/elusive_icon.hpp"
#include "text/ttauri_icon.hpp"
#include <variant>

namespace tt {

/** An image, in different formats.
 */
class icon {
public:
    icon(URL const &url);
    icon(pixel_map<R16G16B16A16SFloat> &&image) noexcept;
    icon(font_glyph_ids const &glyph) noexcept;
    icon(elusive_icon const &icon) noexcept;
    icon(ttauri_icon const &icon) noexcept;

    icon() noexcept;
    icon(icon const &) noexcept;
    icon(icon &&) noexcept = default;
    icon &operator=(icon const &) noexcept;
    icon &operator=(icon &&) noexcept = default;

    [[nodiscard]] operator bool () const noexcept
    {
        return !std::holds_alternative<std::monostate>(image);
    }

    [[nodiscard]] friend bool operator==(icon const &lhs, icon const &rhs) noexcept
    {
        return lhs.image == rhs.image;
    }

private:
    using image_type = std::variant<std::monostate, font_glyph_ids, pixel_map<R16G16B16A16SFloat>>;

    image_type image;

    friend class stencil;
};


}
