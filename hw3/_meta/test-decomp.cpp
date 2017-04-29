__int64 __usercall encodeencode@<rax>(char a1)
{
  char *v1; // r8@1
  int v2; // ecx@1
  __int64 result; // rax@1
  int v4; // edi@2
  char *v5; // rsi@2
  __int64 v6; // rdx@3
  __int64 v7; // rdx@3

  v1 = &a1;
  v2 = 0;
  result = 0LL;
  do
  {
    v4 = v2 + 16;
    v5 = v1;
    do
    {
      v6 = *(_DWORD *)v5;
      v5 += 4;
      v7 = v6 << v2;
      v2 += 4;
      result |= v7;
    }
    while ( v2 != v4 );
    v1 += 16;
  }
  while ( v2 != 64 );
  return result;
}

__int64 __fastcall decodedecode(__int64 a1, unsigned __int64 a2)
{
  __int64 result; // rax@1
  __int64 v3; // r10@1
  int v4; // ecx@1
  int v5; // er9@2
  __int64 v6; // r8@2
  unsigned __int64 v7; // rdx@3

  result = a1;
  v3 = a1;
  v4 = 0;
  do
  {
    v5 = v4 + 16;
    v6 = v3;
    do
    {
      v6 += 4LL;
      v7 = a2 >> v4;
      v4 += 4;
      *(_DWORD *)(v6 - 4) = v7 & 0xF;
    }
    while ( v4 != v5 );
    v3 += 16LL;
  }
  while ( v4 != 64 );
  return result;
}

void __fastcall processprocess(__int64 a1)
{
  __int64 v1; // rbx@1
  __int64 v2; // rbp@3
  char v3; // dl@4
  int v4; // eax@6

  v1 = *(_QWORD *)a1;
  if ( a1 != *(_QWORD *)a1 )
  {
    while ( 1 )
    {
      v2 = *(_QWORD *)v1;
      if ( a1 == *(_QWORD *)v1 )
        break;
      v3 = 0;
      do
      {
        while ( 1 )
        {
          v4 = *(_DWORD *)(v1 + 16);
          if ( v4 == *(_DWORD *)(v2 + 16) )
            break;
          v1 = *(_QWORD *)v1;
          v2 = *(_QWORD *)v1;
          if ( a1 == *(_QWORD *)v1 )
            goto LABEL_8;
        }
        *(_DWORD *)(v1 + 16) = v4 + 1;
        --*(_QWORD *)(a1 + 16);
        std::__detail::_List_node_base::_M_unhook((std::__detail::_List_node_base *)v2);
        operator delete((void *)v2);
        v3 = 1;
        v1 = **(_QWORD **)(v1 + 8);
        v2 = *(_QWORD *)v1;
      }
      while ( a1 != *(_QWORD *)v1 );
LABEL_8:
      if ( !v3 )
        break;
      v1 = *(_QWORD *)a1;
    }
  }
}

