//
//  AppDelegate.m
//  hunyapiyo3
//
//  Created by のんのん on 2022/07/14.
//

#import "AppDelegate.h"

#include "n_game.c"




@interface AppDelegate ()

@property (strong) IBOutlet NSWindow *window;

@property (weak) IBOutlet NonnonGame *n_game;
@property (weak) IBOutlet NSButton *n_button;

@end




@implementation AppDelegate


- (void)awakeFromNib
{
//NSLog( @"awakeFromNib" );

	n_mac_image_window = _window;
	n_gdi_scale_factor = n_mac_image_window.backingScaleFactor;


	[[NSNotificationCenter defaultCenter]
	      addObserver: self
		 selector: @selector( windowWillClose: )
		     name: NSWindowWillCloseNotification
		   object: nil
	];

	// [!] : accent color
	[[NSDistributedNotificationCenter defaultCenter]
		addObserver: self
		   selector: @selector( accentColorChanged: )
		       name: @"AppleColorPreferencesChangedNotification"
		     object: nil
	];

	// [!] : dark mode
	[[NSDistributedNotificationCenter defaultCenter]
		addObserver: self
		   selector: @selector( darkModeChanged: )
		       name: @"AppleInterfaceThemeChangedNotification"
		     object: nil
	];

	[_n_game n_mac_game_canvas_resize:_window width:-1 height:-1];

}

- (void) accentColorChanged:(NSNotification *)notification
{
//NSLog( @"accentColorChanged" );

	[_n_game n_accentColorChanged];

}

- (void) darkModeChanged:(NSNotification *)notification
{
//NSLog( @"darkModeChanged" );

	[_n_game n_accentColorChanged];

}

- (void) windowWillClose:(NSNotification *)notification
{
//NSLog( @"closed" );

	NSWindow *window = notification.object;
	if ( window == self.window )
	{
		[NSApp terminate:self];
	}
}



- (void)applicationWillFinishLaunching:(NSNotification *)aNotification {
//NSLog( @"applicationWillFinishLaunching" );

	[_n_game n_mac_game_canvas_resize:_window width:-1 height:-1];
	
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application
//NSLog( @"applicationDidFinishLaunching" );
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
	// Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
	return YES;
}


- (IBAction)n_hunyapiyo3_menu_readme:(id)sender {

	NSString *helpFilePath = [[NSBundle mainBundle] pathForResource:@"hunyapiyo3" ofType:@"html"];
	NSURL    *helpFileURL  = [NSURL fileURLWithPath:helpFilePath];

	[[NSWorkspace sharedWorkspace] openURL:helpFileURL];

}


@end
