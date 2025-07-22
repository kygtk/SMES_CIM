// Script.cpp: implementation of the CScript class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include	<io.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<Windows.h>
#include	"Script.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CScript::SetScriptFileName( char* szScriptFileName )
{
	strcpy( m_szScriptFileName, szScriptFileName );
}

void CScript::GetString( char* szSectionName, char* szKeyName, char* szString )
{
	GetPrivateProfileString(
		szSectionName,
		szKeyName,
		0,
		szString,
		254,
		m_szScriptFileName
	);
}

void CScript::GetString( char* szSectionName, char* szKeyName, char* szString, long nReadCount )
{
	GetPrivateProfileString(
		szSectionName,
		szKeyName,
		0,
		szString,
		nReadCount,
		m_szScriptFileName
	);
}

void CScript::GetInt( char* szSectionName, char* szKeyName, long& iValue )
{
	iValue = GetPrivateProfileInt(
		szSectionName,
		szKeyName,
		0,
		m_szScriptFileName
	);
}

void CScript::GetFloat( char* szSectionName, char *szKeyName, float &fData )
{
	char	szString[255];

	GetPrivateProfileString(
		szSectionName,
		szKeyName,
		0,
		szString,
		254,
		m_szScriptFileName
	);

	fData = (float)atof(szString);
}

void CScript::SetInt( char* szSectionName, char* szKeyName, long nVal )
{
	char	szValue[16];
	sprintf(szValue,"%d",nVal);

	WritePrivateProfileString( szSectionName, szKeyName, szValue, m_szScriptFileName );
}

void CScript::SetFloat(char* szSectionName, char *szKeyName, float fData)
{
	char	szData[255];
	memset(szData, 0x00, sizeof(szData));
	
	sprintf(szData,"%6.1f", fData);
	WritePrivateProfileString( szSectionName, szKeyName, szData, m_szScriptFileName );
}

void CScript::SetString( char* szSectionName, char* szKeyName, char* szString)
{
	char	szData[255];
	memset(szData, 0x00, sizeof(szData));

	long nLen = strlen(szString);

	if ( nLen > 255 ) 
		nLen = 255;

	for( long i = 0; i < nLen; i++ )
	{
		if ( szString[i] == 0x2E                           ||  // 소숫점(마침표)     "."
			 szString[i] == 0x5F                           ||  // Under Bar          "_"
			 szString[i] == 0x2D                           ||  // Minus              "-"
			(szString[i] >= 0x61 && szString[i] <= 0x7A)   ||  // 영문소문자         "a~z"
			(szString[i] >= 0x41 && szString[i] <= 0x5A)   ||  // 영문대문자         "A~Z"
			(szString[i] >= 0x30 && szString[i] <= 0x39) )     // 숫자               "0~9"  				
			szData[i] = szString[i];
		else continue;
	}
	WritePrivateProfileString( szSectionName, szKeyName, szData, m_szScriptFileName );
}

void CScript::SetString( char* szSectionName, char* szKeyName, char* szString, long nWriteCount )
{
	char	szData[255];
	memset(szData, 0x00, sizeof(szData));

	long nLen = strlen(szString);

	if ( nLen > 255 ) 
		nLen = 255;

	for( long i = 0; i < nLen; i++ )
	{
		if(i < nWriteCount)
		{
			if ( szString[i] == 0x2E                           ||  // 소숫점(마침표)     "."
				 szString[i] == 0x5F                           ||  // Under Bar          "_"
				 szString[i] == 0x2D                           ||  // Minus              "-"
				(szString[i] >= 0x61 && szString[i] <= 0x7A)   ||  // 영문소문자         "a~z"
				(szString[i] >= 0x41 && szString[i] <= 0x5A)   ||  // 영문대문자         "A~Z"
				(szString[i] >= 0x30 && szString[i] <= 0x39) )     // 숫자               "0~9"  				
				szData[i] = szString[i];
			else continue;
		}
	}
	WritePrivateProfileString( szSectionName, szKeyName, szData, m_szScriptFileName );
}
