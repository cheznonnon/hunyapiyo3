// Project Nonnon
// copyright (c) nonnon all rights reserved
// License : GPL http://www.gnu.org/copyleft/gpl.html


// Template File


// [!] : Usage
//
//	1 : replace your NSView (Custom View) to NonnonGame
//	2 : IB : right pane : "Custom Class", "Class", set "NonnonGame"
//	3 : modify behavior


// [!] : trouble shooter
//
//	[ drawRect is not called at redraw ]
//
//		a : call [_n_game display];
//		b : see your @property and connect(DnD) to UI on the Xib canvas
//		c : layer may cause




#ifndef _H_NONNON_MAC_NONNON_GAME
#define _H_NONNON_MAC_NONNON_GAME




#import <Cocoa/Cocoa.h>


#include "../nonnon/mac/_mac.c"
#include "../nonnon/mac/image.c"
#include "../nonnon/mac/sound.c"
#include "../nonnon/mac/window.c"


#include "../nonnon/neutral/bmp/ui/pie_throbber.c"


#include "../nonnon/game/helper.c"
#include "../nonnon/game/transition.c"

#include "../nonnon/win32/gdi.c"




//@interface NonnonGame : NSView <AVAudioPlayerDelegate>
@interface NonnonGame : NSView
@end




#include "hunyapiyo3.c"




@interface NonnonGame ()

@end


@implementation NonnonGame {

	n_hunyapiyo3 hunyapiyo3;
	NSRect       hunyapiyo3_rect;

}

/*
- (instancetype)initWithCoder:(NSCoder *)coder
{
//NSLog( @"initWithCoder" );

	self = [super initWithCoder:coder];
	if ( self )
	{
	}


	return self;
}
*/

- (void) n_mac_game_launch:(NSWindow*)window
{

	n_hunyapiyo3 *p = &hunyapiyo3;


	n_hunyapiyo3_init( p );
//NSLog( @"%d %d", p->sx, p->sy );

	p->self = self;


	CGFloat sx = p->sx;
	CGFloat sy = p->sy + p->bar;


	NSSize size = NSMakeSize( sx,sy );

	[window setContentMinSize:size];
	[window setContentMaxSize:size];
	[window setContentSize   :size];


	hunyapiyo3_rect = NSMakeRect( 0,0,sx,sy );
	[self setFrame:hunyapiyo3_rect];


	n_mac_window_centering( window );


	p->refresh = TRUE;
	n_mac_timer_init( p->self, @selector( n_timer_method ), 1 );

}

- (void) n_accentColorChanged
{

	n_hunyapiyo3 *p = &hunyapiyo3;


	n_bmp_carboncopy( &p->canvas, &p->transition_bmp_old );

	n_hunyapiyo3_metrics( p );
	n_hunyapiyo3_gdi_background( p, &p->bmp_bg );
	n_bmp_flush_fastcopy( &p->bmp_bg, &p->canvas );

	n_bmp_free( &p->bmp_font );

	n_hunyapiyo3_loop( p );

	n_bmp_carboncopy( &p->canvas, &p->transition_bmp_new );


	p->transition_onoff = TRUE;


	// [x] : dark mode : the both have same values
	//
	//	new value of [NSColor controlBackgroundColor] is not set

//static u32 before = 0;
//NSLog( @"%x %x", before, p->color_gradient );
//before = p->color_gradient;

}

/*
- (BOOL) isFlipped
{
	return YES;
}
*/

- (void) n_timer_method
{

	static u32 timer = 0;
	if ( n_game_timer( &timer, 6 ) )
	{
		n_hunyapiyo3_loop( &hunyapiyo3 );
	}


	if ( hunyapiyo3.refresh )
	{
		hunyapiyo3.refresh = FALSE;

		[self display];
	}

//n_posix_sleep( 200 );
}


- (void)drawRect:(NSRect)rect
{
//NSLog( @"drawRect" );

	n_hunyapiyo3 *p = &hunyapiyo3;


	//n_mac_image_nbmp_direct_draw_fast( &p->canvas, &hunyapiyo3_rect, n_posix_false );

	//n_mac_image_imagerep_sync( p->rep, &p->canvas );

	n_mac_image_imagerep_alias_fast( p->rep, &p->canvas );

	n_mac_image_nbmp_direct_draw_faster( p->rep, &hunyapiyo3_rect, n_posix_false );

}


// [Needed] : to make mouseEntered mouseExited mouseMoved

- (void) updateTrackingAreas
{
	[self initTrackingArea];
}

