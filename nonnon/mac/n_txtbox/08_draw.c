// CatPad
// copyright (c) nonnon all rights reserved
// License : GPL http://www.gnu.org/copyleft/gpl.html


// Partial File




#define N_TXTBOX_INDICATOR_SEARCH n_mac_nscolor_argb( 255,255,  0,128 )
#define N_TXTBOX_INDICATOR_CARET  n_mac_nscolor_argb( 255,  0,200,255 )




void
n_mac_image_alpha_grow( n_bmp *bmp, u32 color_replace, n_type_real d )
{

	if ( n_bmp_error( bmp ) ) { return; }


	n_type_gfx x = 0;
	n_type_gfx y = 0;
	n_posix_loop
	{//break;

		u32 color; n_bmp_ptr_get_fast( bmp, x, y, &color );

		int a = n_posix_min( 255, trunc( (n_type_real) n_bmp_a( color ) * d ) );
		int r = n_bmp_r( color_replace );
		int g = n_bmp_g( color_replace );
		int b = n_bmp_b( color_replace );

		color = n_bmp_argb( a,r,g,b );

		n_bmp_ptr_set_fast( bmp, x, y, color );

		x++;
		if ( x >= N_BMP_SX( bmp ) )
		{
			x = 0;
			y++;
			if ( y >= N_BMP_SY( bmp ) ) { break; }
		}
	}


	return;
}




NSColor*
n_txtbox_thin_highlight( n_txt *txt, n_txt *txt_deco, n_type_int i, NSColor *f, NSColor *t )
{

	NSColor *ret = f;

	if ( ( txt_deco != NULL )&&( txt->sy == txt_deco->sy ) )
	{
		n_posix_char *deco = n_txt_get( txt_deco, i );
		if ( ( 2 <= n_posix_strlen( deco ) )&&( deco[ 1 ] == '!' ) )
		{
			ret = n_mac_nscolor_blend( f, t, 0.2 );
		}
	}


	return ret;
}




// internal
n_posix_char*
n_mac_txtbox_path_ellipsis( n_posix_char *path, NSFont *font, CGFloat width_limit )
{

	// [!] : internal :  security : omitted


	if ( path[ 0 ] != '/' )
	{
//NSLog( @"1 : %c", path[ 0 ] );
		return n_string_path_carboncopy( path );
	}


	n_type_int c = n_string_path_split_count( path );
	if ( c <= 1 )
	{
//NSLog( @"2 : %lld", c );
		return n_string_path_carboncopy( path );
	}


	n_posix_char **line = n_memory_new( sizeof(n_posix_char*) * c );


	n_type_int i = 0;
	n_posix_loop
	{
		line[ i ] = n_string_path_split_new( path, i );

		i++;
		if ( i >= c ) { break; }
	}

/*
	i = 0;
	n_posix_loop
	{break;
		NSLog( @"%s", line[ i ] );

		i++;
		if ( i >= c ) { break; }
	}
*/

	n_posix_char *ret = NULL;


	i = 0;
	n_posix_loop
	{

		n_type_int   j = 0;
		n_type_int cch = 0;
		n_posix_loop
		{
			cch += n_posix_strlen( line[ j ] );

			j++;
			if ( j >= c ) { break; }

			cch += 1;
		}

		n_string_free( ret );
		ret = n_string_path_new( cch );

		j = cch = 0;
		n_posix_loop
		{
			cch += n_posix_sprintf( &ret[ cch ], "%s", line[ j ] );

			j++;
			if ( j >= c ) { break; }

			cch += n_posix_sprintf( &ret[ cch ], "/" );
		}
//NSLog( @"%s", s ); //break;


		CGSize size = n_mac_image_text_pixelsize( n_mac_str2nsstring( ret ), font );
		if ( size.width < width_limit ) { break; }
//NSLog( @"%f %f", size.width, width_limit );

		n_memory_free( line[ 1 + i ] );
		line[ 1 + i ] = n_string_path_new( 3 ); n_posix_sprintf_literal( line[ 1 + i ], "..." );


		i++;
		if ( i >= ( c - 1 ) ) { break; } // [!] : the last component will always be visible
	}


	// [!] : clean-up

	i = 0;
	n_posix_loop
	{
		n_string_path_free( line[ i ] );

		i++;
		if ( i >= c ) { break; }
	}

	n_string_path_free( line );


	return ret;
}




@implementation NonnonTxtbox (NonnonTxtboxDraw)


- (BOOL) isFlipped
{
	return YES;
}




- (void) NonnonTxtboxDrawScrollClamp
{

	// [!] : this code is same as drawRect

	n_type_gfx scroll_csy = (n_type_gfx) self.frame.size.height - ( offset_y * 2 );

	const n_type_gfx scroll_pixel_sx = 12;

	if ( n_corner_size == -1 ) { n_corner_size = scroll_pixel_sx; }
//NSLog( @"%f", n_corner_size );

	CGFloat shaft_sy         = scroll_csy - n_corner_size;
	CGFloat items_per_canvas = shaft_sy / font_size.height;
	//CGFloat max_count        = (CGFloat) n_txt_data->sy;
	CGFloat rest             = font_size.height - fmod( shaft_sy, font_size.height );


	CGFloat txt_sy = n_txt_data->sy;

	if ( txt_sy <= 1 )
	{
		scroll = 0;
	} else
	if ( items_per_canvas < txt_sy )
	{
		if ( rest ) { items_per_canvas -= 1.0; }

		CGFloat max_sy = txt_sy - items_per_canvas;

		if ( scroll <       0 ) { scroll =      0; } else
		if ( scroll >= max_sy ) { scroll = max_sy; }
	} else {
		scroll = 0;
	}

//NSLog( @"%lld : %lld %lld", n_focus, caret_fr.cch.y, caret_to.cch.y );

}




- (void) NonnonTxtboxCaretDraw:(void*)zero rect:(NSRect)rect color_bg:(u32)color_bg color_stripe:(u32)color_stripe focus:(n_type_int)focus
{
//return;

	double d;

	if (
		( [self window].keyWindow == FALSE )
		||
		( n_txtbox_first_responder != self )
	)
	{
		d = 0.1;
	} else
	if ( caret_blink_force_onoff )
	{
		d = 1.0;
	} else {
		if ( caret_blink_onoff )
		{
//NSLog( @"On %d", caret_blink_fade.percent );
			d = caret_blink_fade.percent * 0.01;
		} else {
//NSLog( @"Off %d", caret_blink_fade.percent );
			d = caret_blink_fade.percent * 0.01;
			d = 1.0 - d;
		}
	}

//NSLog( @"Percent %f", d );


	u32 color = 0;

	if ( focus & 1 )
	{
		color = color_stripe;
	} else {
		color = color_bg;
	}


	u32 color_caret = 0;

	if ( is_darkmode )
	{
		color_caret = n_bmp_white;
	} else {
		color_caret = n_bmp_black;
	}

	//u32 color_caret = n_mac_nscolor2argb( [NSColor controlAccentColor] );


	color = n_bmp_blend_pixel( color, color_caret, d );

	NSColor *clr = n_mac_argb2nscolor( color );
	n_mac_draw_box( clr, rect );

}

