//===-- lib/semantics/check-allocate.cpp ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "check-data.h"
#include "flang/parser/parse-tree.h"
#include "flang/semantics/tools.h"
#include "flang/semantics/type.h"

namespace Fortran::semantics {

struct DataCheckerInfo {

};

class DataCheckerHelper {
};

void DataChecker::Leave(const parser::DataStmt &) {
  std::cout<< "found a data statement\n";
  if(false)
      CHECK(context_.AnyFatalError());
}
void DataChecker::Leave(const parser::DataStmtRepeat &) {
  std::cout<< "found a data statementrepeat\n";
  if(false)
      CHECK(context_.AnyFatalError());
}
}

