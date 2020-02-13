PROGRAM data02
  type person
    integer :: x
    integer :: y
    character(len=25) :: name
  end type
  integer, parameter :: repeat = 2
  integer :: digit
  !character(len=13), parameter:: fullName = 'Mark Williams'
  integer, parameter::digits(5) = ( /-1,-2,-3,4,5/ )
  type(person) myname
  DATA myname%x, myname%y / digits(1)*35 /
 ! DATA myname%name /fullName(1:4)/
  !DATA digit /digits(1)/
  print *, digits(1)
END PROGRAM data02

