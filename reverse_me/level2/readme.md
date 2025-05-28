gdb
break main 
run 
next disasemble main



   0x565562d0 <+0>:     push   %ebp                         // prologue standard
   0x565562d1 <+1>:     mov    %esp,%ebp                    
   0x565562d3 <+3>:     push   %ebx                         //saving ebx (non volatil)
=> 0x565562d4 <+4>:     sub    $0x54,%esp                   // allocate 0x54 bytes on the pile
   0x565562d7 <+7>:     call   0x565562dc <main+12>         // call the address
   0x565562dc <+12>:    pop    %ebx                         //get the address in %ebx
   0x565562dd <+13>:    add    $0x5d24,%ebx                 //calculate the base address of .data 
   0x565562e3 <+19>:    mov    %ebx,-0x40(%ebp)             //stock base localy
   0x565562e6 <+22>:    movl   $0x0,-0x8(%ebp)              //get a relativ address
   0x565562ed <+29>:    lea    -0x42e5(%ebx),%eax
   0x565562f3 <+35>:    mov    %eax,(%esp)
   0x565562f6 <+38>:    call   0x56556060 <printf@plt>      //print "Entrez votre mot de passe :"
   0x565562fb <+43>:    mov    -0x40(%ebp),%ebx
   0x565562fe <+46>:    lea    -0x35(%ebp),%eax             //read buffer
   0x56556301 <+49>:    lea    -0x42d2(%ebx),%ecx           // string format probably
   0x56556307 <+55>:    mov    %ecx,(%esp)
   0x5655630a <+58>:    mov    %eax,0x4(%esp)
   0x5655630e <+62>:    call   0x565560c0 <__isoc99_scanf@plt>  //scanf
   0x56556313 <+67>:    mov    %eax,-0xc(%ebp)
   0x56556316 <+70>:    mov    $0x1,%eax
   0x5655631b <+75>:    cmp    -0xc(%ebp),%eax                  // check scanf return
   0x5655631e <+78>:    je     0x5655632c <main+92>
   0x56556324 <+84>:    mov    -0x40(%ebp),%ebx
   0x56556327 <+87>:    call   0x56556220 <no>
   0x5655632c <+92>:    movsbl -0x34(%ebp),%ecx             
   0x56556330 <+96>:    mov    $0x30,%eax                        // $0x30 = 0
   0x56556335 <+101>:   cmp    %ecx,%eax                        // input[1] == 0
   0x56556337 <+103>:   je     0x56556345 <main+117>
   0x5655633d <+109>:   mov    -0x40(%ebp),%ebx
   0x56556340 <+112>:   call   0x56556220 <no>
   0x56556345 <+117>:   movsbl -0x35(%ebp),%ecx
   0x56556349 <+121>:   mov    $0x30,%eax
   0x5655634e <+126>:   cmp    %ecx,%eax                        //input[0] == 0
   0x56556350 <+128>:   je     0x5655635e <main+142>
   0x56556356 <+134>:   mov    -0x40(%ebp),%ebx
   0x56556359 <+137>:   call   0x56556220 <no>
   0x5655635e <+142>:   mov    -0x40(%ebp),%ebx
   0x56556361 <+145>:   mov    -0xc(%ebx),%eax
   0x56556367 <+151>:   mov    (%eax),%eax
   0x56556369 <+153>:   mov    -0xc(%ebx),%ecx
   0x5655636f <+159>:   mov    %eax,(%esp)
   0x56556372 <+162>:   call   0x56556070 <fflush@plt>
   0x56556377 <+167>:   mov    -0x40(%ebp),%ebx
   0x5655637a <+170>:   lea    -0x1d(%ebp),%eax                 // buffer size 9
   0x5655637d <+173>:   xor    %ecx,%ecx                       
   0x5655637f <+175>:   mov    %eax,(%esp)
   0x56556382 <+178>:   movl   $0x0,0x4(%esp)
   0x5655638a <+186>:   movl   $0x9,0x8(%esp)
   0x56556392 <+194>:   call   0x565560b0 <memset@plt>          //memeset(buff, 0 , 9)
   0x56556397 <+199>:   movb   $0x64,-0x1d(%ebp)                // pu a "d" at the begining
   0x5655639b <+203>:   movb   $0x0,-0x36(%ebp)
   0x5655639f <+207>:   movl   $0x2,-0x14(%ebp)
   0x565563a6 <+214>:   movl   $0x1,-0x10(%ebp)
   0x565563ad <+221>:   mov    -0x40(%ebp),%ebx
   0x565563b0 <+224>:   lea    -0x1d(%ebp),%ecx
   0x565563b3 <+227>:   mov    %esp,%eax
   0x565563b5 <+229>:   mov    %ecx,(%eax)
   0x565563b7 <+231>:   call   0x565560a0 <strlen@plt>
   0x565563bc <+236>:   mov    %eax,%ecx
   0x565563be <+238>:   xor    %eax,%eax
   0x565563c0 <+240>:   cmp    $0x8,%ecx
   0x565563c3 <+243>:   mov    %al,-0x41(%ebp)
   0x565563c6 <+246>:   jae    0x565563ee <main+286>
   0x565563cc <+252>:   mov    -0x40(%ebp),%ebx
   0x565563cf <+255>:   mov    -0x14(%ebp),%eax
   0x565563d2 <+258>:   mov    %eax,-0x48(%ebp)
   0x565563d5 <+261>:   lea    -0x35(%ebp),%ecx
   0x565563d8 <+264>:   mov    %esp,%eax
   0x565563da <+266>:   mov    %ecx,(%eax)
   0x565563dc <+268>:   call   0x565560a0 <strlen@plt>
   0x565563e1 <+273>:   mov    %eax,%ecx
