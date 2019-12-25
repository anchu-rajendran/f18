!===-- module/__fortran_builtins.f90 ---------------------------------------===!
!
! Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
! See https://llvm.org/LICENSE.txt for license information.
! SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
!
!------------------------------------------------------------------------------!

! These naming shenanigans prevent names from Fortran intrinsic modules
! from being usable on INTRINSIC statements, and force the program
! to USE the standard intrinsic modules in order to access the
! standard names of the procedures.
module __Fortran_builtins
  intrinsic :: __builtin_c_f_pointer
end module
