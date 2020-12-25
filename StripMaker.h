#pragma once
#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <EuroScopePlugIn.h>
#include "CStripMakerPlugin.h"

class CStripMakerApp : public CWinApp
{
public:
	CStripMakerApp();

// Overrides
public:
	virtual BOOL InitInstance();
	CStripMakerPlugIn* gpMyPlugIn = NULL;
	DECLARE_MESSAGE_MAP()
};

