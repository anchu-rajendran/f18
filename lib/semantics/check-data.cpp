//===-- lib/semantics/check-allocate.cpp ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "check-data.h"
#include <variant>
#include <set>
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


void DataChecker::Leave(const parser::DataStmtConstant &) {
//C883-C887
}

void DataChecker::Leave(const parser::DataStmtObject &) {
//C874-C881
}


void DataChecker::Leave(const parser::DataStmtRepeat &dRepeat) {
  if( auto * repeatVal = std::get_if<parser::Scalar<parser::Integer<parser::ConstantSubobject>>>(&dRepeat.u) ) {
    const parser::Designator &designator= repeatVal->thing.thing.thing.value();
    auto &mutate{const_cast<parser::Designator &>(designator)};
    if (auto *dataRef{std::get_if<parser::DataRef>(&mutate.u)}) {
              evaluate::ExpressionAnalyzer analyzer{context_};
              if (MaybeExpr checked{analyzer.Analyze(*dataRef)}) {
		      auto expr = evaluate::Fold(foldingContext_, std::move(checked));
		      int iv = 0;
		      if ( std::optional<std::int64_t> i64{ToInt64(expr)} ) {
			      iv = *i64;
			      if(iv<0) { //C882
				      context_.Say(designator.source,"The repeat count for data value should be positive"_err_en_US);
			      }
		      }
	      }
    }
  }
}
}

