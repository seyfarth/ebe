       program hello
       logical(kind=8) a
       character*16 x
       integer y(10)
       real c
       double precision d
       a = .true.
       c = 1.4
       d = c * 2
       x = 'Hello'
       y(1) = 1
       print *, 'Hello World!',a, y(1), c, x
       stop
       end
