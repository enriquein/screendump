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
	BOOL bAutoName;
	long lJpgQuality;
	void ReadSettings();
	void WriteSettings();
	char szOutputDir[_MAX_PATH];

protected:
	char m_IniPath[_MAX_PATH];
};
