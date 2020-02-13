//===-- lib/semantics/check-allocate.h --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef FORTRAN_SEMANTICS_CHECK_DATA_H_
#define FORTRAN_SEMANTICS_CHECK_DATA_H_

#include "flang/semantics/semantics.h"
<<<<<<< HEAD
#include "flang/common/indirection.h"
#include "flang/evaluate/expression.h"
#include "flang/semantics/semantics.h"
#include "flang/semantics/tools.h"
#include <string>
=======
>>>>>>> 0d596e2b9e01b485db8f9f1017a0e909f75d4a96
#include <iostream>

namespace Fortran::parser {
struct DataStmt;
struct DataStmtRepeat;
<<<<<<< HEAD
class ParseTreeDumper;
=======
>>>>>>> 0d596e2b9e01b485db8f9f1017a0e909f75d4a96
}

namespace Fortran::semantics {
class DataChecker : public virtual BaseChecker {
public:
  DataChecker(SemanticsContext &context) : context_{context} {}
  void Leave(const parser::DataStmt &);
  void Leave(const parser::DataStmtRepeat &);
  void CheckDataStmtRepeatSemantics(const parser::Scalar<parser::Integer<parser::ConstantSubobject>> &);
private:
  SemanticsContext &context_;
};
}
#endif  // FORTRAN_SEMANTICS_CHECK_DATA_H_
