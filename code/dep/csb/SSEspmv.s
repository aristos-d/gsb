# -- Machine type EFI2
# mark_description "Intel(R) C++ Compiler for applications running on Intel(R) 64, Version 11.1    Build 20100414 %s";
# mark_description "-O2 -DAMD -DICC -S";
	.file "SSEspmv.cpp"
	.text
..TXTST0:
# -- Begin  _Z10BitReverset
# mark_begin;
       .align    16,0x90
	.globl _Z10BitReverset
_Z10BitReverset:
# parameter 1: %edi
..B1.1:                         # Preds ..B1.0
..___tag_value__Z10BitReverset.1:                               #134.1
        movzwl    %di, %r8d                                     #135.40
        movl      %r8d, %edx                                    #135.44
        andl      $63, %edx                                     #135.44
        movzbl    BitReverseTable64(%rdx), %eax                 #135.22
        shll      $10, %eax                                     #135.53
        movl      %r8d, %ecx                                    #136.31
        shrl      $6, %ecx                                      #136.31
        andl      $63, %ecx                                     #136.36
        movzbl    BitReverseTable64(%rcx), %esi                 #136.7
        shll      $4, %esi                                      #136.45
        orl       %esi, %eax                                    #136.45
        shrl      $12, %r8d                                     #137.31
        movzbl    BitReverseTable64(%r8), %r9d                  #137.7
        shrl      $2, %r9d                                      #137.46
        orl       %r9d, %eax                                    #137.46
        ret                                                     #139.16
        .align    16,0x90
..___tag_value__Z10BitReverset.2:                               #
                                # LOE
# mark_end;
	.type	_Z10BitReverset,@function
	.size	_Z10BitReverset,.-_Z10BitReverset
	.data
# -- End  _Z10BitReverset
	.text
# -- Begin  _Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj
# mark_begin;
       .align    16,0x90
	.globl _Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj
_Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 8 + %rsp
# parameter 8: 16 + %rsp
# parameter 9: 24 + %rsp
# parameter 10: 32 + %rsp
..B2.1:                         # Preds ..B2.0
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.3:              #164.1
        movq      %r8, %r10                                     #164.1
        xorl      %eax, %eax                                    #165.16
        testl     %ecx, %ecx                                    #165.22
        jbe       ..B2.13       # Prob 10%                      #165.22
                                # LOE rax rdx rbx rbp rsi rdi r9 r10 r12 r13 r14 r15 ecx
..B2.2:                         # Preds ..B2.1
        movl      32(%rsp), %r8d                                #165.28
        movl      %ecx, -8(%rsp)                                #165.28
        movq      %rbx, -16(%rsp)                               #165.28
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.4:              #
        xorl      %r11d, %r11d                                  #165.28
        movq      %r12, -24(%rsp)                               #165.28
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.5:              #
        movq      8(%rsp), %r12                                 #165.28
        movq      %r13, -32(%rsp)                               #165.28
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.6:              #
        movq      16(%rsp), %r13                                #165.28
        movq      %r14, -40(%rsp)                               #165.28
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.7:              #
        movl      24(%rsp), %r14d                               #165.28
        movq      %r15, -48(%rsp)                               #165.28
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.8:              #
        movq      %rbp, -56(%rsp)                               #165.28
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.9:              #
                                # LOE rax rdx rsi rdi r9 r10 r12 r13 r11d r14d
..B2.3:                         # Preds ..B2.11 ..B2.2
        movl      32(%rsp), %ecx                                #168.38
        movl      (%rdx,%rax,4), %ebx                           #167.27
        movl      %r14d, %ebp                                   #167.36
        andl      %ebx, %ebp                                    #167.36
        shrl      %cl, %ebx                                     #168.38
        movzbl    (%rsi,%rax), %ecx                             #169.24
        andl      %r14d, %ebx                                   #168.50
        testb     $8, %cl                                       #170.13
        je        ..B2.5        # Prob 85%                      #170.13
                                # LOE rdx rsi rdi r9 r10 r12 r13 ecx ebx ebp r11d r14d
..B2.4:                         # Preds ..B2.3
        movsd     (%rdi), %xmm0                                 #172.42
        movl      %ebx, %ebx                                    #172.31
        movl      %ebp, %ebp                                    #172.47
        mulsd     (%r10,%rbp,8), %xmm0                          #172.47
        movsd     %xmm0, -120(%rsp)                             #172.4
