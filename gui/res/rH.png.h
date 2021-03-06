#ifndef RH_PNG_H
#define RH_PNG_H

#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/bitmap.h>

static const unsigned char rH_png[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
	0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 
	0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 
	0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 
	0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00, 0x00, 0x67, 
	0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0x63, 0x60, 0x18, 0x05, 
	0xA3, 0x60, 0x14, 0x90, 0x06, 0xFE, 0x43, 0x31, 0xB9, 0xF2, 
	0x18, 0x80, 0x69, 0xA0, 0x7D, 0x34, 0x28, 0x1D, 0x00, 0x0B, 
	0x46, 0x66, 0x06, 0x06, 0x86, 0x56, 0x06, 0x06, 0x86, 0x0B, 
	0x0C, 0x0C, 0x0C, 0xBF, 0x07, 0x22, 0x9E, 0x27, 0x23, 0xB1, 
	0xFF, 0xD3, 0x2A, 0x0D, 0xE0, 0x33, 0xE4, 0x25, 0x03, 0x03, 
	0x43, 0x15, 0x03, 0x03, 0x83, 0x34, 0x16, 0x39, 0x42, 0x98, 
	0x2A, 0x0E, 0xA8, 0xC4, 0x23, 0x47, 0x17, 0x07, 0x68, 0xD2, 
	0x23, 0x1B, 0x92, 0x6A, 0xC8, 0x68, 0x39, 0x30, 0xEA, 0x80, 
	0x51, 0x30, 0x0A, 0x46, 0xC1, 0x28, 0x18, 0x05, 0xA3, 0x60, 
	0xF8, 0x01, 0x00, 0xD6, 0xBD, 0x37, 0xE2, 0xF7, 0xBF, 0xEA, 
	0x9E, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 
	0x42, 0x60, 0x82, 
};

wxBitmap& rH_png_to_wx_bitmap()
{
	static wxMemoryInputStream memIStream( rH_png, sizeof( rH_png ) );
	static wxImage image( memIStream, wxBITMAP_TYPE_PNG );
	static wxBitmap bmp( image );
	return bmp;
};


#endif //RH_PNG_H