--Type <RET> for more, q to quit, c to continue without paging--RET
   0x565563e3 <+275>:   mov    -0x48(%ebp),%eax
   0x565563e6 <+278>:   cmp    %ecx,%eax
   0x565563e8 <+280>:   setb   %al
   0x565563eb <+283>:   mov    %al,-0x41(%ebp)
   0x565563ee <+286>:   mov    -0x41(%ebp),%al
   0x565563f1 <+289>:   test   $0x1,%al
   0x565563f3 <+291>:   jne    0x565563fe <main+302>
   0x565563f9 <+297>:   jmp    0x5655644a <main+378>
   0x565563fe <+302>:   mov    -0x40(%ebp),%ebx
   0x56556401 <+305>:   mov    -0x14(%ebp),%eax
   0x56556404 <+308>:   mov    -0x35(%ebp,%eax,1),%al     //extract char 1
   0x56556408 <+312>:   mov    %al,-0x39(%ebp)
   0x5655640b <+315>:   mov    -0x14(%ebp),%eax
   0x5655640e <+318>:   mov    -0x34(%ebp,%eax,1),%al    //extract char 2
   0x56556412 <+322>:   mov    %al,-0x38(%ebp)
   0x56556415 <+325>:   mov    -0x14(%ebp),%eax
   0x56556418 <+328>:   mov    -0x33(%ebp,%eax,1),%al   //extract char 3
   0x5655641c <+332>:   mov    %al,-0x37(%ebp)
   0x5655641f <+335>:   lea    -0x39(%ebp),%eax
   0x56556422 <+338>:   mov    %eax,(%esp)
   0x56556425 <+341>:   call   0x565560d0 <atoi@plt>      // read the 3 extracted caracters
   0x5655642a <+346>:   mov    %al,%cl
   0x5655642c <+348>:   mov    -0x10(%ebp),%eax
   0x5655642f <+351>:   mov    %cl,-0x1d(%ebp,%eax,1)
   0x56556433 <+355>:   mov    -0x14(%ebp),%eax
   0x56556436 <+358>:   add    $0x3,%eax
   0x56556439 <+361>:   mov    %eax,-0x14(%ebp)
   0x5655643c <+364>:   mov    -0x10(%ebp),%eax
   0x5655643f <+367>:   add    $0x1,%eax
   0x56556442 <+370>:   mov    %eax,-0x10(%ebp)
   0x56556445 <+373>:   jmp    0x565563ad <main+221>
   0x5655644a <+378>:   mov    -0x40(%ebp),%ebx
   0x5655644d <+381>:   mov    -0x10(%ebp),%eax
   0x56556450 <+384>:   movb   $0x0,-0x1d(%ebp,%eax,1)
   0x56556455 <+389>:   lea    -0x1d(%ebp),%ecx                // constructed string
   0x56556458 <+392>:   lea    -0x42cd(%ebx),%edx              // reference
   0x5655645e <+398>:   mov    %esp,%eax
   0x56556460 <+400>:   mov    %edx,0x4(%eax)
   0x56556463 <+403>:   mov    %ecx,(%eax)
   0x56556465 <+405>:   call   0x56556040 <strcmp@plt>          //final cmp
   0x5655646a <+410>:   cmp    $0x0,%eax
   0x5655646d <+413>:   jne    0x56556480 <main+432>
   0x56556473 <+419>:   mov    -0x40(%ebp),%ebx
   0x56556476 <+422>:   call   0x565562a0 <ok>
   0x5655647b <+427>:   jmp    0x56556488 <main+440>
   0x56556480 <+432>:   mov    -0x40(%ebp),%ebx
   0x56556483 <+435>:   call   0x56556220 <no>
   0x56556488 <+440>:   xor    %eax,%eax
   0x5655648a <+442>:   add    $0x54,%esp
   0x5655648d <+445>:   pop    %ebx
   0x5655648e <+446>:   pop    %ebp
   0x5655648f <+447>:   ret   