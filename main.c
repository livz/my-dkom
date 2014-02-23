#include "main.h"

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING uDeviceName, uUserspaceName;
	PDEVICE_OBJECT pDeviceObject;

	DbgPrint("TestDriver - DriverEntry() - Loaded Driver\n");
	
	//we initialize the names of the device and the user link name
	RtlInitUnicodeString(&uDeviceName, kernLink);
	RtlInitUnicodeString(&uUserspaceName, userLink);

	DbgPrint("TestDriver - DriverEntry() - Creating device object...");

	//we create the driver device
	status = IoCreateDevice(
		DriverObject,
		0,
		&uDeviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&pDeviceObject
		);

	if(!NT_SUCCESS(status))
	{
		DbgPrint("Failed\n");

		return(status);
	}

	DbgPrint("Success!\n");
	
	//and the user space name for letting the user space application communicate with our device
	status = IoCreateSymbolicLink(
		&uUserspaceName, 
		&uDeviceName
		);

	DbgPrint("TestDriver - DriverEntry() - Creating userspace sym link...");

	if(!NT_SUCCESS(status))
	{
		DbgPrint("Failed\n");

		IoDeleteDevice(pDeviceObject);

		return(status);
	}

	DbgPrint("Success!\n");
	
	//we attach to the needed MJ functions
	AttachMJFunctions(DriverObject);

	return(status);
}

void
TestDriverUnload(
	PDRIVER_OBJECT pDriverObject
	)
{
	UNICODE_STRING uUserspaceName;
	PDEVICE_OBJECT pActualDeviceObject;
	PDEVICE_OBJECT pNextDeviceObject;

	DbgPrint("TestDriver - TestDriverUnload() - Unloading\n");

	RtlInitUnicodeString(&uUserspaceName, userLink);

	IoDeleteSymbolicLink(&uUserspaceName);

	pActualDeviceObject = pDriverObject->DeviceObject;
	
	//we cycle the device objects list (just for showing, as we have only one)
	do {
		pNextDeviceObject = pActualDeviceObject->NextDevice;

		DbgPrint("TestDriver - TestDriverUnload() - Deleting device 0x%p\n",
			pActualDeviceObject
			);

		IoDeleteDevice(pActualDeviceObject);

		pActualDeviceObject = pNextDeviceObject;

	} while(pActualDeviceObject != NULL);
}

void 
AttachMJFunctions(
	IN PDRIVER_OBJECT pDriverObject
	)
{
	//we hook the MJ function we need
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = TestDriverDeviceControl; //handling IOCTLs
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = TestDriverCreate; //handling CreateFile

	pDriverObject->DriverUnload = TestDriverUnload;
}