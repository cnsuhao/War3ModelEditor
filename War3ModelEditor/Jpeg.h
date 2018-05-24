//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_JPEG_H
#define MAGOS_JPEG_H


//+-----------------------------------------------------------------------------
//| Prevents stupid redefinitions
//+-----------------------------------------------------------------------------
#define XMD_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Buffer.h"


//+-----------------------------------------------------------------------------
//| Included JPEG files (requires C inclusion)
//+-----------------------------------------------------------------------------
extern "C"
{
#include ".\jpeglib.h"
}


//+-----------------------------------------------------------------------------
//| Source manager structure
//+-----------------------------------------------------------------------------
struct JPEG_SOURCE_MANAGER
{
	JPEG_SOURCE_MANAGER()
	{
		SourceBuffer = NULL;
		SourceBufferSize = 0;
		Buffer = NULL;
	}

	jpeg_source_mgr Manager;
	UCHAR* SourceBuffer;
	LONG SourceBufferSize;
	JOCTET* Buffer;
};


//+-----------------------------------------------------------------------------
//| Destination manager structure
//+-----------------------------------------------------------------------------
struct JPEG_DESTINATION_MANAGER
{
	JPEG_DESTINATION_MANAGER()
	{
		DestinationBuffer = NULL;
		DestinationBufferSize = 0;
		Buffer = NULL;
	}

	jpeg_destination_mgr Manager;
	UCHAR* DestinationBuffer;
	LONG DestinationBufferSize;
	JOCTET* Buffer;
};


//+-----------------------------------------------------------------------------
//| Jpeg class
//+-----------------------------------------------------------------------------
class JPEG
{
	public:
		CONSTRUCTOR JPEG();
		DESTRUCTOR ~JPEG();

		BOOL Write(CONST BUFFER& SourceBuffer, BUFFER& TargetBuffer, INT Width, INT Height, INT Quality);
		BOOL Read(CONST BUFFER& SourceBuffer, BUFFER& TargetBuffer, INT* Width = NULL, INT* Height = NULL);

	protected:
		static VOID SetMemorySource(jpeg_decompress_struct* Info, UCHAR* Buffer, ULONG Size);
		static VOID SetMemoryDestination(jpeg_compress_struct* Info, UCHAR* Buffer, ULONG Size);

		static VOID SourceInit(jpeg_decompress_struct* Info);
		static BOOLEAN SourceFill(jpeg_decompress_struct* Info);
		static VOID SourceSkip(jpeg_decompress_struct* Info, LONG NrOfBytes);
		static VOID SourceTerminate(jpeg_decompress_struct* Info);

		static VOID DestinationInit(jpeg_compress_struct* Info);
		static BOOLEAN DestinationEmpty(jpeg_compress_struct* Info);
		static VOID DestinationTerminate(jpeg_compress_struct* Info);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern JPEG Jpeg;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
