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

Highlight *Highlight::create(const std::string_view &lang) {
    if (lang == "markdown" || lang == "md") {
        return new HighlightMarkdown;
    } else if (lang == "c" || //
               lang == "h" || //
               lang == "m") {
        return new HighlightC(is_keyword_c, //
                              is_keyword_c_type, //
                              is_keyword_c_builtin, //
                              is_keyword_c_constant);
    } else if (lang == "c++" || //
               lang == "cxx" || //
               lang == "cpp" || //
               lang == "hpp" || //
               lang == "cc" || //
               lang == "cu") {
        return new HighlightC(is_keyword_cxx, //
                              is_keyword_c_type, //
                              is_keyword_c_builtin, //
                              is_keyword_c_constant);
    } else if (lang == "js" || //
               lang == "json" || //
               lang == "javascript") {
        return new HighlightJs(is_keyword_js, nullptr);
    } else if (lang == "ts" || //
               lang == "typescript") {
        return new HighlightJs(is_keyword_typescript, is_keyword_typescript_type);
    } else if (lang == "java") {
        return new HighlightC(is_keyword_java);
    } else if (lang == "py" || //
               lang == "python") {
        return new HighlightPython;
    } else if (lang == "rs" || //
               lang == "rust") {
        return new HighlightRust;
    } else if (lang == "f" || //
               lang == "fortran") {
        return new HighlightFortran;
    } else if (lang == "cob" || //
               lang == "cbl" || //
               lang == "cobol") {
        return new HighlightCobol;
    } else if (lang == "pas" || //
               lang == "pascal") {
        return new HighlightPascal;
    } else if (lang == "go") {
        return new HighlightGo;
    } else if (lang == "sql") {
        return new HighlightSql;
    } else if (lang == "css") {
        return new HighlightCss;
    } else if (lang == "html" || //
               lang == "xml") {
        return new HighlightHtml;
    } else if (lang == "php") {
        return new HighlightHtml; // sic
    } else if (lang == "php!") {
        return new HighlightPhp;
    } else if (lang == "csharp" || //
               lang == "cs" || //
               lang == "c#") {
        return new HighlightCsharp;
    } else if (lang == "kotlin") {
        return new HighlightC(is_keyword_kotlin);
    } else if (lang == "lua") {
        return new HighlightLua;
    } else if (lang == "lisp" || //
               lang == "el" || //
               lang == "elisp" || //
               lang == "cl" || //
               lang == "clisp" || //
               lang == "scheme" || //
               lang == "clojure") {
        return new HighlightLisp;
    } else if (lang == "ada") {
        return new HighlightAda;
    } else if (lang == "haskell" || //
               lang == "hs") {
        return new HighlightHaskell;
    } else if (lang == "perl" || //
               lang == "pl") {
        return new HighlightPerl;
    } else if (lang == "shell" || //
               lang == "bash" || //
               lang == "sh" || //
               lang == "ksh") {
        return new HighlightShell;
    } else if (lang == "swift") {
        return new HighlightC(is_keyword_swift, is_keyword_swift_type);
    } else if (lang == "d") {
        return new HighlightC(is_keyword_d);
    } else if (lang == "zig") {
        return new HighlightZig;
    } else if (lang == "tcl") {
        return new HighlightTcl;
    } else if (lang == "ruby" || //
               lang == "rb") {
        return new HighlightRuby;
    } else {
        return new HighlightPlain;
    }
}
