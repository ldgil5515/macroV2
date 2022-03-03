#include "StdAfx.h"
#include "AppIni.h"



CAppIni::CAppIni(const char *ini_path)
: CIni(ini_path)
{
	LoadData();
}

CAppIni::~CAppIni()
{
	SaveData();
}

void CAppIni::LoadData()
{
	TransferAllData (false);
}

void CAppIni::SaveData()
{
	TransferAllData (true);
}

void CAppIni::TransferAllData (bool bSave)
{
	Transfer (bSave, "macroOptions",		(DWORD &)macroOptions,	0x88);
	Transfer (bSave, "recOptions",			(DWORD &)recOptions,	0xB7);
	Transfer (bSave, "editOptions",			(DWORD &)editOptions,	0x00);

	Transfer (bSave, "hangulKeyboard",		hangulKeyboard,			0);

	Transfer (bSave, "windowSX",			windowSX,				100);
	Transfer (bSave, "windowSY",			windowSY,				100);
	Transfer (bSave, "debugSX",				debugSX,				200);
	Transfer (bSave, "debugSY",				debugSY,				200);
	Transfer (bSave, "statusSX",			statusSX,				300);
	Transfer (bSave, "statusSY",			statusSY,				300);

	Transfer (bSave, "macroFileName",		macroFileName,			MAX_PATH,	"macro.m");
	Transfer (bSave, "title",				title,					MAX_PATH,	"키보드/마우스 매크로 V2");
	macroFileName[MAX_PATH] = '\0';
	title[MAX_PATH] = '\0';

	Transfer (bSave, "eventDelay",			eventDelay,				33);
	Transfer (bSave, "keyMacroRun",			keyMacroRun,			VK_F12);
	Transfer (bSave, "keyMacroRecord",		keyMacroRecord,			VK_F11);
	Transfer (bSave, "keyMousePosCapture",	keyMousePosCapture,		VK_F10);
	Transfer (bSave, "keyMacroColorStart",	keyMacroColorStart,		VK_F9);
	Transfer (bSave, "keyMacroColorStop",	keyMacroColorStop,		VK_F8);

	Transfer (bSave, "recMouseDistance",	recMouseDistance,		10);
	Transfer (bSave, "recEventTimeInterval",recEventTimeInterval,	100);

	Transfer (bSave, "sequenceStart",		sequenceStart,			1000);
	Transfer (bSave, "macroSel",			macroSel,				0);
}

CAppIni g_ini("key_macro.ini");
