//////////////////////////////////////////////////////
// Windows Control files for Robot applications
// 
// Copyright (c) 2002-2013. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: 
// Written by KwangWoong Yang<page365@gmail.com>
//

#include "stdafx.h"
#include "TabDialog.h"

CTabDialog::CTabDialog()
{
	_tabSelected = -1;
}

CTabDialog::~CTabDialog()
{
	for (int i=0; i<(int)_dialogList.size (); ++i) {
		delete _dialogList[i];
	}
}


BEGIN_MESSAGE_MAP(CTabDialog, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CTabDialog::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel_index = GetCurSel();
	if (0 <= sel_index) {
		ViewChange (sel_index);
	}
	if (pResult) {
		*pResult = 0;
	}
}

void CTabDialog::ViewChange (int new_view)
{
	if (0 <= _tabSelected && _tabSelected < (int)_dialogList.size ()) {
		_dialogList[_tabSelected]->ShowWindow (SW_HIDE);
	}
	if (0 <= new_view && new_view < (int)_dialogList.size ()) {
		_dialogList[new_view]->ShowWindow (SW_SHOW);
		_dialogList[new_view]->SetFocus ();
	}
	_tabSelected = new_view;
}

void CTabDialog::AddDialog (const char *name, CDialog *dialog, int resource_id, int image_id)
{
	TC_ITEM tci;

	tci.mask = TCIF_TEXT | ((0 <= image_id) ? TCIF_IMAGE : 0);
	tci.iImage = image_id;
	tci.pszText	= (LPSTR)(name);
	InsertItem(GetItemCount(), &tci);

	dialog->Create(resource_id, this);
	dialog->SetWindowPos (NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	dialog->ShowWindow (SW_HIDE);

	_dialogList.push_back (dialog);
}

void CTabDialog::TabChange (int tab)
{
	if (tab < 0) tab = 0;
	if ((int)_dialogList.size() - 1 < tab) tab = _dialogList.size() - 1;

	SetCurSel (tab);

	ViewChange (tab);
}

void CTabDialog::OnDestroy() 
{
	CTabCtrl::OnDestroy();
	
	vector<CDialog *>::iterator it;
	for (it=_dialogList.begin (); it!=_dialogList.end (); ++it) {
		(*it)->DestroyWindow ();
	}
}
