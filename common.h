#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define IOCTL_TESTDRIVER_HIDE_PROCESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_NEITHER, FILE_ANY_ACCESS)

DRIVER_INITIALIZE DriverEntry;

__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
	DRIVER_DISPATCH 
	TestDriverDeviceControl;

__drv_dispatchType(IRP_MJ_CREATE)
	DRIVER_DISPATCH
	TestDriverCreate;

DRIVER_UNLOAD TestDriverUnload;

#endif //COMMON_H_INCLUDED