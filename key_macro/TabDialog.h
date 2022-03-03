//////////////////////////////////////////////////////
// Windows Control files for Robot applications
// 
// Copyright (c) 2002-2008. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://www.orobot.net
// Written by KwangWoong Yang<page365@gmail.com>
//

#pragma once
#include <afxcmn.h>
#include <vector>

using namespace std;

class CTabDialog : public CTabCtrl
{
public:
	CTabDialog();
	virtual ~CTabDialog();

protected:
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	int _tabSelected;
	vector<CDialog *> _dialogList;

	void ViewChange (int new_view);

public:
	void AddDialog (const char *name, CDialog *dialog, int resource_id, int image_id = -1);
	void TabChange (int tab);
	int  GetTabSelected () { return _tabSelected; }
	CDialog *GetTabDialog (int tab) { return (0 <= tab && tab < (int)_dialogList.size ()) ? _dialogList[tab] : NULL; }
};
