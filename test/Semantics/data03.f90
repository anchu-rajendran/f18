!Testing data constraints : C874 - C875, C878 - C881 
module m
  contains
    function f(i)
      integer ::i
      integer ::result
      result = i *1024 
    end
    subroutine CheckObject 
      type specialNumbers
        integer :: one
        integer :: numbers(5)
      end type
      type(specialNumbers), parameter ::newNums = specialNumbers(1, (/ 1, 2, 3, 4, 5 /))
      type(specialNumbers) nums
      real :: a[*]
      real :: b(5)
      integer ::x
      real,parameter ::c(5) = (/ 1, 2, 3, 4, 5 /)
      integer ::d(10, 10)
      character ::name(12)
      integer ::ind = 2
      !C874
      !ERROR: Data Implied Do Object must not be a coindexed variable
      DATA(a[i], i = 1, 5) / 5 * 1 /
      !C875
      !ERROR : Data Object variable must be a designator
      DATA f(1) / 1 / 
      !C875
      !ERROR : Subscript must be a constant
      DATA b(ind) / 1 /
      !C875
      !ERROR : Subscript must be a constant
      DATA name( : ind) / 'Ancd' /
      !C875
      !ERROR : Subscript must be a constant
      DATA name(ind:) / 'Ancd' /
      !C878
      !ERROR : Data Object must be a variable
      DATA(c(i), i = 1, 5) / 5 * 1 /
      !C879
      !ERROR : Data Object must be a variable
      DATA(newNums % numbers(i), i = 1, 5) / 5 * 1 /
      !C880
      !ERROR : Data Object in implied-do must be subscripted 
      DATA(nums % one, i = 1, 5) / 5 * 1 /
      !C881
      !ERROR : Subscript must be a constant
      DATA(b(x), i = 1, 5) / 5 * 1 /
      !C881 
      !OK : Correct use
      DATA(nums % numbers(i), i = 1, 5) / 5 * 1 /
      !C881
      !OK : Correct use
      DATA((d(i, j), i = 1, 10), j = 1, 10) / 100 * 1 /
    end 
  end