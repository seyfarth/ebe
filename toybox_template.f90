        program main
            real (kind=8) r8
            real (kind=4) r4
            interface dump
                subroutine d_dump(x)
                real(kind=8) x
                integer(kind=1) b(8)
                print *, 'real (kind=8)'
                return
                end subroutine d_dump
                subroutine f_dump(x)
                real(kind=4) x
                integer(kind=1) b(4)
                print *, 'real (kind=4)'
                return
                end subroutine f_dump
            end interface dump
            r8 = 12.5
            r4 = 12.5
            call dump(r8)
            call dump(r4)
            stop
        end program main
