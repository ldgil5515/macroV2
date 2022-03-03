#include "stdafx.h"
#include "macro_def.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void LoadMacros (const char *fileName)
{
	FILE *fp = fopen (fileName, "rb");
	if (!fp) return;

	long c = 0;
	fread (&c, sizeof(long), 1, fp);

	g_macros.resize(c);
	for (int i=0; i<c; ++i) {
		sMacro &m = g_macros[i];

		fread (m.name,        sizeof(char), 256, fp);
		fread (&m.start_key,  sizeof(long), 1, fp);
		fread (&m.stop_key,   sizeof(long), 1, fp);
		fread (&m.start_time, sizeof(long), 1, fp);
		fread (&m.stop_time,  sizeof(long), 1, fp);
		fread (&m.start_color,sizeof(long), 1, fp);
		fread (&m.stop_color, sizeof(long), 1, fp);
		fread (&m.tolerance,  sizeof(long), 1, fp);
		fread (&m.start_xy,   sizeof(long), 1, fp);
		fread (&m.stop_xy,    sizeof(long), 1, fp);
		fread (&m.options,    sizeof(long), 1, fp);
		
		long d = 0;
		fread (&d, sizeof(long), 1, fp);

		m._item.resize (d);
		for (int j=0; j<d; ++j) {
			fread (&m._item[j], sizeof(sMacroItem), 1, fp);
			
			if(m._item[j].type == MI_STRING) {
				int len = m._item[j].string.length;
				m._item[j].string.string = new char[len+1];
				
				fread (m._item[j].string.string, sizeof(char), len, fp);
				m._item[j].string.string[len] = '\0';
			}
			else if(m._item[j].type == MI_MCALL) {
				m._item[j].mcall.name = new char[256];
				
				fread (m._item[j].mcall.name, sizeof(char), 256, fp);
				m._item[j].mcall.name[255] = '\0';
			}
		}
	}
	fclose (fp);
}

void SaveMacro (const char *fileName)
{
	FILE *fp = fopen (fileName, "wb");
	if (!fp) return;

	long c = g_macros.size();
	fwrite (&c, sizeof(long), 1, fp);

	for (int i=0; i<c; ++i) {
		sMacro &m = g_macros[i];

		fwrite (m.name,        sizeof(char), 256, fp);
		fwrite (&m.start_key,  sizeof(long), 1, fp);
		fwrite (&m.stop_key,   sizeof(long), 1, fp);
		fwrite (&m.start_time, sizeof(long), 1, fp);
		fwrite (&m.stop_time,  sizeof(long), 1, fp);
		fwrite (&m.start_color,sizeof(long), 1, fp);
		fwrite (&m.stop_color, sizeof(long), 1, fp);
		fwrite (&m.tolerance,  sizeof(long), 1, fp);
		fwrite (&m.start_xy,   sizeof(long), 1, fp);
		fwrite (&m.stop_xy,    sizeof(long), 1, fp);
		fwrite (&m.options,    sizeof(long), 1, fp);
		
		long d = m._item.size();
		fwrite (&d, sizeof(long), 1, fp);

		for (int j=0; j<d; ++j) {
			fwrite (&m._item[j], sizeof(sMacroItem), 1, fp);
			
			if(m._item[j].type == MI_STRING) {
				int len = m._item[j].string.length;
				
				fwrite (m._item[j].string.string, sizeof(char), len, fp);
			}
			else if(m._item[j].type == MI_MCALL) {
				fwrite (m._item[j].mcall.name, sizeof(char), 256, fp);
			}
		}
	}
	fclose (fp);
}
