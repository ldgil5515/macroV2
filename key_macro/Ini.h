#pragma once

#include <string>
#include <Windows.h>

using namespace std;

class CIni
{
public:
	CIni(const char *macroFileName);
	virtual ~CIni();

	// Loads settings from ini file
	void   LoadString (const char *keyName, char * returnedString, DWORD nSize, char *defaultValue = "");
	long   LoadInt    (const char *keyName, long   defaultValue = 0);
	double LoadFloat  (const char *keyName, double defaultValue = 0.0);

	// Saves settings to ini file
	void SaveString (const char *keyName, char *value);
	void SaveInt    (const char *keyName, long value);
	void SaveFloat  (const char *keyName, double value);

	void Transfer (bool bSave, const char *keyName, char   *value, DWORD nSize, char *defaultValue = "");
	void Transfer (bool bSave, const char *keyName, bool   &value, bool   defaultValue = false);
	void Transfer (bool bSave, const char *keyName, char   &value, char   defaultValue = 0);
	void Transfer (bool bSave, const char *keyName, int    &value, int    defaultValue = 0);
	void Transfer (bool bSave, const char *keyName, long   &value, long   defaultValue = 0);
	void Transfer (bool bSave, const char *keyName, DWORD  &value, DWORD  defaultValue = 0);
	void Transfer (bool bSave, const char *keyName, float  &value, float  defaultValue = 0.0f);
	void Transfer (bool bSave, const char *keyName, double &value, double defaultValue = 0.0);

private:
	char *_appName;
	char _fileName[MAX_PATH];
};
