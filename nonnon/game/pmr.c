// Nonnon Game
// copyright (c) nonnon all rights reserved
// License : GPL http://www.gnu.org/copyleft/gpl.html


// [Mechanism] : Poor Man's Rasterizer(PMR)
//
//	1 : Make a folder
//	2 : Make many .BMPs in the folder
//	3 : Done!
//
//	[ .BMP ]
//	+ grayscaled text on black background
//	+ files need to be named as "0000.bmp" (hex of Unicode)
//	+ "0000.bmp" will be alternative for a missing glyph




#ifndef _H_NONNON_WIN32_GAME_PMR
#define _H_NONNON_WIN32_GAME_PMR




#include "../neutral/bmp/all.c"


#include "../neutral/path.c"



/*
void
n_game_pmr_gdi
(
	const n_posix_char *folder,
	const n_posix_char *str,
	      n_posix_char *font,
	      n_type_gfx    sx,
	      n_type_gfx    sy
)
{

	// [ Mechanisnm ] : copy and paste then use it

	// [!] : for font developers
	//
	//	for batch output
	//	use when you don't want to install a font in a taraget machine


	n_posix_char f[ N_PATH_MAX ];

	if ( n_path_is_abspath( folder ) )
	{
		n_string_copy( folder, f );
	} else {
		n_path_getcwd( f );
		n_path_maker( f, folder, f );
	}

	if ( n_posix_false == n_posix_stat_is_exist( f ) )
	{
		n_posix_mkdir( f );
	}


#ifdef UNICODE
	n_posix_char   *s = n_string_carboncopy ( str );
#else // #ifdef UNICODE
	       wchar_t *s = n_posix_ansi2unicode( str );
#endif // #ifdef UNICODE


	// Template

	n_posix_char text[ 2 ];

	n_gdi gdi;
	n_gdi_zero( &gdi );

	gdi.sx             = sx;
	gdi.sy             = sy;
	gdi.style          = N_GDI_SMOOTH;

	gdi.base_color_bg  = n_bmp_black;
	gdi.base_color_fg  = n_bmp_black;
	gdi.base_style     = N_GDI_BASE_DEFAULT;

	gdi.frame_style    = N_GDI_FRAME_NOFRAME;

	gdi.text           = text;
	gdi.text_font      = font;
	gdi.text_size      = sy;
	gdi.text_color_fg1 = n_bmp_white;
	gdi.text_color_bg1 = n_bmp_white;
	gdi.text_style     = N_GDI_TEXT_DEFAULT;
	gdi.text_fxsize    = 0;


	n_posix_char name[ N_PATH_MAX ];

	n_type_int i = 0;
	n_posix_loop
	{//break;

		n_posix_sprintf_literal( name, "%s%s%04x.bmp", f, N_POSIX_SLASH, s[ i ] );
//n_posix_debug( name );

		n_posix_sprintf_literal( text, "%c", str[ i ] );


		n_bmp bmp; n_bmp_zero( &bmp );

		n_gdi_bmp( &gdi, &bmp );

		n_bmp_save( &bmp, name );

		n_bmp_free( &bmp );


		i++;
		if ( str[ i ] == N_STRING_CHAR_NUL ) { break; }
	}


	n_memory_free( s );


	return;
}
*/
/*
void
n_game_pmr_resize( n_posix_char *path, n_type_real ratio_x, n_type_real ratio_y, n_posix_bool resample_onoff )
{

	// [ Mechanisnm ] : copy and paste then use it

	// [!] : for font developers
	//
	//	for batch maintenance
	//	use when you make re-scaled font


	n_dir d; n_dir_zero( &d );
	if ( n_dir_load( &d, path ) ) { return; }


	n_type_int i = 0;
	n_posix_loop
	{

		if ( n_string_is_same_literal( ".bmp", n_dir_ext( &d, i ) ) )
		{

			n_posix_char path[ N_PATH_MAX ];
			n_path_maker( n_dir_path( &d, i ), n_dir_name( &d, i ), path );

			n_bmp bmp;
			n_bmp_zero( &bmp );
			n_bmp_load( &bmp, path );

			if ( resample_onoff )
			{

				n_bmp_resampler( &bmp, ratio_x, ratio_y );

			} else {

				n_type_gfx sx = (n_type_real) N_BMP_SX( &bmp ) * ratio_x;
				n_type_gfx sy = (n_type_real) N_BMP_SY( &bmp ) * ratio_y;

				n_bmp_resizer( &bmp, sx,sy, n_bmp_trans, N_BMP_RESIZER_CENTER );

			}

			n_bmp_save( &bmp, path );
			n_bmp_free( &bmp );

		}

		i++;
		if ( i >= n_dir_all( &d ) ) { break; }
	}


	n_dir_free( &d );


	return;
}
*/

