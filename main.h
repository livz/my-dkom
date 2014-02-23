#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "stdhead.h"
#include "common.h"

void AttachMJFunctions(IN PDRIVER_OBJECT pDriverObject);

const WCHAR kernLink[] =  L"\\Device\\ProcHider";
const WCHAR userLink[]  = L"\\DosDevices\\ProcHider";

#endif //MAIN_H_INCLUDED