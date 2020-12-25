// StripMaker.cpp : Defines the initialization routines for the DLL and EuroScope plugin.

#include "pch.h"
#include "framework.h"
#include "StripMaker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CStripMakerApp, CWinApp)
END_MESSAGE_MAP()

// CStripMakerApp construction
CStripMakerApp::CStripMakerApp()
{

}

// The one and only CStripMakerApp object
CStripMakerApp theApp;

// CStripMakerApp initialization
BOOL CStripMakerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
		// create the instance
		* ppPlugInInstance = theApp.gpMyPlugIn = new CStripMakerPlugIn();
}

void __declspec (dllexport) EuroScopePlugInExit(void) {
	delete theApp.gpMyPlugIn;
}