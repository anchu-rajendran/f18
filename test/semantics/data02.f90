module m
contains
  function f(i)
    integer :: i
    integer :: result
    result = i * 1024
  end
  subroutine CheckObject
   integer, parameter::a=1
   !real :: a[*]
   real :: b(5)
   character :: name(12)
   !DATA (a[i], i=1, 100) / 100 * 1 /
   integer :: ind = 2
   DATA f(1) /1/
   DATA b(ind) /  1 /
   DATA name(ind:ind) /'Ancd'/
  end
end
