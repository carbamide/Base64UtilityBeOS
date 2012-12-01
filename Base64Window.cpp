//
//  Base64Window.cpp
//  Base64Utility
//
//  Created by Josh Barrow on 3/14/12.
//  Copyright 2012 Jukaela Enterprises. All rights reserved.
//

#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <File.h>
#include <FilePanel.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <Path.h>
#include <String.h>
#include <TextView.h>
#include "Base64Window.h"
#include "base64.h"

const uint32 ENCODE_BUTTON_MESSAGE = 'enbm';
const uint32 DECODE_BUTTON_MESSAGE = 'dcbm';
const uint32 OPEN_FILE_MESSAGE = 'ofms';
const uint32 SAVE_FILE_MESSAGE = 'sfms';
const uint32 RESET_MESSAGE = 'rsms';
const uint32 QUIT_MESSAGE = 'qtms';
const uint32 ABOUT_MESSAGE = 'abms';

BMessage Base64Arch;

Base64Window::Base64Window(BRect frame) : BWindow (frame, "Base64 Utility", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	BMenu *menu;

	menubar = new BMenuBar(Bounds(), "menubar");
	
	menu = new BMenu("File");
	menu->AddItem(new BMenuItem("Reset", new BMessage(RESET_MESSAGE), 'R'));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Quit", new BMessage(QUIT_MESSAGE), 'Q'));
	
	menubar->AddItem(menu);
	
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("About...", new BMessage(ABOUT_MESSAGE)));
	
	menubar->AddItem(menu);
	
	AddChild(menubar);

	BRect r(frame);
	r.OffsetTo(0, 0);

	BView *background = new BView(r, NULL, B_FOLLOW_ALL_SIDES, 0);

	rgb_color c = ui_color(B_PANEL_BACKGROUND_COLOR);

	background->SetViewColor(c);
	background->SetLowColor(c);

	r.InsetBy(5, 5);

	BRect r2(r);
	r2.top += 15;
	r2.bottom -= 5;
	r2.right = r2.left + 185;

	BBox *encodeBox = new BBox(r2, NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM);
	encodeBox->SetLabel("Encode");

	background->AddChild(encodeBox);

	r2.left = r2.right + 5;
	r2.right = r.right - 5;
	r2.bottom = r.bottom - 5; 
	BBox *decodeBox = new BBox(r2, NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM);
	decodeBox->SetLabel("Decode");

	background->AddChild(decodeBox);

	AddChild(background);

	BRect encodeButtonRect = BRect(10, 15, 175, 30);

	encodeButton = new BButton (encodeButtonRect, NULL, "Encode and Display...", new BMessage(ENCODE_BUTTON_MESSAGE));

	encodeBox->AddChild(encodeButton);

	BRect encodeTextViewRect = BRect(10, 45, 175, 175);

	encodeTextView = new BTextView(encodeTextViewRect, "encode_text_view", BRect(0, 0, 175, 175), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_PULSE_NEEDED);

	encodeBox->AddChild(encodeTextView);

	BRect decodeButtonRect = BRect(10, 150, 175, 30);

	decodeButton = new BButton (decodeButtonRect, NULL, "Decode and Save...", new BMessage(DECODE_BUTTON_MESSAGE));

	decodeBox->AddChild(decodeButton);

	BRect decodeTextViewRect = BRect(10, 15, 175, 145);

	decodeTextView = new BTextView(decodeTextViewRect, "decode_text_view", BRect(0, 0, 175, 175), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_PULSE_NEEDED);

	decodeBox->AddChild(decodeTextView);

	BMessenger self(this);

	openPanel = new BFilePanel(B_OPEN_PANEL, &self, NULL, B_FILE_NODE, false, new BMessage(OPEN_FILE_MESSAGE), NULL);

	savePanel = new BFilePanel(B_SAVE_PANEL, &self, NULL, B_FILE_NODE, false, new BMessage(SAVE_FILE_MESSAGE), NULL);
};


bool Base64Window::QuitRequested()
{
	return true;
}

void Base64Window::MessageReceived(BMessage* message)
{
	BEntry entry;
	entry_ref ref;
	BPath path;

	switch(message->what)
	{
		case ENCODE_BUTTON_MESSAGE:
		{
			openPanel->Show();

			break;
		}
		case DECODE_BUTTON_MESSAGE:
		{
			savePanel->Show();

			break;
		}
		case OPEN_FILE_MESSAGE:
		{
			message->FindRef("refs", &ref);

			entry.SetTo(&ref);
			entry.GetPath(&path);

			encodeFile(path, entry);

			break;
		}
		case SAVE_FILE_MESSAGE:
		{
			decodeFile(message);

			break;
		}
		case RESET_MESSAGE:
		{
			encodeTextView->SelectAll();
			decodeTextView->SelectAll();

			encodeTextView->Delete();
			decodeTextView->Delete();

			break;
		}
		case QUIT_MESSAGE:
		{
			be_app->PostMessage(B_QUIT_REQUESTED);

			break;
		}
		case ABOUT_MESSAGE :
		{
			(new BAlert("About Base64 Utility", "Copyright 2012, Jukaela Enterprises. Visit us at http://www.jukaela.com.", "Close"))->Go();

			break;
		}
	}
}

void Base64Window::encodeFile(BPath pathName, BEntry entry) 
{
	BFile file(pathName.Path(), B_READ_ONLY);

        off_t length;
        char *text;
        file.GetSize(&length);
        text = (char *) malloc(length);
	
	file.Read(text, length);

	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(text), length);

	const char *textEncoded = encoded.c_str();

	text_run_array *currentSelection = new text_run_array;

	encodeTextView->SetText(textEncoded, currentSelection);
}

void Base64Window::decodeFile(BMessage *message)
{
 	entry_ref ref;
        status_t err;
        const char *name;
        BPath path;     
        BEntry entry;  
        FILE *file;      

        if ((err=message->FindRef("directory", &ref)) != B_OK) {
                return;
        }

        if ((err=message->FindString("name", &name)) != B_OK) {
                return;
        }

        if ((err=entry.SetTo(&ref)) != B_OK) {
                return;
        }

        entry.GetPath(&path);
        path.Append(name);  

        if (!(file = fopen(path.Path(), "w"))) {
                return;
        }

	string decodedString = base64_decode(decodeTextView->Text());

	err = fwrite(decodedString.c_str(), 1, decodeTextView->TextLength(), file);

        fclose(file);
}

