#pragma once

enum selEncoder 
{
	sEncBMP,
	sEncJPEG,
	sEncPNG,
	sEncTIFF
};

class CGlobalSettings
{
public:
	CGlobalSettings(void);
	~CGlobalSettings(void);
	selEncoder sEnc;
	bool bAutoName;
	CString szOutputDir;
	long lJpgQuality;
	ReadSettings();
	WriteSettings();
};
