        segment .data
message db      "Hello world!",0x0a,0
        segment .text
        global  main
        extern  printf

main:
        push    rbp
        mov     rbp, rsp
        frame	2, 0, 1
	sub	rsp, frame_size
        lea     rcx, [message]
        call    printf
        leave
        ret
