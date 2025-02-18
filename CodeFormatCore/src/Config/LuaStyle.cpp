#include "CodeFormatCore/Config/LuaStyle.h"
#include "FunctionOption.h"
#include <map>

bool IsNumber(std::string_view source) {
    if (source.empty()) {
        return false;
    }
    for (auto c: source) {
        if (c > '9' || c < '0') {
            return false;
        }
    }
    return true;
}

#define BOOL_OPTION(op)                   \
    if (configMap.count(#op)) {           \
        op = configMap.at(#op) == "true"; \
    }

#define NUMBER_OPTION(op)                                      \
    if (configMap.count(#op) && IsNumber(configMap.at(#op))) { \
        op = std::stoi(configMap.at(#op));                     \
    }

void LuaStyle::Parse(std::map<std::string, std::string, std::less<>> &configMap) {
    if (configMap.count("indent_style")) {
        if (configMap.at("indent_style") == "space") {
            indent_style = IndentStyle::Space;
        } else if (configMap.at("indent_style") == "tab") {
            indent_style = IndentStyle::Tab;
        }
    }

    NUMBER_OPTION(indent_size)

    NUMBER_OPTION(tab_width)

    if (configMap.count("quote_style")) {
        if (configMap.at("quote_style") == "single") {
            quote_style = QuoteStyle::Single;
        } else if (configMap.at("quote_style") == "double") {
            quote_style = QuoteStyle::Double;
        }
    }

    if (configMap.count("table_separator_style")) {
        auto style = configMap.at("table_separator_style");
        if (style == "none") {
            table_separator_style = TableSeparatorStyle::None;
        } else if (style == "comma") {
            table_separator_style = TableSeparatorStyle::Comma;
        } else if (style == "semicolon") {
            table_separator_style = TableSeparatorStyle::Semicolon;
        } else if (style == "only_kv_colon") {
            table_separator_style = TableSeparatorStyle::OnlyKVColon;
        }
    }

    if (configMap.count("max_line_length")) {
        if (IsNumber(configMap.at("max_line_length"))) {
            max_line_length = std::stoi(configMap.at("max_line_length"));
        } else if (configMap.at("max_line_length") == "unset") {
            max_line_length = 998;
        }
    }

    if (configMap.count("end_of_line")) {
        auto lineSeparatorSymbol = configMap.at("end_of_line");
        if (lineSeparatorSymbol == "crlf") {
            end_of_line = EndOfLine::CRLF;
        } else if (lineSeparatorSymbol == "lf") {
            end_of_line = EndOfLine::LF;
        } else if (lineSeparatorSymbol == "cr") {
            end_of_line = EndOfLine::CR;
        } else if (lineSeparatorSymbol == "auto" || lineSeparatorSymbol == "unset") {
#ifndef _WINDOWS
            end_of_line = EndOfLine::LF;
#else
            end_of_line = EndOfLine::CRLF;
#endif
        }
    }

    if (configMap.count("trailing_table_separator")) {
        auto action = configMap.at("trailing_table_separator");
        if (action == "keep") {
            trailing_table_separator = TrailingTableSeparator::Keep;
        } else if (action == "never") {
            trailing_table_separator = TrailingTableSeparator::Never;
        } else if (action == "always") {
            trailing_table_separator = TrailingTableSeparator::Always;
        } else if (action == "smart") {
            trailing_table_separator = TrailingTableSeparator::Smart;
        }
    }

    if (configMap.count("call_arg_parentheses")) {
        if (configMap.at("call_arg_parentheses") == "keep") {
            call_arg_parentheses = CallArgParentheses::Keep;
        } else if (configMap.at("call_arg_parentheses") == "remove") {
            call_arg_parentheses = CallArgParentheses::Remove;
        } else if (configMap.at("call_arg_parentheses") == "remove_string_only") {
            call_arg_parentheses = CallArgParentheses::RemoveStringOnly;
        } else if (configMap.at("call_arg_parentheses") == "remove_table_only") {
            call_arg_parentheses = CallArgParentheses::RemoveTableOnly;
        }
    }

    NUMBER_OPTION(continuation_indent)

    BOOL_OPTION(detect_end_of_line)

    BOOL_OPTION(insert_final_newline)

    BOOL_OPTION(space_around_table_field_list)

    BOOL_OPTION(space_before_attribute)

    BOOL_OPTION(space_before_function_open_parenthesis)

    BOOL_OPTION(space_before_function_call_open_parenthesis)

    BOOL_OPTION(space_before_closure_open_parenthesis)

    if (configMap.count("space_before_function_call_single_arg")) {
        auto &value = configMap.at("space_before_function_call_single_arg");
        if (value == "true" || value == "always") {
            space_before_function_call_single_arg = FunctionSingleArgSpace::Always;
        } else if (value == "only_string") {
            space_before_function_call_single_arg = FunctionSingleArgSpace::OnlyString;
        } else if (value == "only_table") {
            space_before_function_call_single_arg = FunctionSingleArgSpace::OnlyTable;
        } else if (value == "false" || value == "none") {
            space_before_function_call_single_arg = FunctionSingleArgSpace::None;
        }
    }


    BOOL_OPTION(space_before_open_square_bracket)

    BOOL_OPTION(space_inside_function_call_parentheses)

    BOOL_OPTION(space_inside_function_param_list_parentheses)

    BOOL_OPTION(space_inside_square_brackets)

    BOOL_OPTION(space_around_table_append_operator)

    BOOL_OPTION(ignore_spaces_inside_function_call)

    NUMBER_OPTION(space_before_inline_comment)

    BOOL_OPTION(space_around_math_operator)

    BOOL_OPTION(space_after_comma)

    BOOL_OPTION(space_after_comma_in_for_statement)

    BOOL_OPTION(space_around_concat_operator)

    BOOL_OPTION(space_around_logical_operator)

    BOOL_OPTION(space_around_assign_operator)

    BOOL_OPTION(align_call_args)

    BOOL_OPTION(align_function_params)

    if (configMap.count("align_continuous_assign_statement")) {
        if (configMap.at("align_continuous_assign_statement") == "true" || configMap.at("align_continuous_assign_statement") == "when_extra_space") {
            align_continuous_assign_statement = ContinuousAlign::WhenExtraSpace;
        } else if (configMap.at("align_continuous_assign_statement") == "always") {
            align_continuous_assign_statement = ContinuousAlign::Always;
        } else if (configMap.at("align_continuous_assign_statement") == "false" || configMap.at("align_continuous_assign_statement") == "none") {
            align_continuous_assign_statement = ContinuousAlign::None;
        }
    }

    if (configMap.count("align_continuous_rect_table_field")) {
        if (configMap.at("align_continuous_rect_table_field") == "true" || configMap.at("align_continuous_rect_table_field") == "when_extra_space") {
            align_continuous_rect_table_field = ContinuousAlign::WhenExtraSpace;
        } else if (configMap.at("align_continuous_rect_table_field") == "always") {
            align_continuous_rect_table_field = ContinuousAlign::Always;
        } else if (configMap.at("align_continuous_rect_table_field") == "false" || configMap.at("align_continuous_rect_table_field") == "none") {
            align_continuous_rect_table_field = ContinuousAlign::None;
        }
    }

    NUMBER_OPTION(align_continuous_line_space)

    BOOL_OPTION(align_if_branch)

    if (configMap.count("align_array_table")) {
        if (configMap.at("align_array_table") == "true" || configMap.at("align_array_table") == "normal") {
            align_array_table = AlignArrayTable::Normal;
        } else if (configMap.at("align_array_table") == "contain_curly") {
            align_array_table = AlignArrayTable::ContainCurly;
        } else if (configMap.at("align_array_table") == "false" || configMap.at("align_array_table") == "none") {
            align_array_table = AlignArrayTable::None;
        }
    }

    if (configMap.count("align_chain_expr")) {
        if (configMap.at("align_chain_expr") == "none") {
            align_chain_expr = AlignChainExpr::None;
        } else if (configMap.at("align_chain_expr") == "always") {
            align_chain_expr = AlignChainExpr::Always;
        } else if (configMap.at("align_chain_expr") == "only_call_stmt") {
            align_chain_expr = AlignChainExpr::OnlyCallStmt;
        }
    }

    BOOL_OPTION(align_continuous_similar_call_args)

    BOOL_OPTION(align_continuous_inline_comment)

    BOOL_OPTION(never_indent_before_if_condition)

    if (align_if_branch) {
        never_indent_before_if_condition = true;
    }

    BOOL_OPTION(never_indent_comment_on_if_branch)

    BOOL_OPTION(keep_indents_on_empty_lines)

    std::vector<std::pair<std::string, LineSpace &>> fieldList = {
            {"line_space_after_if_statement",              line_space_after_if_statement             },
            {"line_space_after_do_statement",              line_space_after_do_statement             },
            {"line_space_after_while_statement",           line_space_after_while_statement          },
            {"line_space_after_repeat_statement",          line_space_after_repeat_statement         },
            {"line_space_after_for_statement",             line_space_after_for_statement            },
            {"line_space_after_local_or_assign_statement", line_space_after_local_or_assign_statement},
            {"line_space_after_function_statement",        line_space_after_function_statement       },
            {"line_space_after_expression_statement",      line_space_after_expression_statement     },
            {"line_space_after_comment",                   line_space_after_comment                  },
            {"line_space_around_block",                    line_space_around_block                   }
    };

    for (auto &lineOption: fieldList) {
        if (configMap.count(lineOption.first)) {
            std::string value = configMap.at(lineOption.first);
            FunctionOption option;
            option.Parse(value);

            if (option.GetKey() == "keep") {
                lineOption.second = LineSpace(LineSpaceType::Keep);
            } else if (option.GetKey() == "min") {
                auto p1 = option.GetParam(0);
                if (!p1.empty() && IsNumber(p1)) {
                    lineOption.second = LineSpace(LineSpaceType::Min, std::stoi(p1));
                }
            } else if (option.GetKey() == "fixed") {
                auto p1 = option.GetParam(0);
                if (!p1.empty() && IsNumber(p1)) {
                    lineOption.second = LineSpace(LineSpaceType::Fixed, std::stoi(p1));
                }
            } else if (option.GetKey() == "max") {
                auto p1 = option.GetParam(0);
                if (!p1.empty() && IsNumber(p1)) {
                    lineOption.second = LineSpace(LineSpaceType::Max, std::stoi(p1));
                }
            }
        }
    }

    BOOL_OPTION(break_all_list_when_line_exceed)

    BOOL_OPTION(auto_collapse_lines)

    BOOL_OPTION(break_before_braces)

    if (configMap.count("break_table_list")) {
        if (configMap.at("break_table_list") == "never") {
            break_table_list = BreakTableList::Never;
        }
        else if(configMap.at("break_table_list") == "smart") {
            break_table_list = BreakTableList::Smart;
        }
        else if(configMap.at("break_table_list") == "lazy") {
            break_table_list = BreakTableList::Lazy;
        }
    }

    BOOL_OPTION(ignore_space_after_colon)

    BOOL_OPTION(remove_call_expression_list_finish_comma)

    if (configMap.count("end_statement_with_semicolon")) {
        if (configMap.at("end_statement_with_semicolon") == "keep") {
            end_statement_with_semicolon = EndStmtWithSemicolon::Keep;
        } else if (configMap.at("end_statement_with_semicolon") == "always") {
            end_statement_with_semicolon = EndStmtWithSemicolon::Always;
        } else if (configMap.at("end_statement_with_semicolon") == "replace_with_newline") {
            end_statement_with_semicolon = EndStmtWithSemicolon::ReplaceWithNewline;
        } else if (configMap.at("end_statement_with_semicolon") == "same_line") {
            end_statement_with_semicolon = EndStmtWithSemicolon::SameLine;
        }
    }
}
