PROGRAM data02
  type person
    integer :: x
    integer :: y
  end type
  integer, parameter :: repeat = 2
  type(person) myname
  DATA myname%x, myname%y / repeat*35 /
END PROGRAM data02

