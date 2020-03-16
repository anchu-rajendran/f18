//===-- lib/Semantics/check-data.cpp --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "check-data.h"

namespace Fortran::semantics {

void DataChecker::CheckSubscript(const parser::SectionSubscript &subscript) {
  std::visit(
      common::visitors{
          [&](const parser::SubscriptTriplet &triplet) {
            if (const auto &subscriptStart{std::get<0>(triplet.t)}) {
              const auto &parsedExpr{subscriptStart->thing.thing.value()};
              if (const auto *expr{GetExpr(parsedExpr)}) {
                if (!evaluate::IsConstantExpr(*expr)) {  // C875,C881
                  context_.Say(parsedExpr.source,
                      "Subscript must be a constant"_err_en_US);
                }
              }
            }
            if (const auto &subscriptEnd{std::get<1>(triplet.t)}) {
              const auto &parsedExpr{subscriptEnd->thing.thing.value()};
              if (const auto *expr{GetExpr(parsedExpr)}) {
                if (!evaluate::IsConstantExpr(*expr)) {  // C875, C881
                  context_.Say(parsedExpr.source,
                      "Subscript must be a constant"_err_en_US);
                }
              }
            }
            if (const auto &stride{std::get<2>(triplet.t)}) {
              const auto &parsedExpr{stride->thing.thing.value()};
              if (const auto *expr{GetExpr(parsedExpr)}) {
                if (!evaluate::IsConstantExpr(*expr)) {  // C875, C881
                  context_.Say(parsedExpr.source,
                      "Subscript must be a constant"_err_en_US);
                }
              }
            }
          },
          [&](const parser::IntExpr &y) {
            const parser::Expr &parsedExpr{y.thing.value()};
            if (const auto *expr{GetExpr(parsedExpr)}) {
              if (!evaluate::IsConstantExpr(*expr)) {  // C875, C881
                context_.Say(parsedExpr.source,
                    "Subscript must be a constant"_err_en_US);
              }
            }
          },
      },
      subscript.u);
}

// Returns false if  DataRef has no subscript
bool DataChecker::CheckAllSubscriptsInDataRef(
    const parser::DataRef &dataRef, const parser::CharBlock &source) {
  return std::visit(
      common::visitors{
          [&](const parser::Name &) { return false; },
          [&](const common::Indirection<parser::StructureComponent>
                  &structureComp) {
            return CheckAllSubscriptsInDataRef(
                structureComp.value().base, source);
          },
          [&](const common::Indirection<parser::ArrayElement> &arrayElem) {
            for (auto &subscript : arrayElem.value().subscripts) {
              CheckSubscript(subscript);
            }
            CheckAllSubscriptsInDataRef(arrayElem.value().base, source);
            return true;
          },
          [&](const common::Indirection<parser::CoindexedNamedObject>
                  &coindexedObj) {  // C874
            context_.Say(source,
                "Data object must not be a coindexed variable"_err_en_US);
            CheckAllSubscriptsInDataRef(coindexedObj.value().base, source);
            return true;
          },
      },
      dataRef.u);
}

void DataChecker::Leave(const parser::DataStmtConstant &dataConst) {
  if (auto *structure{
          std::get_if<parser::StructureConstructor>(&dataConst.u)}) {
    for (const auto &component :
        std::get<std::list<parser::ComponentSpec>>(structure->t)) {
      const parser::Expr &parsedExpr{
          std::get<parser::ComponentDataSource>(component.t).v.value()};
      if (const auto *expr{GetExpr(parsedExpr)}) {
        if (!evaluate::IsConstantExpr(*expr)) {  // C884
          context_.Say(parsedExpr.source,
              "Structure constructor in data value must be a constant expression"_err_en_US);
        }
      }
    }
  }
}

// TODO: C876, C877
void DataChecker::Leave(const parser::DataImpliedDo &dataImpliedDo) {
  for (const auto &object :
      std::get<std::list<parser::DataIDoObject>>(dataImpliedDo.t)) {
    if (const auto *designator{parser::Unwrap<parser::Designator>(object)}) {
      if (auto *dataRef{std::get_if<parser::DataRef>(&designator->u)}) {
        evaluate::ExpressionAnalyzer exprAnalyzer{context_};
        if (MaybeExpr checked{exprAnalyzer.Analyze(*dataRef)}) {
          if (evaluate::IsConstantExpr(*checked)) {  // C878, C879
            context_.Say(
                designator->source, "Data object must be a variable"_err_en_US);
          }
        }
        if (!CheckAllSubscriptsInDataRef(
                *dataRef, designator->source)) {  // C880
          context_.Say(designator->source,
              "Data implied do object must be subscripted"_err_en_US);
        }
      }
    }
  }
}

void DataChecker::Leave(const parser::DataStmtObject &dataObject) {
  if (std::get_if<common::Indirection<parser::Variable>>(&dataObject.u)) {
    if (const auto *designator{
            parser::Unwrap<parser::Designator>(dataObject)}) {
      if (auto *dataRef{std::get_if<parser::DataRef>(&designator->u)}) {
        CheckAllSubscriptsInDataRef(*dataRef, designator->source);
      }
    } else {  // C875
      context_.Say("Data object variable must be a designator"_err_en_US);
    }
  }
}

void DataChecker::Leave(const parser::DataStmtRepeat &dataRepeat) {
  if (const auto *designator{parser::Unwrap<parser::Designator>(dataRepeat)}) {
    if (auto *dataRef{std::get_if<parser::DataRef>(&designator->u)}) {
      evaluate::ExpressionAnalyzer exprAnalyzer{context_};
      if (MaybeExpr checked{exprAnalyzer.Analyze(*dataRef)}) {
        auto expr{
            evaluate::Fold(context_.foldingContext(), std::move(checked))};
        if (auto i64{ToInt64(expr)}) {
          if (*i64 < 0) {  // C882
            context_.Say(designator->source,
                "Repeat count for data value must not be negative"_err_en_US);
          }
        }
      }
    }
  }
}
}
