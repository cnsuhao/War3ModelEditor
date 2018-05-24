//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Jpeg.h"




//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
JPEG Jpeg;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
JPEG::JPEG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
JPEG::~JPEG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Writes JPEG data
//+-----------------------------------------------------------------------------
BOOL JPEG::Write(CONST BUFFER& SourceBuffer, BUFFER& TargetBuffer, INT Width, INT Height, INT Quality)
{
	INT Stride;
	INT RealSize;
	INT DummySize;
	BUFFER TempBuffer;
	JSAMPROW Pointer[1];
	jpeg_compress_struct Info;
	jpeg_error_mgr ErrorManager;

	Info.err = jpeg_std_error(&ErrorManager);

	DummySize = ((Width * Height * 4) * 2) + 10000;
	TempBuffer.Resize(DummySize);

	jpeg_create_compress(&Info);

	SetMemoryDestination(&Info, reinterpret_cast<UCHAR*>(TempBuffer.GetData()), TempBuffer.GetSize());

	Info.image_width = Width;
	Info.image_height = Height;
	Info.input_components = 4;
	Info.in_color_space = JCS_UNKNOWN;

	jpeg_set_defaults(&Info);
	jpeg_set_quality(&Info, Quality, TRUE);
	jpeg_start_compress(&Info, TRUE);

	Stride = Width * 4;
	while(Info.next_scanline < Info.image_height)
	{
		Pointer[0] = reinterpret_cast<JSAMPROW>(&SourceBuffer[Info.next_scanline * Stride]);
		jpeg_write_scanlines(&Info, Pointer, 1);
	}

	jpeg_finish_compress(&Info);

	RealSize = DummySize - static_cast<INT>(Info.dest->free_in_buffer);
	TargetBuffer.Resize(RealSize);

	memcpy(&TargetBuffer[0], &TempBuffer[0], RealSize);

	jpeg_destroy_compress(&Info);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Reads JPEG data
//+-----------------------------------------------------------------------------
BOOL JPEG::Read(CONST BUFFER& SourceBuffer, BUFFER& TargetBuffer, INT* Width, INT* Height)
{
	INT i;
	INT Stride;
	INT Offset;
	CHAR Opaque;
	JSAMPARRAY Pointer;
	jpeg_decompress_struct Info;
	jpeg_error_mgr ErrorManager;

	Info.err = jpeg_std_error(&ErrorManager);

	jpeg_create_decompress(&Info);
	SetMemorySource(&Info, reinterpret_cast<UCHAR*>(SourceBuffer.GetData()), SourceBuffer.GetSize());
	jpeg_read_header(&Info, TRUE);
	jpeg_start_decompress(&Info);

	if((Info.output_components != 3) && (Info.output_components != 4))
	{
		Error.SetMessage("Nr of channels must be 3 or 4!");
		return FALSE;
	}

	TargetBuffer.Resize(Info.output_width * Info.output_height * 4);
	Stride = Info.output_width * Info.output_components;
	Offset = 0;

	Pointer = (*Info.mem->alloc_sarray)(reinterpret_cast<j_common_ptr>(&Info), JPOOL_IMAGE, Stride, 1);
	while(Info.output_scanline < Info.output_height)
	{
		jpeg_read_scanlines(&Info, Pointer, 1);
		memcpy(&TargetBuffer[Offset], Pointer[0], Stride);
		Offset += Stride;
	}

	jpeg_finish_decompress(&Info);

	(*reinterpret_cast<BYTE*>(&Opaque)) = 255;

	if(Info.output_components == 3)
	{
		for(i = (Info.output_width * Info.output_height - 1); i >= 0; i--)
		{
			TargetBuffer[(i * 4) + 3] = Opaque;
			TargetBuffer[(i * 4) + 2] = TargetBuffer[(i * 3) + 2];
			TargetBuffer[(i * 4) + 1] = TargetBuffer[(i * 3) + 1];
			TargetBuffer[(i * 4) + 0] = TargetBuffer[(i * 3) + 0];
		}
	}

	if(Width != NULL) (*Width) = Info.output_width;
	if(Height != NULL) (*Height) = Info.output_height;

	jpeg_destroy_decompress(&Info);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets the memory source
//+-----------------------------------------------------------------------------
VOID JPEG::SetMemorySource(jpeg_decompress_struct* Info, UCHAR* Buffer, ULONG Size)
{
	JPEG_SOURCE_MANAGER* SourceManager;

	Info->src = reinterpret_cast<jpeg_source_mgr*>((*Info->mem->alloc_small)(reinterpret_cast<j_common_ptr>(Info), JPOOL_PERMANENT, sizeof(JPEG_SOURCE_MANAGER)));
	SourceManager = reinterpret_cast<JPEG_SOURCE_MANAGER*>(Info->src);

	SourceManager->Buffer = reinterpret_cast<JOCTET*>((*Info->mem->alloc_small)(reinterpret_cast<j_common_ptr>(Info), JPOOL_PERMANENT, Size * sizeof(JOCTET)));
	SourceManager->SourceBuffer = Buffer;
	SourceManager->SourceBufferSize = Size;
	SourceManager->Manager.init_source = SourceInit;
	SourceManager->Manager.fill_input_buffer = SourceFill;
	SourceManager->Manager.skip_input_data = SourceSkip;
	SourceManager->Manager.resync_to_restart = jpeg_resync_to_restart;
	SourceManager->Manager.term_source = SourceTerminate;
	SourceManager->Manager.bytes_in_buffer = 0;
	SourceManager->Manager.next_input_byte = NULL;
}


//+-----------------------------------------------------------------------------
//| Sets the memory destination
//+-----------------------------------------------------------------------------
VOID JPEG::SetMemoryDestination(jpeg_compress_struct* Info, UCHAR* Buffer, ULONG Size)
{
	JPEG_DESTINATION_MANAGER* DestinationManager;

	Info->dest = reinterpret_cast<jpeg_destination_mgr*>((*Info->mem->alloc_small)(reinterpret_cast<j_common_ptr>(Info), JPOOL_PERMANENT, sizeof(JPEG_DESTINATION_MANAGER)));
	DestinationManager = reinterpret_cast<JPEG_DESTINATION_MANAGER*>(Info->dest);

	DestinationManager->Buffer = NULL;
	DestinationManager->DestinationBuffer = Buffer;
	DestinationManager->DestinationBufferSize = Size;
	DestinationManager->Manager.init_destination = DestinationInit;
	DestinationManager->Manager.empty_output_buffer = DestinationEmpty;
	DestinationManager->Manager.term_destination = DestinationTerminate;
}


//+-----------------------------------------------------------------------------
//| Initiates the memory source
//+-----------------------------------------------------------------------------
VOID JPEG::SourceInit(jpeg_decompress_struct* Info)
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Fills the memory source
//+-----------------------------------------------------------------------------
BOOLEAN JPEG::SourceFill(jpeg_decompress_struct* Info)
{
	JPEG_SOURCE_MANAGER* SourceManager;

	SourceManager = reinterpret_cast<JPEG_SOURCE_MANAGER*>(Info->src);

	SourceManager->Buffer = SourceManager->SourceBuffer;
	SourceManager->Manager.next_input_byte = SourceManager->Buffer;
	SourceManager->Manager.bytes_in_buffer = SourceManager->SourceBufferSize;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Skips the memory source
//+-----------------------------------------------------------------------------
VOID JPEG::SourceSkip(jpeg_decompress_struct* Info, LONG NrOfBytes)
{
	JPEG_SOURCE_MANAGER* SourceManager;

	SourceManager = reinterpret_cast<JPEG_SOURCE_MANAGER*>(Info->src);

	if(NrOfBytes > 0)
	{
		while(NrOfBytes > static_cast<LONG>(SourceManager->Manager.bytes_in_buffer))
		{
			NrOfBytes -= static_cast<LONG>(SourceManager->Manager.bytes_in_buffer);
			SourceFill(Info);
		}

		SourceManager->Manager.next_input_byte += NrOfBytes;
		SourceManager->Manager.bytes_in_buffer -= NrOfBytes;
	}
}


//+-----------------------------------------------------------------------------
//| Terminates the memory source
//+-----------------------------------------------------------------------------
VOID JPEG::SourceTerminate(jpeg_decompress_struct* Info)
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Initiates the memory destination
//+-----------------------------------------------------------------------------
VOID JPEG::DestinationInit(jpeg_compress_struct* Info)
{
	JPEG_DESTINATION_MANAGER* DestinationManager;

	DestinationManager = reinterpret_cast<JPEG_DESTINATION_MANAGER*>(Info->dest);

	DestinationManager->Buffer = DestinationManager->DestinationBuffer;
	DestinationManager->Manager.next_output_byte = DestinationManager->Buffer;
	DestinationManager->Manager.free_in_buffer = DestinationManager->DestinationBufferSize;
}


//+-----------------------------------------------------------------------------
//| Empties the memory destination
//+-----------------------------------------------------------------------------
BOOLEAN JPEG::DestinationEmpty(jpeg_compress_struct* Info)
{
	JPEG_DESTINATION_MANAGER* DestinationManager;

	DestinationManager = reinterpret_cast<JPEG_DESTINATION_MANAGER*>(Info->dest);

	DestinationManager->Manager.next_output_byte = DestinationManager->Buffer;
	DestinationManager->Manager.free_in_buffer = DestinationManager->DestinationBufferSize;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Terminates the memory destination
//+-----------------------------------------------------------------------------
VOID JPEG::DestinationTerminate(jpeg_compress_struct* Info)
{
	//Empty
}
