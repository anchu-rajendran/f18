module m
contains
  function f(i)
    integer :: i
    integer :: result
    result = i * 1024
  end
  subroutine CheckObject
   type specialNumbers
     integer :: a
     integer :: numbers(5)
   end type
   type Numbers
     integer :: numbers(5)
   end type
   type(specialNumbers), parameter :: newNums = specialNumbers(1, (/1,2,3,4,5/)) 
   type(Numbers) nums
   real :: a[*]
   real :: b(5)
   integer :: x
   real, parameter :: c(5) = (/1,2,3,4,5/)
   character :: name(12)
   DATA (a[i], i=1, 100) / 100 * 1 /
   DATA (c(i), i=1, 100) / 100 * 1 /
   DATA (b(x), i=1, 100) / 100 * 1 /
   DATA (newNums%numbers(i), i=1, 100) / 100 * 1 /
   DATA (newNums%a, i=1, 100) / 100 * 1 /
   integer :: ind = 2
   DATA f(1) /1/
   DATA b(ind) /  1 /
   DATA name(ind:ind) /'Ancd'/
  end
end
