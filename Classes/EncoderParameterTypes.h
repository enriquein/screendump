#pragma once
enum selEncoder 
{
	sEncBMP,    // BMP (Bitmap): uncompressed format. Huge filesize.
	sEncJPEG,   // JPG: compressed format, uses lossy compression (adjustable).
	sEncPNG,    // PNG (Portable Network Graphics): compressed format, uses lossless compression (highest quality, dramatically smaller filesize than BMP)
	sEncTIFF    // TIFF (Tagged Image Format): Not actually implemented yet
};