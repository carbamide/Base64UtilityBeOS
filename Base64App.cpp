//
//  Base64App.cpp
//  Base64Utility
//
//  Created by Josh Barrow on 3/14/12.
//  Copyright 2012 Jukaela Enterprises. All rights reserved.
//

#include "Base64App.h"

int main(int, char**)
{
	Base64App* app = new Base64App();
	app->Run();

	return(0);
}

Base64App::Base64App() : BApplication(APP_SIG)
{
}

void Base64App::ReadyToRun()
{
	BRect windowRect;

	windowRect.Set(100, 100, 490, 315);

	aWindow = new Base64Window(windowRect);
	aWindow->Show();
}
