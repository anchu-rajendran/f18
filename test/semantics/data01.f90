PROGRAM test
  type person
    integer :: age
    character(len=25) :: name
  end type
  type(person) myname
  integer :: dummy
  dummy = 26
  DATA myname / PERSON(dummy, 'Anchu') /
END PROGRAM test

