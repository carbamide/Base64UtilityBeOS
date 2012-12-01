//
//  Base64App.h
//  Base64Utility
//
//  Created by Josh Barrow on 3/14/12.
//  Copyright 2012 Jukaela Enterprises. All rights reserved.
//

#include <Window.h>
#include <Button.h>
#include <TextView.h>
#include <Path.h>
#include <MenuBar.h>

class Base64Window : public BWindow 
{
public:
	BFilePanel *openPanel;
	BFilePanel *savePanel;
	
	BButton *encodeButton;
	BButton *decodeButton;
	

	Base64Window(BRect frame);
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage* message);
private:
	BTextView *encodeTextView;
	BTextView *decodeTextView;
	BMenuBar *menubar;

	void Base64Window::encodeFile(BPath path, BEntry entry);

	void Base64Window::decodeFile(BMessage *message);
};
