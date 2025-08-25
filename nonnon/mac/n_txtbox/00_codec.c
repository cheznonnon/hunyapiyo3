// CatPad
// copyright (c) nonnon all rights reserved
// License : GPL http://www.gnu.org/copyleft/gpl.html




#ifndef _H_NONNON_MAC_TXTBOX_00_CODEC
#define _H_NONNON_MAC_TXTBOX_00_CODEC




#define N_TXT_UNICODE_UTF8_NO_BOM 4




void
n_txt_utf8_new( n_txt *txt )
{

	int newline = txt->newline;
	int unicode = txt->unicode;

	n_txt_new( txt );

	txt->newline = newline;
	txt->unicode = unicode;


	return;
}

n_posix_bool
n_txt_load_utf8_is_binary( void *stream, n_type_int byte )
{

	n_posix_bool  ret = n_posix_false;
	char         *ptr = stream;


	n_type_int i = 0;
	n_posix_loop
	{
		if ( i >= byte ) { break; }

		if ( ptr[ i ] == 0 ) { ret = n_posix_true; break; }

		i++;
	}


	return ret;
}

#define n_txt_load_utf8(          v, fname        ) n_txt_load_utf8_internal( v, fname,   NULL,    0 )
#define n_txt_load_utf8_onmemory( v, stream, byte ) n_txt_load_utf8_internal( v,  NULL, stream, byte )

// internal
n_posix_bool
n_txt_load_utf8_internal( n_txt *txt, const n_posix_char *fname, void *stream, n_type_int byte )
{

	// [!] : load_onmemory() : don't free "stream" after calling this function


	if ( txt == NULL ) { return n_posix_true; }


	n_type_int bom_offset = 0;


	txt->unicode = N_TXT_UNICODE_UTF8_NO_BOM;

	if ( stream == NULL )
	{

		FILE *fp = n_posix_fopen_read( fname );
		if ( fp == NULL ) { return n_posix_true; }

		byte = n_posix_stat_size( fname );


		// [!] : remember original spec

		u8 sniffer[ 3 ]; n_posix_fread( sniffer, 3, 1, fp );

		if ( ( byte >= 3 )&&( n_unicode_bom_is_utf8    ( sniffer, 3 ) ) )
		{
			txt->unicode = N_TXT_UNICODE_UTF;
			n_posix_fseek( fp, 3, SEEK_SET );
			byte -= 3;
		} else
		if ( ( byte >= 2 )&&( n_unicode_bom_is_utf16_le( sniffer, 2 ) ) )
		{
			txt->unicode = N_TXT_UNICODE_LIL;
			n_posix_fseek( fp, 0, SEEK_SET );
		} else
		if ( ( byte >= 2 )&&( n_unicode_bom_is_utf16_be( sniffer, 2 ) ) )
		{
			txt->unicode = N_TXT_UNICODE_BIG;
			n_posix_fseek( fp, 0, SEEK_SET );
		} else {
			txt->unicode = N_TXT_UNICODE_UTF8_NO_BOM;
			n_posix_fseek( fp, 0, SEEK_SET );
		}
//NSLog( @"unicode : %d", txt->unicode );

		stream = n_memory_new( byte );
		n_posix_fread( stream, byte, 1, fp );

		n_posix_fclose( fp );

	} else {

		bom_offset = 0;

	}


	// Decoder

	if ( txt->unicode == N_TXT_UNICODE_UTF8_NO_BOM )
	{
//NSLog( @"N_TXT_UNICODE_UTF8_NO_BOM" );

		n_type_int   cb = byte;
		char       *ptr = n_unicode_alloccopy( &cb, stream );

		n_unicode_utf8_decode_no_bom( ptr, cb );

		n_posix_bool error  = n_memory_is_same( ptr, stream, byte );
		n_posix_bool binary = n_posix_false;
		if ( error ) { binary = n_txt_load_utf8_is_binary( stream, byte ); }
//NSLog( @"Error %d : Binary : %d", error, binary );

		n_memory_free( ptr );

		if ( binary )
		{
			txt->readonly = n_posix_true;

			txt->unicode = N_TXT_UNICODE_NIL;
			txt->newline = N_TXT_NEWLINE_BINARY;
		} else
		if ( error )
		{
//NSLog( @"Non-Unicode" );
//NSLog( @"Stream %s", stream );
//NSLog( @"Pointer %s", ptr );
//NSLog( @"Byte %d", byte );
//NSLog( @"%d", memcmp( stream, ptr, byte ) );

			txt->readonly = n_posix_true;

			txt->unicode = N_TXT_UNICODE_NIL;
			txt->newline = n_txt_newline_check( stream, byte );

		} else {

			txt->newline = n_txt_newline_check( stream, byte );

		}

	} else
	if ( txt->unicode == N_TXT_UNICODE_UTF )
	{

		txt->newline = n_txt_newline_check( stream, byte );

	} else
	if ( txt->unicode == N_TXT_UNICODE_NIL )
	{

		// [!] : curretly handled as UTF-8 No BOM

		txt->unicode = N_TXT_UNICODE_UTF8_NO_BOM;

		txt->newline = n_txt_newline_check( stream, byte );

	} else {

		char *ptr = n_unicode_alloccopy( &byte, stream );

		if ( txt->unicode == N_TXT_UNICODE_BIG )
		{
			n_unicode_endianness( ptr, byte );
		}

		n_unicode_utf8_encode( ptr, byte );


		n_memory_free( stream );


		// [x] : inefficient way

		byte   = strlen( &ptr[ 3 ] );
		stream = n_string_new( byte );
		sprintf( stream, "%s", &ptr[ 3 ] );


		txt->newline = n_txt_newline_check( stream, byte );

	}


//NSLog( @"%lld %s", byte, stream );

	n_vector_load_onmemory( (n_vector*) txt, stream, byte );

//NSLog( @"%s", txt->stream );

//NSLog( @"%d", txt->readonly );


	return n_posix_false;
}

