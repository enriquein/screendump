// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
//		File:		file_ver.inl
//					------------
//
//		Content:	Declaration of class FileVersionInfo inline funcs.
//					Included by file_ver.h.
//
//
//		Created:		01/24/2002
//						Thomas Weller
//
//		Last modified:	02/02/2002
//						Thomas Weller
//
//
// ----------------------------------------------------------------------------
//
//	Remarks
//	-------
//
//	See file_ver.h.
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


//*****************************************************************************
//*****************************************************************************
//
//					FileVersionInfo Attributes
//					---------------------------
//
//*****************************************************************************
//*****************************************************************************



// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::RefreshVersionInfo
//					------------------------------------
//
//					Read current file's version info resource again.
//
//  Access level:	public
//
//	Declaration:	BOOL RefreshVersionInfo()
//
//	Returns:		TRUE if successful, otherwise FALSE.
//
//	Remarks:
//	-------
//  See the ReadVersionInfo() member function.
//
// ----------------------------------------------------------------------------

// read current file's info block again
inline BOOL FileVersionInfo::RefreshVersionInfo()
{
	return (m_strFilename.IsEmpty()) ? FALSE :
									   ReadVersionInfo(m_strFilename);
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetFilename
//					-----------------------------
//
//					Retreive name of current file.
//
//  Access level:	public
//
//	Declaration:	const CString& GetFilename() const
//
//	Returns:		The name of the current file, if any.
//
// ----------------------------------------------------------------------------

// current file's name
inline const CString& FileVersionInfo::GetFilename() const
	{ return m_strFilename; }


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::IsValid
//					-------------------------
//
//					Check if any version info is currently available.
//
//  Access level:	public
//
//	Declaration:	BOOL IsValid() const
//
//	Returns:		TRUE if any version info is currently available.
//
// ----------------------------------------------------------------------------

// any version info currently available ?
inline BOOL FileVersionInfo::IsValid() const
	{ return (m_pVersionInfo != NULL); }


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetCharSet
//					----------------------------
//
//					Get character set this file is built for.
//
//  Access level:	public
//
//	Declaration:	WORD GetCharSet() const
//
//	Returns:		The code-page identifier. 0 indicates an error.
//
//	Remarks
//	-------
//	See "Windows Base Services/National Language support" in the Platform
//	SDK for information about code pages, languages and related stuff.
//
// ----------------------------------------------------------------------------

// get code-page identifier
inline WORD FileVersionInfo::GetCharSet() const
	{ return HIWORD(m_dwTransInfo); }


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetLanguageId
//					-------------------------------
//
//					Get language id this file is built for.
//
//  Access level:	public
//
//	Declaration:	WORD GetLanguageId() const
//
//	Returns:		The language identifier. 0 indicates an error.
//
//	Remarks
//	-------
//	See "Windows Base Services/National Language support" in the Platform
//	SDK for information about code pages, languages and related stuff
//	(especially "Language Identifiers").
//
// ----------------------------------------------------------------------------

// get language identifier
inline WORD FileVersionInfo::GetLanguageId() const
	{ return LOWORD(m_dwTransInfo); }


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetLanguageName
//					---------------------------------
//
//					Get this file's language as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetLanguageName() const
//
//	Returns:		The Language description or an empty CString.
//
//	Remarks
//	-------
//	The returned description will be localized (e.g. "Deutsch (Standard)" on
//	german Windows versions, but "German (Standard)" on english versions for
//	the same file).
//
// ----------------------------------------------------------------------------

// name of the file's language
inline CString FileVersionInfo::GetLanguageName() const
{ 
	CString strRes;
	LPTSTR	lpszRes = strRes.GetBuffer(_MAX_PATH);
	if (::GetLocaleInfo(MAKELCID(GetLanguageId(), SORT_DEFAULT),
						LOCALE_SLANGUAGE,
						lpszRes,
						_MAX_PATH
					   ) == 0
	   ) return CString(_T(""));
	strRes.ReleaseBuffer();
	return strRes;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetFileVersionString
//					--------------------------------------
//
//					Get the file version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetFileVersionString() const
//
//	Returns:		The file version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.#.#.#".
//
// ----------------------------------------------------------------------------

// file version as CString (both major and minor)
inline CString FileVersionInfo::GetFileVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d.%d.%d"), HIWORD (m_fixedFileInfo.dwFileVersionMS),
									 LOWORD (m_fixedFileInfo.dwFileVersionMS),
									 HIWORD (m_fixedFileInfo.dwFileVersionLS),
									 LOWORD (m_fixedFileInfo.dwFileVersionLS)
				 );
	return strRes;
}

// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetCustomFileVersionString
//					--------------------------------------
//
//					Get the file version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetFileVersionString() const
//
//	Returns:		The file version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.# (rev #)".
//  If it's flagged as a pre-release version, BETA will be appended after 
//  rev #.
// ----------------------------------------------------------------------------

// file version as CString (both major and minor)
inline CString FileVersionInfo::GetCustomFileVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d (rev %d"), HIWORD (m_fixedFileInfo.dwFileVersionMS),
									 LOWORD (m_fixedFileInfo.dwFileVersionMS),
									 LOWORD (m_fixedFileInfo.dwFileVersionLS)
				 );
	strRes += IsPreRelease() ? _T(" Beta)") : _T(")");
	return strRes;
}

// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetMajorFileVersionString
//					-------------------------------------------
//
//					Get the major file version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetMajorFileVersionString() const
//
//	Returns:		The major file version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.#".
//
// ----------------------------------------------------------------------------

// file version as CString (major)
inline CString FileVersionInfo::GetMajorFileVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d"), HIWORD (m_fixedFileInfo.dwFileVersionMS),
							   LOWORD (m_fixedFileInfo.dwFileVersionMS)
				 );
	return strRes;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetMinorFileVersionString
//					-------------------------------------------
//
//					Get the minor file version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetMinorFileVersionString() const
//
//	Returns:		The minor file version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.#".
//
// ----------------------------------------------------------------------------

// file version as CString (minor)
inline CString FileVersionInfo::GetMinorFileVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d"), HIWORD (m_fixedFileInfo.dwFileVersionLS),
							   LOWORD (m_fixedFileInfo.dwFileVersionLS)
				 );
	return strRes;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetMajorFileVersion
//					-------------------------------------
//
//					Get the major file version.
//
//  Access level:	public
//
//	Declaration:	DWORD GetMajorFileVersion() const
//
//	Returns:		The major file version or 0xFFFFFFFF if an error occured.
//
// ----------------------------------------------------------------------------

// file version as DWORD (major)
inline DWORD FileVersionInfo::GetMajorFileVersion() const
{ 
	return (IsValid()) ? m_fixedFileInfo.dwFileVersionMS :
						 0xFFFFFFFF;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetMinorFileVersion
//					-------------------------------------
//
//					Get the minor file version.
//
//  Access level:	public
//
//	Declaration:	DWORD GetMinorFileVersion() const
//
//	Returns:		The minor file version or 0xFFFFFFFF if an error occured.
//
// ----------------------------------------------------------------------------

// file version as DWORD (minor)
inline DWORD FileVersionInfo::GetMinorFileVersion() const
{ 
	return (IsValid()) ? m_fixedFileInfo.dwFileVersionLS :
						 0xFFFFFFFF;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetProductVersionString
//					-----------------------------------------
//
//					Get the product version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetFileVersionString() const
//
//	Returns:		The product version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.#.#.#".
//
//	The term "product" refers to the software product, the current file
//	is shipped with (e.g. the Windows version for a file like KERNEL.dll).
//	Can be identical with the file version, if the file is the product (or 
//	the software developer was not too accurate).
//
// ----------------------------------------------------------------------------

// product version as CString (both major and minor)
inline CString FileVersionInfo::GetProductVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d.%d.%d"), 
					HIWORD (m_fixedFileInfo.dwProductVersionMS),
					LOWORD (m_fixedFileInfo.dwProductVersionMS),
					HIWORD (m_fixedFileInfo.dwProductVersionLS),
					LOWORD (m_fixedFileInfo.dwProductVersionLS)
				 );
	return strRes;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetMajorProductVersionString
//					----------------------------------------------
//
//					Get the major product version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetMajorProductVersionString() const
//
//	Returns:		The major product version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.#".
//  See also the remarks section of the GetProductVersionString() member
//	function.
//
// ----------------------------------------------------------------------------

// product version as CString (major)
inline CString FileVersionInfo::GetMajorProductVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d"), HIWORD (m_fixedFileInfo.dwProductVersionMS),
							   LOWORD (m_fixedFileInfo.dwProductVersionMS)
				 );
	return strRes;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetMinorProductVersionString
//					----------------------------------------------
//
//					Get the minor product version as CString.
//
//  Access level:	public
//
//	Declaration:	CString GetMinorProductVersionString() const
//
//	Returns:		The minor product version or an empty CString.
//
//	Remarks
//	-------
//	The returned string has the format "#.#".
//  See also the remarks section of the GetProductVersionString() member
//	function.
//
// ----------------------------------------------------------------------------

// product version as CString (minor)
inline CString FileVersionInfo::GetMinorProductVersionString() const
{ 
	if (!m_pVersionInfo) return CString(_T(""));

	CString strRes;
	strRes.Format(_T("%d.%d"), HIWORD (m_fixedFileInfo.dwProductVersionLS),
							   LOWORD (m_fixedFileInfo.dwProductVersionLS)
				 );
	return strRes;
}