- (void) NonnonTxtboxDrawFindIconMake:(void*)zero color:(u32)color_replace offset:(n_type_gfx)o
{
//return;

	n_bmp bmp; n_bmp_carboncopy( self.n_findbox_bmp_icon, &bmp );


	int replace_r = n_bmp_r( color_replace );
	int replace_g = n_bmp_g( color_replace );
	int replace_b = n_bmp_b( color_replace );

	n_type_gfx x = 0;
	n_type_gfx y = 0;
	n_posix_loop
	{//break;
		u32 c; n_bmp_ptr_get_fast( &bmp, x,y, &c );

		int a = n_bmp_a( c );
		if ( a != 0 )
		{
			int r = replace_r;
			int g = replace_g;
			int b = replace_b;

			c = n_bmp_argb( a,r,g,b );

			n_bmp_ptr_set_fast( &bmp, x,y, c );
		}

		x++;
		if ( x >= N_BMP_SX( &bmp ) )
		{
			x = 0;
			y++;
			if ( y >= N_BMP_SY( &bmp ) ) { break; }
		}
	}


	CGFloat sz = self.frame.size.height - 6;
/*
	int i = 0;
	n_posix_loop
	{
		n_bmp_flush_antialias( &bmp, 1.0 );

		i++;
		if ( sz > ( N_BMP_SY( &bmp ) / i ) ) { break; }
	}

	n_mac_image_alpha_grow( &bmp, color_replace, 3 );
*/

	CGFloat c = ( self.frame.size.height - sz ) / 2;
	CGFloat p = 0;

	if ( find_icon_is_pressed )
	{
		p = 1;
	}

	find_icon_rect = NSMakeRect( o+p,c+p, sz,sz );


//n_bmp_save( &bmp, "/Users/nonnon/Desktop/ret.bmp" );
	find_icon_cache_nsimage = n_mac_image_nbmp2nsimage( &bmp );


	// [!] : find_icon_cache_nsimage seems to use this pointer
	//n_bmp_free_fast( &bmp );


	return;
}

- (NSRect) NonnonTxtboxDrawTextAdjust:(NSString*) text rect:(NSRect) rect
{

	CGSize size = n_mac_image_text_pixelsize( text, font );

	CGFloat cx = ( rect.size.width  - size.width  ) / 2;
	CGFloat cy = ( rect.size.height - size.height ) / 2;

	NSRect ret = NSMakeRect(
		rect.origin.x    + cx,
		rect.origin.y    + cy,
		rect.size.width  - cx,
		rect.size.height - cy
	);

	return ret;
}