n_type_int
n_txt_wcslen( u16 *utf_16_str )
{

	n_type_int i = 0;
	n_posix_loop
	{
		if ( utf_16_str[ i ] == 0 ) { break; }

		i++;
	}
	
	return i;
}

n_posix_bool
n_txt_save_utf8( n_txt *txt, const n_posix_char *fname )
{

	if ( n_txt_error( txt ) ) { return n_posix_true; }


	if ( txt->readonly ) { return n_posix_true; }


	n_txt_stream( txt );


	// Encoder

	n_posix_bool  is_allocated;
	char         *ptr;
	n_type_int    byte;

//NSLog( @"%d", txt->unicode );

	if ( ( txt->unicode == N_TXT_UNICODE_LIL )||( txt->unicode == N_TXT_UNICODE_BIG ) )
	{
//NSLog( @"%s", (char*) txt->stream );
/*
		txt->unicode = N_TXT_UNICODE_UTF8_NO_BOM;

		is_allocated = n_posix_false;

		byte = txt->byte;
		ptr  = txt->stream;
*/

		is_allocated = n_posix_true;

		n_type_int cch = n_posix_strlen( txt->stream );

		// [!] : enough size
		byte = cch * sizeof( u16 ) * 10;
		ptr  = n_memory_new( byte );

		n_posix_sprintf_literal( ptr, "%s", (char*) txt->stream );
//NSLog( @"PTR %s", (char*) ptr );

		n_unicode_utf8_decode_no_bom( ptr, byte );
//NSLog( @"n_unicode_utf8_decode_no_bom() : %s", (char*) ptr );

		n_unicode_bom_restore_utf16_le( ptr, byte );

		// [!] : wchar_t is 32-bit
		byte = n_txt_wcslen( (u16*) ptr ) * sizeof( u16 );

		if ( txt->unicode == N_TXT_UNICODE_BIG )
		{
			n_unicode_endianness( ptr, byte );
		}

	} else
	if ( txt->unicode == N_TXT_UNICODE_UTF )
	{

		is_allocated = n_posix_false;

		byte = n_posix_strlen( txt->stream ) + 3;
		ptr  = n_memory_new( byte );

		n_posix_sprintf_literal( ptr, "%s", (char*) txt->stream );

		n_unicode_bom_restore_utf8( ptr, byte );

	} else
	//if ( txt->unicode == N_TXT_UNICODE_UTF8_NO_BOM )
	{

		// [!] : currently non-unicode formats are handled as UTF-8 always

		is_allocated = n_posix_false;

		byte = txt->byte;
		ptr  = txt->stream;

	}


	n_posix_bool ret = n_posix_false;


	FILE *fp = n_posix_fopen_write( fname );
	if ( fp == NULL )
	{
		ret = n_posix_true;
	} else {
		n_posix_fwrite( ptr, byte, 1, fp );
	}
	n_posix_fclose( fp );


	if ( is_allocated ) { n_memory_free( ptr ); }


	return ret;
}




void
n_txt_decoration_init( n_txt *txt, n_txt *txt_deco )
{

	if ( txt_deco == NULL ) { return; }

	n_txt_utf8_new( txt_deco );

	n_type_int i = 0;
	n_posix_loop
	{

		n_txt_set( txt_deco, i, N_STRING_SPACE );

		i++;
		if ( i >= txt->sy ) { break; }
	}


	return;
}




#endif // _H_NONNON_MAC_TXTBOX_00_CODEC

