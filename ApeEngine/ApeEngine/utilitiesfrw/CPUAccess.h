#ifndef _CPU_ACCESS_H
#define _CPU_ACCESS_H

// CPU Access enumerations for textures and buffers.
enum class CPUAccess
{
	None = 0,			 // No CPU access.
	Read = (1 << 0),	 // CPU reads permitted.
	Write = (1 << 1),	 // CPU writes permitted.
	ReadWrite = Read | Write
};

#endif