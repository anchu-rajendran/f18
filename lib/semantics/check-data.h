//===-- lib/semantics/check-allocate.h --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef FORTRAN_SEMANTICS_CHECK_DATA_H_
#define FORTRAN_SEMANTICS_CHECK_DATA_H_

#include "flang/evaluate/fold.h"
#include "flang/semantics/semantics.h"
#include "flang/semantics/tools.h"

namespace Fortran::parser {
struct DataStmt;
struct DataStmtRepeat;
}

namespace Fortran::semantics {
class DataChecker : public virtual BaseChecker {
public:
  DataChecker(SemanticsContext &context)
    : context_{context}, foldingContext_{context_.foldingContext()} {}
  void Leave(const parser::DataStmtRepeat &);
  void Leave(const parser::DataStmtConstant &);
  void Leave(const parser::DataStmtObject &);

private:
  SemanticsContext &context_;
  evaluate::FoldingContext &foldingContext_;
};
}
#endif  // FORTRAN_SEMANTICS_CHECK_DATA_H_