- (void)drawRect:(NSRect)dirtyRect
{
//NSLog( @"drawRect" );

//u32 tick = n_posix_tickcount();

//NSLog( @"%lld", n_test_txt.sy );


	// [x] : Sonoma Xcode 15 : rect has window size at the first run

	{
		NSRect rect_local = [super bounds];
		if (
			( rect_local.size.width  < dirtyRect.size.width  )
			||
			( rect_local.size.height < dirtyRect.size.height )
		)
		{
			dirtyRect = rect_local;
		}
	}


	// [x] : partial redraw : not supported yet

	BOOL whole_redraw;

	if ( ( redraw_fy == -1 )||( redraw_ty == -1 ) )
	{
		whole_redraw = TRUE;

		redraw_fy = 0;
		redraw_ty = n_txt_data->sy;
	} else {
		whole_redraw = FALSE;
	}


	BOOL delete_circle_onoff = FALSE;
	if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
	{
		if ( 0 != n_posix_strlen( n_txt_get( n_txt_data, 0 ) ) )
		{
			delete_circle_onoff = TRUE;
		}
	}


	// [!] : Metrics

	is_darkmode = n_mac_is_darkmode();

	[self NonnonTxtboxDrawScrollClamp];

	n_type_gfx csx = (n_type_gfx) self.frame.size.width;
	n_type_gfx csy = (n_type_gfx) self.frame.size.height;


	caret_centered_offset = 0;

	if (
		( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
		||
		( self.n_mode == N_MAC_TXTBOX_MODE_ONELINE )

	)
	{
		caret_centered_offset = ( ( font_size.height + ( offset_y * 2 ) ) - csy ) / 2;
	} else {
		CGFloat minim = font_size.height + ( offset_y * 2 );
		if ( ( csx < minim )||( csy < minim ) ) { return; }
	}

	if ( caret_fr.cch.y >= n_txt_data->sy ) { caret_fr.cch.y = n_txt_data->sy - 1; }


	CGFloat findbox_border_blend = 0.0;


	// [!] : Metrics 2 : Colors

	NSColor *nscolor_text;
	if ( is_darkmode )
	{
		nscolor_text = n_mac_nscolor_argb( 255,222,222,222 );
	} else {
		nscolor_text = [NSColor textColor];
	}

	NSColor *nscolor_back  = [NSColor textBackgroundColor];
	if ( is_grayed )
	{
		nscolor_text = n_mac_nscolor_blend( nscolor_back, nscolor_text, 0.55 );
		nscolor_back = n_mac_nscolor_blend( nscolor_back, nscolor_text, 0.05 );
	}

	u32        color_bg    = n_mac_nscolor2argb( nscolor_back );
	u32        color_fg    = n_mac_nscolor2argb( nscolor_text );
	u32        color_frame = n_bmp_blend_pixel( color_bg, color_fg, 0.25 );
	NSColor *nscolor_frame = n_mac_argb2nscolor( color_frame );
	NSColor *nscolor_text_normal = nscolor_text;

	NSColor *nscolor_text_highlight;
	if ( is_darkmode )
	{
		nscolor_text_highlight = nscolor_text;
	} else {
		nscolor_text_highlight = nscolor_back;
	}

	u32        color_stripe;
	NSColor *nscolor_stripe;

	if ( is_darkmode )
	{
		  color_stripe = n_bmp_blend_pixel( color_bg, color_fg, 0.10 );
		nscolor_stripe = n_mac_argb2nscolor( color_stripe );
	} else {
		  color_stripe = n_bmp_blend_pixel( color_bg, color_fg, 0.05 );
		nscolor_stripe = n_mac_argb2nscolor( color_stripe );
	}
	
	u32        color_crlf = n_bmp_blend_pixel( color_bg, color_fg, 0.25 );
	NSColor *nscolor_crlf = n_mac_argb2nscolor( color_crlf );

	NSColor *nscolor_accent = [NSColor controlAccentColor];

	NSColor *nscolor_nofocus;
	if ( is_darkmode )
	{
		nscolor_nofocus = n_mac_nscolor_argb( 255,100,100,100 );
	} else {
		nscolor_nofocus = n_mac_nscolor_argb( 255,200,200,200 );
	}

#ifdef N_TXTBOX_IME_ENABLE
	//NSColor *nscolor_ime = [nscolor_accent blendedColorWithFraction:0.33 ofColor:nscolor_stripe];
	NSColor *nscolor_ime = nscolor_accent;
#endif


	// [!] : Contents

	if (
		( redraw_fy == 0 )
		&&
		( redraw_ty == n_txt_data->sy )
	)
	{
		if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
		{
			//
		} else {
			n_mac_draw_box( nscolor_back, self.frame );
		}
	}
//n_mac_draw_box( n_mac_nscolor_argb( 255,0,200,255 ), self.frame );


	// [!] : Find Icon

	if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
	{
		if ( self.n_findbox_bmp_icon != NULL )
		{
			// [!] : for reducing CPU usage

			BOOL focus_cur = ( n_txtbox_first_responder == self );
//NSLog( @"Prev %d : Cur %d", find_icon_focus_prev, focus_cur );


			BOOL fade_init = FALSE;

			static int fade_type = 0;

			if ( ( focus_cur )&&( find_icon_focus_prev == FALSE ) ) 
			{
//NSLog( @"Up" );
				fade_init = TRUE;
				fade_type = 1;
			} else
			if ( ( focus_cur == FALSE )&&( find_icon_focus_prev ) ) 
			{
//NSLog( @"Down" );
				fade_init = TRUE;
				fade_type = 2;
			}

			find_icon_focus_prev = focus_cur;

			if ( fade_init )
			{
//NSLog( @"Init" );
				fade_init = FALSE;

				n_bmp_fade_init( &find_icon_fade, n_bmp_black );
				n_bmp_fade_go  ( &find_icon_fade, n_bmp_white );
			}


			BOOL blend_onoff = FALSE;

			n_type_real d = 1.0;
			if ( find_icon_fade.stop == n_posix_false )
			{
				blend_onoff = TRUE;

				d = find_icon_fade.percent * 0.01;
				if ( fade_type == 2 ) { d = 1.0 - d; }
			}


			u32 color_nrml = n_bmp_blend_pixel( color_fg, color_bg, 0.25 );
			u32 color_main;

			if ( blend_onoff )
			{
				color_main = n_bmp_blend_pixel( color_nrml, n_bmp_white, d );
			} else
			if ( focus_cur )
			{
				color_main = n_bmp_white;
			} else {
				color_main = color_nrml;
			}


			// [!] : for reducing CPU usage

			static u32  prv_color = 0;
			static BOOL prv_press = FALSE;

			if ( ( prv_color == 0 )||( prv_color != color_main ) )
			{
				[self NonnonTxtboxDrawFindIconMake:NULL color:color_main offset:offset_x];
			} else
			if ( prv_press != find_icon_is_pressed )
			{
				[self NonnonTxtboxDrawFindIconMake:NULL color:color_main offset:offset_x];
			}
//NSLog( @"%x %x", prv_color, color_main );
//NSLog( @"%d %d", prv_press, find_icon_is_pressed );
//NSLog( @"%d", find_icon_fade.percent );

			prv_color = color_main;
			prv_press = find_icon_is_pressed;


			NSColor *nscolor_bg;// = nscolor_back;
			NSColor *nscolor_fg = [NSColor controlAccentColor];

			if ( focus_cur )
			{
				nscolor_bg = nscolor_back;
			} else {
				nscolor_bg = nscolor_stripe;
			}

			if ( blend_onoff )
			{
				NSColor *nscolor = n_mac_nscolor_blend( nscolor_bg, nscolor_fg, d );
				n_mac_draw_circle( nscolor, find_icon_rect );

				findbox_border_blend = d;
			} else
			if ( focus_cur )
			{
				n_mac_draw_circle( nscolor_fg, find_icon_rect );

				findbox_border_blend = 1.0;
			} else {
				//n_mac_draw_circle( nscolor_bg, find_icon_rect );

				findbox_border_blend = 0.0;
			}

			NSRect rect_local = n_mac_rect_resize( find_icon_rect, -2 );
			[find_icon_cache_nsimage drawInRect:rect_local];
		}
	}


	// [!] : n_txt rendering engine

//NSLog( @"%f %f", caret_fr.pxl.x, caret_to.pxl.x );

	NSMutableDictionary *attr = [NSMutableDictionary dictionary];
	[attr setObject:font forKey:NSFontAttributeName];

	NSMutableDictionary *attr_crlf = [NSMutableDictionary dictionary];
	[attr_crlf setObject:linenumber_font forKey:NSFontAttributeName];
	[attr_crlf setObject:nscolor_crlf    forKey:NSForegroundColorAttributeName];

#ifdef N_TXTBOX_IME_ENABLE

	NSMutableDictionary *attr_ime = [NSMutableDictionary dictionary];
	[attr_ime setObject:font forKey:NSFontAttributeName];
	//[attr_ime setObject:[NSNumber numberWithInt:NSUnderlineStyleDouble] forKey:NSUnderlineStyleAttributeName ];

#endif

	NSRect listbox_rect = NSMakeRect( 0,0,0,0 );

#ifdef N_TXTBOX_IME_ENABLE

	NSRect  underline_rect = NSMakeRect( -1,-1,-1,-1 );
	CGFloat underline_fx   = -1;
	CGFloat underline_tx   = -1;

#endif


	// [!] : Background : before text rendering : for some fonts like "g"

	NSRect rect_main = NSMakeRect( padding, offset_y - caret_centered_offset, self.frame.size.width - ( offset_x * 2 ), font_size.height );

	CGFloat max_sy = self.frame.size.height - ( offset_y * 2 );

	{

		NSRect rect_local = rect_main;

		n_type_int i = scroll;
		n_posix_loop
		{//break;

			NSColor *nscolor;
			if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
			{
				nscolor = nscolor_back;
			} else
			if ( i & 1 )
			{
				nscolor = nscolor_stripe;
			} else {
				nscolor = nscolor_back;
			}

			nscolor = n_txtbox_thin_highlight( n_txt_data, n_txt_deco, i, nscolor, nscolor_accent );

			if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
			{
				n_mac_draw_box( nscolor, self.frame );
			} else {
				n_mac_draw_box( nscolor, rect_local );
			}

			rect_local.origin.y += font_size.height;
			if ( rect_local.origin.y > max_sy ) { break; }

			i++;
		}

	}


	// [!] : Background : and calculation for caret

	NSRect ime_caret_rect = NSMakeRect( 0,0,0,0 );

	n_type_int i = scroll;
//NSLog( @"%f %lld", scroll, n_mac_listbox_txt.sy );
	n_posix_loop
	{//break;

		if ( ( redraw_fy <= i )&&( i < redraw_ty )&&( i < n_txt_data->sy ) )
		{

			n_posix_char *line = n_txt_get( n_txt_data, i );

			n_type_int index = 0;
			CGFloat    sx    = 0;
			n_type_int tab   = 0;
			n_posix_loop
			{//break;
				if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

				CGSize     char_size;
				n_type_int char_index;

				n_posix_char *character;
				character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );
//NSLog( @"#%lld : %ld", i, strlen( character ) );

//NSLog( @"%lld", char_index );
				index += char_index;
				sx    += char_size.width;
			}


			// [!] : End-of-Line Marker

			if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
			{
				//
			} else
			if ( self.n_mode == N_MAC_TXTBOX_MODE_ONELINE )
			{
				//
			} else
			if ( self.n_mode == N_MAC_TXTBOX_MODE_LISTBOX )
			{
				//
			} else {
				NSRect char_rect = rect_main;
				char_rect.origin.x += sx;

				NSString *crlf = @"\xe2\x86\xa9";
				[crlf drawInRect:char_rect withAttributes:attr_crlf];
			}


#ifdef N_TXTBOX_IME_ENABLE

			if ( ( ime_onoff )&&( i == n_focus ) )
			{

				n_posix_char *line = n_mac_nsstring2str( ime_nsstr );

				n_type_int index  = 0;
				CGFloat    sx     = ime_caret_fr.pxl.x;
				n_type_int tab    = 0;
				n_type_int glyph  = 0;
				n_type_int ime_sx = sx;
				n_posix_loop
				{//break;
					if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

					CGSize     char_size;
					n_type_int char_index;
					NSRect     char_rect = rect_main;

					n_posix_char *character;
					character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );
//NSLog( @"#%lld : %ld", i, strlen( character ) );

					char_rect.origin.x   += sx;
					char_rect.origin.x   -= 1;
					char_rect.size.width  = char_size.width + 1;


					index += char_index;
					sx    += char_size.width;
					glyph += 1;
					if ( glyph == ime_caret_offset ) { ime_sx = sx; }
				}

				n_string_free( line );


				// [!] : Fake Caret : IME version

				{
					n_type_gfx oy = (n_type_gfx) scroll * font_size.height;
					n_type_gfx  x = (n_type_gfx) padding + (n_type_gfx) ime_sx;
					n_type_gfx  y = (n_type_gfx) offset_y + ime_caret_fr.pxl.y - oy - caret_centered_offset;
					n_type_gfx sx = (n_type_gfx) N_MAC_TXTBOX_CARET_SIZE;
					n_type_gfx sy = (n_type_gfx) font_size.height;

					ime_caret_rect = NSMakeRect( x,y,sx,sy );
				}

			}
#endif

		}


		rect_main.origin.y += font_size.height;
		if ( rect_main.origin.y > max_sy ) { break; }


		i++;
	}


	// [!] : Fake Caret #1 : IME is later applied

