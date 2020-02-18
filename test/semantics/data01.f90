module m1
  type person
    integer :: x
    integer :: y
  end type
  integer, parameter::digits(5) = ( /-11,-22,-33,44,55/ )
end

subroutine s1
  use m1
  integer, parameter :: repeat=-1
  type(person) myname
  !ERROR: The repeat count for data value should be positive
  DATA myname%x, myname%y / repeat*35 /
  !ERROR: The repeat count for data value should be positive
  DATA myname%x, myname%y / digits(1)*35 /
end