#define N_GAME_PMR_DRAW_L2R 0
#define N_GAME_PMR_DRAW_R2L 1
#define N_GAME_PMR_DRAW_U2D 2
#define N_GAME_PMR_DRAW_D2U 3

// [ Mechanism ]
//
//	n_game_pmr_calc( pmr_folder, string, size_needed, &sx, &sy, &ratio, direction );
//	n_game_pmr_draw( pmr_folder, string, &bmp_canvas, x, y, ratio, direction );

#define n_game_pmr_calc( f,s, z, sx,sy, r,    d ) n_game_pmr_draw_internal( f,s, z, n_posix_true ,   sx,  sy,   r, NULL, 0,0, 0, 0, d )
#define n_game_pmr_draw( f,s,    b,x,y, r, c, d ) n_game_pmr_draw_internal( f,s, 0, n_posix_false, NULL,NULL,NULL,    b, x,y, r, c, d )

void
n_game_pmr_draw_internal
(
	const n_posix_char *folder,
	const n_posix_char *str,
	      n_type_gfx    size_needed,
	      n_posix_bool  calc_only,
	      n_type_gfx   *ret_sx,
	      n_type_gfx   *ret_sy,
	      n_type_real  *ret_ratio,
	      n_bmp        *bmp,
	      n_type_gfx    x,
	      n_type_gfx    y,
	      n_type_real   ratio,
	      u32           color,
	      int           direction
)
{

	// [!] : currently single line only


	if ( n_string_is_empty( str ) ) { return; }


	n_posix_char f[ N_PATH_MAX ];

	if ( n_path_is_abspath( folder ) )
	{
		n_string_copy( folder, f );
	} else {
		n_path_getcwd( f );
		n_path_maker( f, folder, f );
	}


#ifdef UNICODE
	n_posix_char   *s = n_string_carboncopy ( str );
#else // #ifdef UNICODE
	       wchar_t *s = n_posix_ansi2unicode( str );
#endif // #ifdef UNICODE


	n_posix_char name[ N_PATH_MAX ];

	n_type_gfx xx = 0;
	n_type_gfx yy = 0;

	n_type_int i = 0;
	n_posix_loop
	{//break;

		n_posix_sprintf_literal( name, "%s%s%04x.bmp", f, N_POSIX_SLASH, s[ i ] );
//n_posix_debug( name );

		n_bmp bmp_char; n_bmp_zero( &bmp_char );

		if ( n_bmp_load( &bmp_char, name ) )
		{

			// [!] : fallback

			n_posix_sprintf_literal( name, "%s%s0000.bmp", f, N_POSIX_SLASH );

			if ( n_bmp_load( &bmp_char, name ) )
			{
				n_bmp_new( &bmp_char, 1,1 );
			}

		}

		if ( size_needed != N_BMP_SY( &bmp_char ) )
		{
			n_type_real r = (n_type_real) size_needed / N_BMP_SY( &bmp_char );
			if ( ret_ratio != NULL ) { (*ret_ratio) = ratio = r; }
		}

		n_bmp_resampler( &bmp_char, ratio, ratio );

		if ( calc_only == n_posix_false )
		{
			n_bmp_rasterizer( &bmp_char, bmp, x + xx, y + yy, color, n_posix_false );
		}

		if ( direction == N_GAME_PMR_DRAW_L2R )
		{
			xx += N_BMP_SX( &bmp_char );

			if ( ret_sx != NULL ) { (*ret_sx) = xx; }
			if ( ret_sy != NULL ) { (*ret_sy) = N_BMP_SY( &bmp_char ); }
		} else
		if ( direction == N_GAME_PMR_DRAW_R2L )
		{
			xx -= N_BMP_SX( &bmp_char );

			if ( ret_sx != NULL ) { (*ret_sx) = xx; }
			if ( ret_sy != NULL ) { (*ret_sy) = N_BMP_SY( &bmp_char ); }
		} else
		if ( direction == N_GAME_PMR_DRAW_U2D )
		{
			yy += N_BMP_SY( &bmp_char );

			if ( ret_sx != NULL ) { (*ret_sx) = N_BMP_SX( &bmp_char ); }
			if ( ret_sy != NULL ) { (*ret_sy) = yy; }
		} else
		if ( direction == N_GAME_PMR_DRAW_D2U )
		{
			yy -= N_BMP_SY( &bmp_char );

			if ( ret_sx != NULL ) { (*ret_sx) = N_BMP_SX( &bmp_char ); }
			if ( ret_sy != NULL ) { (*ret_sy) = yy; }
		}// else

		n_bmp_free( &bmp_char );


		i++;
		if ( str[ i ] == N_STRING_CHAR_NUL ) { break; }
	}


	n_memory_free( s );


	return;
}


#endif // _H_NONNON_WIN32_GAME_PMR

