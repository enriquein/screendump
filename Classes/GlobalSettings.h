#pragma once
#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

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
	BOOL bEnableHog;
	long lJpgQuality;
	void ReadSettings();
	void WriteSettings();
	CString szOutputDir;

protected:
	CString m_IniPath;
};
#endif