# Begin ASM
        movq  (%r12,%rbx,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -120(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r12,%rbx,8)
	jnz 1b
	
# End ASM                                                       #172.4
        movsd     (%rdi), %xmm1                                 #173.43
        mulsd     (%r9,%rbx,8), %xmm1                           #173.48
        movsd     %xmm1, -112(%rsp)                             #173.4
# Begin ASM
        movq  (%r13,%rbp,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -112(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r13,%rbp,8)
	jnz 1b
	
# End ASM                                                       #173.4
        addq      $8, %rdi                                      #174.4
                                # LOE rdx rsi rdi r9 r10 r12 r13 ecx ebx ebp r11d r14d
..B2.5:                         # Preds ..B2.4 ..B2.3
        testb     $4, %cl                                       #177.13
        je        ..B2.7        # Prob 85%                      #177.13
                                # LOE rdx rsi rdi r9 r10 r12 r13 ecx ebx ebp r11d r14d
..B2.6:                         # Preds ..B2.5
        lea       1(%rbx), %r15d                                #179.36
        movl      %ebp, %ebp                                    #179.47
        movsd     (%r10,%rbp,8), %xmm0                          #179.47
        mulsd     (%rdi), %xmm0                                 #179.47
        movsd     %xmm0, -104(%rsp)                             #179.4
# Begin ASM
        movq  (%r12,%r15,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -104(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r12,%r15,8)
	jnz 1b
	
# End ASM                                                       #179.4
        movsd     (%r9,%r15,8), %xmm1                           #180.48
        mulsd     (%rdi), %xmm1                                 #180.48
        movsd     %xmm1, -96(%rsp)                              #180.4
# Begin ASM
        movq  (%r13,%rbp,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -96(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r13,%rbp,8)
	jnz 1b
	
# End ASM                                                       #180.4
        addq      $8, %rdi                                      #181.4
                                # LOE rdx rsi rdi r9 r10 r12 r13 ecx ebx ebp r11d r14d
..B2.7:                         # Preds ..B2.6 ..B2.5
        testb     $2, %cl                                       #184.13
        je        ..B2.9        # Prob 85%                      #184.13
                                # LOE rdx rsi rdi r9 r10 r12 r13 ecx ebx ebp r11d r14d
..B2.8:                         # Preds ..B2.7
        movl      %ebx, %ebx                                    #186.31
        lea       1(%rbp), %r15d                                #186.52
        movsd     (%r10,%r15,8), %xmm0                          #186.47
        mulsd     (%rdi), %xmm0                                 #186.47
        movsd     %xmm0, -88(%rsp)                              #186.4
# Begin ASM
        movq  (%r12,%rbx,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -88(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r12,%rbx,8)
	jnz 1b
	
# End ASM                                                       #186.4
        movsd     (%r9,%rbx,8), %xmm1                           #187.48
        mulsd     (%rdi), %xmm1                                 #187.48
        movsd     %xmm1, -80(%rsp)                              #187.4
# Begin ASM
        movq  (%r13,%r15,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -80(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r13,%r15,8)
	jnz 1b
	
# End ASM                                                       #187.4
        addq      $8, %rdi                                      #188.4
                                # LOE rdx rsi rdi r9 r10 r12 r13 ecx ebx ebp r11d r14d
..B2.9:                         # Preds ..B2.8 ..B2.7
        testb     $1, %cl                                       #191.13
        je        ..B2.11       # Prob 85%                      #191.13
                                # LOE rdx rsi rdi r9 r10 r12 r13 ebx ebp r11d r14d
..B2.10:                        # Preds ..B2.9
        incl      %ebx                                          #193.36
        incl      %ebp                                          #193.52
        movsd     (%r10,%rbp,8), %xmm0                          #193.47
        mulsd     (%rdi), %xmm0                                 #193.47
        movsd     %xmm0, -72(%rsp)                              #193.4
# Begin ASM
        movq  (%r12,%rbx,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -72(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r12,%rbx,8)
	jnz 1b
	
# End ASM                                                       #193.4
        movsd     (%r9,%rbx,8), %xmm1                           #194.48
        mulsd     (%rdi), %xmm1                                 #194.48
        movsd     %xmm1, -64(%rsp)                              #194.4
# Begin ASM
        movq  (%r13,%rbp,8), %rax 
	xorpd %xmm0, %xmm0 
	movsd -64(%rsp), %xmm0
	1:
	movq  %rax,  %xmm1
	addsd %xmm0, %xmm1
	movq  %xmm1, %r8  
	lock cmpxchgq %r8, (%r13,%rbp,8)
	jnz 1b
	
# End ASM                                                       #194.4
        addq      $8, %rdi                                      #195.4
                                # LOE rdx rsi rdi r9 r10 r12 r13 r11d r14d
..B2.11:                        # Preds ..B2.10 ..B2.9
        incl      %r11d                                         #165.28
        movl      %r11d, %eax                                   #165.28
        movl      %eax, %r11d                                   #165.2
        cmpl      -8(%rsp), %r11d                               #165.22
        jb        ..B2.3        # Prob 82%                      #165.22
                                # LOE rax rdx rsi rdi r9 r10 r12 r13 r11d r14d
..B2.12:                        # Preds ..B2.11
        movq      -16(%rsp), %rbx                               #
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.10:             #
        movq      -24(%rsp), %r12                               #
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.11:             #
        movq      -32(%rsp), %r13                               #
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.12:             #
        movq      -40(%rsp), %r14                               #
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.13:             #
        movq      -48(%rsp), %r15                               #
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.14:             #
        movq      -56(%rsp), %rbp                               #
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.15:             #
                                # LOE rbx rbp r12 r13 r14 r15
..B2.13:                        # Preds ..B2.12 ..B2.1
        ret                                                     #198.1
        .align    16,0x90
..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.16:             #
                                # LOE
# mark_end;
	.type	_Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj,@function
	.size	_Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj,.-_Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj
	.data
# -- End  _Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj
	.text
# -- Begin  _Z6SSEsymPKdPKhPKjjS0_Pdjjs
# mark_begin;
       .align    16,0x90
	.globl _Z6SSEsymPKdPKhPKjjS0_Pdjjs
_Z6SSEsymPKdPKhPKjjS0_Pdjjs:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 192 + %rsp
# parameter 8: 200 + %rsp
# parameter 9: 208 + %rsp
..B3.1:                         # Preds ..B3.0
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.17:                  #209.1
        subq      $184, %rsp                                    #209.1
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.18:                  #
        movq      %rdx, %r10                                    #209.1
        movswl    208(%rsp), %edx                               #209.1
        imull     %edx, %ecx                                    #214.26
        addq      $-8, %rdi                                     #210.36
        xorl      %r11d, %r11d                                  #214.17
        testl     %ecx, %ecx                                    #214.26
        jbe       ..B3.7        # Prob 10%                      #214.26
                                # LOE rbx rbp rsi rdi r8 r9 r10 r12 r13 r14 r15 ecx r11d
..B3.2:                         # Preds ..B3.1
        movl      192(%rsp), %eax                               #218.48
        movl      %ecx, 160(%rsp)                               #241.105
        movl      200(%rsp), %ecx                               #241.105
        movq      %rbx, 40(%rsp)                                #241.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.19:                  #
        movq      %r12, 32(%rsp)                                #241.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.20:                  #
        movq      %r13, 24(%rsp)                                #241.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.21:                  #
        movq      %r14, 16(%rsp)                                #241.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.22:                  #
        movswl    208(%rsp), %r14d                              #241.105
        movq      %r15, 8(%rsp)                                 #241.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.23:                  #
        pxor      %xmm0, %xmm0                                  #241.105
        movq      %rbp, (%rsp)                                  #241.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.24:                  #
        movl      %eax, %edx                                    #218.48
        movq      %rdx, 168(%rsp)                               #241.105
        movq      %rdi, %r13                                    #241.105
                                # LOE rsi r8 r9 r10 r13 ecx r11d r14d xmm0
..B3.3:                         # Preds ..B3.5 ..B3.2
        movl      192(%rsp), %ebx                               #217.36
        movl      %r11d, %eax                                   #216.24
        xorl      %edx, %edx                                    #216.24
        divl      %r14d                                         #216.24
        incl      %r11d                                         #250.11
        movl      %eax, %r15d                                   #216.24
        movl      (%r10,%r15,4), %ebp                           #217.25
        movzbl    (%r15,%rsi), %edi                             #220.37
        andl      %ebp, %ebx                                    #217.36
        shrl      %cl, %ebp                                     #218.38
        movl      %edi, %edx                                    #220.37
        notq      %rdx                                          #221.28
        movq      %rdx, %r12                                    #221.36
        shlq      $60, %r12                                     #221.36
        andq      168(%rsp), %rbp                               #218.48
        movsd     (%r9,%rbp,8), %xmm1                           #231.35
        movhpd    8(%r9,%rbp,8), %xmm1                          #231.35
        movsd     (%r8,%rbp,8), %xmm3                           #236.35
        movhpd    8(%r8,%rbp,8), %xmm3                          #236.35
        shlq      $61, %rdx                                     #222.32
        movq      %r12, 144(%rsp)                               #221.23
        movq      %rdx, 152(%rsp)                               #222.24
        movq      144(%rsp), %xmm6                              #225.41
        movhpd    152(%rsp), %xmm6                              #225.21
        movaps    %xmm6, %xmm11                                 #229.21
        movl      %ebx, %r12d                                   #234.37
        movddup   (%r8,%r12,8), %xmm2                           #234.21
        psllq     $2, %xmm11                                    #229.21
        incl      %ebx                                          #235.41
        movddup   (%r8,%rbx,8), %xmm4                           #235.21
        movaps    %xmm0, 64(%rsp)                               #241.21
        movdqa    64(%rsp), %xmm8                               #241.21
        movaps    %xmm6, 80(%rsp)                               #241.21
        pshufd    $245, 80(%rsp), %xmm7                         #241.21
        movaps    %xmm0, 112(%rsp)                              #242.21
        movdqa    112(%rsp), %xmm13                             #242.21
        movaps    %xmm11, 128(%rsp)                             #242.21
        pshufd    $245, 128(%rsp), %xmm12                       #242.21
        movl      %edi, %edx                                    #241.96
        andl      $8, %edx                                      #241.96
        popcntl   %edx, %edx                                    #241.70
        movl      %edx, %edx                                    #241.67
        movups    (%r13,%rdx,8), %xmm5                          #241.67
        psrad     $31, %xmm7                                    #241.21
        movaps    %xmm5, 48(%rsp)                               #241.21
        movdqa    %xmm7, %xmm9                                  #241.21
        pandn     48(%rsp), %xmm9                               #241.21
        pand      %xmm7, %xmm8                                  #241.21
        por       %xmm8, %xmm9                                  #241.21
        movdqa    %xmm9, 48(%rsp)                               #241.21
        movaps    48(%rsp), %xmm5                               #241.21
        mulpd     %xmm5, %xmm2                                  #244.24
        mulpd     %xmm3, %xmm5                                  #246.21
        movl      %edi, %edx                                    #242.96
        andl      $14, %edx                                     #242.96
        popcntl   %edx, %edx                                    #242.70
        movl      %edx, %edx                                    #242.67
        movups    (%r13,%rdx,8), %xmm10                         #242.67
        psrad     $31, %xmm12                                   #242.21
        movaps    %xmm10, 96(%rsp)                              #242.21
        movdqa    %xmm12, %xmm14                                #242.21
        pandn     96(%rsp), %xmm14                              #242.21
        pand      %xmm12, %xmm13                                #242.21
        por       %xmm13, %xmm14                                #242.21
        movdqa    %xmm14, 96(%rsp)                              #242.21
        movaps    96(%rsp), %xmm15                              #242.21
        mulpd     %xmm15, %xmm4                                 #245.24
        mulpd     %xmm15, %xmm3                                 #247.21
        addpd     %xmm1, %xmm2                                  #244.13
        addpd     %xmm2, %xmm4                                  #245.13
        movl      %r11d, %eax                                   #250.14
        xorl      %edx, %edx                                    #250.14
        divl      %r14d                                         #250.14
        cmpl      %eax, %r15d                                   #250.19
        je        ..B3.5        # Prob 50%                      #250.19
                                # LOE rbx rbp rsi r8 r9 r10 r12 r13 ecx edi r11d r14d xmm0 xmm3 xmm4 xmm5
..B3.4:                         # Preds ..B3.3
        andl      $15, %edi                                     #250.58
        popcntl   %edi, %edx                                    #250.30
        movl      %edx, %edx                                    #250.30
        lea       (%r13,%rdx,8), %r13                           #250.24
                                # LOE rbx rbp rsi r8 r9 r10 r12 r13 ecx r11d r14d xmm0 xmm3 xmm4 xmm5
..B3.5:                         # Preds ..B3.4 ..B3.3
        cmpl      160(%rsp), %r11d                              #214.26
        movaps    %xmm5, %xmm1                                  #254.5
        unpckhpd  %xmm5, %xmm5                                  #257.5
        movaps    %xmm3, %xmm2                                  #255.5
        unpckhpd  %xmm3, %xmm3                                  #258.5
        addsd     %xmm5, %xmm1                                  #257.5
        addsd     (%r9,%r12,8), %xmm1                           #257.29
        movsd     %xmm1, (%r9,%r12,8)                           #257.5
        addsd     %xmm3, %xmm2                                  #258.5
        addsd     (%r9,%rbx,8), %xmm2                           #258.29
        movsd     %xmm2, (%r9,%rbx,8)                           #258.5
        movaps    %xmm4, (%r9,%rbp,8)                           #259.19
        jb        ..B3.3        # Prob 82%                      #214.26
                                # LOE rsi r8 r9 r10 r13 ecx r11d r14d xmm0
..B3.6:                         # Preds ..B3.5
        movq      40(%rsp), %rbx                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.25:                  #
        movq      32(%rsp), %r12                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.26:                  #
        movq      24(%rsp), %r13                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.27:                  #
        movq      16(%rsp), %r14                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.28:                  #
        movq      8(%rsp), %r15                                 #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.29:                  #
        movq      (%rsp), %rbp                                  #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.30:                  #
                                # LOE rbx rbp r12 r13 r14 r15
..B3.7:                         # Preds ..B3.6 ..B3.1
        addq      $184, %rsp                                    #261.1
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.31:                  #
        ret                                                     #261.1
        .align    16,0x90
..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.32:                  #
                                # LOE
# mark_end;
	.type	_Z6SSEsymPKdPKhPKjjS0_Pdjjs,@function
	.size	_Z6SSEsymPKdPKhPKjjS0_Pdjjs,.-_Z6SSEsymPKdPKhPKjjS0_Pdjjs
	.data
# -- End  _Z6SSEsymPKdPKhPKjjS0_Pdjjs
	.text
# -- Begin  _Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs
# mark_begin;
       .align    16,0x90
	.globl _Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs
_Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 192 + %rsp
# parameter 8: 200 + %rsp
# parameter 9: 208 + %rsp
# parameter 10: 216 + %rsp
# parameter 11: 224 + %rsp
..B4.1:                         # Preds ..B4.0
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.33:            #271.1
        subq      $184, %rsp                                    #271.1
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.34:            #
        movswl    224(%rsp), %r11d                              #271.1
        imull     %r11d, %ecx                                   #276.26
        addq      $-8, %rdi                                     #272.36
        xorl      %r10d, %r10d                                  #276.17
        testl     %ecx, %ecx                                    #276.26
        jbe       ..B4.7        # Prob 10%                      #276.26
                                # LOE rdx rbx rbp rsi rdi r8 r9 r12 r13 r14 r15 ecx r10d
..B4.2:                         # Preds ..B4.1
        movl      208(%rsp), %eax                               #280.48
        movl      %ecx, 168(%rsp)                               #303.105
        movq      %rdx, 176(%rsp)                               #303.105
        movq      %rbx, 40(%rsp)                                #303.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.35:            #
        movq      %r12, 32(%rsp)                                #303.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.36:            #
        movswl    224(%rsp), %r12d                              #303.105
        movq      %r13, 24(%rsp)                                #303.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.37:            #
        movq      200(%rsp), %r13                               #303.105
        movq      %r14, 16(%rsp)                                #303.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.38:            #
        movq      192(%rsp), %r14                               #303.105
        movq      %r15, 8(%rsp)                                 #303.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.39:            #
        pxor      %xmm0, %xmm0                                  #303.105
        movl      %eax, %r11d                                   #280.48
        movq      %r11, 160(%rsp)                               #303.105
        movq      %rbp, (%rsp)                                  #303.105
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.40:            #
                                # LOE rsi rdi r8 r9 r13 r14 r10d r12d xmm0
..B4.3:                         # Preds ..B4.5 ..B4.2
        movq      176(%rsp), %rbp                               #279.25
        movl      208(%rsp), %ebx                               #279.36
        movl      216(%rsp), %ecx                               #280.38
        movl      %r10d, %eax                                   #278.24
        xorl      %edx, %edx                                    #278.24
        divl      %r12d                                         #278.24
        incl      %r10d                                         #312.11
        movl      %eax, %r15d                                   #278.24
        movl      (%rbp,%r15,4), %ebp                           #279.25
        andl      %ebp, %ebx                                    #279.36
        shrl      %cl, %ebp                                     #280.38
        movzbl    (%r15,%rsi), %ecx                             #282.37
        movl      %ecx, %edx                                    #282.37
        andq      160(%rsp), %rbp                               #280.48
        movsd     (%r14,%rbp,8), %xmm1                          #293.35
        movhpd    8(%r14,%rbp,8), %xmm1                         #293.35
        movsd     (%r9,%rbp,8), %xmm3                           #298.35
        movhpd    8(%r9,%rbp,8), %xmm3                          #298.35
        notq      %rdx                                          #283.28
        movq      %rdx, %r11                                    #283.36
        shlq      $60, %r11                                     #283.36
        shlq      $61, %rdx                                     #284.32
        movq      %r11, 144(%rsp)                               #283.23
        movq      %rdx, 152(%rsp)                               #284.24
        movq      144(%rsp), %xmm6                              #287.41
        movhpd    152(%rsp), %xmm6                              #287.21
        movaps    %xmm6, %xmm11                                 #291.21
        movl      %ebx, %r11d                                   #296.37
        movddup   (%r8,%r11,8), %xmm2                           #296.21
        psllq     $2, %xmm11                                    #291.21
        incl      %ebx                                          #297.41
        movddup   (%r8,%rbx,8), %xmm4                           #297.21
        movaps    %xmm0, 64(%rsp)                               #303.21
        movdqa    64(%rsp), %xmm8                               #303.21
        movaps    %xmm6, 80(%rsp)                               #303.21
        pshufd    $245, 80(%rsp), %xmm7                         #303.21
        movaps    %xmm0, 112(%rsp)                              #304.21
        movdqa    112(%rsp), %xmm13                             #304.21
        movaps    %xmm11, 128(%rsp)                             #304.21
        pshufd    $245, 128(%rsp), %xmm12                       #304.21
        movl      %ecx, %edx                                    #303.96
        andl      $8, %edx                                      #303.96
        popcntl   %edx, %edx                                    #303.70
        movl      %edx, %edx                                    #303.67
        movups    (%rdi,%rdx,8), %xmm5                          #303.67
        psrad     $31, %xmm7                                    #303.21
        movaps    %xmm5, 48(%rsp)                               #303.21
        movdqa    %xmm7, %xmm9                                  #303.21
        pandn     48(%rsp), %xmm9                               #303.21
        pand      %xmm7, %xmm8                                  #303.21
        por       %xmm8, %xmm9                                  #303.21
        movdqa    %xmm9, 48(%rsp)                               #303.21
        movaps    48(%rsp), %xmm5                               #303.21
        mulpd     %xmm5, %xmm2                                  #306.24
        mulpd     %xmm3, %xmm5                                  #308.21
        movl      %ecx, %edx                                    #304.96
        andl      $14, %edx                                     #304.96
        popcntl   %edx, %edx                                    #304.70
        movl      %edx, %edx                                    #304.67
        movups    (%rdi,%rdx,8), %xmm10                         #304.67
        psrad     $31, %xmm12                                   #304.21
        movaps    %xmm10, 96(%rsp)                              #304.21
        movdqa    %xmm12, %xmm14                                #304.21
        pandn     96(%rsp), %xmm14                              #304.21
        pand      %xmm12, %xmm13                                #304.21
        por       %xmm13, %xmm14                                #304.21
        movdqa    %xmm14, 96(%rsp)                              #304.21
        movaps    96(%rsp), %xmm15                              #304.21
        mulpd     %xmm15, %xmm4                                 #307.24
        mulpd     %xmm15, %xmm3                                 #309.21
        addpd     %xmm1, %xmm2                                  #306.13
        addpd     %xmm2, %xmm4                                  #307.13
        movl      %r10d, %eax                                   #312.14
        xorl      %edx, %edx                                    #312.14
        divl      %r12d                                         #312.14
        cmpl      %eax, %r15d                                   #312.19
        je        ..B4.5        # Prob 50%                      #312.19
                                # LOE rbx rbp rsi rdi r8 r9 r11 r13 r14 ecx r10d r12d xmm0 xmm3 xmm4 xmm5
..B4.4:                         # Preds ..B4.3
        andl      $15, %ecx                                     #312.58
        popcntl   %ecx, %edx                                    #312.30
        movl      %edx, %edx                                    #312.30
        lea       (%rdi,%rdx,8), %rdi                           #312.24
                                # LOE rbx rbp rsi rdi r8 r9 r11 r13 r14 r10d r12d xmm0 xmm3 xmm4 xmm5
..B4.5:                         # Preds ..B4.4 ..B4.3
        cmpl      168(%rsp), %r10d                              #276.26
        movaps    %xmm5, %xmm1                                  #316.5
        unpckhpd  %xmm5, %xmm5                                  #319.5
        movaps    %xmm3, %xmm2                                  #317.5
        unpckhpd  %xmm3, %xmm3                                  #320.5
        addsd     %xmm5, %xmm1                                  #319.5
        addsd     (%r13,%r11,8), %xmm1                          #319.30
        movsd     %xmm1, (%r13,%r11,8)                          #319.5
        addsd     %xmm3, %xmm2                                  #320.5
        addsd     (%r13,%rbx,8), %xmm2                          #320.30
        movsd     %xmm2, (%r13,%rbx,8)                          #320.5
        movaps    %xmm4, (%r14,%rbp,8)                          #321.19
        jb        ..B4.3        # Prob 82%                      #276.26
                                # LOE rsi rdi r8 r9 r13 r14 r10d r12d xmm0
..B4.6:                         # Preds ..B4.5
        movq      40(%rsp), %rbx                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.41:            #
        movq      32(%rsp), %r12                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.42:            #
        movq      24(%rsp), %r13                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.43:            #
        movq      16(%rsp), %r14                                #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.44:            #
        movq      8(%rsp), %r15                                 #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.45:            #
        movq      (%rsp), %rbp                                  #
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.46:            #
                                # LOE rbx rbp r12 r13 r14 r15
..B4.7:                         # Preds ..B4.6 ..B4.1
        addq      $184, %rsp                                    #323.1
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.47:            #
        ret                                                     #323.1
        .align    16,0x90
..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.48:            #
                                # LOE
# mark_end;
	.type	_Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs,@function
	.size	_Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs,.-_Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs
	.data
# -- End  _Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs
	.text
# -- Begin  _Z7SSEspmvPKdPKhPKjjS0_Pdjjj
# mark_begin;
       .align    16,0x90
	.globl _Z7SSEspmvPKdPKhPKjjS0_Pdjjj
_Z7SSEspmvPKdPKhPKjjS0_Pdjjj:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 176 + %rsp
# parameter 8: 184 + %rsp
# parameter 9: 192 + %rsp
..B5.1:                         # Preds ..B5.0
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.49:                 #337.1
        subq      $168, %rsp                                    #337.1
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.50:                 #
        movl      %ecx, %eax                                    #337.1
        addq      $-8, %rdi                                     #338.36
        xorl      %r11d, %r11d                                  #342.19
        testl     %eax, %eax                                    #342.26
        jbe       ..B5.5        # Prob 10%                      #342.26
                                # LOE rdx rbx rbp rsi rdi r8 r9 r11 r12 r13 r14 r15 eax
..B5.2:                         # Preds ..B5.1
        movl      192(%rsp), %ecx                               #367.124
        movq      %rbx, 40(%rsp)                                #342.32
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.51:                 #
        movq      %r12, 32(%rsp)                                #342.32
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.52:                 #
        movq      %r13, 24(%rsp)                                #342.32
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.53:                 #
        movl      184(%rsp), %r13d                              #342.32
        movq      %r14, 16(%rsp)                                #342.32
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.54:                 #
        pxor      %xmm0, %xmm0                                  #367.124
        movq      %r15, 8(%rsp)                                 #342.32
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.55:                 #
        movq      %rbp, (%rsp)                                  #342.32
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.56:                 #
        xorl      %r10d, %r10d                                  #342.32
        movl      176(%rsp), %ebp                               #342.32
                                # LOE rdx rsi rdi r8 r9 r11 r13 eax ecx ebp r10d xmm0
..B5.3:                         # Preds ..B5.3 ..B5.2
        movl      (%rdx,%r11,4), %ebx                           #344.25
        movzbl    (%rsi,%r11), %r11d                            #347.37
        movl      %ebp, %r12d                                   #344.36
        andl      %ebx, %r12d                                   #344.36
        shrl      %cl, %ebx                                     #345.38
        movl      %r11d, %r15d                                  #347.37
        notq      %r15                                          #348.28
        andq      %r13, %rbx                                    #345.48
        movsd     (%r9,%rbx,8), %xmm6                           #358.35
        movhpd    8(%r9,%rbx,8), %xmm6                          #358.35
        movq      %r15, %r14                                    #348.36
        shlq      $60, %r14                                     #348.36
        shlq      $61, %r15                                     #349.32
        incl      %r10d                                         #342.32
        movq      %r14, 144(%rsp)                               #348.23
        movl      %r11d, %r14d                                  #367.116
        movq      %r15, 152(%rsp)                               #349.24
        movddup   (%r8,%r12,8), %xmm12                          #361.21
        incl      %r12d                                         #362.41
        movddup   (%r8,%r12,8), %xmm13                          #362.21
        movq      144(%rsp), %xmm2                              #352.41
        movhpd    152(%rsp), %xmm2                              #352.21
        movaps    %xmm0, 64(%rsp)                               #367.41
        movdqa    64(%rsp), %xmm4                               #367.41
        movaps    %xmm2, 80(%rsp)                               #367.41
        pshufd    $245, 80(%rsp), %xmm3                         #367.41
        movaps    %xmm0, 112(%rsp)                              #368.41
        movaps    %xmm2, %xmm8                                  #356.21
        movdqa    112(%rsp), %xmm10                             #368.41
        andl      $8, %r14d                                     #367.116
        psllq     $2, %xmm8                                     #356.21
        movaps    %xmm8, 128(%rsp)                              #368.41
        pshufd    $245, 128(%rsp), %xmm9                        #368.41
        popcntl   %r14d, %r15d                                  #367.90
        movl      %r15d, %r15d                                  #367.87
        movups    (%rdi,%r15,8), %xmm1                          #367.87
        movaps    %xmm1, 48(%rsp)                               #367.41
        psrad     $31, %xmm3                                    #367.41
        movdqa    %xmm3, %xmm5                                  #367.41
        pand      %xmm3, %xmm4                                  #367.41
        pandn     48(%rsp), %xmm5                               #367.41
        por       %xmm4, %xmm5                                  #367.41
        movdqa    %xmm5, 48(%rsp)                               #367.41
        mulpd     48(%rsp), %xmm12                              #367.24
        movl      %r11d, %r12d                                  #368.116
        andl      $14, %r12d                                    #368.116
        addpd     %xmm6, %xmm12                                 #367.13
        popcntl   %r12d, %r14d                                  #368.90
        movl      %r14d, %r14d                                  #368.87
        movups    (%rdi,%r14,8), %xmm7                          #368.87
        movaps    %xmm7, 96(%rsp)                               #368.41
        psrad     $31, %xmm9                                    #368.41
        movdqa    %xmm9, %xmm11                                 #368.41
        pand      %xmm9, %xmm10                                 #368.41
        pandn     96(%rsp), %xmm11                              #368.41
        por       %xmm10, %xmm11                                #368.41
        movdqa    %xmm11, 96(%rsp)                              #368.41
        mulpd     96(%rsp), %xmm13                              #368.24
        andl      $15, %r11d                                    #371.39
        popcntl   %r11d, %r11d                                  #371.11
        addpd     %xmm12, %xmm13                                #368.13
        movaps    %xmm13, (%r9,%rbx,8)                          #374.19
        movl      %r11d, %r11d                                  #371.11
        lea       (%rdi,%r11,8), %rdi                           #371.5
        movl      %r10d, %r11d                                  #342.32
        movl      %r11d, %r10d                                  #342.3
        cmpl      %eax, %r10d                                   #342.26
        jb        ..B5.3        # Prob 82%                      #342.26
                                # LOE rdx rsi rdi r8 r9 r11 r13 eax ecx ebp r10d xmm0
..B5.4:                         # Preds ..B5.3
        movq      40(%rsp), %rbx                                #
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.57:                 #
        movq      32(%rsp), %r12                                #
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.58:                 #
        movq      24(%rsp), %r13                                #
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.59:                 #
        movq      16(%rsp), %r14                                #
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.60:                 #
        movq      8(%rsp), %r15                                 #
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.61:                 #
        movq      (%rsp), %rbp                                  #
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.62:                 #
                                # LOE rbx rbp r12 r13 r14 r15
..B5.5:                         # Preds ..B5.4 ..B5.1
        addq      $168, %rsp                                    #376.1
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.63:                 #
        ret                                                     #376.1
        .align    16,0x90
..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.64:                 #
                                # LOE
# mark_end;
	.type	_Z7SSEspmvPKdPKhPKjjS0_Pdjjj,@function
	.size	_Z7SSEspmvPKdPKhPKjjS0_Pdjjj,.-_Z7SSEspmvPKdPKhPKjjS0_Pdjjj
	.data
# -- End  _Z7SSEspmvPKdPKhPKjjS0_Pdjjj
	.text
# -- Begin  _Z7SSEspmvPKdPKtPKjjS0_Pdjjj
# mark_begin;
       .align    16,0x90
	.globl _Z7SSEspmvPKdPKtPKjjS0_Pdjjj
_Z7SSEspmvPKdPKtPKjjS0_Pdjjj:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 448 + %rsp
# parameter 8: 456 + %rsp
# parameter 9: 464 + %rsp
..B6.1:                         # Preds ..B6.0
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.65:                 #391.1
        subq      $440, %rsp                                    #391.1
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.66:                 #
        movl      %ecx, %eax                                    #391.1
        addq      $-8, %rdi                                     #392.36
        xorl      %r11d, %r11d                                  #396.19
        testl     %eax, %eax                                    #396.26
        jbe       ..B6.5        # Prob 10%                      #396.26
                                # LOE rdx rbx rbp rsi rdi r8 r9 r11 r12 r13 r14 r15 eax
..B6.2:                         # Preds ..B6.1
        movl      464(%rsp), %ecx                               #433.127
        movq      %rbx, 32(%rsp)                                #396.32
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.67:                 #
        movq      %r12, 24(%rsp)                                #396.32
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.68:                 #
        movl      448(%rsp), %r12d                              #396.32
        movq      %r13, 16(%rsp)                                #396.32
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.69:                 #
        pxor      %xmm6, %xmm6                                  #433.127
        movq      %r14, 8(%rsp)                                 #396.32
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.70:                 #
        movq      %r15, (%rsp)                                  #396.32
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.71:                 #
        xorl      %r10d, %r10d                                  #396.32
                                # LOE rdx rbp rsi rdi r8 r9 r11 eax ecx r10d r12d xmm6
..B6.3:                         # Preds ..B6.3 ..B6.2
        movl      (%rdx,%r11,4), %ebx                           #398.25
        movzwl    (%rsi,%r11,2), %r14d                          #401.37
        movl      %r12d, %r13d                                  #398.36
        andl      %ebx, %r13d                                   #398.36
        shrl      %cl, %ebx                                     #399.38
        andl      456(%rsp), %ebx                               #399.48
        movl      %r14d, %r11d                                  #401.37
        notq      %r11                                          #402.28
        movq      %r11, %r15                                    #402.36
        shlq      $48, %r15                                     #402.36
        shlq      $49, %r11                                     #403.32
        movq      %r15, 40(%rsp)                                #402.23
        movq      %r11, 432(%rsp)                               #403.24
        movq      40(%rsp), %xmm4                               #406.41
        movhpd    432(%rsp), %xmm4                              #406.21
        movaps    %xmm4, %xmm8                                  #410.21
        movl      %ebx, %r11d                                   #414.35
        movaps    %xmm4, %xmm7                                  #411.21
        movaps    %xmm4, %xmm10                                 #412.21
        movsd     (%r9,%r11,8), %xmm1                           #414.35
        movhpd    8(%r9,%r11,8), %xmm1                          #414.35
        psllq     $2, %xmm8                                     #410.21
        psllq     $4, %xmm7                                     #411.21
        psllq     $6, %xmm10                                    #412.21
        addl      $2, %ebx                                      #415.40
        movsd     (%r9,%rbx,8), %xmm3                           #415.35
        movhpd    8(%r9,%rbx,8), %xmm3                          #415.35
        movddup   (%r8,%r13,8), %xmm5                           #418.21
        lea       1(%r13), %r15d                                #419.41
        movddup   (%r8,%r15,8), %xmm12                          #419.21
        lea       2(%r13), %r15d                                #420.41
        movddup   (%r8,%r15,8), %xmm9                           #420.21
        addl      $3, %r13d                                     #421.41
        movddup   (%r8,%r13,8), %xmm11                          #421.21
        movaps    %xmm6, 64(%rsp)                               #433.41
        movdqa    64(%rsp), %xmm15                              #433.41
        movaps    %xmm4, 80(%rsp)                               #433.41
        pshufd    $245, 80(%rsp), %xmm14                        #433.41
        movaps    %xmm6, 112(%rsp)                              #434.41
        movaps    %xmm7, 128(%rsp)                              #434.41
        movaps    %xmm6, 160(%rsp)                              #435.41
        movaps    %xmm8, 176(%rsp)                              #435.41
        movaps    %xmm6, 208(%rsp)                              #436.41
        movaps    %xmm10, 224(%rsp)                             #436.41
        movaps    %xmm6, 256(%rsp)                              #447.41
        movaps    %xmm6, 304(%rsp)                              #448.41
        movaps    %xmm6, 352(%rsp)                              #449.41
        movaps    %xmm6, 400(%rsp)                              #450.41
        movl      %r14d, %r15d                                  #433.116
        andl      $32768, %r15d                                 #433.116
        popcntl   %r15d, %r13d                                  #433.90
        movl      %r13d, %r13d                                  #433.87
        movups    (%rdi,%r13,8), %xmm13                         #433.87
        psrad     $31, %xmm14                                   #433.41
        movaps    %xmm13, 48(%rsp)                              #433.41
        movdqa    %xmm14, %xmm0                                 #433.41
        pandn     48(%rsp), %xmm0                               #433.41
        pand      %xmm14, %xmm15                                #433.41
        pshufd    $245, 128(%rsp), %xmm14                       #434.41
        por       %xmm15, %xmm0                                 #433.41
        movdqa    %xmm0, 48(%rsp)                               #433.41
        movdqa    112(%rsp), %xmm0                              #434.41
        movaps    48(%rsp), %xmm2                               #433.41
        mulpd     %xmm5, %xmm2                                  #433.24
        psllq     $8, %xmm4                                     #433.174
        movaps    %xmm4, 272(%rsp)                              #447.41
        pshufd    $245, 272(%rsp), %xmm4                        #447.41
        addpd     %xmm1, %xmm2                                  #433.13
        movl      %r14d, %r15d                                  #434.116
        andl      $63488, %r15d                                 #434.116
        popcntl   %r15d, %r13d                                  #434.90
        movl      %r13d, %r13d                                  #434.87
        movups    (%rdi,%r13,8), %xmm13                         #434.87
        psrad     $31, %xmm14                                   #434.41
        movaps    %xmm13, 96(%rsp)                              #434.41
        movdqa    %xmm14, %xmm1                                 #434.41
        pandn     96(%rsp), %xmm1                               #434.41
        pand      %xmm14, %xmm0                                 #434.41
        por       %xmm0, %xmm1                                  #434.41
        pshufd    $245, 176(%rsp), %xmm0                        #435.41
        movdqa    %xmm1, 96(%rsp)                               #434.41
        movdqa    160(%rsp), %xmm1                              #435.41
        movaps    96(%rsp), %xmm15                              #434.41
        mulpd     %xmm15, %xmm5                                 #434.24
        psllq     $8, %xmm7                                     #434.174
        movaps    %xmm7, 320(%rsp)                              #448.41
        pshufd    $245, 320(%rsp), %xmm7                        #448.41
        addpd     %xmm3, %xmm5                                  #434.13
        movl      %r14d, %r15d                                  #435.116
        andl      $57344, %r15d                                 #435.116
        popcntl   %r15d, %r13d                                  #435.90
        movl      %r13d, %r13d                                  #435.87
        movups    (%rdi,%r13,8), %xmm3                          #435.87
        psrad     $31, %xmm0                                    #435.41
        movaps    %xmm3, 144(%rsp)                              #435.41
        movdqa    %xmm0, %xmm13                                 #435.41
        pandn     144(%rsp), %xmm13                             #435.41
        pand      %xmm0, %xmm1                                  #435.41
        pshufd    $245, 224(%rsp), %xmm0                        #436.41
        movdqa    208(%rsp), %xmm3                              #436.41
        por       %xmm1, %xmm13                                 #435.41
        movdqa    %xmm13, 144(%rsp)                             #435.41
        movaps    144(%rsp), %xmm1                              #435.41
        mulpd     %xmm12, %xmm1                                 #435.24
        psllq     $8, %xmm8                                     #435.174
        movaps    %xmm8, 368(%rsp)                              #449.41
        pshufd    $245, 368(%rsp), %xmm8                        #449.41
        addpd     %xmm2, %xmm1                                  #435.13
        movl      %r14d, %r15d                                  #436.116
        andl      $65024, %r15d                                 #436.116
        popcntl   %r15d, %r13d                                  #436.90
        movl      %r13d, %r13d                                  #436.87
        movups    (%rdi,%r13,8), %xmm2                          #436.87
        psrad     $31, %xmm0                                    #436.41
        movaps    %xmm2, 192(%rsp)                              #436.41
        movdqa    %xmm0, %xmm13                                 #436.41
        pandn     192(%rsp), %xmm13                             #436.41
        pand      %xmm0, %xmm3                                  #436.41
        por       %xmm3, %xmm13                                 #436.41
        movdqa    256(%rsp), %xmm0                              #447.41
        movdqa    %xmm13, 192(%rsp)                             #436.41
        movaps    192(%rsp), %xmm2                              #436.41
        mulpd     %xmm2, %xmm12                                 #436.24
        psllq     $8, %xmm10                                    #436.174
        movaps    %xmm10, 416(%rsp)                             #450.41
        pshufd    $245, 416(%rsp), %xmm10                       #450.41
        addpd     %xmm5, %xmm12                                 #436.13
        movl      %r14d, %r15d                                  #447.116
        andl      $65408, %r15d                                 #447.116
        popcntl   %r15d, %r13d                                  #447.90
        movl      %r13d, %r13d                                  #447.87
        movups    (%rdi,%r13,8), %xmm5                          #447.87
        psrad     $31, %xmm4                                    #447.41
        movaps    %xmm5, 240(%rsp)                              #447.41
        movdqa    %xmm4, %xmm2                                  #447.41
        pandn     240(%rsp), %xmm2                              #447.41
        pand      %xmm4, %xmm0                                  #447.41
        por       %xmm0, %xmm2                                  #447.41
        movdqa    %xmm2, 240(%rsp)                              #447.41
        movaps    240(%rsp), %xmm0                              #447.41
        mulpd     %xmm9, %xmm0                                  #447.24
        movl      %r14d, %r15d                                  #448.116
        psrad     $31, %xmm7                                    #448.41
        movdqa    %xmm7, %xmm2                                  #448.41
        andl      $65528, %r15d                                 #448.116
        addpd     %xmm1, %xmm0                                  #447.13
        popcntl   %r15d, %r13d                                  #448.90
        movl      %r13d, %r13d                                  #448.87
        movups    (%rdi,%r13,8), %xmm1                          #448.87
        movaps    %xmm1, 288(%rsp)                              #448.41
        movdqa    304(%rsp), %xmm1                              #448.41
        pandn     288(%rsp), %xmm2                              #448.41
        pand      %xmm7, %xmm1                                  #448.41
        por       %xmm1, %xmm2                                  #448.41
        movdqa    352(%rsp), %xmm1                              #449.41
        movdqa    %xmm2, 288(%rsp)                              #448.41
        movaps    288(%rsp), %xmm3                              #448.41
        mulpd     %xmm3, %xmm9                                  #448.24
        movl      %r14d, %r15d                                  #449.116
        andl      $65504, %r15d                                 #449.116
        addpd     %xmm12, %xmm9                                 #448.13
        popcntl   %r15d, %r13d                                  #449.90
        movl      %r13d, %r13d                                  #449.87
        movups    (%rdi,%r13,8), %xmm12                         #449.87
        movaps    %xmm12, 336(%rsp)                             #449.41
        psrad     $31, %xmm8                                    #449.41
        movdqa    %xmm8, %xmm2                                  #449.41
        pand      %xmm8, %xmm1                                  #449.41
        pandn     336(%rsp), %xmm2                              #449.41
        por       %xmm1, %xmm2                                  #449.41
        movdqa    %xmm2, 336(%rsp)                              #449.41
        movaps    336(%rsp), %xmm3                              #449.41
        mulpd     %xmm11, %xmm3                                 #449.24
        movl      %r14d, %r15d                                  #450.116
        andl      $65534, %r15d                                 #450.116
        addpd     %xmm0, %xmm3                                  #449.13
        movaps    %xmm3, (%r9,%r11,8)                           #456.19
        popcntl   %r15d, %r13d                                  #450.90
        movl      %r13d, %r13d                                  #450.87
        movups    (%rdi,%r13,8), %xmm0                          #450.87
        movaps    %xmm0, 384(%rsp)                              #450.41
        movdqa    400(%rsp), %xmm0                              #450.41
        psrad     $31, %xmm10                                   #450.41
        movdqa    %xmm10, %xmm1                                 #450.41
        pandn     384(%rsp), %xmm1                              #450.41
        pand      %xmm10, %xmm0                                 #450.41
        por       %xmm0, %xmm1                                  #450.41
        movdqa    %xmm1, 384(%rsp)                              #450.41
        movaps    384(%rsp), %xmm10                             #450.41
        mulpd     %xmm10, %xmm11                                #450.24
        popcntl   %r14d, %r14d                                  #453.11
        movl      %r14d, %r14d                                  #453.11
        addpd     %xmm9, %xmm11                                 #450.13
        movaps    %xmm11, (%r9,%rbx,8)                          #457.19
        lea       (%rdi,%r14,8), %rdi                           #453.5
        incl      %r10d                                         #396.32
        movl      %r10d, %r11d                                  #396.32
        movl      %r11d, %r10d                                  #396.3
        cmpl      %eax, %r10d                                   #396.26
        jb        ..B6.3        # Prob 82%                      #396.26
                                # LOE rdx rbp rsi rdi r8 r9 r11 eax ecx r10d r12d xmm6
..B6.4:                         # Preds ..B6.3
        movq      32(%rsp), %rbx                                #
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.72:                 #
        movq      24(%rsp), %r12                                #
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.73:                 #
        movq      16(%rsp), %r13                                #
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.74:                 #
        movq      8(%rsp), %r14                                 #
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.75:                 #
        movq      (%rsp), %r15                                  #
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.76:                 #
                                # LOE rbx rbp r12 r13 r14 r15
..B6.5:                         # Preds ..B6.4 ..B6.1
        addq      $440, %rsp                                    #459.1
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.77:                 #
        ret                                                     #459.1
        .align    16,0x90
..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.78:                 #
                                # LOE
# mark_end;
	.type	_Z7SSEspmvPKdPKtPKjjS0_Pdjjj,@function
	.size	_Z7SSEspmvPKdPKtPKjjS0_Pdjjj,.-_Z7SSEspmvPKdPKtPKjjS0_Pdjjj
	.data
# -- End  _Z7SSEspmvPKdPKtPKjjS0_Pdjjj
	.text
# -- Begin  _Z7SSEspmvPKdPKmPKjjS0_Pdjjj
# mark_begin;
       .align    16,0x90
	.globl _Z7SSEspmvPKdPKmPKjjS0_Pdjjj
_Z7SSEspmvPKdPKmPKjjS0_Pdjjj:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 16 + %rbp
# parameter 8: 24 + %rbp
# parameter 9: 32 + %rbp
..B7.1:                         # Preds ..B7.0
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.79:                 #464.1
        pushq     %rbp                                          #464.1
        movq      %rsp, %rbp                                    #464.1
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.80:                 #
        subq      $1600, %rsp                                   #464.1
        addq      $-8, %rdi                                     #465.36
        xorl      %r10d, %r10d                                  #469.19
        testl     %ecx, %ecx                                    #469.26
        jbe       ..B7.5        # Prob 10%                      #469.26
                                # LOE rdx rbx rsi rdi r8 r9 r10 r12 r13 r14 r15 ecx
..B7.2:                         # Preds ..B7.1
        movl      32(%rbp), %r11d                               #506.141
        movl      %ecx, -8(%rbp)                                #469.32
        movq      %rbx, -1568(%rbp)                             #469.32
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.82:                 #
        movq      %r12, -1576(%rbp)                             #469.32
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.83:                 #
        movq      %r13, -1584(%rbp)                             #469.32
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.84:                 #
        pxor      %xmm3, %xmm3                                  #506.141
        movq      %r14, -1592(%rbp)                             #469.32
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.85:                 #
        movq      %r15, -1600(%rbp)                             #469.32
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.86:                 #
        xorl      %eax, %eax                                    #469.32
                                # LOE rdx rsi rdi r8 r9 r10 eax r11d xmm3
..B7.3:                         # Preds ..B7.8 ..B7.2
        movl      16(%rbp), %r12d                               #471.36
        movl      (%rdx,%r10,4), %ebx                           #471.25
        movq      (%rsi,%r10,8), %r10                           #473.26
        andl      %ebx, %r12d                                   #471.36
        movl      %r11d, %ecx                                   #472.38
        shrl      %cl, %ebx                                     #472.38
        andl      24(%rbp), %ebx                                #472.48
        movq      %r10, %rcx                                    #473.26
        notq      %rcx                                          #473.26
        movq      %rcx, -1560(%rbp)                             #473.23
        movl      %ebx, %r15d                                   #475.35
        movsd     (%r9,%r15,8), %xmm1                           #475.35
        movhpd    8(%r9,%r15,8), %xmm1                          #475.35
        lea       2(%rbx), %r14d                                #476.40
        movsd     (%r9,%r14,8), %xmm14                          #476.35
        movhpd    8(%r9,%r14,8), %xmm14                         #476.35
        lea       4(%rbx), %r13d                                #477.40
        movsd     (%r9,%r13,8), %xmm9                           #477.35
        movhpd    8(%r9,%r13,8), %xmm9                          #477.35
        addl      $6, %ebx                                      #478.40
        movsd     (%r9,%rbx,8), %xmm0                           #478.35
        movhpd    8(%r9,%rbx,8), %xmm0                          #478.35
        addq      %rcx, %rcx                                    #481.32
        movq      %rcx, -16(%rbp)                               #481.24
        movq      -1560(%rbp), %xmm6                            #482.41
        movhpd    -16(%rbp), %xmm6                              #482.21
        movaps    %xmm6, %xmm8                                  #486.21
        movaps    %xmm6, %xmm7                                  #487.21
        movaps    %xmm6, %xmm5                                  #488.21
        movddup   (%r8,%r12,8), %xmm10                          #491.21
        psllq     $2, %xmm8                                     #486.21
        psllq     $4, %xmm7                                     #487.21
        psllq     $6, %xmm5                                     #488.21
        lea       1(%r12), %ecx                                 #492.41
        movddup   (%r8,%rcx,8), %xmm12                          #492.21
        lea       2(%r12), %ecx                                 #493.41
        movddup   (%r8,%rcx,8), %xmm11                          #493.21
        lea       3(%r12), %ecx                                 #494.41
        movddup   (%r8,%rcx,8), %xmm4                           #494.21
        movaps    %xmm3, -1536(%rbp)                            #506.41
        movaps    %xmm6, -1520(%rbp)                            #506.41
        pshufd    $245, -1520(%rbp), %xmm13                     #506.41
        movaps    %xmm3, -1488(%rbp)                            #507.41
        movaps    %xmm7, -1472(%rbp)                            #507.41
        movaps    %xmm3, -1440(%rbp)                            #508.41
        movaps    %xmm8, -1424(%rbp)                            #508.41
        movaps    %xmm3, -1392(%rbp)                            #509.41
        movaps    %xmm5, -1376(%rbp)                            #509.41
        movaps    %xmm3, -1344(%rbp)                            #520.41
        movaps    %xmm3, -1296(%rbp)                            #521.41
        movaps    %xmm3, -1248(%rbp)                            #522.41
        movaps    %xmm3, -1200(%rbp)                            #523.41
        movaps    %xmm3, -1152(%rbp)                            #526.41
        movaps    %xmm3, -1104(%rbp)                            #527.41
        movaps    %xmm3, -1056(%rbp)                            #528.41
        movaps    %xmm3, -1008(%rbp)                            #529.41
        movaps    %xmm3, -960(%rbp)                             #531.41
        movaps    %xmm3, -912(%rbp)                             #532.41
        movaps    %xmm3, -864(%rbp)                             #533.41
        movaps    %xmm3, -816(%rbp)                             #534.41
        movq      $0x8000000000000000, %rcx                     #506.118
        andq      %r10, %rcx                                    #506.118
        popcntq   %rcx, %rcx                                    #506.90
        movups    (%rdi,%rcx,8), %xmm2                          #506.87
        movq      $0xf800000000000000, %rcx                     #507.118
        movaps    %xmm2, -1552(%rbp)                            #506.41
        psrad     $31, %xmm13                                   #506.41
        movdqa    %xmm13, %xmm15                                #506.41
        pandn     -1552(%rbp), %xmm15                           #506.41
        movdqa    -1536(%rbp), %xmm2                            #506.41
        pand      %xmm13, %xmm2                                 #506.41
        por       %xmm2, %xmm15                                 #506.41
        pshufd    $245, -1472(%rbp), %xmm2                      #507.41
        movdqa    -1488(%rbp), %xmm13                           #507.41
        movdqa    %xmm15, -1552(%rbp)                           #506.41
        movaps    -1552(%rbp), %xmm15                           #506.41
        mulpd     %xmm10, %xmm15                                #506.24
        psllq     $8, %xmm6                                     #506.188
        movaps    %xmm6, -1328(%rbp)                            #520.41
        addpd     %xmm1, %xmm15                                 #506.13
        andq      %r10, %rcx                                    #507.118
        popcntq   %rcx, %rcx                                    #507.90
        movups    (%rdi,%rcx,8), %xmm1                          #507.87
        movaps    %xmm1, -1504(%rbp)                            #507.41
        movq      $0xe000000000000000, %rcx                     #508.118
        psrad     $31, %xmm2                                    #507.41
        movdqa    %xmm2, %xmm1                                  #507.41
        pandn     -1504(%rbp), %xmm1                            #507.41
        pand      %xmm2, %xmm13                                 #507.41
        movdqa    -1440(%rbp), %xmm2                            #508.41
        por       %xmm13, %xmm1                                 #507.41
        movdqa    %xmm1, -1504(%rbp)                            #507.41
        pshufd    $245, -1424(%rbp), %xmm1                      #508.41
        movaps    -1504(%rbp), %xmm13                           #507.41
        mulpd     %xmm10, %xmm13                                #507.24
        psllq     $8, %xmm7                                     #507.188
        movaps    %xmm7, -1280(%rbp)                            #521.41
        addpd     %xmm14, %xmm13                                #507.13
        andq      %r10, %rcx                                    #508.118
        popcntq   %rcx, %rcx                                    #508.90
        movups    (%rdi,%rcx,8), %xmm14                         #508.87
        movaps    %xmm14, -1456(%rbp)                           #508.41
        movq      $0xfe00000000000000, %rcx                     #509.118
        psrad     $31, %xmm1                                    #508.41
        movdqa    %xmm1, %xmm14                                 #508.41
        pandn     -1456(%rbp), %xmm14                           #508.41
        pand      %xmm1, %xmm2                                  #508.41
        pshufd    $245, -1376(%rbp), %xmm1                      #509.41
        por       %xmm2, %xmm14                                 #508.41
        movdqa    -1392(%rbp), %xmm2                            #509.41
        movdqa    %xmm14, -1456(%rbp)                           #508.41
        movaps    -1456(%rbp), %xmm14                           #508.41
        mulpd     %xmm12, %xmm14                                #508.24
        psllq     $8, %xmm8                                     #508.188
        movaps    %xmm8, -1232(%rbp)                            #522.41
        addpd     %xmm15, %xmm14                                #508.13
        andq      %r10, %rcx                                    #509.118
        popcntq   %rcx, %rcx                                    #509.90
        movups    (%rdi,%rcx,8), %xmm15                         #509.87
        movaps    %xmm15, -1408(%rbp)                           #509.41
        movq      $0xff80000000000000, %rcx                     #520.118
        psrad     $31, %xmm1                                    #509.41
        movdqa    %xmm1, %xmm15                                 #509.41
        pandn     -1408(%rbp), %xmm15                           #509.41
        pand      %xmm1, %xmm2                                  #509.41
        pshufd    $245, -1328(%rbp), %xmm1                      #520.41
        por       %xmm2, %xmm15                                 #509.41
        movdqa    %xmm15, -1408(%rbp)                           #509.41
        movdqa    -1344(%rbp), %xmm15                           #520.41
        movaps    -1408(%rbp), %xmm2                            #509.41
        mulpd     %xmm12, %xmm2                                 #509.24
        psllq     $8, %xmm5                                     #509.188
        movaps    %xmm5, -1184(%rbp)                            #523.41
        addpd     %xmm13, %xmm2                                 #509.13
        andq      %r10, %rcx                                    #520.118
        popcntq   %rcx, %rcx                                    #520.90
        movups    (%rdi,%rcx,8), %xmm13                         #520.87
        movaps    %xmm13, -1360(%rbp)                           #520.41
        movq      $0xfff8000000000000, %rcx                     #521.118
        psrad     $31, %xmm1                                    #520.41
        movdqa    %xmm1, %xmm13                                 #520.41
        pandn     -1360(%rbp), %xmm13                           #520.41
        pand      %xmm1, %xmm15                                 #520.41
        por       %xmm15, %xmm13                                #520.41
        movdqa    -1296(%rbp), %xmm15                           #521.41
        movdqa    %xmm13, -1360(%rbp)                           #520.41
        pshufd    $245, -1280(%rbp), %xmm13                     #521.41
        movaps    -1360(%rbp), %xmm1                            #520.41
        mulpd     %xmm11, %xmm1                                 #520.24
        psllq     $8, %xmm6                                     #520.188
        movaps    %xmm6, -1136(%rbp)                            #526.41
        addpd     %xmm14, %xmm1                                 #520.13
        andq      %r10, %rcx                                    #521.118
        popcntq   %rcx, %rcx                                    #521.90
        movups    (%rdi,%rcx,8), %xmm14                         #521.87
        movaps    %xmm14, -1312(%rbp)                           #521.41
        movq      $0xffe0000000000000, %rcx                     #522.118
        psrad     $31, %xmm13                                   #521.41
        movdqa    %xmm13, %xmm14                                #521.41
        pandn     -1312(%rbp), %xmm14                           #521.41
        pand      %xmm13, %xmm15                                #521.41
        pshufd    $245, -1232(%rbp), %xmm13                     #522.41
        por       %xmm15, %xmm14                                #521.41
        movdqa    -1248(%rbp), %xmm15                           #522.41
        movdqa    %xmm14, -1312(%rbp)                           #521.41
        movaps    -1312(%rbp), %xmm14                           #521.41
        mulpd     %xmm11, %xmm14                                #521.24
        psllq     $8, %xmm7                                     #521.188
        movaps    %xmm7, -1088(%rbp)                            #527.41
        addpd     %xmm2, %xmm14                                 #521.13
        andq      %r10, %rcx                                    #522.118
        popcntq   %rcx, %rcx                                    #522.90
        movups    (%rdi,%rcx,8), %xmm2                          #522.87
        movaps    %xmm2, -1264(%rbp)                            #522.41
        movq      $0xfffe000000000000, %rcx                     #523.118
        psrad     $31, %xmm13                                   #522.41
        movdqa    %xmm13, %xmm2                                 #522.41
        pandn     -1264(%rbp), %xmm2                            #522.41
        pand      %xmm13, %xmm15                                #522.41
        pshufd    $245, -1184(%rbp), %xmm13                     #523.41
        por       %xmm15, %xmm2                                 #522.41
        movdqa    -1200(%rbp), %xmm15                           #523.41
        movdqa    %xmm2, -1264(%rbp)                            #522.41
        movaps    -1264(%rbp), %xmm2                            #522.41
        mulpd     %xmm4, %xmm2                                  #522.24
        psllq     $8, %xmm8                                     #522.188
        movaps    %xmm8, -1040(%rbp)                            #528.41
        addpd     %xmm1, %xmm2                                  #522.13
        andq      %r10, %rcx                                    #523.118
        popcntq   %rcx, %rcx                                    #523.90
        movups    (%rdi,%rcx,8), %xmm1                          #523.87
        movaps    %xmm1, -1216(%rbp)                            #523.41
        movq      $0xffff800000000000, %rcx                     #526.118
        psrad     $31, %xmm13                                   #523.41
        movdqa    %xmm13, %xmm1                                 #523.41
        pandn     -1216(%rbp), %xmm1                            #523.41
        pand      %xmm13, %xmm15                                #523.41
        pshufd    $245, -1136(%rbp), %xmm13                     #526.41
        por       %xmm15, %xmm1                                 #523.41
        movdqa    -1152(%rbp), %xmm15                           #526.41
        movdqa    %xmm1, -1216(%rbp)                            #523.41
        movaps    -1216(%rbp), %xmm1                            #523.41
        mulpd     %xmm4, %xmm1                                  #523.24
        psllq     $8, %xmm5                                     #523.188
        movaps    %xmm5, -992(%rbp)                             #529.41
        addpd     %xmm14, %xmm1                                 #523.13
        andq      %r10, %rcx                                    #526.118
        popcntq   %rcx, %rcx                                    #526.90
        movups    (%rdi,%rcx,8), %xmm14                         #526.87
        movaps    %xmm14, -1168(%rbp)                           #526.41
        movq      $0xfffff80000000000, %rcx                     #527.118
        psrad     $31, %xmm13                                   #526.41
        movdqa    %xmm13, %xmm14                                #526.41
        pandn     -1168(%rbp), %xmm14                           #526.41
        pand      %xmm13, %xmm15                                #526.41
        movdqa    -1104(%rbp), %xmm13                           #527.41
        por       %xmm15, %xmm14                                #526.41
        movdqa    %xmm14, -1168(%rbp)                           #526.41
        pshufd    $245, -1088(%rbp), %xmm14                     #527.41
        movaps    -1168(%rbp), %xmm15                           #526.41
        mulpd     %xmm10, %xmm15                                #526.24
        psllq     $8, %xmm6                                     #526.188
        movaps    %xmm6, -944(%rbp)                             #531.41
        addpd     %xmm9, %xmm15                                 #526.13
        andq      %r10, %rcx                                    #527.118
        popcntq   %rcx, %rcx                                    #527.90
        movups    (%rdi,%rcx,8), %xmm9                          #527.87
        movaps    %xmm9, -1120(%rbp)                            #527.41
        movq      $0xffffe00000000000, %rcx                     #528.118
        psrad     $31, %xmm14                                   #527.41
        movdqa    %xmm14, %xmm9                                 #527.41
        pandn     -1120(%rbp), %xmm9                            #527.41
        pand      %xmm14, %xmm13                                #527.41
        movdqa    -1056(%rbp), %xmm14                           #528.41
        por       %xmm13, %xmm9                                 #527.41
        movdqa    %xmm9, -1120(%rbp)                            #527.41
        mulpd     -1120(%rbp), %xmm10                           #527.24
        pshufd    $245, -1040(%rbp), %xmm9                      #528.41
        psllq     $8, %xmm7                                     #527.188
        movaps    %xmm7, -848(%rbp)                             #533.41
        addpd     %xmm0, %xmm10                                 #527.13
        andq      %r10, %rcx                                    #528.118
        popcntq   %rcx, %rcx                                    #528.90
        movups    (%rdi,%rcx,8), %xmm0                          #528.87
        movaps    %xmm0, -1072(%rbp)                            #528.41
        movq      $0xfffffe0000000000, %rcx                     #529.118
        psrad     $31, %xmm9                                    #528.41
        movdqa    %xmm9, %xmm13                                 #528.41
        pandn     -1072(%rbp), %xmm13                           #528.41
        pand      %xmm9, %xmm14                                 #528.41
        por       %xmm14, %xmm13                                #528.41
        movdqa    %xmm13, -1072(%rbp)                           #528.41
        movaps    -1072(%rbp), %xmm13                           #528.41
        mulpd     %xmm12, %xmm13                                #528.24
        psllq     $8, %xmm8                                     #528.188
        movaps    %xmm8, -896(%rbp)                             #532.41
        addpd     %xmm15, %xmm13                                #528.13
        pshufd    $245, -992(%rbp), %xmm15                      #529.41
        andq      %r10, %rcx                                    #529.118
        popcntq   %rcx, %rcx                                    #529.90
        movups    (%rdi,%rcx,8), %xmm0                          #529.87
        movaps    %xmm0, -1024(%rbp)                            #529.41
        movdqa    -1008(%rbp), %xmm0                            #529.41
        psrad     $31, %xmm15                                   #529.41
        movdqa    %xmm15, %xmm9                                 #529.41
        pandn     -1024(%rbp), %xmm9                            #529.41
        movq      $0xffffff8000000000, %rcx                     #531.118
        pand      %xmm15, %xmm0                                 #529.41
        movdqa    -912(%rbp), %xmm15                            #532.41
        por       %xmm0, %xmm9                                  #529.41
        pshufd    $245, -944(%rbp), %xmm0                       #531.41
        movdqa    %xmm9, -1024(%rbp)                            #529.41
        mulpd     -1024(%rbp), %xmm12                           #529.24
        movdqa    -960(%rbp), %xmm9                             #531.41
        psllq     $8, %xmm5                                     #529.188
        movaps    %xmm5, -800(%rbp)                             #534.41
        addpd     %xmm10, %xmm12                                #529.13
        andq      %r10, %rcx                                    #531.118
        popcntq   %rcx, %rcx                                    #531.90
        movups    (%rdi,%rcx,8), %xmm10                         #531.87
        movaps    %xmm10, -976(%rbp)                            #531.41
        movq      $0xffffffe000000000, %rcx                     #532.118
        psrad     $31, %xmm0                                    #531.41
        movdqa    %xmm0, %xmm14                                 #531.41
        pandn     -976(%rbp), %xmm14                            #531.41
        pand      %xmm0, %xmm9                                  #531.41
        por       %xmm9, %xmm14                                 #531.41
        movdqa    %xmm14, -976(%rbp)                            #531.41
        movaps    -976(%rbp), %xmm14                            #531.41
        mulpd     %xmm11, %xmm14                                #531.24
        psllq     $8, %xmm6                                     #531.188
        andq      %r10, %rcx                                    #532.118
        addpd     %xmm13, %xmm14                                #531.13
        pshufd    $245, -896(%rbp), %xmm13                      #532.41
        popcntq   %rcx, %rcx                                    #532.90
        movups    (%rdi,%rcx,8), %xmm10                         #532.87
        movaps    %xmm10, -928(%rbp)                            #532.41
        movq      $0xfffffff800000000, %rcx                     #533.118
        pshufd    $245, -848(%rbp), %xmm10                      #533.41
        psrad     $31, %xmm13                                   #532.41
        movdqa    %xmm13, %xmm0                                 #532.41
        pandn     -928(%rbp), %xmm0                             #532.41
        pand      %xmm13, %xmm15                                #532.41
        por       %xmm15, %xmm0                                 #532.41
        movdqa    %xmm0, -928(%rbp)                             #532.41
        movaps    -928(%rbp), %xmm0                             #532.41
        mulpd     %xmm4, %xmm0                                  #532.24
        psllq     $8, %xmm8                                     #532.188
        andq      %r10, %rcx                                    #533.118
        addpd     %xmm14, %xmm0                                 #532.13
        movdqa    -864(%rbp), %xmm14                            #533.41
        popcntq   %rcx, %rcx                                    #533.90
        movups    (%rdi,%rcx,8), %xmm9                          #533.87
        movaps    %xmm9, -880(%rbp)                             #533.41
        movq      $0xfffffffe00000000, %rcx                     #534.118
        pshufd    $245, -800(%rbp), %xmm9                       #534.41
        psrad     $31, %xmm10                                   #533.41
        movdqa    %xmm10, %xmm13                                #533.41
        pandn     -880(%rbp), %xmm13                            #533.41
        pand      %xmm10, %xmm14                                #533.41
        por       %xmm14, %xmm13                                #533.41
        movdqa    -816(%rbp), %xmm10                            #534.41
        movdqa    %xmm13, -880(%rbp)                            #533.41
        mulpd     -880(%rbp), %xmm11                            #533.24
        psllq     $8, %xmm7                                     #533.188
        andq      %r10, %rcx                                    #534.118
        addpd     %xmm12, %xmm11                                #533.13
        popcntq   %rcx, %rcx                                    #534.90
        movups    (%rdi,%rcx,8), %xmm12                         #534.87
        movaps    %xmm12, -832(%rbp)                            #534.41
        psrad     $31, %xmm9                                    #534.41
        movdqa    %xmm9, %xmm14                                 #534.41
        pand      %xmm9, %xmm10                                 #534.41
        pandn     -832(%rbp), %xmm14                            #534.41
        por       %xmm10, %xmm14                                #534.41
        movdqa    %xmm14, -832(%rbp)                            #534.41
        mulpd     -832(%rbp), %xmm4                             #534.24
        psllq     $8, %xmm5                                     #534.188
        lea       4(%r12), %ecx                                 #539.33
        addpd     %xmm11, %xmm4                                 #534.13
        movddup   (%r8,%rcx,8), %xmm9                           #539.13
        lea       5(%r12), %ecx                                 #540.33
        movddup   (%r8,%rcx,8), %xmm12                          #540.13
        lea       6(%r12), %ecx                                 #541.33
        movddup   (%r8,%rcx,8), %xmm10                          #541.13
        addl      $7, %r12d                                     #542.33
        movddup   (%r8,%r12,8), %xmm11                          #542.13
        movaps    %xmm3, -768(%rbp)                             #544.41
        movdqa    -768(%rbp), %xmm13                            #544.41
        movaps    %xmm6, -752(%rbp)                             #544.41
        pshufd    $245, -752(%rbp), %xmm14                      #544.41
        movaps    %xmm3, -720(%rbp)                             #545.41
        movaps    %xmm7, -704(%rbp)                             #545.41
        movq      %r10, %r12                                    #544.118
        andq      $-2147483648, %r12                            #544.118
        popcntq   %r12, %r12                                    #544.90
        movups    (%rdi,%r12,8), %xmm15                         #544.87
        psrad     $31, %xmm14                                   #544.41
        movaps    %xmm15, -784(%rbp)                            #544.41
        movdqa    %xmm14, %xmm15                                #544.41
        pandn     -784(%rbp), %xmm15                            #544.41
        pand      %xmm14, %xmm13                                #544.41
        por       %xmm13, %xmm15                                #544.41
        movdqa    %xmm15, -784(%rbp)                            #544.41
        movaps    -784(%rbp), %xmm14                            #544.41
        mulpd     %xmm9, %xmm14                                 #544.24
        psllq     $8, %xmm6                                     #544.188
        movq      %r10, %r12                                    #545.118
        addpd     %xmm2, %xmm14                                 #544.13
        andq      $-134217728, %r12                             #545.118
        popcntq   %r12, %r12                                    #545.90
        movups    (%rdi,%r12,8), %xmm2                          #545.87
        movaps    %xmm2, -736(%rbp)                             #545.41
                                # LOE rdx rbx rsi rdi r8 r9 r10 r13 r14 r15 eax r11d xmm0 xmm1 xmm3 xmm4 xmm5 xmm6 xmm7 xmm8 xmm9 xmm10 xmm11 xmm12 xmm14
..B7.8:                         # Preds ..B7.3
        movdqa    -720(%rbp), %xmm13                            #545.41
        pshufd    $245, -704(%rbp), %xmm2                       #545.41
        movaps    %xmm3, -672(%rbp)                             #546.41
        movaps    %xmm8, -656(%rbp)                             #546.41
        movaps    %xmm3, -624(%rbp)                             #547.41
        movaps    %xmm5, -608(%rbp)                             #547.41
        movaps    %xmm3, -576(%rbp)                             #549.41
        movaps    %xmm6, -560(%rbp)                             #549.41
        movaps    %xmm3, -528(%rbp)                             #550.41
        movaps    %xmm3, -480(%rbp)                             #551.41
        movaps    %xmm3, -432(%rbp)                             #552.41
        movaps    %xmm3, -384(%rbp)                             #554.41
        movaps    %xmm3, -336(%rbp)                             #555.41
        movaps    %xmm3, -288(%rbp)                             #556.41
        movaps    %xmm3, -240(%rbp)                             #557.41
        movaps    %xmm3, -192(%rbp)                             #559.41
        movaps    %xmm3, -144(%rbp)                             #560.41
        movaps    %xmm3, -96(%rbp)                              #561.41
        movaps    %xmm3, -48(%rbp)                              #562.41
        psrad     $31, %xmm2                                    #545.41
        movdqa    %xmm2, %xmm15                                 #545.41
        pand      %xmm2, %xmm13                                 #545.41
        pandn     -736(%rbp), %xmm15                            #545.41
        pshufd    $245, -656(%rbp), %xmm2                       #546.41
        por       %xmm13, %xmm15                                #545.41
        movdqa    %xmm15, -736(%rbp)                            #545.41
        movdqa    -672(%rbp), %xmm15                            #546.41
        movaps    -736(%rbp), %xmm13                            #545.41
        mulpd     %xmm9, %xmm13                                 #545.24
        psllq     $8, %xmm7                                     #545.188
        movaps    %xmm7, -512(%rbp)                             #550.41
        addpd     %xmm1, %xmm13                                 #545.13
        movq      %r10, %r12                                    #546.118
        andq      $-536870912, %r12                             #546.118
        popcntq   %r12, %r12                                    #546.90
        movups    (%rdi,%r12,8), %xmm1                          #546.87
        psrad     $31, %xmm2                                    #546.41
        movaps    %xmm1, -688(%rbp)                             #546.41
        movdqa    %xmm2, %xmm1                                  #546.41
        pandn     -688(%rbp), %xmm1                             #546.41
        pand      %xmm2, %xmm15                                 #546.41
        por       %xmm15, %xmm1                                 #546.41
        movdqa    -624(%rbp), %xmm15                            #547.41
        movdqa    %xmm1, -688(%rbp)                             #546.41
        pshufd    $245, -608(%rbp), %xmm1                       #547.41
        movaps    -688(%rbp), %xmm2                             #546.41
        mulpd     %xmm12, %xmm2                                 #546.24
        psllq     $8, %xmm8                                     #546.188
        movaps    %xmm8, -464(%rbp)                             #551.41
        addpd     %xmm14, %xmm2                                 #546.13
        movq      %r10, %r12                                    #547.118
        andq      $-33554432, %r12                              #547.118
        popcntq   %r12, %r12                                    #547.90
        movups    (%rdi,%r12,8), %xmm14                         #547.87
        psrad     $31, %xmm1                                    #547.41
        movaps    %xmm14, -640(%rbp)                            #547.41
        movdqa    %xmm1, %xmm14                                 #547.41
        pandn     -640(%rbp), %xmm14                            #547.41
        pand      %xmm1, %xmm15                                 #547.41
        por       %xmm15, %xmm14                                #547.41
        movdqa    -576(%rbp), %xmm15                            #549.41
        movdqa    %xmm14, -640(%rbp)                            #547.41
        pshufd    $245, -560(%rbp), %xmm14                      #549.41
        movaps    -640(%rbp), %xmm1                             #547.41
        mulpd     %xmm12, %xmm1                                 #547.24
        psllq     $8, %xmm5                                     #547.188
        movaps    %xmm5, -416(%rbp)                             #552.41
        addpd     %xmm13, %xmm1                                 #547.13
        movq      %r10, %r12                                    #549.118
        andq      $-8388608, %r12                               #549.118
        popcntq   %r12, %r12                                    #549.90
        movups    (%rdi,%r12,8), %xmm13                         #549.87
        psrad     $31, %xmm14                                   #549.41
        movaps    %xmm13, -592(%rbp)                            #549.41
        movdqa    %xmm14, %xmm13                                #549.41
        pandn     -592(%rbp), %xmm13                            #549.41
        pand      %xmm14, %xmm15                                #549.41
        por       %xmm15, %xmm13                                #549.41
        movdqa    -528(%rbp), %xmm14                            #550.41
        movdqa    %xmm13, -592(%rbp)                            #549.41
        pshufd    $245, -512(%rbp), %xmm13                      #550.41
        movaps    -592(%rbp), %xmm15                            #549.41
        mulpd     %xmm10, %xmm15                                #549.24
        psllq     $8, %xmm6                                     #549.188
        movaps    %xmm6, -368(%rbp)                             #554.41
        addpd     %xmm2, %xmm15                                 #549.13
        movq      %r10, %r12                                    #550.118
        andq      $-524288, %r12                                #550.118
        popcntq   %r12, %r12                                    #550.90
        movups    (%rdi,%r12,8), %xmm2                          #550.87
        psrad     $31, %xmm13                                   #550.41
        movaps    %xmm2, -544(%rbp)                             #550.41
        movdqa    %xmm13, %xmm2                                 #550.41
        pandn     -544(%rbp), %xmm2                             #550.41
        pand      %xmm13, %xmm14                                #550.41
        movdqa    -480(%rbp), %xmm13                            #551.41
        por       %xmm14, %xmm2                                 #550.41
        movdqa    %xmm2, -544(%rbp)                             #550.41
        pshufd    $245, -464(%rbp), %xmm2                       #551.41
        movaps    -544(%rbp), %xmm14                            #550.41
        mulpd     %xmm10, %xmm14                                #550.24
        psllq     $8, %xmm7                                     #550.188
        movaps    %xmm7, -320(%rbp)                             #555.41
        addpd     %xmm1, %xmm14                                 #550.13
        movq      %r10, %r12                                    #551.118
        andq      $-2097152, %r12                               #551.118
        popcntq   %r12, %r12                                    #551.90
        movups    (%rdi,%r12,8), %xmm1                          #551.87
        psrad     $31, %xmm2                                    #551.41
        movaps    %xmm1, -496(%rbp)                             #551.41
        movdqa    %xmm2, %xmm1                                  #551.41
        pandn     -496(%rbp), %xmm1                             #551.41
        pand      %xmm2, %xmm13                                 #551.41
        pshufd    $245, -416(%rbp), %xmm2                       #552.41
        por       %xmm13, %xmm1                                 #551.41
        movdqa    -432(%rbp), %xmm13                            #552.41
        movdqa    %xmm1, -496(%rbp)                             #551.41
        movaps    -496(%rbp), %xmm1                             #551.41
        mulpd     %xmm11, %xmm1                                 #551.24
        psllq     $8, %xmm8                                     #551.188
        movaps    %xmm8, -272(%rbp)                             #556.41
        addpd     %xmm15, %xmm1                                 #551.13
        movaps    %xmm1, (%r9,%r15,8)                           #568.19
        movq      %r10, %r12                                    #552.118
        andq      $-131072, %r12                                #552.118
        popcntq   %r12, %r12                                    #552.90
        movups    (%rdi,%r12,8), %xmm15                         #552.87
        psrad     $31, %xmm2                                    #552.41
        movaps    %xmm15, -448(%rbp)                            #552.41
        movdqa    %xmm2, %xmm15                                 #552.41
        pandn     -448(%rbp), %xmm15                            #552.41
        pand      %xmm2, %xmm13                                 #552.41
        por       %xmm13, %xmm15                                #552.41
        pshufd    $245, -368(%rbp), %xmm13                      #554.41
        movdqa    %xmm15, -448(%rbp)                            #552.41
        movdqa    -384(%rbp), %xmm15                            #554.41
        movaps    -448(%rbp), %xmm2                             #552.41
        mulpd     %xmm11, %xmm2                                 #552.24
        psllq     $8, %xmm5                                     #552.188
        movaps    %xmm5, -224(%rbp)                             #557.41
        addpd     %xmm14, %xmm2                                 #552.13
        movaps    %xmm2, (%r9,%r14,8)                           #569.19
        movq      %r10, %r12                                    #554.118
        andq      $-32768, %r12                                 #554.118
        popcntq   %r12, %r12                                    #554.90
        movups    (%rdi,%r12,8), %xmm14                         #554.87
        psrad     $31, %xmm13                                   #554.41
        movaps    %xmm14, -400(%rbp)                            #554.41
        movdqa    %xmm13, %xmm14                                #554.41
        pandn     -400(%rbp), %xmm14                            #554.41
        pand      %xmm13, %xmm15                                #554.41
        por       %xmm15, %xmm14                                #554.41
        movdqa    -336(%rbp), %xmm15                            #555.41
        movdqa    %xmm14, -400(%rbp)                            #554.41
        pshufd    $245, -320(%rbp), %xmm14                      #555.41
        movaps    -400(%rbp), %xmm13                            #554.41
        mulpd     %xmm9, %xmm13                                 #554.24
        psllq     $8, %xmm6                                     #554.188
        movaps    %xmm6, -176(%rbp)                             #559.41
        pshufd    $245, -176(%rbp), %xmm6                       #559.41
        addpd     %xmm0, %xmm13                                 #554.13
        movq      %r10, %r12                                    #555.118
        andq      $-2048, %r12                                  #555.118
        popcntq   %r12, %r12                                    #555.90
        movups    (%rdi,%r12,8), %xmm0                          #555.87
        psrad     $31, %xmm14                                   #555.41
        movaps    %xmm0, -352(%rbp)                             #555.41
        movdqa    %xmm14, %xmm0                                 #555.41
        pandn     -352(%rbp), %xmm0                             #555.41
        pand      %xmm14, %xmm15                                #555.41
        por       %xmm15, %xmm0                                 #555.41
        movdqa    -288(%rbp), %xmm14                            #556.41
        movdqa    %xmm0, -352(%rbp)                             #555.41
        mulpd     -352(%rbp), %xmm9                             #555.24
        pshufd    $245, -272(%rbp), %xmm0                       #556.41
        psllq     $8, %xmm7                                     #555.188
        movaps    %xmm7, -128(%rbp)                             #560.41
        pshufd    $245, -128(%rbp), %xmm7                       #560.41
        addpd     %xmm4, %xmm9                                  #555.13
        movq      %r10, %r12                                    #556.118
        andq      $-8192, %r12                                  #556.118
        popcntq   %r12, %r12                                    #556.90
        movups    (%rdi,%r12,8), %xmm4                          #556.87
        psrad     $31, %xmm0                                    #556.41
        movaps    %xmm4, -304(%rbp)                             #556.41
        movdqa    %xmm0, %xmm15                                 #556.41
        pandn     -304(%rbp), %xmm15                            #556.41
        pand      %xmm0, %xmm14                                 #556.41
        pshufd    $245, -224(%rbp), %xmm0                       #557.41
        por       %xmm14, %xmm15                                #556.41
        movdqa    -240(%rbp), %xmm14                            #557.41
        movdqa    %xmm15, -304(%rbp)                            #556.41
        movaps    -304(%rbp), %xmm4                             #556.41
        mulpd     %xmm12, %xmm4                                 #556.24
        psllq     $8, %xmm8                                     #556.188
        movaps    %xmm8, -80(%rbp)                              #561.41
        pshufd    $245, -80(%rbp), %xmm8                        #561.41
        addpd     %xmm13, %xmm4                                 #556.13
        movq      %r10, %r12                                    #557.118
        andq      $-512, %r12                                   #557.118
        popcntq   %r12, %r12                                    #557.90
        movups    (%rdi,%r12,8), %xmm13                         #557.87
        psrad     $31, %xmm0                                    #557.41
        movaps    %xmm13, -256(%rbp)                            #557.41
        movdqa    %xmm0, %xmm15                                 #557.41
        pandn     -256(%rbp), %xmm15                            #557.41
        pand      %xmm0, %xmm14                                 #557.41
        movdqa    -192(%rbp), %xmm0                             #559.41
        por       %xmm14, %xmm15                                #557.41
        movdqa    %xmm15, -256(%rbp)                            #557.41
        mulpd     -256(%rbp), %xmm12                            #557.24
        psllq     $8, %xmm5                                     #557.188
        movaps    %xmm5, -32(%rbp)                              #562.41
        pshufd    $245, -32(%rbp), %xmm5                        #562.41
        addpd     %xmm9, %xmm12                                 #557.13
        movq      %r10, %r12                                    #559.118
        andq      $-128, %r12                                   #559.118
        popcntq   %r12, %r12                                    #559.90
        movups    (%rdi,%r12,8), %xmm9                          #559.87
        psrad     $31, %xmm6                                    #559.41
        movaps    %xmm9, -208(%rbp)                             #559.41
        movdqa    %xmm6, %xmm13                                 #559.41
        pandn     -208(%rbp), %xmm13                            #559.41
        pand      %xmm6, %xmm0                                  #559.41
        por       %xmm0, %xmm13                                 #559.41
        movdqa    -144(%rbp), %xmm0                             #560.41
        movdqa    %xmm13, -208(%rbp)                            #559.41
        movaps    -208(%rbp), %xmm6                             #559.41
        mulpd     %xmm10, %xmm6                                 #559.24
        movq      %r10, %r12                                    #560.118
        andq      $-8, %r12                                     #560.118
        addpd     %xmm4, %xmm6                                  #559.13
        popcntq   %r12, %r12                                    #560.90
        movups    (%rdi,%r12,8), %xmm4                          #560.87
        movaps    %xmm4, -160(%rbp)                             #560.41
        psrad     $31, %xmm7                                    #560.41
        movdqa    %xmm7, %xmm4                                  #560.41
        pand      %xmm7, %xmm0                                  #560.41
        pandn     -160(%rbp), %xmm4                             #560.41
        por       %xmm0, %xmm4                                  #560.41
        movdqa    -96(%rbp), %xmm0                              #561.41
        movdqa    %xmm4, -160(%rbp)                             #560.41
        mulpd     -160(%rbp), %xmm10                            #560.24
        movq      %r10, %r12                                    #561.118
        andq      $-32, %r12                                    #561.118
        addpd     %xmm12, %xmm10                                #560.13
        popcntq   %r12, %r12                                    #561.90
        movups    (%rdi,%r12,8), %xmm12                         #561.87
        movaps    %xmm12, -112(%rbp)                            #561.41
        psrad     $31, %xmm8                                    #561.41
        movdqa    %xmm8, %xmm4                                  #561.41
        pand      %xmm8, %xmm0                                  #561.41
        pandn     -112(%rbp), %xmm4                             #561.41
        por       %xmm0, %xmm4                                  #561.41
        movdqa    %xmm4, -112(%rbp)                             #561.41
        movaps    -112(%rbp), %xmm7                             #561.41
        mulpd     %xmm11, %xmm7                                 #561.24
        movq      %r10, %r12                                    #562.118
        andq      $-2, %r12                                     #562.118
        addpd     %xmm6, %xmm7                                  #561.13
        movaps    %xmm7, (%r9,%r13,8)                           #570.19
        popcntq   %r12, %r12                                    #562.90
        movups    (%rdi,%r12,8), %xmm6                          #562.87
        movaps    %xmm6, -64(%rbp)                              #562.41
        movdqa    -48(%rbp), %xmm6                              #562.41
        psrad     $31, %xmm5                                    #562.41
        movdqa    %xmm5, %xmm0                                  #562.41
        pandn     -64(%rbp), %xmm0                              #562.41
        pand      %xmm5, %xmm6                                  #562.41
        por       %xmm6, %xmm0                                  #562.41
        movdqa    %xmm0, -64(%rbp)                              #562.41
        mulpd     -64(%rbp), %xmm11                             #562.24
        popcntq   %r10, %r10                                    #565.11
        lea       (%rdi,%r10,8), %rdi                           #565.5
        addpd     %xmm10, %xmm11                                #562.13
        movaps    %xmm11, (%r9,%rbx,8)                          #571.19
        incl      %eax                                          #469.32
        movl      %eax, %r10d                                   #469.32
        movl      %r10d, %eax                                   #469.3
        cmpl      -8(%rbp), %eax                                #469.26
        jb        ..B7.3        # Prob 82%                      #469.26
                                # LOE rdx rsi rdi r8 r9 r10 eax r11d xmm3
..B7.4:                         # Preds ..B7.8
        movq      -1568(%rbp), %rbx                             #
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.87:                 #
        movq      -1576(%rbp), %r12                             #
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.88:                 #
        movq      -1584(%rbp), %r13                             #
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.89:                 #
        movq      -1592(%rbp), %r14                             #
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.90:                 #
        movq      -1600(%rbp), %r15                             #
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.91:                 #
                                # LOE rbx r12 r13 r14 r15
..B7.5:                         # Preds ..B7.4 ..B7.1
        movq      %rbp, %rsp                                    #573.1
        popq      %rbp                                          #573.1
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.92:                 #
        ret                                                     #573.1
        .align    16,0x90
..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.93:                 #
                                # LOE
# mark_end;
	.type	_Z7SSEspmvPKdPKmPKjjS0_Pdjjj,@function
	.size	_Z7SSEspmvPKdPKmPKjjS0_Pdjjj,.-_Z7SSEspmvPKdPKmPKjjS0_Pdjjj
	.data
# -- End  _Z7SSEspmvPKdPKmPKjjS0_Pdjjj
	.text
# -- Begin  _Z11popcountallPKhPjm
# mark_begin;
       .align    16,0x90
	.globl _Z11popcountallPKhPjm
_Z11popcountallPKhPjm:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
..B8.1:                         # Preds ..B8.0
..___tag_value__Z11popcountallPKhPjm.94:                        #576.1
        movq      %rdx, %r8                                     #578.16
        shrq      $3, %r8                                       #578.16
        xorl      %ecx, %ecx                                    #579.14
        xorl      %eax, %eax                                    #
        testq     %r8, %r8                                      #579.20
        jbe       ..B8.5        # Prob 10%                      #579.20
                                # LOE rax rdx rcx rbx rbp rsi rdi r8 r12 r13 r14 r15
..B8.3:                         # Preds ..B8.1 ..B8.3
        movzbl    (%rdi,%rcx,8), %r9d                           #581.36
        movzbl    1(%rdi,%rcx,8), %r11d                         #582.38
        andl      $15, %r9d                                     #581.45
        popcntl   %r9d, %r10d                                   #581.17
        andl      $15, %r11d                                    #582.49
        popcntl   %r11d, %r9d                                   #582.19
        movl      %r10d, (%rax,%rsi)                            #581.3
        movzbl    2(%rdi,%rcx,8), %r10d                         #583.38
        movl      %r9d, 4(%rax,%rsi)                            #582.3
        movzbl    3(%rdi,%rcx,8), %r9d                          #584.38
        andl      $15, %r10d                                    #583.49
        popcntl   %r10d, %r11d                                  #583.19
        movl      %r11d, 8(%rax,%rsi)                           #583.3
        movzbl    4(%rdi,%rcx,8), %r11d                         #585.38
        andl      $15, %r9d                                     #584.49
        popcntl   %r9d, %r10d                                   #584.19
        movl      %r10d, 12(%rax,%rsi)                          #584.3
        movzbl    5(%rdi,%rcx,8), %r10d                         #586.38
        andl      $15, %r11d                                    #585.49
        popcntl   %r11d, %r9d                                   #585.19
        movl      %r9d, 16(%rax,%rsi)                           #585.3
        movzbl    6(%rdi,%rcx,8), %r9d                          #587.38
        andl      $15, %r10d                                    #586.49
        popcntl   %r10d, %r11d                                  #586.19
        andl      $15, %r9d                                     #587.49
        movl      %r11d, 20(%rax,%rsi)                          #586.3
        movzbl    7(%rdi,%rcx,8), %r11d                         #588.38
        popcntl   %r9d, %r10d                                   #587.19
        andl      $15, %r11d                                    #588.49
        popcntl   %r11d, %r9d                                   #588.19
        movl      %r10d, 24(%rax,%rsi)                          #587.3
        movl      %r9d, 28(%rax,%rsi)                           #588.3
        incq      %rcx                                          #579.26
        addq      $32, %rax                                     #579.26
        cmpq      %r8, %rcx                                     #579.20
        jb        ..B8.3        # Prob 82%                      #579.20
                                # LOE rax rdx rcx rbx rbp rsi rdi r8 r12 r13 r14 r15
..B8.5:                         # Preds ..B8.3 ..B8.1
        shlq      $3, %r8                                       #590.18
        cmpq      %rdx, %r8                                     #590.23
        jae       ..B8.9        # Prob 10%                      #590.23
                                # LOE rdx rbx rbp rsi rdi r8 r12 r13 r14 r15
..B8.7:                         # Preds ..B8.5 ..B8.7
        movzbl    (%r8,%rdi), %eax                              #592.34
        andl      $15, %eax                                     #592.41
        popcntl   %eax, %ecx                                    #592.15
        movl      %ecx, (%rsi,%r8,4)                            #592.3
        incq      %r8                                           #590.28
        cmpq      %rdx, %r8                                     #590.23
        jb        ..B8.7        # Prob 82%                      #590.23
                                # LOE rdx rbx rbp rsi rdi r8 r12 r13 r14 r15
..B8.9:                         # Preds ..B8.7 ..B8.5
        ret                                                     #594.1
        .align    16,0x90
..___tag_value__Z11popcountallPKhPjm.95:                        #
                                # LOE
# mark_end;
	.type	_Z11popcountallPKhPjm,@function
	.size	_Z11popcountallPKhPjm,.-_Z11popcountallPKhPjm
	.data
# -- End  _Z11popcountallPKhPjm
	.text
# -- Begin  _Z11popcountallPKtPjm
# mark_begin;
       .align    16,0x90
	.globl _Z11popcountallPKtPjm
_Z11popcountallPKtPjm:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
..B9.1:                         # Preds ..B9.0
..___tag_value__Z11popcountallPKtPjm.96:                        #597.1
        movq      %rdx, %r9                                     #598.16
        shrq      $3, %r9                                       #598.16
        xorl      %r8d, %r8d                                    #599.14
        xorl      %ecx, %ecx                                    #
        xorl      %eax, %eax                                    #
        testq     %r9, %r9                                      #599.20
        jbe       ..B9.5        # Prob 10%                      #599.20
                                # LOE rax rdx rcx rbx rbp rsi rdi r8 r9 r12 r13 r14 r15
..B9.3:                         # Preds ..B9.1 ..B9.3
        movzwl    (%rcx,%rdi), %r10d                            #601.36
        popcntl   %r10d, %r11d                                  #601.17
        movzwl    2(%rcx,%rdi), %r10d                           #602.38
        movl      %r11d, (%rax,%rsi)                            #601.3
        popcntl   %r10d, %r11d                                  #602.19
        movzwl    4(%rcx,%rdi), %r10d                           #603.38
        movl      %r11d, 4(%rax,%rsi)                           #602.3
        popcntl   %r10d, %r11d                                  #603.19
        movzwl    6(%rcx,%rdi), %r10d                           #604.38
        incq      %r8                                           #599.26
        movl      %r11d, 8(%rax,%rsi)                           #603.3
        popcntl   %r10d, %r11d                                  #604.19
        movzwl    8(%rcx,%rdi), %r10d                           #605.38
        movl      %r11d, 12(%rax,%rsi)                          #604.3
        popcntl   %r10d, %r11d                                  #605.19
        movzwl    10(%rcx,%rdi), %r10d                          #606.38
        movl      %r11d, 16(%rax,%rsi)                          #605.3
        popcntl   %r10d, %r11d                                  #606.19
        movzwl    12(%rcx,%rdi), %r10d                          #607.38
        movl      %r11d, 20(%rax,%rsi)                          #606.3
        popcntl   %r10d, %r11d                                  #607.19
        movzwl    14(%rcx,%rdi), %r10d                          #608.38
        addq      $16, %rcx                                     #599.26
        movl      %r11d, 24(%rax,%rsi)                          #607.3
        popcntl   %r10d, %r11d                                  #608.19
        movl      %r11d, 28(%rax,%rsi)                          #608.3
        addq      $32, %rax                                     #599.26
        cmpq      %r9, %r8                                      #599.20
        jb        ..B9.3        # Prob 82%                      #599.20
                                # LOE rax rdx rcx rbx rbp rsi rdi r8 r9 r12 r13 r14 r15
..B9.5:                         # Preds ..B9.3 ..B9.1
        shlq      $3, %r9                                       #610.18
        cmpq      %rdx, %r9                                     #610.23
        jae       ..B9.9        # Prob 10%                      #610.23
                                # LOE rdx rbx rbp rsi rdi r9 r12 r13 r14 r15
..B9.7:                         # Preds ..B9.5 ..B9.7
        movzwl    (%rdi,%r9,2), %eax                            #612.34
        popcntl   %eax, %ecx                                    #612.15
        movl      %ecx, (%rsi,%r9,4)                            #612.3
        incq      %r9                                           #610.28
        cmpq      %rdx, %r9                                     #610.23
        jb        ..B9.7        # Prob 82%                      #610.23
                                # LOE rdx rbx rbp rsi rdi r9 r12 r13 r14 r15
..B9.9:                         # Preds ..B9.7 ..B9.5
        ret                                                     #614.1
        .align    16,0x90
..___tag_value__Z11popcountallPKtPjm.97:                        #
                                # LOE
# mark_end;
	.type	_Z11popcountallPKtPjm,@function
	.size	_Z11popcountallPKtPjm,.-_Z11popcountallPKtPjm
	.data
# -- End  _Z11popcountallPKtPjm
	.text
# -- Begin  _Z11popcountallPKmPjm
# mark_begin;
       .align    16,0x90
	.globl _Z11popcountallPKmPjm
_Z11popcountallPKmPjm:
# parameter 1: %rdi
# parameter 2: %rsi
# parameter 3: %rdx
..B10.1:                        # Preds ..B10.0
..___tag_value__Z11popcountallPKmPjm.98:                        #617.1
        movq      %rdx, %r9                                     #618.16
        shrq      $3, %r9                                       #618.16
        xorl      %r10d, %r10d                                  #619.14
        xorl      %r11d, %r11d                                  #
        xorl      %r8d, %r8d                                    #
        testq     %r9, %r9                                      #619.20
        jbe       ..B10.5       # Prob 10%                      #619.20
                                # LOE rdx rbx rbp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15
..B10.2:                        # Preds ..B10.1
        movq      $0x5555555555555555, %rcx                     #621.17
        movq      $0x3333333333333333, %rax                     #621.17
        movq      %rbx, -8(%rsp)                                #621.17
..___tag_value__Z11popcountallPKmPjm.99:                        #
        movq      %r12, -16(%rsp)                               #621.17
..___tag_value__Z11popcountallPKmPjm.100:                       #
        movq      %r13, -24(%rsp)                               #621.17
..___tag_value__Z11popcountallPKmPjm.101:                       #
        movq      $0xf0f0f0f0f0f0f0f, %rbx                      #621.17
                                # LOE rax rdx rcx rbx rbp rsi rdi r8 r9 r10 r11 r14 r15
..B10.3:                        # Preds ..B10.3 ..B10.2
        movq      (%r11,%rdi), %r13                             #621.37
        movq      %r13, %r12                                    #621.17
        shrq      $1, %r12                                      #621.17
        incq      %r10                                          #619.26
        andq      %rcx, %r12                                    #621.17
        subq      %r12, %r13                                    #621.17
        movq      %r13, %r12                                    #621.17
        shrq      $2, %r12                                      #621.17
        andq      %rax, %r13                                    #621.17
        andq      %rax, %r12                                    #621.17
        addq      %r13, %r12                                    #621.17
        movq      %r12, %r13                                    #621.17
        shrq      $4, %r13                                      #621.17
        addq      %r13, %r12                                    #621.17
        andq      %rbx, %r12                                    #621.17
        movq      %r12, %r13                                    #621.17
        shrq      $8, %r13                                      #621.17
        addq      %r13, %r12                                    #621.17
        movq      %r12, %r13                                    #621.17
        shrq      $16, %r13                                     #621.17
        addq      %r13, %r12                                    #621.17
        movq      %r12, %r13                                    #621.17
        shrq      $32, %r13                                     #621.17
        addq      %r13, %r12                                    #621.17
        movq      8(%r11,%rdi), %r13                            #622.39
        movzbl    %r12b, %r12d                                  #621.17
        movl      %r12d, (%r8,%rsi)                             #621.3
        movq      %r13, %r12                                    #622.19
        shrq      $1, %r12                                      #622.19
        andq      %rcx, %r12                                    #622.19
        subq      %r12, %r13                                    #622.19
        movq      %r13, %r12                                    #622.19
        shrq      $2, %r12                                      #622.19
        andq      %rax, %r13                                    #622.19
        andq      %rax, %r12                                    #622.19
        addq      %r13, %r12                                    #622.19
        movq      %r12, %r13                                    #622.19
        shrq      $4, %r13                                      #622.19
        addq      %r13, %r12                                    #622.19
        andq      %rbx, %r12                                    #622.19
        movq      %r12, %r13                                    #622.19
        shrq      $8, %r13                                      #622.19
        addq      %r13, %r12                                    #622.19
        movq      %r12, %r13                                    #622.19
        shrq      $16, %r13                                     #622.19
        addq      %r13, %r12                                    #622.19
        movq      %r12, %r13                                    #622.19
        shrq      $32, %r13                                     #622.19
        addq      %r13, %r12                                    #622.19
        movq      16(%r11,%rdi), %r13                           #623.39
        movzbl    %r12b, %r12d                                  #622.19
        movl      %r12d, 4(%r8,%rsi)                            #622.3
        movq      %r13, %r12                                    #623.19
        shrq      $1, %r12                                      #623.19
        andq      %rcx, %r12                                    #623.19
        subq      %r12, %r13                                    #623.19
        movq      %r13, %r12                                    #623.19
        shrq      $2, %r12                                      #623.19
        andq      %rax, %r13                                    #623.19
        andq      %rax, %r12                                    #623.19
        addq      %r13, %r12                                    #623.19
        movq      %r12, %r13                                    #623.19
        shrq      $4, %r13                                      #623.19
        addq      %r13, %r12                                    #623.19
        andq      %rbx, %r12                                    #623.19
        movq      %r12, %r13                                    #623.19
        shrq      $8, %r13                                      #623.19
        addq      %r13, %r12                                    #623.19
        movq      %r12, %r13                                    #623.19
        shrq      $16, %r13                                     #623.19
        addq      %r13, %r12                                    #623.19
        movq      %r12, %r13                                    #623.19
        shrq      $32, %r13                                     #623.19
        addq      %r13, %r12                                    #623.19
        movq      24(%r11,%rdi), %r13                           #624.39
        movzbl    %r12b, %r12d                                  #623.19
        movl      %r12d, 8(%r8,%rsi)                            #623.3
        movq      %r13, %r12                                    #624.19
        shrq      $1, %r12                                      #624.19
        andq      %rcx, %r12                                    #624.19
        subq      %r12, %r13                                    #624.19
        movq      %r13, %r12                                    #624.19
        shrq      $2, %r12                                      #624.19
        andq      %rax, %r13                                    #624.19
        andq      %rax, %r12                                    #624.19
        addq      %r13, %r12                                    #624.19
        movq      %r12, %r13                                    #624.19
        shrq      $4, %r13                                      #624.19
        addq      %r13, %r12                                    #624.19
        andq      %rbx, %r12                                    #624.19
        movq      %r12, %r13                                    #624.19
        shrq      $8, %r13                                      #624.19
        addq      %r13, %r12                                    #624.19
        movq      %r12, %r13                                    #624.19
        shrq      $16, %r13                                     #624.19
        addq      %r13, %r12                                    #624.19
        movq      %r12, %r13                                    #624.19
        shrq      $32, %r13                                     #624.19
        addq      %r13, %r12                                    #624.19
        movq      32(%r11,%rdi), %r13                           #625.39
        movzbl    %r12b, %r12d                                  #624.19
        movl      %r12d, 12(%r8,%rsi)                           #624.3
        movq      %r13, %r12                                    #625.19
        shrq      $1, %r12                                      #625.19
        andq      %rcx, %r12                                    #625.19
        subq      %r12, %r13                                    #625.19
        movq      %r13, %r12                                    #625.19
        shrq      $2, %r12                                      #625.19
        andq      %rax, %r13                                    #625.19
        andq      %rax, %r12                                    #625.19
        addq      %r13, %r12                                    #625.19
        movq      %r12, %r13                                    #625.19
        shrq      $4, %r13                                      #625.19
        addq      %r13, %r12                                    #625.19
        andq      %rbx, %r12                                    #625.19
        movq      %r12, %r13                                    #625.19
        shrq      $8, %r13                                      #625.19
        addq      %r13, %r12                                    #625.19
        movq      %r12, %r13                                    #625.19
        shrq      $16, %r13                                     #625.19
        addq      %r13, %r12                                    #625.19
        movq      %r12, %r13                                    #625.19
        shrq      $32, %r13                                     #625.19
        addq      %r13, %r12                                    #625.19
        movq      40(%r11,%rdi), %r13                           #626.39
        movzbl    %r12b, %r12d                                  #625.19
        movl      %r12d, 16(%r8,%rsi)                           #625.3
        movq      %r13, %r12                                    #626.19
        shrq      $1, %r12                                      #626.19
        andq      %rcx, %r12                                    #626.19
        subq      %r12, %r13                                    #626.19
        movq      %r13, %r12                                    #626.19
        shrq      $2, %r12                                      #626.19
        andq      %rax, %r13                                    #626.19
        andq      %rax, %r12                                    #626.19
        addq      %r13, %r12                                    #626.19
        movq      %r12, %r13                                    #626.19
        shrq      $4, %r13                                      #626.19
        addq      %r13, %r12                                    #626.19
        andq      %rbx, %r12                                    #626.19
        movq      %r12, %r13                                    #626.19
        shrq      $8, %r13                                      #626.19
        addq      %r13, %r12                                    #626.19
        movq      %r12, %r13                                    #626.19
        shrq      $16, %r13                                     #626.19
        addq      %r13, %r12                                    #626.19
        movq      %r12, %r13                                    #626.19
        shrq      $32, %r13                                     #626.19
        addq      %r13, %r12                                    #626.19
        movq      48(%r11,%rdi), %r13                           #627.39
        movzbl    %r12b, %r12d                                  #626.19
        movl      %r12d, 20(%r8,%rsi)                           #626.3
        movq      %r13, %r12                                    #627.19
        shrq      $1, %r12                                      #627.19
        andq      %rcx, %r12                                    #627.19
        subq      %r12, %r13                                    #627.19
        movq      %r13, %r12                                    #627.19
        shrq      $2, %r12                                      #627.19
        andq      %rax, %r13                                    #627.19
        andq      %rax, %r12                                    #627.19
        addq      %r13, %r12                                    #627.19
        movq      %r12, %r13                                    #627.19
        shrq      $4, %r13                                      #627.19
        addq      %r13, %r12                                    #627.19
        andq      %rbx, %r12                                    #627.19
        movq      %r12, %r13                                    #627.19
        shrq      $8, %r13                                      #627.19
        addq      %r13, %r12                                    #627.19
        movq      %r12, %r13                                    #627.19
        shrq      $16, %r13                                     #627.19
        addq      %r13, %r12                                    #627.19
        movq      %r12, %r13                                    #627.19
        shrq      $32, %r13                                     #627.19
        addq      %r13, %r12                                    #627.19
        movq      56(%r11,%rdi), %r13                           #628.39
        movzbl    %r12b, %r12d                                  #627.19
        movl      %r12d, 24(%r8,%rsi)                           #627.3
        movq      %r13, %r12                                    #628.19
        shrq      $1, %r12                                      #628.19
        addq      $64, %r11                                     #619.26
        andq      %rcx, %r12                                    #628.19
        subq      %r12, %r13                                    #628.19
        movq      %r13, %r12                                    #628.19
        shrq      $2, %r12                                      #628.19
        andq      %rax, %r13                                    #628.19
        andq      %rax, %r12                                    #628.19
        addq      %r13, %r12                                    #628.19
        movq      %r12, %r13                                    #628.19
        shrq      $4, %r13                                      #628.19
        addq      %r13, %r12                                    #628.19
        andq      %rbx, %r12                                    #628.19
        movq      %r12, %r13                                    #628.19
        shrq      $8, %r13                                      #628.19
        addq      %r13, %r12                                    #628.19
        movq      %r12, %r13                                    #628.19
        shrq      $16, %r13                                     #628.19
        addq      %r13, %r12                                    #628.19
        movq      %r12, %r13                                    #628.19
        shrq      $32, %r13                                     #628.19
        addq      %r13, %r12                                    #628.19
        movzbl    %r12b, %r12d                                  #628.19
        movl      %r12d, 28(%r8,%rsi)                           #628.3
        addq      $32, %r8                                      #619.26
        cmpq      %r9, %r10                                     #619.20
        jb        ..B10.3       # Prob 82%                      #619.20
                                # LOE rax rdx rcx rbx rbp rsi rdi r8 r9 r10 r11 r14 r15
..B10.4:                        # Preds ..B10.3
        movq      -8(%rsp), %rbx                                #
..___tag_value__Z11popcountallPKmPjm.102:                       #
        movq      -16(%rsp), %r12                               #
..___tag_value__Z11popcountallPKmPjm.103:                       #
        movq      -24(%rsp), %r13                               #
..___tag_value__Z11popcountallPKmPjm.104:                       #
                                # LOE rdx rbx rbp rsi rdi r9 r12 r13 r14 r15
..B10.5:                        # Preds ..B10.1 ..B10.4
        shlq      $3, %r9                                       #630.18
        cmpq      %rdx, %r9                                     #630.23
        jae       ..B10.9       # Prob 10%                      #630.23
                                # LOE rdx rbx rbp rsi rdi r9 r12 r13 r14 r15
..B10.6:                        # Preds ..B10.5
        movq      $0x5555555555555555, %rcx                     #621.17
        movq      $0x3333333333333333, %r8                      #621.17
        movq      $0xf0f0f0f0f0f0f0f, %r10                      #621.17
                                # LOE rdx rcx rbx rbp rsi rdi r8 r9 r10 r12 r13 r14 r15
..B10.7:                        # Preds ..B10.7 ..B10.6
        movq      (%rdi,%r9,8), %r11                            #632.35
        movq      %r11, %rax                                    #632.15
        shrq      $1, %rax                                      #632.15
        andq      %rcx, %rax                                    #632.15
        subq      %rax, %r11                                    #632.15
        movq      %r11, %rax                                    #632.15
        shrq      $2, %rax                                      #632.15
        andq      %r8, %r11                                     #632.15
        andq      %r8, %rax                                     #632.15
        addq      %r11, %rax                                    #632.15
        movq      %rax, %r11                                    #632.15
        shrq      $4, %r11                                      #632.15
        addq      %r11, %rax                                    #632.15
        andq      %r10, %rax                                    #632.15
        movq      %rax, %r11                                    #632.15
        shrq      $8, %r11                                      #632.15
        addq      %r11, %rax                                    #632.15
        movq      %rax, %r11                                    #632.15
        shrq      $16, %r11                                     #632.15
        addq      %r11, %rax                                    #632.15
        movq      %rax, %r11                                    #632.15
        shrq      $32, %r11                                     #632.15
        addq      %r11, %rax                                    #632.15
        movzbl    %al, %eax                                     #632.15
        movl      %eax, (%rsi,%r9,4)                            #632.3
        incq      %r9                                           #630.28
        cmpq      %rdx, %r9                                     #630.23
        jb        ..B10.7       # Prob 82%                      #630.23
                                # LOE rdx rcx rbx rbp rsi rdi r8 r9 r10 r12 r13 r14 r15
..B10.9:                        # Preds ..B10.7 ..B10.5
        ret                                                     #634.1
        .align    16,0x90
..___tag_value__Z11popcountallPKmPjm.105:                       #
                                # LOE
# mark_end;
	.type	_Z11popcountallPKmPjm,@function
	.size	_Z11popcountallPKmPjm,.-_Z11popcountallPKmPjm
	.data
# -- End  _Z11popcountallPKmPjm
	.text
# -- Begin  __sti__$E
# mark_begin;
       .align    16,0x90
__sti__$E:
..B11.1:                        # Preds ..B11.0
..___tag_value___sti__$E.106:                                   #
        pushq     %rsi                                          #
..___tag_value___sti__$E.107:                                   #
        movl      $_ZSt8__ioinit, %edi                          #72.25
..___tag_value___sti__$E.108:                                   #72.25
        call      _ZNSt8ios_base4InitC1Ev                       #72.25
..___tag_value___sti__$E.109:                                   #
                                # LOE rbx rbp r12 r13 r14 r15
..B11.2:                        # Preds ..B11.1
        movl      $_ZNSt8ios_base4InitD1Ev, %edi                #72.25
        movl      $_ZSt8__ioinit, %esi                          #72.25
        movl      $__dso_handle, %edx                           #72.25
        xorl      %eax, %eax                                    #72.25
..___tag_value___sti__$E.110:                                   #72.25
        call      __cxa_atexit                                  #72.25
..___tag_value___sti__$E.111:                                   #
                                # LOE rbx rbp r12 r13 r14 r15
..B11.3:                        # Preds ..B11.2
        popq      %rcx                                          #72.25
..___tag_value___sti__$E.112:                                   #
        ret                                                     #72.25
        .align    16,0x90
..___tag_value___sti__$E.113:                                   #
                                # LOE
# mark_end;
	.type	__sti__$E,@function
	.size	__sti__$E,.-__sti__$E
	.data
# -- End  __sti__$E
	.bss
	.align 4
	.align 1
_ZSt8__ioinit:
	.type	_ZSt8__ioinit,@object
	.size	_ZSt8__ioinit,1
	.space 1	# pad
	.section .rodata, "a"
	.align 32
	.align 32
BitReverseTable64:
	.byte	0
	.byte	32
	.byte	16
	.byte	48
	.byte	8
	.byte	40
	.byte	24
	.byte	56
	.byte	4
	.byte	36
	.byte	20
	.byte	52
	.byte	12
	.byte	44
	.byte	28
	.byte	60
	.byte	2
	.byte	34
	.byte	18
	.byte	50
	.byte	10
	.byte	42
	.byte	26
	.byte	58
	.byte	6
	.byte	38
	.byte	22
	.byte	54
	.byte	14
	.byte	46
	.byte	30
	.byte	62
	.byte	1
	.byte	33
	.byte	17
	.byte	49
	.byte	9
	.byte	41
	.byte	25
	.byte	57
	.byte	5
	.byte	37
	.byte	21
	.byte	53
	.byte	13
	.byte	45
	.byte	29
	.byte	61
	.byte	3
	.byte	35
	.byte	19
	.byte	51
	.byte	11
	.byte	43
	.byte	27
	.byte	59
	.byte	7
	.byte	39
	.byte	23
	.byte	55
	.byte	15
	.byte	47
	.byte	31
	.byte	63
	.type	BitReverseTable64,@object
	.size	BitReverseTable64,64
	.section .ctors, "wa"
	.align 8
__init_0:
	.type	__init_0,@object
	.size	__init_0,8
	.quad	__sti__$E
	.data
	.hidden __dso_handle
# mark_proc_addr_taken __sti__$E;
# mark_proc_addr_taken _ZNSt8ios_base4InitD1Ev;
	.weak pthread_setspecific
	.weak pthread_getspecific
	.weak pthread_key_delete
	.weak pthread_key_create
	.weak pthread_cond_timedwait
	.weak pthread_cond_wait
	.weak pthread_cond_broadcast
	.weak pthread_cond_signal
	.weak pthread_cond_destroy
	.weak pthread_mutexattr_settype
	.weak pthread_mutexattr_destroy
	.weak pthread_mutexattr_init
	.weak pthread_mutex_unlock
	.weak pthread_mutex_timedlock
	.weak pthread_mutex_lock
	.weak pthread_mutex_trylock
	.weak pthread_mutex_destroy
	.weak pthread_mutex_init
	.weak pthread_cancel
	.weak pthread_once
	.weak pthread_equal
	.weak pthread_self
	.weak pthread_detach
	.weak pthread_join
	.weak pthread_create
	.weak sched_yield
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .debug_line
	.section .debug_line
.debug_line_seg:
	.align 1
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
	.4byte 0x0000001c
	.4byte 0x00000000
	.byte 0x01
	.4byte 0x004c507a
	.byte 0x01
	.byte 0x78
	.byte 0x10
	.byte 0x0a
	.byte 0x00
	.8byte __gxx_personality_v0
	.byte 0x00
	.4byte 0x9008070c
	.byte 0x01
	.4byte 0x00000024
	.4byte 0x00000024
	.8byte ..___tag_value__Z10BitReverset.1
	.8byte ..___tag_value__Z10BitReverset.2-..___tag_value__Z10BitReverset.1
	.byte 0x08
	.8byte 0x0000000000000000
	.4byte 0x00000000
	.2byte 0x0000
	.byte 0x00
	.4byte 0x0000007c
	.4byte 0x0000004c
	.8byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.3
	.8byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.16-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.3
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.4-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.3
	.byte 0x83
	.byte 0x03
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.5-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.4
	.byte 0x8c
	.byte 0x04
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.6-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.5
	.byte 0x8d
	.byte 0x05
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.7-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.6
	.byte 0x8e
	.byte 0x06
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.8-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.7
	.byte 0x8f
	.byte 0x07
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.9-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.8
	.byte 0x86
	.byte 0x08
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.10-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.9
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.11-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.10
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.12-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.11
	.byte 0x09
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.13-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.12
	.byte 0x09
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.14-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.13
	.byte 0x09
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.15-..___tag_value__Z6symcsrPKdPKhPKjjS0_S0_PdS5_jj.14
	.byte 0x09
	.2byte 0x0606
	.4byte 0x00000000
	.byte 0x00
	.4byte 0x0000008c
	.4byte 0x000000cc
	.8byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.17
	.8byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.32-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.17
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.18-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.17
	.byte 0x0c
	.2byte 0xc007
	.byte 0x01
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.19-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.18
	.byte 0x83
	.byte 0x13
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.20-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.19
	.byte 0x8c
	.byte 0x14
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.21-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.20
	.byte 0x8d
	.byte 0x15
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.22-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.21
	.byte 0x8e
	.byte 0x16
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.23-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.22
	.byte 0x8f
	.byte 0x17
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.24-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.23
	.byte 0x86
	.byte 0x18
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.25-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.24
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.26-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.25
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.27-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.26
	.byte 0x09
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.28-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.27
	.byte 0x09
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.29-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.28
	.byte 0x09
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.30-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.29
	.byte 0x09
	.2byte 0x0606
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.31-..___tag_value__Z6SSEsymPKdPKhPKjjS0_Pdjjs.30
	.byte 0x0c
	.2byte 0x0807
	.4byte 0x00000000
	.4byte 0x0000008c
	.4byte 0x0000015c
	.8byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.33
	.8byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.48-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.33
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.34-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.33
	.byte 0x0c
	.2byte 0xc007
	.byte 0x01
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.35-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.34
	.byte 0x83
	.byte 0x13
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.36-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.35
	.byte 0x8c
	.byte 0x14
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.37-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.36
	.byte 0x8d
	.byte 0x15
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.38-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.37
	.byte 0x8e
	.byte 0x16
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.39-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.38
	.byte 0x8f
	.byte 0x17
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.40-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.39
	.byte 0x86
	.byte 0x18
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.41-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.40
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.42-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.41
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.43-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.42
	.byte 0x09
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.44-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.43
	.byte 0x09
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.45-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.44
	.byte 0x09
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.46-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.45
	.byte 0x09
	.2byte 0x0606
	.byte 0x04
	.4byte ..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.47-..___tag_value__Z6SSEsymPKdPKhPKjjS0_S0_PdS5_jjs.46
	.byte 0x0c
	.2byte 0x0807
	.4byte 0x00000000
	.4byte 0x0000008c
	.4byte 0x000001ec
	.8byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.49
	.8byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.64-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.49
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.50-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.49
	.byte 0x0c
	.2byte 0xb007
	.byte 0x01
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.51-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.50
	.byte 0x83
	.byte 0x11
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.52-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.51
	.byte 0x8c
	.byte 0x12
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.53-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.52
	.byte 0x8d
	.byte 0x13
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.54-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.53
	.byte 0x8e
	.byte 0x14
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.55-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.54
	.byte 0x8f
	.byte 0x15
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.56-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.55
	.byte 0x86
	.byte 0x16
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.57-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.56
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.58-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.57
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.59-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.58
	.byte 0x09
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.60-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.59
	.byte 0x09
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.61-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.60
	.byte 0x09
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.62-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.61
	.byte 0x09
	.2byte 0x0606
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.63-..___tag_value__Z7SSEspmvPKdPKhPKjjS0_Pdjjj.62
	.byte 0x0c
	.2byte 0x0807
	.4byte 0x00000000
	.4byte 0x0000007c
	.4byte 0x0000027c
	.8byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.65
	.8byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.78-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.65
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.66-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.65
	.byte 0x0c
	.2byte 0xc007
	.byte 0x03
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.67-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.66
	.byte 0x83
	.byte 0x34
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.68-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.67
	.byte 0x8c
	.byte 0x35
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.69-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.68
	.byte 0x8d
	.byte 0x36
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.70-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.69
	.byte 0x8e
	.byte 0x37
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.71-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.70
	.byte 0x8f
	.byte 0x38
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.72-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.71
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.73-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.72
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.74-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.73
	.byte 0x09
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.75-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.74
	.byte 0x09
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.76-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.75
	.byte 0x09
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.77-..___tag_value__Z7SSEspmvPKdPKtPKjjS0_Pdjjj.76
	.byte 0x0c
	.2byte 0x0807
	.2byte 0x0000
	.byte 0x00
	.4byte 0x00000084
	.4byte 0x000002fc
	.8byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.79
	.8byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.93-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.79
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.80-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.79
	.byte 0x0c
	.2byte 0x1006
	.byte 0x86
	.byte 0x02
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.82-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.80
	.byte 0x83
	.2byte 0x01c6
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.83-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.82
	.byte 0x8c
	.2byte 0x01c7
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.84-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.83
	.byte 0x8d
	.2byte 0x01c8
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.85-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.84
	.byte 0x8e
	.2byte 0x01c9
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.86-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.85
	.byte 0x8f
	.2byte 0x01ca
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.87-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.86
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.88-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.87
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.89-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.88
	.byte 0x09
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.90-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.89
	.byte 0x09
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.91-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.90
	.byte 0x09
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.92-..___tag_value__Z7SSEspmvPKdPKmPKjjS0_Pdjjj.91
	.byte 0x09
	.2byte 0x0606
	.4byte 0x00000000
	.byte 0x00
	.4byte 0x00000024
	.4byte 0x00000384
	.8byte ..___tag_value__Z11popcountallPKhPjm.94
	.8byte ..___tag_value__Z11popcountallPKhPjm.95-..___tag_value__Z11popcountallPKhPjm.94
	.byte 0x08
	.8byte 0x0000000000000000
	.4byte 0x00000000
	.2byte 0x0000
	.byte 0x00
	.4byte 0x00000024
	.4byte 0x000003ac
	.8byte ..___tag_value__Z11popcountallPKtPjm.96
	.8byte ..___tag_value__Z11popcountallPKtPjm.97-..___tag_value__Z11popcountallPKtPjm.96
	.byte 0x08
	.8byte 0x0000000000000000
	.4byte 0x00000000
	.2byte 0x0000
	.byte 0x00
	.4byte 0x0000004c
	.4byte 0x000003d4
	.8byte ..___tag_value__Z11popcountallPKmPjm.98
	.8byte ..___tag_value__Z11popcountallPKmPjm.105-..___tag_value__Z11popcountallPKmPjm.98
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value__Z11popcountallPKmPjm.99-..___tag_value__Z11popcountallPKmPjm.98
	.byte 0x83
	.byte 0x02
	.byte 0x04
	.4byte ..___tag_value__Z11popcountallPKmPjm.100-..___tag_value__Z11popcountallPKmPjm.99
	.byte 0x8c
	.byte 0x03
	.byte 0x04
	.4byte ..___tag_value__Z11popcountallPKmPjm.101-..___tag_value__Z11popcountallPKmPjm.100
	.byte 0x8d
	.byte 0x04
	.byte 0x04
	.4byte ..___tag_value__Z11popcountallPKmPjm.102-..___tag_value__Z11popcountallPKmPjm.101
	.byte 0x09
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value__Z11popcountallPKmPjm.103-..___tag_value__Z11popcountallPKmPjm.102
	.byte 0x09
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value__Z11popcountallPKmPjm.104-..___tag_value__Z11popcountallPKmPjm.103
	.byte 0x09
	.2byte 0x0d0d
	.2byte 0x0000
	.4byte 0x00000034
	.4byte 0x00000424
	.8byte ..___tag_value___sti__$E.106
	.8byte ..___tag_value___sti__$E.113-..___tag_value___sti__$E.106
	.byte 0x08
	.8byte 0x0000000000000000
	.byte 0x04
	.4byte ..___tag_value___sti__$E.107-..___tag_value___sti__$E.106
	.byte 0x0c
	.2byte 0x1007
	.byte 0x04
	.4byte ..___tag_value___sti__$E.112-..___tag_value___sti__$E.107
	.byte 0x0c
	.2byte 0x0807
	.4byte 0x00000000
	.2byte 0x0000
	.byte 0x00
# End