//NSLog( @"%d %d", ime_onoff, ime_delay );

	if ( ime_onoff )
	{
		//[self NonnonTxtboxCaretDraw:nil rect:ime_caret_rect color_bg:color_bg color_stripe:color_stripe focus:n_focus];
	} else
	if ( ( self.n_mode == N_MAC_TXTBOX_MODE_LISTBOX )&&( self.n_listbox_edit_onoff == FALSE ) )
	{
		//
	} else
	if ( delete_circle_fade_pressed_phase )
	{
		//
	} else
	if ( [self NonnonTxtboxCaretIsOnScreen] )
	{
		if ( ( delete_circle_onoff )&&( caret_pt.x > ( csx - csy + offset_x ) ) )
		{
			//
		} else {
			NSRect rect_local = NSMakeRect( caret_pt.x, caret_pt.y - caret_centered_offset, N_MAC_TXTBOX_CARET_SIZE, font_size.height );
			[self NonnonTxtboxCaretDraw:nil rect:rect_local color_bg:color_bg color_stripe:color_stripe focus:n_focus];
		}
	}


	// [!] : Main

	rect_main = NSMakeRect( padding, offset_y - caret_centered_offset, self.frame.size.width - ( offset_x * 2 ), font_size.height );

	max_sy = self.frame.size.height - ( offset_y * 2 );

	i = scroll;