unsigned __int64 __fastcall nextMap(unsigned __int64 a1, signed int a2)
{
  unsigned __int64 v2; // rbp@1
  __int64 *v3; // r12@1
  int v4; // esi@1
  __int64 *v5; // rdi@1
  int v6; // ecx@2
  __int64 *v7; // rdx@2
  unsigned __int64 v8; // rax@3
  __int64 *v9; // r14@9
  __int64 *v10; // r13@10
  int v11; // er15@11
  void *v12; // rdi@13
  __int64 *v13; // rdx@13
  void *v14; // rax@13
  int v15; // ecx@14
  void **v16; // r13@15
  signed int v17; // edi@17
  __int64 v18; // r8@17
  __int64 v19; // rax@17
  int v20; // ecx@18
  char *v21; // rsi@18
  __int64 v22; // rdx@19
  __int64 v23; // rdx@19
  unsigned int v24; // esi@22
  __int64 *v25; // rcx@22
  __int64 v26; // rdx@23
  int v27; // edi@26
  int v28; // ecx@26
  __int64 v29; // r8@26
  unsigned __int64 v30; // rdx@26
  __int64 v31; // rax@27
  __int64 *v32; // r14@34
  __int64 v33; // r13@35
  int v34; // er15@36
  void *v35; // rdi@38
  __int64 *v36; // rdx@38
  void *v37; // rax@38
  int v38; // ecx@39
  void **v39; // r13@40
  __int64 v40; // rax@43
  __int64 v41; // rax@44
  __int64 *v42; // r14@46
  __int64 v43; // r13@47
  int v44; // er15@48
  void *v45; // rdi@50
  __int64 *v46; // rdx@50
  void *v47; // rax@50
  int v48; // ecx@51
  void **v49; // r13@52
  __int64 v50; // rax@55
  __int64 *v51; // r14@56
  __int64 *v52; // r13@57
  int v53; // er15@58
  void *v54; // rdi@60
  __int64 *v55; // rdx@60
  void *v56; // rax@60
  int v57; // ecx@61
  void **v58; // r13@62
  __int64 v59; // rax@65
  unsigned __int64 result; // rax@67
  signed __int64 v61; // rdx@69
  int v62; // edi@69
  int v63; // eax@69
  __int64 v64; // r8@69
  int v65; // ecx@70
  char *v66; // rsi@70
  __int64 v67; // rdx@71
  __int64 v68; // rdx@71
  signed int v69; // [sp+14h] [bp-134h]@1
  void *v70; // [sp+20h] [bp-128h]@10
  void **v71; // [sp+28h] [bp-120h]@10
  __int64 v72; // [sp+30h] [bp-118h]@10
  __int64 v73; // [sp+40h] [bp-108h]@1
  __int64 v74; // [sp+48h] [bp-100h]@17
  __int64 v75; // [sp+50h] [bp-F8h]@16
  __int64 v76; // [sp+58h] [bp-F0h]@17
  __int64 v77; // [sp+60h] [bp-E8h]@17
  __int64 v78; // [sp+68h] [bp-E0h]@17
  __int64 v79; // [sp+70h] [bp-D8h]@17
  __int64 v80; // [sp+78h] [bp-D0h]@17
  __int64 v81; // [sp+80h] [bp-C8h]@17
  __int64 v82; // [sp+88h] [bp-C0h]@17
  __int64 v83; // [sp+90h] [bp-B8h]@17
  __int64 v84; // [sp+98h] [bp-B0h]@17
  __int64 v85; // [sp+A0h] [bp-A8h]@17
  __int64 v86; // [sp+A8h] [bp-A0h]@17
  __int64 v87; // [sp+B0h] [bp-98h]@17
  __int64 v88; // [sp+B8h] [bp-90h]@17
  __int64 v89; // [sp+C0h] [bp-88h]@69
  __int64 v90; // [sp+C8h] [bp-80h]@69
  __int64 v91; // [sp+D0h] [bp-78h]@69
  __int64 v92; // [sp+D8h] [bp-70h]@69
  __int64 v93; // [sp+E0h] [bp-68h]@69
  __int64 v94; // [sp+E8h] [bp-60h]@69
  __int64 v95; // [sp+F0h] [bp-58h]@69
  __int64 v96; // [sp+F8h] [bp-50h]@69
  __int64 v97; // [sp+108h] [bp-40h]@1

  v2 = a1;
  v3 = &v73;
  v69 = a2;
  v4 = 16;
  v97 = *MK_FP(__FS__, 40LL);
  v5 = &v73;
  do
  {
    v6 = v4 - 16;
    v7 = v5;
    do
    {
      v7 = (__int64 *)((char *)v7 + 4);
      v8 = v2 >> v6;
      v6 += 4;
      *((_DWORD *)v7 - 1) = v8 & 0xF;
    }
    while ( v6 != v4 );
    v4 = v6 + 16;
    v5 += 2;
  }
  while ( v6 != 64 );
  if ( v69 == 1 )
  {
    v51 = &v73;
    do
    {
      v72 = 0LL;
      v70 = &v70;
      v71 = &v70;
      v52 = v51;
      do
      {
        v53 = *((_DWORD *)v52 + 12);
        if ( v53 )
        {
          v59 = operator new(0x18uLL);
          *(_QWORD *)v59 = 0LL;
          *(_QWORD *)(v59 + 8) = 0LL;
          *(_DWORD *)(v59 + 16) = v53;
          std::__detail::_List_node_base::_M_hook(
            (std::__detail::_List_node_base *)v59,
            (std::__detail::_List_node_base *)&v70);
          ++v72;
        }
        v52 -= 2;
      }
      while ( v52 != v51 - 8 );
      processprocess((__int64)&v70);
      v54 = v70;
      *((_DWORD *)v51 + 12) = 0;
      v55 = v51;
      *((_DWORD *)v51 + 8) = 0;
      *((_DWORD *)v51 + 4) = 0;
      *(_DWORD *)v51 = 0;
      v56 = v54;
      if ( v54 != &v70 )
      {
        do
        {
          v57 = *((_DWORD *)v56 + 4);
          v56 = *(void **)v56;
          v55 -= 2;
          *((_DWORD *)v55 + 16) = v57;
        }
        while ( v56 != &v70 );
        do
        {
          v58 = *(void ***)v54;
          operator delete(v54);
          v54 = v58;
        }
        while ( v58 != &v70 );
      }
      v51 = (__int64 *)((char *)v51 + 4);
    }
    while ( &v75 != v51 );
    goto LABEL_17;
  }
  if ( v69 <= 1 )
  {
    if ( !v69 )
    {
      v42 = &v73;
      do
      {
        v72 = 0LL;
        v70 = &v70;
        v43 = 0LL;
        v71 = &v70;
        do
        {
          v44 = *(_DWORD *)((char *)v42 + v43 + 12);
          if ( v44 )
          {
            v50 = operator new(0x18uLL);
            *(_QWORD *)v50 = 0LL;
            *(_QWORD *)(v50 + 8) = 0LL;
            *(_DWORD *)(v50 + 16) = v44;
            std::__detail::_List_node_base::_M_hook(
              (std::__detail::_List_node_base *)v50,
              (std::__detail::_List_node_base *)&v70);
            ++v72;
          }
          v43 -= 4LL;
        }
        while ( v43 != -16 );
        processprocess((__int64)&v70);
        v45 = v70;
        *((_DWORD *)v42 + 3) = 0;
        v46 = v42;
        *((_DWORD *)v42 + 2) = 0;
        *((_DWORD *)v42 + 1) = 0;
        *(_DWORD *)v42 = 0;
        v47 = v45;
        if ( v45 != &v70 )
        {
          do
          {
            v48 = *((_DWORD *)v47 + 4);
            v47 = *(void **)v47;
            v46 = (__int64 *)((char *)v46 - 4);
            *((_DWORD *)v46 + 4) = v48;
          }
          while ( v47 != &v70 );
          do
          {
            v49 = *(void ***)v45;
            operator delete(v45);
            v45 = v49;
          }
          while ( v49 != &v70 );
        }
        v42 += 2;
      }
      while ( &v81 != v42 );
      goto LABEL_17;
    }
  }
  else
  {
    if ( v69 == 2 )
    {
      v32 = &v73;
      do
      {
        v72 = 0LL;
        v70 = &v70;
        v33 = 0LL;
        v71 = &v70;
        do
        {
          v34 = *(_DWORD *)((char *)v32 + v33);
          if ( v34 )
          {
            v40 = operator new(0x18uLL);
            *(_QWORD *)v40 = 0LL;
            *(_QWORD *)(v40 + 8) = 0LL;
            *(_DWORD *)(v40 + 16) = v34;
            std::__detail::_List_node_base::_M_hook(
              (std::__detail::_List_node_base *)v40,
              (std::__detail::_List_node_base *)&v70);
            ++v72;
          }
          v33 += 4LL;
        }
        while ( v33 != 16 );
        processprocess((__int64)&v70);
        v35 = v70;
        *(_DWORD *)v32 = 0;
        v36 = v32;
        *((_DWORD *)v32 + 1) = 0;
        *((_DWORD *)v32 + 2) = 0;
        *((_DWORD *)v32 + 3) = 0;
        v37 = v35;
        if ( v35 != &v70 )
        {
          do
          {
            v38 = *((_DWORD *)v37 + 4);
            v37 = *(void **)v37;
            v36 = (__int64 *)((char *)v36 + 4);
            *((_DWORD *)v36 - 1) = v38;
          }
          while ( v37 != &v70 );
          do
          {
            v39 = *(void ***)v35;
            operator delete(v35);
            v35 = v39;
          }
          while ( v39 != &v70 );
        }
        v32 += 2;
      }
      while ( &v81 != v32 );
      goto LABEL_17;
    }
    if ( v69 == 3 )
    {
      v9 = &v73;
      do
      {
        v72 = 0LL;
        v70 = &v70;
        v71 = &v70;
        v10 = v9;
        do
        {
          v11 = *(_DWORD *)v10;
          if ( *(_DWORD *)v10 )
          {
            v41 = operator new(0x18uLL);
            *(_QWORD *)v41 = 0LL;
            *(_QWORD *)(v41 + 8) = 0LL;
            *(_DWORD *)(v41 + 16) = v11;
            std::__detail::_List_node_base::_M_hook(
              (std::__detail::_List_node_base *)v41,
              (std::__detail::_List_node_base *)&v70);
            ++v72;
          }
          v10 += 2;
        }
        while ( v10 != v9 + 8 );
        processprocess((__int64)&v70);
        v12 = v70;
        *(_DWORD *)v9 = 0;
        v13 = v9;
        *((_DWORD *)v9 + 4) = 0;
        *((_DWORD *)v9 + 8) = 0;
        *((_DWORD *)v9 + 12) = 0;
        v14 = v12;
        if ( v12 != &v70 )
        {
          do
          {
            v15 = *((_DWORD *)v14 + 4);
            v14 = *(void **)v14;
            v13 += 2;
            *((_DWORD *)v13 - 4) = v15;
          }
          while ( v14 != &v70 );
          do
          {
            v16 = *(void ***)v12;
            operator delete(v12);
            v12 = v16;
          }
          while ( v16 != &v70 );
        }
        v9 = (__int64 *)((char *)v9 + 4);
      }
      while ( &v75 != v9 );
      goto LABEL_17;
    }
  }
  _fprintf_chk(*(&stderr + 0x40000000), 1LL, 2101LL);
LABEL_17:
  v17 = 16;
  v18 = 0LL;
  v81 = v73;
  v82 = v74;
  v83 = v75;
  v84 = v76;
  v85 = v77;
  v86 = v78;
  v87 = v79;
  v88 = v80;
  v19 = 0LL;
  do
  {
    v20 = v18 * 16;
    v21 = (char *)&v81 + v18 * 16;
    do
    {
      v22 = *(_DWORD *)v21;
      v21 += 4;
      v23 = v22 << v20;
      v20 += 4;
      v19 |= v23;
    }
    while ( v17 != v20 );
    ++v18;
    v17 += 16;
  }
  while ( v18 != 4 );
  if ( v2 == v19 )
    return v2;
  v24 = 0;
  v25 = &v73;
  do
  {
    v26 = 0LL;
    do
    {
      v24 += *(_DWORD *)((char *)v25 + v26) < 1u;
      v26 += 4LL;
    }
    while ( v26 != 16 );
    v25 += 2;
  }
  while ( &v81 != v25 );
  v27 = 0;
  v28 = 0;
  v29 = v19 ^ (v19 << 5 * (unsigned __int8)v69);
  v30 = (v19 ^ (unsigned __int64)(v19 << 5 * (unsigned __int8)v69)) % (signed int)v24;
LABEL_27:
  v31 = 0LL;
  while ( *((_DWORD *)v3 + v31) )
  {
LABEL_31:
    ++v31;
    if ( v31 == 4 )
    {
      ++v27;
      v3 += 2;
      if ( v27 == 4 )
        _assert_fail("false", "lib.cpp", 0x7Au, "long long unsigned int nextMap(long long unsigned int, int)");
      goto LABEL_27;
    }
  }
  if ( (_DWORD)v30 != v28 )
  {
    ++v28;
    goto LABEL_31;
  }

  // here?
  v61 = (signed int)v31 + 4LL * v27;
  v62 = 16;
  v63 = -(v29 - 10 * ((unsigned __int64)((unsigned __int128)(0x0CCCCCCCCCCCCCCCDLL * v29) >> 64) >> 3) < 1);
  v64 = 0LL;
  *((_DWORD *)&v73 + v61) = ~v63 + 2;


  v89 = v73;
  v90 = v74;
  v91 = v75;
  v92 = v76;
  v93 = v77;
  v94 = v78;
  v95 = v79;
  v96 = v80;

  // encode to uint64_t
  result = 0LL;
  do
  {
    v65 = v64 * 16;
    v66 = (char *)&v89 + v64 * 16;
    do
    {
      v67 = *(_DWORD *)v66;
      v66 += 4;
      v68 = v67 << v65;
      v65 += 4;
      result |= v68;
    }
    while ( v65 != v62 );
    ++v64;
    v62 = v65 + 16;
  }
  while ( v64 != 4 );
  return result;

    }
