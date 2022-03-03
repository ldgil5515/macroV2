#include "stdafx.h"
#include "Ini.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#pragma warning(disable:4996)

CIni::CIni(const char *macroFileName)
{
	_appName = "Property";

	::GetCurrentDirectory(MAX_PATH, _fileName);
	_tcscat(_fileName, "\\");
	_tcscat(_fileName, macroFileName);
}

CIni::~CIni()
{
}

void CIni::SaveString(const char *keyName, char *value)
{
	int strLen = strlen(value);
	char *strQuot = new char [strLen + 3];
	
	strQuot[0] = '\"';
	strncpy (strQuot + 1, value, strLen);
	strQuot[strLen + 1] = '\"';
	strQuot[strLen + 2] = '\0';

	WritePrivateProfileString(_appName, keyName, strQuot, _fileName);

	delete [] strQuot;
}

void CIni::SaveInt(const char *keyName, long value)
{
	char str[32+1];

	_snprintf (str, 32, "%d", value);
	str[32] = '\0';

	WritePrivateProfileString(_appName, keyName, str, _fileName);
}

void CIni::SaveFloat(const char *keyName, double value)
{
	char str[64+1];

	_snprintf (str, 64, "%g", value);
	str[64] = '\0';

	WritePrivateProfileString(_appName, keyName, str, _fileName);
}

void CIni::LoadString(const char *keyName, char *returnedString, DWORD nSize, char *defaultValue)
{
	GetPrivateProfileString(_appName, keyName, defaultValue, returnedString, nSize, _fileName);	
}

long CIni::LoadInt(const char *keyName, long defaultValue)
{
	return GetPrivateProfileInt(_appName, keyName, defaultValue, _fileName);
}

double CIni::LoadFloat(const char *keyName, double defaultValue)
{
	char defStr[64+1];
	_snprintf (defStr, 64, "%g", defaultValue);
	defStr[64] = '\0';

	char returnedString[128+1] = "";
	GetPrivateProfileString(_appName, keyName, defStr, returnedString, 128, _fileName);	
	returnedString[128] = '\0';

	return atof (returnedString);
}

void CIni::Transfer (bool bSave, const char *keyName, char *value, DWORD nSize, char *defaultValue)
{
	if (bSave) SaveString (keyName, value);
	else       LoadString (keyName, value, nSize, defaultValue);
}

void CIni::Transfer (bool bSave, const char *keyName, bool &value, bool defaultValue)
{
	if (bSave)   SaveInt (keyName, value ? 1 : 0);
	else value = LoadInt (keyName, defaultValue) ? true : false;
}

void CIni::Transfer (bool bSave, const char *keyName, char &value, char defaultValue)
{
	if (bSave)        SaveInt (keyName, (int)value);
	else value = (int)LoadInt (keyName, (int)defaultValue);
}

void CIni::Transfer (bool bSave, const char *keyName, int &value, int defaultValue)
{
	if (bSave)        SaveInt (keyName, (int)value);
	else value = (int)LoadInt (keyName, (int)defaultValue);
}

void CIni::Transfer (bool bSave, const char *keyName, long &value, long defaultValue)
{
	if (bSave)        SaveInt (keyName, (int)value);
	else value = (int)LoadInt (keyName, (int)defaultValue);
}

void CIni::Transfer (bool bSave, const char *keyName, DWORD &value, DWORD defaultValue)
{
	if (bSave)        SaveInt (keyName, (int)value);
	else value = (int)LoadInt (keyName, (int)defaultValue);
}

void CIni::Transfer (bool bSave, const char *keyName, float &value, float defaultValue)
{
	if (bSave)          SaveFloat (keyName, value);
	else value = (float)LoadFloat (keyName, defaultValue);
}

void CIni::Transfer (bool bSave, const char *keyName, double &value, double defaultValue)
{
	if (bSave)   SaveFloat (keyName, value);
	else value = LoadFloat (keyName, defaultValue);
}