//NSLog( @"%f %lld", scroll, n_mac_listbox_txt.sy );
	n_posix_loop
	{//break;

		if ( self.n_mode == N_MAC_TXTBOX_MODE_LISTBOX )
		{
			if ( i == self.n_focus )
			{
				if ( self.n_listbox_edit_onoff )
				{
					listbox_rect = rect_main;
					listbox_rect.size.width = csx - padding - offset_x;
				} else {
					if ( n_txt_data->readonly )
					{
						n_mac_draw_box( nscolor_nofocus, rect_main );
					} else {
						n_mac_draw_box( nscolor_accent , rect_main );
					}
				}
			}
		}

		if ( ( redraw_fy <= i )&&( i < redraw_ty )&&( i < n_txt_data->sy ) )
		{
//[text drawInRect:r withAttributes:attr];

			n_posix_char *line = n_txt_get( n_txt_data, i );


			// [!] : don't use edit : currently not supported

			n_posix_char *line_ellipsis = NULL;
			if ( n_path_ellipsis_onoff )
			{
				n_type_int cch = n_posix_strlen( line );
				if ( ( cch >= n_path_ellipsis_offset )&&( line[ n_path_ellipsis_offset ] == '/' ) )
				{
					line_ellipsis = n_mac_txtbox_path_ellipsis( &line[ n_path_ellipsis_offset ], font, (n_type_real) csx * 0.8 );
//NSLog( @"%s", line_ellipsis );
					if ( n_path_ellipsis_offset != 0 )
					{
						n_posix_char *l = n_string_alloccopy( cch * 10, line );
						n_posix_sprintf_literal( &l[ n_path_ellipsis_offset ], "%s", line_ellipsis );
						n_string_path_free( line_ellipsis );
						line_ellipsis = l;
					}

					line = line_ellipsis;
				}
			}


			if ( ( n_txt_deco != NULL )&&( n_txt_data->sy == n_txt_deco->sy ) )
			{
				n_posix_char *deco = n_txt_get( n_txt_deco, i );

				// [x] : italic : no better way

				if ( ( self.n_listbox_edit_onoff )&&( i == n_focus ) )
				{
					NSNumber *n = [NSNumber numberWithDouble:0.0];
					[attr setObject:n forKey:NSObliquenessAttributeName ];

					NSNumber *u = [NSNumber numberWithInt:NSUnderlineStyleNone];
					[attr setObject:u forKey:NSUnderlineStyleAttributeName ];
				} else
				if ( deco[ 0 ] == ' ' )
				{
					NSNumber *n = [NSNumber numberWithDouble:0.2];
					[attr setObject:n forKey:NSObliquenessAttributeName ];

					NSNumber *u = [NSNumber numberWithInt:NSUnderlineStyleNone];
					[attr setObject:u forKey:NSUnderlineStyleAttributeName ];
				} else
				if ( deco[ 0 ] == 'B' )
				{
					NSNumber *n = [NSNumber numberWithDouble:0.0];
					[attr setObject:n forKey:NSObliquenessAttributeName ];

					NSNumber *u = [NSNumber numberWithInt:NSUnderlineStyleNone];
					[attr setObject:u forKey:NSUnderlineStyleAttributeName ];
				} else
				if ( deco[ 0 ] == 'u' )
				{
					NSNumber *n = [NSNumber numberWithDouble:0.2];
					[attr setObject:n forKey:NSObliquenessAttributeName ];

					NSNumber *u = [NSNumber numberWithInt:NSUnderlineStyleSingle];
					[attr setObject:u forKey:NSUnderlineStyleAttributeName ];
				} else
				if ( deco[ 0 ] == 'U' )
				{
					NSNumber *n = [NSNumber numberWithDouble:0.0];
					[attr setObject:n forKey:NSObliquenessAttributeName ];

					NSNumber *u = [NSNumber numberWithInt:NSUnderlineStyleSingle];
					[attr setObject:u forKey:NSUnderlineStyleAttributeName ];
				}
			}


			BOOL    gradient_onoff  = FALSE;
			NSRect  gradient_rect   = NSMakeRect( 0,0,0,0 );
			CGFloat gradient_cut    = (CGFloat) csx - csy - csy;
			BOOL    roundrect_onoff = FALSE;

			if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
			{
				roundrect_onoff = TRUE;

				n_type_int index = 0;
				CGFloat    sx    = 0;
				n_type_int tab   = 0;
				n_posix_loop
				{//break;
					if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

					CGSize     char_size;
					n_type_int char_index;
					NSRect     char_rect = rect_main;

					n_posix_char *character;
					character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );

					char_rect.origin.x   += sx;
					char_rect.origin.x   -= 1;
					char_rect.size.width  = char_size.width + 1;


					index += char_index;
					sx    += char_size.width;

//NSLog( @"%f %f", ceil( sx ), (CGFloat) csx - csy - csy );
					if ( ceil( sx ) > gradient_cut )
					{
						gradient_onoff = TRUE;

						gradient_rect = char_rect;
						gradient_rect.origin.x   = gradient_cut - csy;
						gradient_rect.size.width = csx - gradient_rect.origin.x;

						roundrect_onoff = FALSE;

						break;
					}
				}
			}


			int round_mode = N_MAC_DRAW_ROUNDRECT_LEFT;

			{
				n_type_int index = 0;
				CGFloat    sx    = 0;
				n_type_int tab   = 0;
				n_posix_loop
				{//break;
					if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

					CGSize     char_size;
					n_type_int char_index;
					NSRect     char_rect = rect_main;

					n_posix_char *character;
					character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );

					char_rect.origin.x   += sx;
					char_rect.origin.x   -= 1;
					char_rect.size.width  = char_size.width + 1;

					NSPoint pt = NSMakePoint( sx, i );

					sx += char_size.width;

					BOOL selected = n_mac_txtbox_is_selected( pt, caret_fr.pxl.x, caret_fr.cch.y, caret_to.pxl.x, caret_to.cch.y );
					if ( selected )
					{
						BOOL focus = ( n_txtbox_first_responder == self );

						NSColor *nscolor;
						if ( focus )
						{
							nscolor = nscolor_accent;
						} else {
							nscolor = nscolor_nofocus;
						}

						if ( delete_circle_fade_pressed.stop == FALSE )
						{
							NSColor *f = nscolor;
							NSColor *t = [NSColor whiteColor];

							nscolor = n_mac_nscolor_blend( f, t, delete_circle_fade_pressed.percent * 0.01 );
						}

						if ( ( ( gradient_onoff ) )&&( ceil( sx ) > gradient_rect.origin.x ) )
						{
							n_mac_draw_gradient( nscolor, nscolor_back, gradient_rect );

							break;
						} else
						if ( roundrect_onoff )
						{
							const n_type_gfx round_param = 3;
							n_mac_draw_roundrect_partial( nscolor, char_rect, round_param, round_mode );

							if (
								( line[ index + 1 ] != N_STRING_CHAR_NUL )
								&&
								( line[ index + 2 ] == N_STRING_CHAR_NUL )
							)
							{
								round_mode = N_MAC_DRAW_ROUNDRECT_RIGHT;
							} else {
								round_mode = N_MAC_DRAW_ROUNDRECT_NONE;
							}
						} else {
							n_mac_draw_box( nscolor, char_rect );
						}
					}

					index += char_index;

				}
			}


			n_type_int index = 0;
			CGFloat    sx    = 0;
			n_type_int tab   = 0;
			n_posix_loop
			{//break;
				if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

				CGSize     char_size;
				n_type_int char_index;
				NSRect     char_rect = rect_main;

				n_posix_char *character;
				character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );
//NSLog( @"#%lld : %ld", i, strlen( character ) );

				char_rect.origin.x   += sx;
				char_rect.origin.x   -= 1;
				char_rect.size.width  = char_size.width + 1;


				NSPoint pt = NSMakePoint( sx, i );

				BOOL selected = n_mac_txtbox_is_selected( pt, caret_fr.pxl.x, caret_fr.cch.y, caret_to.pxl.x, caret_to.cch.y );
				if ( ( self.n_mode == N_MAC_TXTBOX_MODE_LISTBOX )&&( self.n_listbox_edit_onoff == FALSE ) )
				{
					if ( i == self.n_focus )
					{
						selected = TRUE;
					}
				}


				if ( line[ index ] == N_STRING_CHAR_TAB )
				{

					NSRect rect_local = char_rect; rect_local.size.width = 0.5;

					n_mac_draw_box( nscolor_crlf, rect_local );

				} else
				//
				{

					NSColor *nscolor;
					if ( selected )
					{
						nscolor = nscolor_text_highlight;
					} else {
						nscolor = nscolor_text_normal;
					}
	
					if ( delete_circle_fade_pressed.stop == FALSE )
					{
						NSColor *f = nscolor;
						NSColor *t = [NSColor whiteColor];

						nscolor = n_mac_nscolor_blend( f, t, delete_circle_fade_pressed.percent * 0.01 );
					}

					[attr setObject:nscolor forKey:NSForegroundColorAttributeName];

					NSString *text = n_mac_str2nsstring( character );
					char_rect = [self NonnonTxtboxDrawTextAdjust:text rect:char_rect];
					[text drawInRect:char_rect withAttributes:attr];
//NSLog( @"%@", text );

				}

//NSLog( @"%lld", char_index );
				index += char_index;
				sx    += char_size.width;

				if ( gradient_onoff )
				{
//NSLog( @"%f %f", sx, (CGFloat) csx - csy - csy - o );
					if ( sx >= ( csx - csy - csy - offset_x - csy ) )
					{
						nscolor_text_normal = n_mac_nscolor_blend( nscolor_text_normal, nscolor_stripe, 0.5 );
					}
				}

			}


