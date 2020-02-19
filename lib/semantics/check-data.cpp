//===-- lib/semantics/check-allocate.cpp ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "check-data.h"
#include "resolve-names-utils.h"
#include "flang/common/indirection.h"
#include "flang/evaluate/fold.h"
#include "flang/evaluate/tools.h"
#include "flang/evaluate/type.h"
#include "flang/parser/parse-tree-visitor.h"
#include "flang/parser/parse-tree.h"
#include "flang/parser/tools.h"
#include "flang/semantics/expression.h"
#include "flang/semantics/semantics.h"
#include "flang/semantics/symbol.h"

namespace Fortran::semantics {

void DataChecker::Leave(const parser::DataStmtConstant &dataConst) {
  // C884
  if (auto *structure{
          std::get_if<parser::StructureConstructor>(&dataConst.u)}) {
    for (const auto &component :
        std::get<std::list<parser::ComponentSpec>>(structure->t)) {
      const parser::Expr &parsedExpr{
          std::get<parser::ComponentDataSource>(component.t).v.value()};
      const auto *expr{GetExpr(parsedExpr)};
      if (!evaluate::IsConstantExpr(*expr)) {  // C884
        context_.Say(parsedExpr.source,
            "The Structure Constructor in DATA value should be a constant expression"_err_en_US);
      }
    }
  }
  // TODO: C886  and C887 for dataConstant
}

void DataChecker::Leave(const parser::DataStmtObject &) {
  // C874-C881
}

void DataChecker::Leave(const parser::DataStmtRepeat &dataRepeat) {
  if (auto *repeatVal{std::get_if<
          parser::Scalar<parser::Integer<parser::ConstantSubobject>>>(
          &dataRepeat.u)}) {
    const parser::Designator &designator{repeatVal->thing.thing.thing.value()};
    if (auto *dataRef{std::get_if<parser::DataRef>(&designator.u)}) {
      evaluate::ExpressionAnalyzer exprAnalyzer{context_};
      if (MaybeExpr checked{exprAnalyzer.Analyze(*dataRef)}) {
        auto expr{evaluate::Fold(foldingContext_, std::move(checked))};
        if (auto i64{ToInt64(expr)}) {
          if (*i64 < 0) {  // C882
            context_.Say(designator.source,
                "The repeat count for data value should be positive"_err_en_US);
          }
        }
      }
    }
  }
}
}
