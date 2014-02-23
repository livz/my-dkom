#ifndef OFFSETS_H_INCLUDED
#define OFFSETS_H_INCLUDED

//typedef     char*   PEPROCESS;    /* Pointer to opaque EPROCESS blob */

// EPROCESS offsets for Windows NT 4.0 (UNVERIFIED)
/*  ULONG       */ #define WNT4_OFFSET_UniqueProcessId      148        // +0x094
/*  LIST_ENTRY  */ #define WNT4_OFFSET_ActiveProcessLinks   152        // +0x098
 
// EPROCESS offsets for Windows 2000 (UNVERIFIED)
/*  ULONG       */ #define W2K_OFFSET_UniqueProcessId      156         // +0x09C
/*  LIST_ENTRY  */ #define W2K_OFFSET_ActiveProcessLinks   160         // +0x0A0
 
// EPROCESS offsets for Windows Server 2003 (UNVERIFIED)
/*  ULONG       */ #define W2K3_OFFSET_UniqueProcessId      132        // +0x084 
/*  LIST_ENTRY  */ #define W2K3_OFFSET_ActiveProcessLinks   136        // +0x088 
 
// EPROCESS offsets for Windows XP build 2600 (5.1.2600)
// Obtained from undocumented.h
/*  ULONG       */ #define WXP_OFFSET_UniqueProcessId      132         // +0x084 
/*  LIST_ENTRY  */ #define WXP_OFFSET_ActiveProcessLinks   136         // +0x088
/*  UCHAR[16]   */ #define WXP_OFFSET_ImageFileName        372         // +0x174 

// EPROCESS offsets for Windows 7 build 7601 (6.1.7601)
/*  ULONG       */ #define W7_OFFSET_UniqueProcessId      180          // +0x0B4  
/*  LIST_ENTRY  */ #define W7_OFFSET_ActiveProcessLinks   184          // +0x0B8
/*  UCHAR[16]   */ #define W7_OFFSET_ImageFileName        364          // +0x16C  

#endif //OFFSETS_H_INCLUDED