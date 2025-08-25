// Nonnon Nyaurism
// copyright (c) nonnon all rights reserved
// License : GPL http://www.gnu.org/copyleft/gpl.html




#ifndef _H_NONNON_NEUTRAL_WAV_TRANSFORM
#define _H_NONNON_NEUTRAL_WAV_TRANSFORM




#include "../wav.c"


#include "./_error.c"
#include "./sample.c"




void
n_wav_resampler( n_wav *f, n_wav *t, n_type_real ratio )
{

	if ( n_wav_error_format( f ) ) { return; }
	if ( n_wav_error_format( t ) ) { return; }


	if ( ratio == 1.0 )
	{

		n_wav_free( t );
		n_wav_carboncopy( f, t );

	} else
	if ( ratio < 1.0 )
	{

		// Shrink : bilinear interpolation

		u32 count = N_WAV_COUNT( f );

		n_type_real l, r;

		u32 x = 0;
		n_posix_loop
		{//break;

			n_wav_sample_get( f, x, &l, &r );

			u32 tx = (u32) trunc( (n_type_real) x * ratio );

			n_wav_sample_mix( t, tx, l, r, 0.5, 0.5 );


			x++;
			if ( x >= count ) { break; }
		}

	} else {

		// Enlarge : nearest neighbor

		ratio = 1.0 / ratio;

		u32 count = N_WAV_COUNT( t );

		n_type_real l, r;

		u32 x = 0;
		n_posix_loop
		{//break;

			u32 fx = (u32) trunc( (n_type_real) x * ratio );

			n_wav_sample_get( f, fx, &l, &r );

			// [!] : something is needed

			n_wav_sample_set( t,  x,  l,  r );


			x++;
			if ( x >= count ) { break; }
		}

	}


	return;
}

#define N_WAV_RESIZER_NORMAL   0
#define N_WAV_RESIZER_CENTER   1
#define N_WAV_RESIZER_RESAMPLE 2

void
n_wav_resizer( n_wav *wav, u32 msec, int mode )
{

	if ( n_wav_error_format( wav ) ) { return; }


	if ( msec == 0 ) { return; }


	n_posix_bool is_shrink = n_posix_false;

	// [!] : nothing to do

	if ( msec == N_WAV_MSEC( wav ) ) { return; }

	if ( msec < N_WAV_MSEC( wav ) ) { is_shrink = n_posix_true; }


	n_wav to; n_wav_zero( &to );
	n_wav_new( &to, msec );

	u32 size;
	if ( is_shrink )
	{
		size = N_WAV_SIZE( &to );
	} else {
		size = N_WAV_SIZE( wav );
	}


	if ( mode == N_WAV_RESIZER_NORMAL )
	{

		n_memory_copy( N_WAV_PTR( wav ), N_WAV_PTR( &to ), size );

	} else
	if ( mode == N_WAV_RESIZER_CENTER )
	{

		u32 f,t;

		if ( is_shrink )
		{
			f = ( N_WAV_SIZE( wav ) - N_WAV_SIZE( &to ) ) / 2;
			t = 0;
		} else {
			f = 0;
			t = ( N_WAV_SIZE( &to ) - N_WAV_SIZE( wav ) ) / 2;
		}

		n_memory_copy( &N_WAV_PTR( wav )[ f ], &N_WAV_PTR( &to )[ t ], size );

	} else
	if ( mode == N_WAV_RESIZER_RESAMPLE )
	{

		n_wav_resampler( wav, &to, (n_type_real) N_WAV_SIZE( &to ) / N_WAV_SIZE( wav ) );

	}


	n_wav_free( wav );
	n_wav_alias( &to, wav );


	return;
}




#endif // _H_NONNON_NEUTRAL_WAV_TRANSFORM

