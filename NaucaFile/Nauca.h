#pragma once
#include "resource.h"

class CNaucaApp : public CWinApp
{
public:
	CNaucaApp();

public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CNaucaApp theApp;
