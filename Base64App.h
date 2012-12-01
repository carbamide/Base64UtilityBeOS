//
//  Base64App.h
//  Base64Utility
//
//  Created by Josh Barrow on 3/14/12.
//  Copyright 2012 Jukaela Enterprises. All rights reserved.
//

#include <Application.h>
#include "Base64Window.h"

#define APP_SIG "application/x-vnd.Jukaela-Base64"

class Base64App : public BApplication
{
public:
	Base64App();
	virtual void ReadyToRun();
private:
	Base64Window* aWindow;
};