#ifdef N_TXTBOX_IME_ENABLE

			if ( ( ime_onoff )&&( i == n_focus ) )
			{

				n_posix_char *line = n_mac_nsstring2str( ime_nsstr );

				n_type_int range_f = ime_focus.location;
				n_type_int range_t = ime_focus.location + ime_focus.length;
				//if ( range_f == range_t ) { range_f = 0; }
//NSLog( @"%lld %lld", range_f, range_t );

				n_type_int index  = 0;
				CGFloat    sx     = ime_caret_fr.pxl.x;
				n_type_int tab    = 0;
				n_type_int glyph  = 0;
				n_type_int ime_sx = sx;
				n_posix_loop
				{//break;
					if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

					CGSize     char_size;
					n_type_int char_index;
					NSRect     char_rect = rect_main;

					n_posix_char *character;
					character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );
//NSLog( @"#%lld : %ld", i, strlen( character ) );

					char_rect.origin.x   += sx;
					char_rect.origin.x   -= 1;
					char_rect.size.width  = char_size.width + 1;


					// [!] : underline based

					if ( ( glyph >= range_f )&&( glyph < range_t ) )
					{
						if ( underline_fx == -1 ) { underline_fx = char_rect.origin.x; }
						underline_tx = char_rect.origin.x + char_rect.size.width;
					}

					[attr_ime setObject:nscolor_text_normal    forKey:NSForegroundColorAttributeName];


					if ( i & 1 )
					{
						n_mac_draw_box( nscolor_stripe, char_rect );
					} else {
						n_mac_draw_box( nscolor_back  , char_rect );
					}


					//NSString *text = n_mac_str2nsstring( character );
					//char_rect = [self NonnonTxtboxDrawTextAdjust:text rect:char_rect];
					//[text drawInRect:char_rect withAttributes:attr_ime];

					index += char_index;
					sx    += char_size.width;
					glyph += 1;
					if ( glyph == ime_caret_offset ) { ime_sx = sx; }
				}

				n_string_free( line );


				// [!] : Underline : draw later

				underline_rect = NSMakeRect(
					padding + ime_caret_fr.pxl.x,
					rect_main.origin.y + rect_main.size.height - 2,
					sx - ime_caret_fr.pxl.x,
					2
				);

			}

#endif

			n_string_free( line_ellipsis );

		}

		if ( self.n_mode == N_MAC_TXTBOX_MODE_LISTBOX )
		{
			if ( i == self.n_focus )
			{
				if ( self.n_listbox_edit_onoff )
				{
					NSRect rect_local = listbox_rect;

					rect_local.origin.x--;
					rect_local.origin.y--;
					//rect_local.size.width += 2;
					rect_local.size.height += 2;

					n_mac_draw_frame( nscolor_accent, rect_local );
				}
			}
		}


		rect_main.origin.y += font_size.height;
		if ( rect_main.origin.y > max_sy ) { break; }


		i++;
	}


	// [!] : Fake Caret #2 : IME

	if ( ime_onoff )
	{
		[self NonnonTxtboxCaretDraw:nil rect:ime_caret_rect color_bg:color_bg color_stripe:color_stripe focus:n_focus];
	}


	// [!] : Main #2

	rect_main = NSMakeRect( padding, offset_y - caret_centered_offset, self.frame.size.width - ( offset_x * 2 ), font_size.height );

	max_sy = self.frame.size.height - ( offset_y * 2 );

	i = scroll;