-(void) initTrackingArea
{
	NSTrackingAreaOptions options =
	(
		NSTrackingActiveAlways          |
		NSTrackingInVisibleRect         |
		NSTrackingMouseEnteredAndExited |
		NSTrackingMouseMoved
	);

	NSTrackingArea *area = [
		[NSTrackingArea alloc] initWithRect:[self bounds]
		                            options:options
		                              owner:self
		                           userInfo:nil
	];

	[self addTrackingArea:area];
}

-(void) mouseEntered:(NSEvent *)theEvent
{
//NSLog(@"entered");
}

-(void) mouseExited:(NSEvent *)theEvent
{
//NSLog(@"exited");
}

- (void) mouseMoved:(NSEvent*) theEvent
{
//NSLog( @"mouseMoved" );

//NSPoint pt = [theEvent locationInWindow];
//NSLog( @"%f %f", pt.x, pt.y );

	hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );

}

- (void) rightMouseDown:(NSEvent *)theEvent
{
//NSLog( @"right mouseUp" );

	//if ( [theEvent clickCount] != 1 ) { return; }

	if ( hunyapiyo3.animation_onoff ) { return; }

	hunyapiyo3.click_menu = n_posix_true;

	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_MAIN )
	{
		hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );
		n_hunyapiyo3_main( &hunyapiyo3 );
	} else
	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_PERC )
	{
		if ( hunyapiyo3.answer_check_onoff )
		{
			n_hunyapiyo3_sound_play( &hunyapiyo3, N_HUNYAPIYO3_SOUND_CLICK );
			hunyapiyo3.answer_check_onoff = n_posix_false;
		} else {
			n_hunyapiyo3_sound_play( &hunyapiyo3, N_HUNYAPIYO3_SOUND_CLICK );
			hunyapiyo3.answer_check_onoff = n_posix_true;
		}

		hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );
		n_hunyapiyo3_last( &hunyapiyo3 );
	}

	hunyapiyo3.click_menu = n_posix_false;

}

- (void) mouseUp:(NSEvent*) theEvent
{
//NSLog( @"mouseUp" );

	//if ( [theEvent clickCount] != 1 ) { return; }

	if ( hunyapiyo3.animation_onoff ) { return; }

	hunyapiyo3.click = n_posix_true;

	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_CLCK )
	{
		hunyapiyo3.phase = N_HUNYAPIYO3_PHASE_STRT;
	} else
	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_MAIN )
	{
		hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );
		n_hunyapiyo3_main( &hunyapiyo3 );
	} else
	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_PERC )
	{
		hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );
		n_hunyapiyo3_last( &hunyapiyo3 );
	}

	hunyapiyo3.click = n_posix_false;

}

- (void) mouseDown:(NSEvent*) theEvent
{
//NSLog(@"mouseDown");

	//if ( [theEvent clickCount] != 1 ) { return; }

	if ( hunyapiyo3.animation_onoff ) { return; }

	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_MAIN )
	{

		hunyapiyo3.button_press_offset = hunyapiyo3.button_press_offset_default;

		hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );
		n_hunyapiyo3_main( &hunyapiyo3 );
	} else
	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_PERC )
	{
		hunyapiyo3.button_press_offset = hunyapiyo3.button_press_offset_default;

		hunyapiyo3.pt = n_mac_image_position_flip( &hunyapiyo3.canvas, [theEvent locationInWindow] );
		n_hunyapiyo3_last( &hunyapiyo3 );
	}

}

- (void) mouseDragged:(NSEvent*) theEvent
{
//NSLog(@"mouseDragged");
}

- (void)scrollWheel:(NSEvent *)theEvent
{
//NSLog(@"scrollWheel");

	if ( hunyapiyo3.animation_onoff ) { return; }

	if ( hunyapiyo3.phase == N_HUNYAPIYO3_PHASE_MAIN )
	{
//NSLog( @"%lu %f %f", theEvent.phase, [theEvent deltaY], [theEvent scrollingDeltaY] );

		if ( 0 > [theEvent deltaY] )
		{
			hunyapiyo3.click      = n_posix_true;	
		} else {
			hunyapiyo3.click_menu = n_posix_true;
		}
		n_hunyapiyo3_main( &hunyapiyo3 );

		hunyapiyo3.click = hunyapiyo3.click_menu = n_posix_false;

	}

}


- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
//NSLog(@"audioPlayerDidFinishPlaying");

	// [x] : this is called after more than sound msec

	//hunyapiyo3.sound_lock = n_posix_false;
}


@end


#endif // _H_NONNON_MAC_NONNON_GAME


