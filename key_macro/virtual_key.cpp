#include "stdafx.h"
#include "virtual_key.h"


sVirtulKeyInfo g_vkList[] = {
	{ 0x01,	"LM",		"Left mouse button"			},
	{ 0x02, "RM",		"Right mouse button"		},
	{ 0x03, "CB",		"Control-break processing"	},
	{ 0x04, "MM",		"Middle mouse button"		},
	{ 0x05, "X1",		"X1 mouse button"			},
	{ 0x06, "X2",		"X2 mouse button"			},
	{ 0x08, "BS",		"BACKSPACE key"				},
	{ 0x09, "TAB",		"TAB key"					},
	{ 0x0A, "MSU",		"Mouse scroll up"			},	// Not a default virtual key
	{ 0x0B, "MSD",		"Mouse scroll down"			},	// Not a default virtual key
	{ 0x0C, "CLR",		"CLEAR key"					},
	{ 0x0D, "ENT",		"ENTER key"					},
	{ 0x10, "SHF",		"SHIFT key"					},
	{ 0x11, "CTL",		"CTRL key"					},
	{ 0x12, "ALT",		"ALT key"					},
	{ 0x13, "PAU",		"PAUSE key"					},
	{ 0x14, "CAP",		"CAPS LOCK key"				},
//	{ 0x15, "IME",		"Input Method Editor (IME, Kana mode)" },
//	{ 0x15, "IME",		"IME Hanguel mode"			},
	{ 0x15, "IME",		"IME Hangul mode"			},
	{ 0x17, "IME",		"IME Junja mode"			},
	{ 0x18, "IME",		"IME final mode"			},
	{ 0x19, "IME",		"IME Hanja mode"			},
//	{ 0x19, "IME",		"IME Kanji mode"			},
	{ 0x1B, "ESC",		"ESC key"					},
	{ 0x1C, "IME",		"IME convert"				},
	{ 0x1D, "IME",		"IME nonconvert"			},
	{ 0x1E, "IME",		"IME accept"				},
	{ 0x1F, "IME",		"IME mode change request"	},
	{ 0x20, "SPA",		"SPACEBAR"					},
	{ 0x21, "PGU",		"PAGE UP key"				},
	{ 0x22, "PGD",		"PAGE DOWN key"				},
	{ 0x23, "END",		"END key"					},
	{ 0x24, "HOM",		"HOME key"					},
	{ 0x25, "LA",		"LEFT ARROW key"			},
	{ 0x26, "UA",		"UP ARROW key"				},
	{ 0x27, "RA",		"RIGHT ARROW key"			},
	{ 0x28, "DA",		"DOWN ARROW key"			},
	{ 0x29, "SEL",		"SELECT key"				},
	{ 0x2A, "PRT",		"PRINT key"					},
	{ 0x2B, "EXE",		"EXECUTE key"				},
	{ 0x2C, "PS",		"PRINT SCREEN key"			},
	{ 0x2D, "INS",		"INS key"					},
	{ 0x2E, "DEL",		"DEL key"					},
	{ 0x2F, "HLP",		"HELP key"					},
	{ 0x30, "0",		"0 key"						},
	{ 0x31, "1",		"1 key" 					},
	{ 0x32, "2",		"2 key" 					},
	{ 0x33, "3",		"3 key" 					},
	{ 0x34, "4",		"4 key" 					},
	{ 0x35, "5",		"5 key" 					},
	{ 0x36, "6",		"6 key" 					},
	{ 0x37, "7",		"7 key" 					},
	{ 0x38, "8",		"8 key" 					},
	{ 0x39, "9",		"9 key" 					},
	{ 0x41, "A",		"A key" 					},
	{ 0x42, "B",		"B key" 					},
	{ 0x43, "C",		"C key" 					},
	{ 0x44, "D",		"D key" 					},
	{ 0x45, "E",		"E key" 					},
	{ 0x46, "F",		"F key" 					},
	{ 0x47, "G",		"G key" 					},
	{ 0x48, "H",		"H key" 					},
	{ 0x49, "I",		"I key" 					},
	{ 0x4A, "J",		"J key" 					},
	{ 0x4B, "K",		"K key" 					},
	{ 0x4C, "L",		"L key" 					},
	{ 0x4D, "M",		"M key" 					},
	{ 0x4E, "N",		"N key" 					},
	{ 0x4F, "O",		"O key" 					},
	{ 0x50, "P",		"P key" 					},
	{ 0x51, "Q",		"Q key" 					},
	{ 0x52, "R",		"R key" 					},
	{ 0x53, "S",		"S key" 					},
	{ 0x54, "T",		"T key" 					},
	{ 0x55, "U",		"U key" 					},
	{ 0x56, "V",		"V key" 					},
	{ 0x57, "W",		"W key" 					},
	{ 0x58, "X",		"X key" 					},
	{ 0x59, "Y",		"Y key" 					},
	{ 0x5A, "Z",		"Z key"						},
	{ 0x5B, "LW",		"Left Windows key"			},
	{ 0x5C, "RW",		"Right Windows key"			},
	{ 0x5D, "APP",		"Applications key"			},
	{ 0x5F, "CS",		"Computer Sleep key"		},
	{ 0x60, "NK0",		"Numeric keypad 0 key"		},
	{ 0x61, "NK1",		"Numeric keypad 1 key"		},
	{ 0x62, "NK2",		"Numeric keypad 2 key"		},
	{ 0x63, "NK3",		"Numeric keypad 3 key"		},
	{ 0x64, "NK4",		"Numeric keypad 4 key"		},
	{ 0x65, "NK5",		"Numeric keypad 5 key"		},
	{ 0x66, "NK6",		"Numeric keypad 6 key"		},
	{ 0x67, "NK7",		"Numeric keypad 7 key"		},
	{ 0x68, "NK8",		"Numeric keypad 8 key"		},
	{ 0x69, "NK9",		"Numeric keypad 9 key"		},
	{ 0x6A, "MP",		"Multiply key"				},
	{ 0x6B, "ADD",		"Add key"					},
	{ 0x6C, "SEP",		"Separator key"				},
	{ 0x6D, "SUB",		"Subtract key"				},
	{ 0x6E, "DEC",		"Decimal key"				},
	{ 0x6F, "DIV",		"Divide key"				},
	{ 0x70, "F1",		"F1 key"					},
	{ 0x71, "F2",		"F2 key"					},
	{ 0x72, "F3",		"F3 key"					},
	{ 0x73, "F4",		"F4 key"					},
	{ 0x74, "F5",		"F5 key"					},
	{ 0x75, "F6",		"F6 key"					},
	{ 0x76, "F7",		"F7 key"					},
	{ 0x77, "F8",		"F8 key"					},
	{ 0x78, "F9",		"F9 key"					},
	{ 0x79, "F10",		"F10 key"					},
	{ 0x7A, "F11",		"F11 key"					},
	{ 0x7B, "F12",		"F12 key"					},
	{ 0x90, "NL",		"NUM LOCK key"				},
	{ 0x91, "SL",		"SCROLL LOCK key"			},
	{ 0xA0, "LS",		"Left SHIFT key"			},
	{ 0xA1, "RS",		"Right SHIFT key"			},
	{ 0xA2, "LC",		"Left CONTROL key"			},
	{ 0xA3, "RC",		"Right CONTROL key"			},
	{ 0xA4, "LM",		"Left MENU key"				},
	{ 0xA5, "RM",		"Right MENU key"			},
	{ 0xA6, "BB",		"Browser Back key"			},
	{ 0xA7, "BF",		"Browser Forward key"		},
	{ 0xA8, "BR",		"Browser Refresh key"		},
	{ 0xA9, "BS",		"Browser Stop key"			},
	{ 0xAA, "BSE",		"Browser Search key"		},
	{ 0xAB, "BF",		"Browser Favorites key"		},
	{ 0xAC, "BSH",		"Browser Start and Home key"},
	{ 0xAD, "VM",		"Volume Mute key"			},
	{ 0xAE, "VD",		"Volume Down key"			},
	{ 0xAF, "VU",		"Volume Up key"				},
	{ 0xB0, "NT",		"Next Track key"			},
	{ 0xB1, "PT",		"Previous Track key"		},
	{ 0xB2, "SM",		"Stop Media key"			},
	{ 0xB3, "PPM",		"Play/Pause Media key"		},
	{ 0xB4, "SMA",		"Start Mail key"			},
	{ 0xB5, "SM",		"Select Media key"			},
	{ 0xB6, "SA1",		"Start Application 1 key"	},
	{ 0xB7, "SA2",		"Start Application 2 key"	},
	{ 0xBA, ";:",		";: key"					},
	{ 0xBB, "=+",		"=+ key" 					},
	{ 0xBC, ",<",		",< key" 					},
	{ 0xBD, "-_",		"-_ key" 					},
	{ 0xBE, ".>",		".> key" 					},
	{ 0xBF, "/?",		"/? key" 					},
	{ 0xC0, "`~",		"`~ key" 					},
	{ 0xDB, "[{",		"[{ key" 					},
	{ 0xDC, "\\|",		"\\| key" 					},
	{ 0xDD, "]}",		"]} key" 					},
	{ 0xDE, "'\"",		"'\" key" 					},
	{   -1, "",			"" 							},
};			

static int vkListSize = sizeof(g_vkList)/sizeof(sVirtulKeyInfo);


int GetVkIndex (int vk_code)
{
	for (int i=0; i<256 && g_vkList[i].vk_code != -1; ++i) {
		if (vk_code == g_vkList[i].vk_code) {
			return i;
		}
	}
	return -1;
}

int GetVkCode (int index)
{
	if (0 <= index && index < vkListSize) {
		return g_vkList[index].vk_code;
	}
	return -1;
}

const char *GetVkDesc (int index)
{
	if (0 <= index && index < vkListSize) {
		return g_vkList[index].vk_desc;
	}
	return "(Unknown)";
}

const char *GetVkDescFromVK (int vk_code)
{
	int index = GetVkIndex (vk_code);
	if (index >= 0) {
		return 	g_vkList[index].vk_desc;
	}
	return "(Unknown)";
}


const char *GetVkNameFromVK (int vk_code)
{
	int index = GetVkIndex (vk_code);
	if (index >= 0) {
		return 	g_vkList[index].vk_name;
	}
	return "(Unknown)";
}
