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
#include "assignment.h"
#include "check-omp-structure.h"
#include "mod-file.h"
#include "program-tree.h"
#include "resolve-names-utils.h"
#include "rewrite-parse-tree.h"
#include "flang/common/Fortran.h"
#include "flang/common/default-kinds.h"
#include "flang/common/indirection.h"
#include "flang/common/restorer.h"
#include "flang/evaluate/characteristics.h"
#include "flang/evaluate/common.h"
#include "flang/evaluate/fold.h"
#include "flang/evaluate/intrinsics.h"
#include "flang/evaluate/tools.h"
#include "flang/evaluate/type.h"
#include "flang/evaluate/formatting.h"
#include "flang/parser/parse-tree-visitor.h"
#include "flang/parser/parse-tree.h"
#include "flang/parser/tools.h"
#include "flang/semantics/attr.h"
#include "flang/semantics/expression.h"
#include "flang/semantics/scope.h"
#include "flang/semantics/semantics.h"
#include "flang/semantics/symbol.h"
#include "flang/parser/parse-tree.h"
#include "flang/semantics/tools.h"
#include "flang/semantics/type.h"

namespace Fortran::semantics {

void DataChecker::CheckDataStmtRepeatSemantics(const parser::Scalar<parser::Integer<parser::ConstantSubobject>> &dRepeatSubobj){
    const auto name{parser::GetLastName((dRepeatSubobj.thing).thing.thing.value())};
    const Symbol * symbol{name.symbol? &name.symbol->GetUltimate() : nullptr};
    const Symbol * root{GetAssociationRoot(*symbol)};
    if(IsNamedConstant(*root)) {
      if (const auto *object{(*root).detailsIf<semantics::ObjectEntityDetails>()}) {
        if(const auto init{object->init()}) {
              if ( std::optional<std::int64_t> i64{ToInt64(init)} ) {
                int iv = *i64;
                if (iv<0) {
			context_.Say(name.source,"The Named Constant should be initialized with a positive value"_err_en_US);
                }
              }
         }else {
            //C882
            context_.Say(name.source,"Named Constant should be initialized"_err_en_US);
         }
      }
    }else{
      std::cout <<"not name\n";
    }
}
struct DataCheckerInfo {

};

class DataCheckerHelper {
};

void DataChecker::Leave(const parser::DataStmt &) {
  std::cout<< "found a data statement\n";
  if(false)
      CHECK(context_.AnyFatalError());
}

void DataChecker::Leave(const parser::DataStmtRepeat &dRepeat) {
  if( auto * repeatVal = std::get_if<parser::Scalar<parser::Integer<parser::NamedConstant>>>(&dRepeat.u) )
    std::cout<< "1\n";
  else if( auto * repeatVal = std::get_if<parser::IntLiteralConstant>(&dRepeat.u) )
    std::cout<< "2\n";
  else if( auto * repeatVal = std::get_if<parser::Scalar<parser::Integer<parser::ConstantSubobject>>>(&dRepeat.u) ) {
    std::cout<< "3\n";
    CheckDataStmtRepeatSemantics(*repeatVal);

  }
}
}

