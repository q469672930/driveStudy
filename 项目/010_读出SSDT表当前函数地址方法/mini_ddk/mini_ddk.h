#include <ntddk.h>
#define INITCODE code_seg("INIT") 
#define PAGECODE code_seg("PAGE") /*表示内存不足时，可以被置换到硬盘*/
#pragma INITCODE /*指的代码运行后 就从内存释放掉*/
NTSTATUS CreateMyDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;/*用来返回创建设备*/

	//创建设备名称
	UNICODE_STRING devName;
	UNICODE_STRING symLinkName; // 
	RtlInitUnicodeString(&devName,L"\\Device\\yjxDDK_Device");/*对devName初始化字串为 "\\Device\\yjxDDK_Device"*/

	//创建设备
	status = IoCreateDevice( pDriverObject,\
		0,\
		&devName,\
		FILE_DEVICE_UNKNOWN,\
		0, TRUE,\
		&pDevObj);
	if (!NT_SUCCESS(status))
	{
		if (status==STATUS_INSUFFICIENT_RESOURCES)
		{
			KdPrint(("资源不足 STATUS_INSUFFICIENT_RESOURCES"));
		}
		if (status==STATUS_OBJECT_NAME_EXISTS )
		{
			KdPrint(("指定对象名存在"));
		}
		if (status==STATUS_OBJECT_NAME_COLLISION)
		{
			KdPrint(("//对象名有冲突"));
		}
		KdPrint(("设备创建失败...++++++++"));
		return status;
	}
	KdPrint(("设备创建成功...++++++++"));

	pDevObj->Flags |= DO_BUFFERED_IO;
	//创建符号链接

	RtlInitUnicodeString(&symLinkName,L"\\??\\yjx888");
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) /*status等于0*/
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;
}

VOID DDK_Unload (IN PDRIVER_OBJECT pDriverObject); //前置说明 卸载例程
NTSTATUS ddk_DispatchRoutine_CONTROL(IN PDEVICE_OBJECT pDevobj,IN PIRP pIrp	);//派遣函数
typedef struct _ServiceDescriptorTable {
	PVOID ServiceTableBase; //System Service Dispatch Table 的基地址  
	PVOID ServiceCounterTable;
	//包含着 SSDT 中每个服务被调用次数的计数器。这个计数器一般由sysenter 更新。 
	unsigned int NumberOfServices;//由 ServiceTableBase 描述的服务的数目。  
	PVOID ParamTableBase; //包含每个系统服务参数字节数表的基地址-系统服务参数表 
}*PServiceDescriptorTable;  
extern PServiceDescriptorTable KeServiceDescriptorTable;