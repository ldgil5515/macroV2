#pragma once

struct sVirtulKeyInfo {
	int vk_code;
	const char *vk_name;
	const char *vk_desc;
};

extern sVirtulKeyInfo g_vkList[];

extern int GetVkIndex (int vk_code);
extern int GetVkCode (int index);
extern const char *GetVkDesc (int index);
extern const char *GetVkDescFromVK (int vk_code);
extern const char *GetVkNameFromVK (int vk_code);
