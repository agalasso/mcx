#ifndef CC_PNG_H
#define CC_PNG_H

#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/bitmap.h>

static const unsigned char cc_png[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
	0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 
	0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 
	0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 
	0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00, 0x00, 0xA4, 
	0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0xED, 0xD4, 0x3D, 0x0E, 
	0x01, 0x51, 0x14, 0x80, 0xD1, 0x83, 0x05, 0x68, 0xED, 0x41, 
	0xA5, 0xD3, 0x89, 0xE8, 0x2D, 0x41, 0x23, 0x96, 0x44, 0xA1, 
	0x56, 0x2B, 0x74, 0x34, 0x12, 0x16, 0x61, 0x0B, 0x12, 0x24, 
	0x1A, 0x5A, 0x9A, 0x29, 0x44, 0xFC, 0xCC, 0x1B, 0x12, 0x8A, 
	0x7B, 0x92, 0xD7, 0x7D, 0xB9, 0xB9, 0xC9, 0x7B, 0x33, 0x84, 
	0x10, 0xC2, 0x9F, 0xEA, 0x61, 0x89, 0x2D, 0x0E, 0x58, 0xA0, 
	0xF3, 0x41, 0x97, 0x64, 0x8A, 0xCB, 0x93, 0x53, 0xA4, 0x4B, 
	0xD2, 0xCF, 0x06, 0x9C, 0x30, 0x42, 0x1B, 0x55, 0xB4, 0x30, 
	0x2F, 0xD0, 0x25, 0x5B, 0x65, 0x83, 0x87, 0x5F, 0xEA, 0x92, 
	0xED, 0xB2, 0xC1, 0x8D, 0x2F, 0x75, 0x6F, 0x95, 0x7F, 0xFD, 
	0xDA, 0xEF, 0x17, 0xD8, 0xDC, 0xDC, 0xF1, 0x2B, 0x79, 0xBB, 
	0xB7, 0x2A, 0x0F, 0x16, 0xEA, 0xA2, 0x8E, 0x1A, 0xCE, 0x38, 
	0xA2, 0x89, 0x31, 0x26, 0x89, 0x5D, 0xB2, 0x12, 0x66, 0x39, 
	0x3E, 0xAF, 0xBC, 0x5D, 0xE1, 0x25, 0x06, 0x58, 0x63, 0x9F, 
	0x3D, 0xB8, 0xF9, 0x83, 0x1F, 0x4C, 0xDE, 0x2E, 0x84, 0x10, 
	0x42, 0xF8, 0x6F, 0x57, 0x64, 0x2B, 0x46, 0x26, 0xC8, 0x3D, 
	0xF2, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 
	0xAE, 0x42, 0x60, 0x82, 
};

wxBitmap& cc_png_to_wx_bitmap()
{
	static wxMemoryInputStream memIStream( cc_png, sizeof( cc_png ) );
	static wxImage image( memIStream, wxBITMAP_TYPE_PNG );
	static wxBitmap bmp( image );
	return bmp;
};


#endif //CC_PNG_H