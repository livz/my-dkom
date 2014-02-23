#include "MJCreate.h"

NTSTATUS
TestDriverCreate(
	PDEVICE_OBJECT pDeviceObject, 
	PIRP pIrp
)
{
	NTSTATUS status = STATUS_SUCCESS;
	
	//we need this dummy function just to respond to CreateFile request
	//for processes opening a handle to our device	
	return(status);
}