//NSLog( @"%f %lld", scroll, n_mac_listbox_txt.sy );
	n_posix_loop
	{//break;

		if ( ( redraw_fy <= i )&&( i < redraw_ty )&&( i < n_txt_data->sy ) )
		{

#ifdef N_TXTBOX_IME_ENABLE

			if ( ( ime_onoff )&&( i == n_focus ) )
			{

				n_posix_char *line = n_mac_nsstring2str( ime_nsstr );

				n_type_int index  = 0;
				CGFloat    sx     = ime_caret_fr.pxl.x;
				n_type_int tab    = 0;
				n_type_int glyph  = 0;
				n_type_int ime_sx = sx;
				n_posix_loop
				{//break;
					if ( line[ index ] == N_STRING_CHAR_NUL ) { break; }

					CGSize     char_size;
					n_type_int char_index;
					NSRect     char_rect = rect_main;

					n_posix_char *character;
					character = n_mac_txtbox_character( font, font_size, (u8*) line, index, &char_size, &char_index, &tab );
//NSLog( @"#%lld : %ld", i, strlen( character ) );

					char_rect.origin.x   += sx;
					char_rect.origin.x   -= 1;
					char_rect.size.width  = char_size.width + 1;


					[attr_ime setObject:nscolor_text_normal    forKey:NSForegroundColorAttributeName];


					NSString *text = n_mac_str2nsstring( character );
					char_rect = [self NonnonTxtboxDrawTextAdjust:text rect:char_rect];
					[text drawInRect:char_rect withAttributes:attr_ime];

					index += char_index;
					sx    += char_size.width;
					glyph += 1;
					if ( glyph == ime_caret_offset ) { ime_sx = sx; }
				}

				n_string_free( line );

			}

#endif

		}


		rect_main.origin.y += font_size.height;
		if ( rect_main.origin.y > max_sy ) { break; }


		i++;
	}


	// [!] : Underline

	if ( underline_rect.origin.x != -1 )
	{

		n_bmp_flip_onoff = n_posix_true;

		n_type_gfx  x = underline_rect.origin.x;
		n_type_gfx  y = underline_rect.origin.y;
		n_type_gfx sx = underline_rect.size.width;
		n_type_gfx sy = 4;//underline_rect.size.height;

		n_type_gfx circle = sy;
		n_type_gfx step   = circle * 1.5;

		n_type_gfx xx = 0;
		n_posix_loop
		{//break;

			NSColor *clr = n_mac_argb2nscolor( color_crlf );
			NSRect   rct = NSMakeRect( x + xx,y,circle,circle );

			if ( underline_fx != -1 )
			{
				if ( ( ( x + xx ) >= underline_fx )&&( ( x + xx ) <= underline_tx ) )
				{
					clr = nscolor_ime;
				}
			}

			n_mac_draw_circle( clr, rct );

			xx += step;
			if ( xx >= sx ) { break; }
		}

		n_bmp_flip_onoff = n_posix_false;
	}


	// [!] : Line Number

	if (
		( self.n_mode == N_MAC_TXTBOX_MODE_EDITBOX )
		||
		( self.n_mode == N_MAC_TXTBOX_MODE_LISTBOX )
	)
	{
//{static int i = 0;NSLog( @"%d", i );i++;}

		n_bmp_flip_onoff = n_posix_true;

		n_type_int     i = 0;
		n_type_gfx pxl_y = 0;
		n_posix_loop
		{

			BOOL semi_indicator = FALSE;

			if ( ( n_txt_deco != NULL )&&( n_txt_data->sy == n_txt_deco->sy ) )
			{
				semi_indicator = TRUE;

				n_posix_char *deco = n_txt_get( n_txt_deco, scroll + i );

				if ( ( self.n_listbox_edit_onoff )&&( i == n_focus ) )
				{
					//
				} else
				if ( deco[ 0 ] == ' ' )
				{
					semi_indicator = FALSE;
				} else
				if ( deco[ 0 ] == 'B' )
				{
					//
				} else
				if ( deco[ 0 ] == 'u' )
				{
					semi_indicator = FALSE;
				} else
				if ( deco[ 0 ] == 'U' )
				{
					//
				}
			}

			n_mac_txtbox_draw_linenumber
			(
				linenumber_font,
				i, scroll, n_txt_data->sy,
				(n_type_int) MIN( caret_fr.cch.y, caret_to.cch.y ),
				(n_type_int) MAX( caret_fr.cch.y, caret_to.cch.y ),
				offset_x, offset_y + pxl_y, linenumber_size.width, font_size.height,
				nscolor_back,
				nscolor_text,
				n_txtbox_thin_highlight( n_txt_data, n_txt_deco, i + scroll, nscolor_back, nscolor_accent ),
				self.n_option_linenumber,
				semi_indicator
			);

			i++;

			pxl_y += font_size.height;
			if ( pxl_y > max_sy ) { break; }

		}

		n_bmp_flip_onoff = n_posix_false;
	}


	// [!] : Delete Circle

	if ( delete_circle_onoff )
	{
		NSColor *color_bg;

		if ( delete_circle_fade_pressed.stop == FALSE )
		{
			NSColor *f = [NSColor  grayColor];
			NSColor *t = [NSColor whiteColor];

			color_bg = n_mac_nscolor_blend( f, t, delete_circle_fade_pressed.percent * 0.01 );
		} else
		if ( delete_circle_is_pressed )
		{
			color_bg = [NSColor blackColor];

			delete_circle_fade_hovered.stop =  TRUE;
			delete_circle_is_hovered        = FALSE;
		} else
		if ( delete_circle_fade_hovered.stop == FALSE )
		{
//NSLog( @"%x : %d", delete_circle_fade.color_fg, delete_circle_fade.percent );

			NSColor *f;
			if ( n_txtbox_first_responder == self )
			{
				f = nscolor_accent;
			} else {
				f = [NSColor grayColor];
			}

			NSColor *t = n_mac_nscolor_argb( 255, 255, 0, 128 );

			if ( delete_circle_fade_hovered.color_fg == n_bmp_white )
			{
				color_bg = n_mac_nscolor_blend( f, t, delete_circle_fade_hovered.percent * 0.01 );
			} else {
				color_bg = n_mac_nscolor_blend( t, f, delete_circle_fade_hovered.percent * 0.01 );
			}
		} else
		if ( delete_circle_is_hovered )
		{
			color_bg = n_mac_nscolor_argb( 255, 255, 0, 128 );
		} else
		if ( n_txtbox_first_responder == self )
		{
			color_bg = nscolor_accent;
		} else {
			color_bg = [NSColor grayColor];
		}

		CGFloat sz = csy - ( offset_y * 2 );
		delete_circle_rect = NSMakeRect( csx - sz - offset_x, offset_y, sz, sz );
		n_mac_draw_circle( color_bg, delete_circle_rect );


		n_gdi gdi; n_gdi_zero( &gdi );

		gdi.sx                  = 15;
		gdi.sy                  = 15;

		gdi.base_color_bg       = n_bmp_white_invisible;
		gdi.base_color_fg       = n_bmp_white_invisible;
		gdi.text                = n_posix_literal( "×" );
		gdi.text_color_main     = n_bmp_white;
		gdi.text_size           = csy;

		n_bmp bmp1; n_bmp_zero( &bmp1 );
		n_gdi_bmp( &gdi, &bmp1 );

		n_bmp bmp2; n_bmp_zero( &bmp2 ); n_bmp_new_fast( &bmp2, gdi.sx, gdi.sy );
		n_bmp_flush( &bmp2, gdi.base_color_bg );
		n_bmp_transcopy( &bmp1, &bmp2, 0,0,gdi.sx,gdi.sy, 0,-1 );

//n_bmp_save( &bmp2, "/Users/nonnon/Desktop/ret.bmp" );

		NSRect rect_local = n_mac_rect_resize( delete_circle_rect, -2 );
//NSLog( @"%f %f", rect.origin.y, rect.size.height );

		n_mac_image_nbmp_direct_draw( &bmp2, &rect_local, NO );

		n_bmp_free_fast( &bmp1 );
		n_bmp_free_fast( &bmp2 );
	}


	// [!] : Scrollbar

	if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
	{
		//
	} else
	if ( self.n_mode == N_MAC_TXTBOX_MODE_ONELINE )
	{
		//
	} else {

		// [!] : Fake Scroller

//NSLog( @"%f %f", page, txt_sy );


		// [!] : this code needs to be the same as NonnonTxtboxDrawScrollClamp

		n_type_gfx scroll_csy = (n_type_gfx) self.frame.size.height - ( offset_y * 2 );

		const n_type_gfx scroll_pixel_sx = 12;

		if ( n_corner_size == -1 ) { n_corner_size = scroll_pixel_sx; }
//NSLog( @"%f", n_corner_size );

		CGFloat shaft_sy         = scroll_csy - n_corner_size;
		CGFloat items_per_canvas = shaft_sy / font_size.height;
		CGFloat max_count        = (CGFloat) n_txt_data->sy;
		CGFloat rest             = font_size.height - fmod( shaft_sy, font_size.height );


		BOOL onoff = FALSE;

//NSLog( @"%f %f", items_per_canvas, max_count );
		if ( trunc( items_per_canvas ) < max_count )
		{
			onoff = TRUE;

			if ( rest ) { items_per_canvas -= 1.0; }
//NSLog( @"Rest %f", rest );

			n_type_gfx scrsx = scroll_pixel_sx;
			n_type_gfx scr_x = csx - offset_x - scrsx;


			CGFloat step = font_size.height * ( shaft_sy / ( max_count * font_size.height ) );
			CGFloat page = max_count / items_per_canvas;

			n_type_gfx scr_y = offset_y + ( scroll * step );
			n_type_gfx scrsy = MAX( 12, scroll_csy / page );

			scroll_step = shaft_sy / ( max_count * font_size.height );
			scroll_page = items_per_canvas;


			// [!] : for hit test
//scroller_rect = NSMakeRect( 0, 0, 0, 0 );
			scroller_rect_shaft = NSMakeRect( scr_x, offset_y, scrsx, shaft_sy );
			scroller_rect_thumb = NSMakeRect( scr_x,    scr_y, scrsx,    scrsy );


			u32 color = color_fg;

			n_bmp_flip_onoff = n_posix_true;

			u32 color_shaft = n_bmp_alpha_replace_pixel( color, 16 );

			{
				NSColor *clr = n_mac_argb2nscolor( color_shaft );
				NSRect   rct = NSMakeRect( scr_x, offset_y, scrsx, shaft_sy );
				n_mac_draw_roundrect( clr, rct, scrsx / 2 );
			}

			if ( n_search_marker != NULL )
			{
				if ( n_search_marker_count != n_txt_data->sy )
				{
					[self NonnonTxtboxSearchMarkerReset:NO];
				} else {
					n_type_int i = 0;
					n_posix_loop
					{
						if ( n_search_marker[ i ] )
						{
							NSRect rect_local = NSMakeRect( scr_x, i * step, scrsx, 1 );
							n_mac_draw_box( N_TXTBOX_INDICATOR_SEARCH, rect_local );
						}

						i++;
						if ( i >= n_txt_data->sy ) { break; }
					}
				}
			}


			int hot    =  64;
			int normal =  96;
			int press  = 128;

			u32 color_thumb;
			if ( scrollbar_fade_captured_onoff )
			{
//NSLog( @"scrollbar_fade_captured_onoff" );

				u32 f = n_bmp_alpha_replace_pixel( color, press  );
				u32 t = n_bmp_alpha_replace_pixel( color, hot    );
				if ( thumb_is_captured )
				{
//NSLog( @"thumb_is_captured" );
					color_thumb = n_bmp_blend_pixel( t, f, (n_type_real) scrollbar_fade.percent * 0.01 );
				} else {
//NSLog( @"else" );
					if ( thumb_is_hovered )
					{
						t = n_bmp_alpha_replace_pixel( color, hot    );
					} else {
						t = n_bmp_alpha_replace_pixel( color, normal );
					}
					color_thumb = n_bmp_blend_pixel( f, t, (n_type_real) scrollbar_fade.percent * 0.01 );
				}
//NSLog( @"%d", scrollbar_fade.percent );
			} else
			if ( scrollbar_fade_hovered_onoff )
			{
//NSLog( @"scrollbar_fade_hovered_onoff" );

				u32 f = n_bmp_alpha_replace_pixel( color, hot    );
				u32 t = n_bmp_alpha_replace_pixel( color, normal );
				if ( thumb_is_captured )
				{
					color_thumb = n_bmp_alpha_replace_pixel( color, press  );
				} else
				if ( thumb_is_hovered )
				{
					color_thumb = n_bmp_blend_pixel( t, f, (n_type_real) scrollbar_fade.percent * 0.01 );
				} else {
					color_thumb = n_bmp_blend_pixel( f, t, (n_type_real) scrollbar_fade.percent * 0.01 );
				}
			} else {
//NSLog( @"else" );

				if ( thumb_is_captured )
				{
					color_thumb = n_bmp_alpha_replace_pixel( color, press  );
				} else
				if ( thumb_is_hovered )
				{
					color_thumb = n_bmp_alpha_replace_pixel( color, hot    );
				} else {
					color_thumb = n_bmp_alpha_replace_pixel( color, normal );
				}
			}

			{
				NSColor *clr = n_mac_argb2nscolor( color_thumb );
				NSRect   rct = NSMakeRect( scr_x,scr_y, scrsx,scrsy );
				n_mac_draw_roundrect( clr, rct, scrsx / 2 );
			}


			// [!] : caret indicator on a shaft

			NSRect rect_local = NSMakeRect( scr_x, offset_y + ( step * n_focus ), scrsx, 1 );
			n_mac_draw_box( N_TXTBOX_INDICATOR_CARET, rect_local );


			n_bmp_flip_onoff = n_posix_false;
		}


		// [!] : right-side margin

		NSRect rect_padding = NSMakeRect( csx - offset_x, 0, offset_x, csy );
		n_mac_draw_box( nscolor_back, rect_padding );


		// [!] : bottom-side margin

		{
			CGFloat o = offset_y;
			if ( n_border_separator_only ) { o -= 2; }

			rect_padding = NSMakeRect( 0, csy - o, csx, o );
			n_mac_draw_box( nscolor_back, rect_padding );
		}

	}
	

	// [!] : Frame

	if ( self.n_mode == N_MAC_TXTBOX_MODE_EDITBOX )
	{
		if ( self.n_border_separator_only )
		{
			NSRect rect_separator = NSMakeRect( 0, 0, csx, 2 );
			n_mac_draw_box( nscolor_frame, rect_separator );
		} else {
			[self setWantsLayer:YES];
			[self.layer setBorderWidth:2];

			CGFloat r = 10;

			[self.layer setBorderColor    :nscolor_frame.CGColor];
			[self.layer setBackgroundColor:nscolor_back .CGColor];

			[self.layer setCornerRadius:r];
		}
	
	} else
	if ( self.n_mode == N_MAC_TXTBOX_MODE_ONELINE )
	{
		[self setWantsLayer:YES];
		[self.layer setBorderWidth:2];

		CGFloat r = csy / 4;

		[self.layer setBorderColor    :nscolor_frame.CGColor];
		[self.layer setBackgroundColor:nscolor_back .CGColor];

		[self.layer setCornerRadius:r];
	} else
	if ( self.n_mode == N_MAC_TXTBOX_MODE_FINDBOX )
	{
		[self setWantsLayer:YES];
		[self.layer setBorderWidth:2];

		CGFloat r = csy / 2;

		NSColor *nscolor_bg = nscolor_frame;
		NSColor *nscolor_fg = [NSColor controlAccentColor];
		NSColor *nscolor    = n_mac_nscolor_blend( nscolor_bg, nscolor_fg, findbox_border_blend );

		if ( n_txtbox_first_responder == self )
		{
			[self.layer setBorderColor    :nscolor       .CGColor];
			[self.layer setBackgroundColor:nscolor_back  .CGColor];
		} else {
			[self.layer setBorderColor    :nscolor       .CGColor];
			[self.layer setBackgroundColor:nscolor_stripe.CGColor];
		}
		[self.layer setCornerRadius:r];
	} else {
		[self setWantsLayer:YES];
		[self.layer setBorderWidth:2];

		CGFloat r = 10;

		[self.layer setBorderColor    :nscolor_frame.CGColor];
		[self.layer setBackgroundColor:nscolor_back .CGColor];

		[self.layer setCornerRadius:r];
	}

/*
	if ( debug )
	{
		n_mac_draw_box( [NSColor blackColor], self.frame );
	}
*/

	redraw_fy = -1;
	redraw_ty = -1;

/*
	{
		NSColor *clr = n_mac_nscolor_argb( 255, 0,200,255 );
		NSRect   rct = NSMakeRect( 100,100,200,200 );
		n_mac_draw_roundrect( clr, rct, 10 );
	}
*/
/*
	{
		NSColor *clr = n_mac_nscolor_argb( 255, 0,200,255 );
		NSRect   rct = NSMakeRect( 100,100,200,200 );
		n_mac_draw_circle( clr, rct );
	}
*/

//NSLog( @"%d", (int) n_posix_tickcount() - tick );
}


@end

