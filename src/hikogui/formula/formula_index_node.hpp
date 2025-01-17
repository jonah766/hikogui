// Copyright Take Vos 2020-2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "formula_binary_operator_node.hpp"
#include "../macros.hpp"

hi_export_module(hikogui.formula.formula_index_node);

namespace hi { inline namespace v1 {

hi_export struct formula_index_node final : formula_binary_operator_node {
    formula_index_node(size_t line_nr, size_t column_nr, std::unique_ptr<formula_node> lhs, std::unique_ptr<formula_node> rhs) :
        formula_binary_operator_node(line_nr, column_nr, std::move(lhs), std::move(rhs))
    {
    }

    datum evaluate(formula_evaluation_context &context) const override
    {
        auto lhs_ = lhs->evaluate(context);
        auto rhs_ = rhs->evaluate(context);

        if (holds_alternative<datum::map_type>(lhs_) and not lhs_.contains(rhs_)) {
            throw operation_error(std::format("{}:{}: Unknown key '{}'.", line_nr, column_nr, rhs_));
        }

        try {
            return lhs_[rhs_];
        } catch (std::exception const &e) {
            throw operation_error(std::format("{}:{}: Can not evaluate indexing operation.\n{}", line_nr, column_nr, e.what()));
        }
    }

    datum &evaluate_lvalue(formula_evaluation_context &context) const override
    {
        auto &lhs_ = lhs->evaluate_lvalue(context);
        auto rhs_ = rhs->evaluate(context);
        try {
            return lhs_[rhs_];
        } catch (std::exception const &e) {
            throw operation_error(std::format("{}:{}: Can not evaluate indexing operation.\n{}", line_nr, column_nr, e.what()));
        }
    }

    std::string string() const noexcept override
    {
        return std::format("({}[{}])", *lhs, *rhs);
    }
};

}} // namespace hi::inline v1
