// -*- mode:c++;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
// vi: set et ft=cpp ts=4 sts=4 sw=4 fenc=utf-8 :vi
//
// Copyright 2024 Mozilla Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "highlight.h"

#include <ctype.h>

#define NORMAL 0
#define WORD 1
#define QUOTE 2
#define QUOTE_BACKSLASH 3
#define DQUOTE 4
#define DQUOTE_BACKSLASH 5
#define TICK 6
#define TICK_BACKSLASH 7
#define VAR 8
#define VAR2 9
#define COMMENT 10

HighlightPerl::HighlightPerl() {
}

HighlightPerl::~HighlightPerl() {
}

void HighlightPerl::feed(std::string *r, std::string_view input) {
    int c;
    for (size_t i = 0; i < input.size(); ++i) {
        c = input[i] & 255;
        switch (t_) {

        Normal:
        case NORMAL:
            if (!isascii(c) || isalpha(c) || c == '_') {
                t_ = WORD;
                word_ += c;
            } else if (c == '\'') {
                t_ = QUOTE;
                *r += HI_STRING;
                *r += c;
            } else if (c == '"') {
                t_ = DQUOTE;
                *r += HI_STRING;
                *r += c;
            } else if (c == '`') {
                t_ = TICK;
                *r += HI_STRING;
                *r += c;
            } else if (c == '$') {
                *r += c;
                t_ = VAR;
            } else if (c == '@' || c == '%') {
                *r += c;
                *r += HI_VAR;
                t_ = VAR2;
            } else if (c == '#') {
                *r += HI_COMMENT;
                *r += c;
                t_ = COMMENT;
            } else {
                *r += c;
            }
            break;

        Word:
        case WORD:
            if (!isascii(c) || isalnum(c) || c == '_') {
                word_ += c;
            } else {
                if (is_keyword_perl(word_.data(), word_.size())) {
                    *r += HI_KEYWORD;
                    *r += word_;
                    *r += HI_RESET;
                } else {
                    *r += word_;
                }
                word_.clear();
                t_ = NORMAL;
                goto Normal;
            }
            break;

        case VAR:
            t_ = VAR2;
            if (c == '{') {
                *r += c;
                *r += HI_VAR;
                break;
            } else {
                *r += HI_VAR;
            }
            // fallthrough

        case VAR2:
            if (!isascii(c) || isalpha(c) || isdigit(c) || c == '_') {
                *r += c;
            } else {
                *r += HI_RESET;
                t_ = NORMAL;
                goto Normal;
            }
            break;

        case COMMENT:
            if (c == '\n') {
                *r += HI_RESET;
                *r += c;
                t_ = NORMAL;
            } else {
                *r += c;
            }
            break;

        case QUOTE:
            *r += c;
            if (c == '\'') {
                *r += HI_RESET;
                t_ = NORMAL;
            } else if (c == '\\') {
                t_ = QUOTE_BACKSLASH;
            }
            break;

        case QUOTE_BACKSLASH:
            *r += c;
            t_ = QUOTE;
            break;

        case DQUOTE:
            *r += c;
            if (c == '"') {
                *r += HI_RESET;
                t_ = NORMAL;
            } else if (c == '\\') {
                t_ = DQUOTE_BACKSLASH;
            }
            break;

        case DQUOTE_BACKSLASH:
            *r += c;
            t_ = DQUOTE;
            break;

        case TICK:
            *r += c;
            if (c == '`') {
                *r += HI_RESET;
                t_ = NORMAL;
            } else if (c == '\\') {
                t_ = TICK_BACKSLASH;
            }
            break;

        case TICK_BACKSLASH:
            *r += c;
            t_ = TICK;
            break;

        default:
            __builtin_unreachable();
        }
    }
}

void HighlightPerl::flush(std::string *r) {
    switch (t_) {
    case WORD:
        if (is_keyword_perl(word_.data(), word_.size())) {
            *r += HI_KEYWORD;
            *r += word_;
            *r += HI_RESET;
        } else {
            *r += word_;
        }
        word_.clear();
        break;
    case VAR2:
    case TICK:
    case TICK_BACKSLASH:
    case QUOTE:
    case QUOTE_BACKSLASH:
    case DQUOTE:
    case DQUOTE_BACKSLASH:
    case COMMENT:
        *r += HI_RESET;
        break;
    default:
        break;
    }
    t_ = NORMAL;
}