// product version as DWORD (major)
inline DWORD FileVersionInfo::GetMajorProductVersion() const
{ 
	return (IsValid()) ? m_fixedFileInfo.dwFileVersionMS :
						 0xFFFFFFFF;
}

// product version as DWORD (minor)
inline DWORD FileVersionInfo::GetMinorProductVersion() const
{ 
	return (IsValid()) ? m_fixedFileInfo.dwProductVersionLS : 
						 0xFFFFFFFF;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetFileDate
//					-----------------------------
//
//					Get the file's creation date and time.
//
//  Access level:	public
//
//	Declaration:	FILETIME GetFileDate() const
//
//	Returns:		The file's creation date and time if successful.
//					Otherwise the returned FILETIME struct will be {0, 0}.
//
// ----------------------------------------------------------------------------

// date and time stamp as FILETIME
inline FILETIME FileVersionInfo::GetFileDate() const
{
	FILETIME ftRes;
	ftRes.dwHighDateTime = m_fixedFileInfo.dwFileDateMS;
	ftRes.dwLowDateTime  = m_fixedFileInfo.dwFileDateLS;
	return ftRes;
}


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetTargetOs
//					-----------------------------
//
//					Get the operating system this file was built for.
//
//  Access level:	public
//
//	Declaration:	DWORD GetTargetOs() const
//
//	Returns:		target operating system identifier if successful,
//					otherwise 0.
//
//	Remarks
//	-------
//	See the documention for the VS_FIXEDFILEINFO struct (dwFileOS section) in
//	the Platform SDK documentation for a description of possible return values.
//
// ----------------------------------------------------------------------------

// this file's target operating system identifier (DWORD)
inline DWORD FileVersionInfo::GetTargetOs() const
	{ return m_fixedFileInfo.dwFileOS; }


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetFileType
//					-----------------------------
//
//					Get this file's type.
//
//  Access level:	public
//
//	Declaration:	DWORD GetFileType() const
//
//	Returns:		A file type identifier if successful, otherwise 0.
//
//	Remarks
//	-------
//	See the documention for the VS_FIXEDFILEINFO struct (dwFileType section) in
//	the Platform SDK documentation for a description of possible return values.
//
// ----------------------------------------------------------------------------

// this file's type (DWORD)
inline DWORD FileVersionInfo::GetFileType() const
	{ return m_fixedFileInfo.dwFileType; }


// ----------------------------------------------------------------------------
//
//	Function:		FileVersionInfo::GetFileSubtype
//					--------------------------------
//
//					Get this file's subtype.
//
//  Access level:	public
//
//	Declaration:	DWORD GetFileSubtype() const
//
//	Returns:		A file subtype identifier if successful, otherwise 0.
//
//	Remarks
//	-------
//	See the documention for the VS_FIXEDFILEINFO struct (dwFileSubype section)
//	in the Platform SDK documentation for a list of possible return values.
//	For many file types (e.g. "application" or "dll") the subtype
//	will not be defined (i.e. the notation "subtype" makes no sense).
// 
// ----------------------------------------------------------------------------

// this file's subtype (DWORD)
inline DWORD FileVersionInfo::GetFileSubtype() const
	{ return m_fixedFileInfo.dwFileSubtype; }

// debug version ?
inline BOOL FileVersionInfo::IsDebugVersion() const
	{ return ((IsValid() && IsFlagSet(VS_FF_DEBUG))); }

// has file been modified compared to the original of the same version ?
inline BOOL FileVersionInfo::IsPatched() const
	{ return ((IsValid() && IsFlagSet(VS_FF_PATCHED))); }

// development version ?
inline BOOL FileVersionInfo::IsPreRelease() const
	{ return ((IsValid() && IsFlagSet(VS_FF_PRERELEASE))); }

// private build (e.g. by someone else than the org. distributor) ?
inline BOOL FileVersionInfo::IsPrivateBuild() const
	{ return ((IsValid() && IsFlagSet(VS_FF_PRIVATEBUILD))); }

// special build compared to other files of same version number ?
inline BOOL FileVersionInfo::IsSpecialBuild() const
	{ return ((IsValid() && IsFlagSet(VS_FF_SPECIALBUILD))); }



//*****************************************************************************
//*****************************************************************************
//
//					FileVersionInfo Implementation
//					-------------------------------
//
//*****************************************************************************
//*****************************************************************************

// see if a specific flag is valid
inline BOOL FileVersionInfo::IsFlagSet(DWORD dwFlag) const
{ 
	return ((m_fixedFileInfo.dwFileFlagsMask & dwFlag) &&
			(m_fixedFileInfo.dwFileFlags & dwFlag)
		   ); 
}

