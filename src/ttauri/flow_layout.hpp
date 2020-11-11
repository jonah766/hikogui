// Copyright 2020 Pokitec
// All rights reserved.

#pragma once

#include "required.hpp"
#include "interval.hpp"
#include "alignment.hpp"
#include "ranged_numeric.hpp"
#include <vector>
#include <optional>

namespace tt {

/** Layout algorithm.
 */
class flow_layout {
public:
    flow_layout() noexcept : margins(), items() {
        clear();
    }

    void clear() noexcept
    {
        margins.clear();
        margins.push_back(0.0f);
        items.clear();
        cache_extent = {};
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return items.size();
    }

    void
    update(ssize_t index, finterval extent, ranged_int<3> resistance, float margin, relative_base_line base_line) noexcept
    {
        tt_assume(index >= 0);

        cache_extent = {};

        tt_assume(index < std::ssize(items));
        tt_assume(index + 1 < std::ssize(margins));
        items[index].update(extent, resistance, base_line);
        margins[index] = std::max(margins[index], margin);
        margins[index+1] = std::max(margins[index+1], margin);
    }

    /** Calculate the size of the combined items in the layout.
     */
    [[nodiscard]] finterval extent() const noexcept {
        if (cache_extent) {
            return *cache_extent;
        }

        auto r = finterval{0.0f};

        for (ttlet &margin: margins) {
            r += margin;
        }
        for (ttlet &item: items) {
            r += item.extent();
        }

        cache_extent = r;
        return r;
    }

    

    /** Update the layout of all items based based on the total size.
     */
    void update_layout(float total_size) noexcept {
        tt_assume(cache_extent);
        auto minimum_size = extent().minimum();
        auto extra_size = total_size - minimum_size;

        set_items_to_minimum_size();

        for (ttlet resistance : ranged_int<3>::range()) {
            auto nr_can_grow = number_of_items_that_can_grow(resistance);
            while (extra_size >= 1.0f && nr_can_grow != 0) {
                nr_can_grow = grow_items(nr_can_grow, resistance, extra_size);
            }
        }

        calculate_positions();
    }

   
    /**
     * @param first The first index
     * @param last The index one beyond the last.
     */
    [[nodiscard]] std::pair<float, float> get_offset_and_size(ssize_t first, ssize_t last) const noexcept
    {
        tt_assume(first >= 0 && first < std::ssize(items));
        tt_assume(last > 0 && last <= std::ssize(items));

        auto offset = items[first].offset;
        auto size = (items[last - 1].offset + items[last - 1].size) - offset;
        return {offset, size};
    }

    [[nodiscard]] std::pair<float, float> get_offset_and_size(ssize_t index) const noexcept
    {
        return get_offset_and_size(index, index + 1);
        
    }

    [[nodiscard]] relative_base_line get_base_line(ssize_t index) const noexcept
    {
        tt_assume(index >= 0 && index < ssize(items));
        return items[index].base_line;
    }

    /** Grow layout to include upto new_size of items.
     */
    void reserve(ssize_t new_size) noexcept
    {
        while (std::ssize(items) < new_size) {
            items.emplace_back();
        }

        while (std::ssize(margins) < new_size + 1) {
            margins.push_back(0.0f);
        }

        tt_assume(margins.size() == items.size() + 1);
    }

private:
    struct flow_layout_item {
        constexpr flow_layout_item() noexcept : _extent(0.0f, std::numeric_limits<float>::max()), resistance(1), base_line() {}

        constexpr flow_layout_item(flow_layout_item const &rhs) noexcept = default;
        constexpr flow_layout_item(flow_layout_item &&rhs) noexcept = default;
        constexpr flow_layout_item &operator=(flow_layout_item const &rhs) noexcept = default;
        constexpr flow_layout_item &operator=(flow_layout_item &&rhs) noexcept = default;

        constexpr void update(finterval a_extent, ranged_int<3> _resistance, relative_base_line _base_line) noexcept
        {
            _extent = intersect(_extent, a_extent);
            switch (static_cast<int>(_resistance)) {
            case 0: // No resistance has lower priority than strong resistance.
                resistance = resistance == 2 ? 2 : 0;
                break;
            case 1: // Normal resistance will not change the value.
                break;
            case 2: // Strong resistance overrides all
                resistance = 2;
                break;
            default:
                tt_no_default();
            }
            base_line = std::max(base_line, _base_line);
        }

        constexpr finterval extent() const noexcept
        {
            return _extent;
        }

        finterval _extent;
        ranged_int<3> resistance;
        relative_base_line base_line;

        float offset;
        float size;
    };

    /* The margin between the items, margin[0] is the margin
     * before the first item. margin[items.size()] is the margin
     * after the last item. margins.size() == items.size() + 1.
     */
    std::vector<float> margins;
    std::vector<flow_layout_item> items;

    mutable std::optional<finterval> cache_extent;

    
    void set_items_to_minimum_size() noexcept
    {
        for (auto &&item : items) {
            item.size = std::ceil(item.extent().minimum());
        }
    }

    [[nodiscard]] ssize_t number_of_items_that_can_grow(ranged_int<3> resistance) const noexcept
    {
        auto nr_non_max = ssize_t{0};

        for (auto &&item : items) {
            if (item.resistance == resistance && item.size < item.extent()) {
                ++nr_non_max;
            }
        }
        return nr_non_max;
    }

    [[nodiscard]] ssize_t grow_items(ssize_t nr_non_max, ranged_int<3> resistance, float &extra_size) noexcept
    {
        ttlet extra_size_per_item = std::ceil(extra_size / nr_non_max);

        nr_non_max = 0;
        for (auto &&item : items) {
            if (item.resistance == resistance) {
                auto old_size = item.size;

                ttlet extra_size_this_item = std::min(extra_size, extra_size_per_item);

                item.size = std::ceil(clamp(item.size + extra_size_this_item, item.extent()));
                extra_size -= item.size - old_size;

                if (item.size < item.extent()) {
                    ++nr_non_max;
                }
            }
        }
        return nr_non_max;
    }

    void calculate_positions() noexcept
    {
        auto offset = 0.0f;
        for (ssize_t i = 0; i != std::ssize(items); ++i) {
            offset += margins[i];
            items[i].offset = std::floor(offset);
            offset += items[i].size;
        }
    }
};

}