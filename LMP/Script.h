// Script.h: interface for the CScript class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCRIPT_H__
#define _SCRIPT_H__

#pragma once

class CScript  
{
public:
	void	SetScriptFileName( char* szScriptFileName );	// Script File Name Setting

	void	GetString( char* szSectionName, char* szKeyName, char* szString );					// Get String
	void	GetString( char* szSectionName, char* szKeyName, char* szString, long nReadCount );	// Get String
	void	GetInt( char* szSectionName, char* szKeyName, long& iValue );						// Get Integer
	void	GetFloat( char* szSectionName, char *szKeyName, float& fData);
	
	void    SetInt( char* szSectionName, char* szKeyName, long nVal );
	void	SetFloat( char* szSectionName, char *szKeyName, float fData );
	void	SetString( char* szSectionName, char* szKeyName, char* szString);
	void	SetString( char* szSectionName, char* szKeyName, char* szString, long nWriteCount);

protected:
	char	m_szScriptFileName[ 255 ];	// Script( INI ) File Name
};

#endif // _SCRIPT_H__
