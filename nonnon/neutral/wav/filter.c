// Nonnon Nyaurism
// copyright (c) nonnon all rights reserved
// License : GPL http://www.gnu.org/copyleft/gpl.html




#ifndef _H_NONNON_NEUTRAL_WAV_FILTER
#define _H_NONNON_NEUTRAL_WAV_FILTER




#include "../random.c"


#include "./_error.c"
#include "./_piano.c"
#include "./sample.c"




n_posix_bool
n_wav_sample_is_accessible( n_wav *wav, u32 x )
{
	if ( ( x >= 0 )&&( x < N_WAV_COUNT( wav ) ) ) { return n_posix_true; }

	return n_posix_false;
}

#define n_wav_cosine( w, hz, r_l, r_r ) n_wav_cosine_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_cosine_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_cosine( hz, xx );
			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_sine( w, hz, r_l, r_r ) n_wav_sine_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_sine_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sine( hz, xx );
			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_sawtooth( w, hz, r_l, r_r ) n_wav_sawtooth_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_sawtooth_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sawtooth( hz, xx );
			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_square( w, hz, r_l, r_r ) n_wav_square_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_square_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_square( hz, xx );
			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_sandstorm( w, hz, r_l, r_r ) n_wav_sandstorm_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_sandstorm_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	n_random_shuffle();


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sandstorm( hz, xx );
			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_pinknoise( w, hz, r_l, r_r ) n_wav_pinknoise_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_pinknoise_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	n_random_shuffle();


	// [!] : this code comes from Audacity's source code

	n_type_real buf0 = 1, buf1 = 1, buf2 = 1, buf3 = 1, buf4 = 1, buf5 = 1, buf6 = 1;

	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{

			n_type_real white = ( rand() / ( ( (n_type_real) RAND_MAX ) / 2.0 ) ) - 1.0;

			buf0 = (  0.99886 * buf0 ) + ( 0.0555179 * white );
			buf1 = (  0.99332 * buf1 ) + ( 0.0750759 * white );
			buf2 = (  0.96900 * buf2 ) + ( 0.1538520 * white );
			buf3 = (  0.86650 * buf3 ) + ( 0.3104856 * white );
			buf4 = (  0.55000 * buf4 ) + ( 0.5329522 * white );
			buf5 = ( -0.76160 * buf5 ) - ( 0.0168980 * white );

			n_type_real d = buf0 + buf1 + buf2 + buf3 + buf4 + buf5 + buf6 + ( white * 0.5362 );

			d *= SHRT_MAX;
			d *= 0.025;

			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );

			buf6 = white * 0.115926;

		}


		f++;

	}


	return;
}

#define n_wav_fade( w, hz, r_l, r_r ) n_wav_fade_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_fade_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r, d;
			n_wav_sample_get( wav, xx, &l, &r );

			d = (n_type_real) f / sx / 2;
			l = l * fabs( sin( N_WAV_2PI * d ) );
			r = r * fabs( sin( N_WAV_2PI * d ) );

			n_wav_sample_mix( wav, xx, l, r, ratio_l, ratio_r );
		}


		f++;

	}


	return;
}

#define n_wav_fade_in( w, hz, r_l, r_r ) n_wav_fade_in_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_fade_in_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx / 2;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r, d;
			n_wav_sample_get( wav, xx, &l, &r );

			d = (n_type_real) f / sx / 2;
			l = l * fabs( sin( N_WAV_2PI * d ) );
			r = r * fabs( sin( N_WAV_2PI * d ) );

			n_wav_sample_mix( wav, xx, l, r, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_fade_out( w, hz, r_l, r_r ) n_wav_fade_out_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_fade_out_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = sx / 2;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r, d;
			n_wav_sample_get( wav, xx, &l, &r );

			d = (n_type_real) f / sx / 2;
			l = l * fabs( sin( N_WAV_2PI * d ) );
			r = r * fabs( sin( N_WAV_2PI * d ) );

			n_wav_sample_mix( wav, xx, l, r, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_delay( w, hz, r_l, r_r ) n_wav_delay_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_delay_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 offset = (u32) ( N_WAV_RATE( wav ) / hz );
	if ( offset >= sx ) { return; }


	n_wav w; n_wav_carboncopy( wav, &w );


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r;
			n_wav_sample_get( &w, xx, &l, &r );

			if ( n_wav_sample_is_accessible( wav, xx + offset ) )
			{
				n_wav_sample_mix( wav, xx + offset, l, r, ratio_l, ratio_r );
			}
		}

		f++;

	}


	n_wav_free( &w );


	return;
}

#define n_wav_feedback_left( w, hz, r_l, r_r ) n_wav_feedback_left_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_feedback_left_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 offset = (u32) ( N_WAV_RATE( wav ) / hz );
	if ( offset >= sx ) { return; }


	n_wav w; n_wav_carboncopy( wav, &w );


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r;

			if ( n_wav_sample_is_accessible( &w, xx + offset ) )
			{
				n_wav_sample_get( &w, xx + offset, &l, &r );
			} else {
				l = r = 0;
			}

			n_wav_sample_mix( wav, xx, l, r, ratio_l, ratio_r );
		}

		f++;

	}


	n_wav_free( &w );


	return;
}

#define n_wav_feedback_right( w, hz, r_l, r_r ) n_wav_feedback_right_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_feedback_right_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 offset = (u32) ( N_WAV_RATE( wav ) / hz );
	if ( offset >= sx ) { return; }


	n_wav w; n_wav_carboncopy( wav, &w );


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r;

			if ( n_wav_sample_is_accessible( &w, xx - offset ) )
			{
				n_wav_sample_get( &w, xx - offset, &l, &r );
			} else {
				l = r = 0;
			}

			n_wav_sample_mix( wav, xx, l, r, ratio_l, ratio_r );
		}

		f++;

	}


	n_wav_free( &w );


	return;
}

#define n_wav_monaural( w, hz, r_l, r_r ) n_wav_monaural_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_monaural_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r;

			n_wav_sample_get( wav, xx, &l, &r );

			l = r = ( l + r ) / 2;

			n_wav_sample_mix( wav, xx, l, r, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_tremolo( w, hz, r_l, r_r ) n_wav_tremolo_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_tremolo_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sine_coeff( hz, xx );

			n_type_real l,r;
			n_wav_sample_get( wav, x + f, &l, &r );

			if ( ratio_l != 0 ) { l = d * l * ratio_l; }
			if ( ratio_r != 0 ) { r = d * r * ratio_r; }

			n_wav_sample_set( wav, xx, l, r );
		}

		f++;

	}


	return;
}

#define n_wav_tone_up( w, hz, r_l, r_r ) n_wav_tone_up_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_tone_up_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	const u32 turn = sx / 2;


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sine( n_wav_piano[ N_WAV_PIANO_MIDDLE + ( ( xx ) > turn ) ], xx );

			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_tone_down( w, hz, r_l, r_r ) n_wav_tone_down_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_tone_down_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	const u32 turn = sx / 2;


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sine( n_wav_piano[ N_WAV_PIANO_MIDDLE + ( ( xx ) < turn ) ], xx );

			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_tone_updown( w, hz, r_l, r_r ) n_wav_tone_updown_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_tone_updown_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	const u32 quarter = sx / 4;


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 p = x + f;
		n_type_real d;

		if ( ( p > quarter )&&( p < ( sx - quarter ) ) )
		{
			d = n_wav_sample_sine( n_wav_piano[ N_WAV_PIANO_MIDDLE + 1 ], p );
		} else {
			d = n_wav_sample_sine( n_wav_piano[ N_WAV_PIANO_MIDDLE + 0 ], p );
		}

		n_wav_sample_mix( wav, p, d, d, ratio_l, ratio_r );

		f++;

	}


	return;
}

#define n_wav_tone_downup( w, hz, r_l, r_r ) n_wav_tone_downup_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_tone_downup_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	const u32 quarter = sx / 4;


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 p = x + f;
		n_type_real d;

		if ( ( p > quarter )&&( p < ( sx - quarter ) ) )
		{
			d = n_wav_sample_sine( n_wav_piano[ N_WAV_PIANO_MIDDLE + 0 ], p );
		} else {
			d = n_wav_sample_sine( n_wav_piano[ N_WAV_PIANO_MIDDLE + 1 ], p );
		}

		n_wav_sample_mix( wav, p, d, d, ratio_l, ratio_r );

		f++;

	}


	return;
}

#define n_wav_distortion( w, hz, r_l, r_r ) n_wav_distortion_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_distortion_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	// [!] : "hz" is not used


	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	n_type_real hi_l = 0.0;
	n_type_real hi_r = 0.0;


	// Phase 1 : get peak value

	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r; n_wav_sample_get( wav, xx, &l, &r );

			l = fabs( l );
			r = fabs( r );

			if ( hi_l < l ) { hi_l = l; }
			if ( hi_r < r ) { hi_r = r; }
		}

		f++;

	}


	// Phase 2 : apply

	hi_l *= 1.0 - ratio_l;
	hi_r *= 1.0 - ratio_r;

	f = 0;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r; n_wav_sample_get( wav, xx, &l, &r );

			if ( fabs( l ) >= hi_l ) { if ( l > 0 ) { l = hi_l; } else { l = -hi_l; } }
			if ( fabs( r ) >= hi_r ) { if ( r > 0 ) { r = hi_r; } else { r = -hi_r; } }

			n_wav_sample_set( wav, xx, l, r );
		}

		f++;

	}


	return;
}

#define n_wav_normalize( w, hz, r_l, r_r ) n_wav_normalize_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_normalize_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	// [!] : "hz" is not used


	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	n_type_real hi_l = 0.0;
	n_type_real hi_r = 0.0;


	// Phase 1 : get peak value

	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r; n_wav_sample_get( wav, xx, &l, &r );

			l = fabs( l );
			r = fabs( r );

			if ( hi_l < l ) { hi_l = l; }
			if ( hi_r < r ) { hi_r = r; }
		}

		f++;

	}


	// Phase 2 : apply

	hi_l = hi_r = n_posix_min_n_type_real( hi_l, hi_r );

	n_type_real a_l = (n_type_real) N_WAV_AMP * ratio_l;
	n_type_real a_r = (n_type_real) N_WAV_AMP * ratio_r;

	f = 0;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real l,r; n_wav_sample_get( wav, xx, &l, &r );

			if ( hi_l != 0 ) { l = a_l * ( l / hi_l ); }
			if ( hi_r != 0 ) { r = a_r * ( r / hi_r ); }

			n_wav_sample_set( wav, xx, l, r );
		}

		f++;

	}


	return;
}

#define n_wav_marsian( w, hz, r_l, r_r ) n_wav_marsian_partial( w, hz, 0, N_WAV_COUNT( w ), r_l, r_r )

void
n_wav_marsian_partial( n_wav *wav, n_type_real hz, u32 x, u32 sx, n_type_real ratio_l, n_type_real ratio_r )
{

	// [!] : extra-terrestrial chatter


	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	const u32 unit = (u32) ( (n_type_real) 44100 * 0.125 );


	n_random_shuffle();


	u32 i = 0;
	u32 j = 0;
	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_type_real d = n_wav_sample_sine( n_wav_piano[ i ], xx );

			j++;
			if ( j >= unit ) { i = n_random_range( N_WAV_PIANO_MAX ); j = 0; }

			n_wav_sample_mix( wav, xx, d, d, ratio_l, ratio_r );
		}

		f++;

	}


	return;
}

#define n_wav_smoother( w ) n_wav_smoother_partial( w, 0, N_WAV_COUNT( w ) )

void
n_wav_smoother_partial( n_wav *wav, u32 x, u32 sx )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 count = N_WAV_COUNT( wav );


	const u32      samples = 5;
	const u32 half_samples = samples / 2;


	u32 f = 0;
	u32 t = sx;
	n_posix_loop
	{

		if ( f >= t ) { break; }


		n_type_real l = 0;
		n_type_real r = 0;

		u32 xx = x + f;

		if (
			( n_wav_sample_is_accessible( wav, xx ) )
			&&
			( ( xx ) >= half_samples )
		)
		{

			u32 avr = 0;

			u32 xxx = x + f - half_samples;
			n_posix_loop
			{

				if ( ( xxx >= 0 )&&( xxx < count ) )
				{

					n_type_real ll, rr;
					n_wav_sample_get( wav, xxx, &ll, &rr );

					l += ll;
					r += rr;

					avr++;

				}


				xxx++;
				if ( xxx > ( x + f + half_samples ) ) { break; }
			}


			l /= avr;
			r /= avr;

			n_wav_sample_set( wav, x + f, l, r );

		}


		f++;

	}


	return;
}




void
n_wav_mute( n_wav *wav, u32 x, u32 sx )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	u32 f = 0;
	n_posix_loop
	{//break;

		u32 xx = x + f;

		if ( n_wav_sample_is_accessible( wav, xx ) )
		{
			n_wav_sample_set( wav, xx, 0, 0 );
		}

		f++;
		if ( f >= sx ) { break; }
	}


	return;
}

void
n_wav_delete( n_wav *wav, u32 x, u32 sx )
{

	if ( n_wav_error_format( wav ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	if ( sx == 0 ) { return; }


	n_wav ret; n_wav_zero( &ret ); n_wav_new_by_sample( &ret, N_WAV_COUNT( wav ) - sx );

	u32 i = 0;
	u32 j = 0;
	n_posix_loop
	{//break;

		if ( i >= x ) { break; }

		if ( i >= N_WAV_COUNT(  wav ) ) { break; }
		if ( j >= N_WAV_COUNT( &ret ) ) { break; }

		if (
			( n_wav_sample_is_accessible(  wav, i ) )
			&&
			( n_wav_sample_is_accessible( &ret, j ) )
		)
		{
			n_type_real l,r;
			n_wav_sample_get(  wav, i, &l, &r );
			n_wav_sample_set( &ret, j,  l,  r );
		}

		i++;
		j++;

	}

	i = 0;
	n_posix_loop
	{//break;

		u32 pos = x + sx + i;
		if ( pos >= N_WAV_COUNT(  wav ) ) { break; }
		if (   j >= N_WAV_COUNT( &ret ) ) { break; }

		if (
			( n_wav_sample_is_accessible(  wav, pos ) )
			&&
			( n_wav_sample_is_accessible( &ret,   j ) )
		)
		{
			n_type_real l,r;
			n_wav_sample_get(  wav, pos, &l, &r );
			n_wav_sample_set( &ret,   j,  l,  r );
		}

		i++;
		j++;

	}

	n_wav_free( wav );
	n_wav_alias( &ret, wav );


	return;
}

void
n_wav_insert( n_wav *wav, n_wav *ins, u32 x, u32 sx )
{

	if ( n_wav_error_format( wav ) ) { return; }
	if ( n_wav_error_format( ins ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( wav, x ) ) { return; }


	n_wav ret; n_wav_zero( &ret ); n_wav_new_by_sample( &ret, N_WAV_COUNT( wav ) + N_WAV_COUNT( ins ) - sx );


	u32 i = 0;
	u32 j = 0;
	n_posix_loop
	{//break;

		if ( i >= x ) { break; }

		if ( i >= N_WAV_COUNT(  wav ) ) { break; }
		if ( j >= N_WAV_COUNT( &ret ) ) { break; }

		if (
			( n_wav_sample_is_accessible(  wav, i ) )
			&&
			( n_wav_sample_is_accessible( &ret, j ) )
		)
		{
			n_type_real l,r;
			n_wav_sample_get(  wav, i, &l, &r );
			n_wav_sample_set( &ret, j,  l,  r );
		}

		i++;
		j++;

	}

	i = 0;
	n_posix_loop
	{//break;

		if ( i >= N_WAV_COUNT(  ins ) ) { break; }
		if ( j >= N_WAV_COUNT( &ret ) ) { break; }

		if (
			( n_wav_sample_is_accessible(  ins, i ) )
			&&
			( n_wav_sample_is_accessible( &ret, j ) )
		)
		{
			n_type_real l,r;
			n_wav_sample_get(  ins, i, &l, &r );
			n_wav_sample_set( &ret, j,  l,  r );
		}

		i++;
		j++;

	}

	i = x + sx;
	n_posix_loop
	{//break;

		if ( i >= N_WAV_COUNT(  wav ) ) { break; }
		if ( j >= N_WAV_COUNT( &ret ) ) { break; }

		if (
			( n_wav_sample_is_accessible(  wav, i ) )
			&&
			( n_wav_sample_is_accessible( &ret, j ) )
		)
		{
			n_type_real l,r;
			n_wav_sample_get(  wav, i, &l, &r );
			n_wav_sample_set( &ret, j,  l,  r );
		}

		i++;
		j++;

	}


	n_wav_free( wav );
	n_wav_alias( &ret, wav );


	return;
}




#define N_WAV_COPY_SET 0
#define N_WAV_COPY_MIX 1
#define N_WAV_COPY_ADD 2

#define n_wav_set( f,t, l,r ) n_wav_copy( f,t, 0,0, 0, l,r, N_WAV_COPY_SET )
#define n_wav_mix( f,t, l,r ) n_wav_copy( f,t, 0,0, 0, l,r, N_WAV_COPY_MIX )
#define n_wav_add( f,t, l,r ) n_wav_copy( f,t, 0,0, 0, l,r, N_WAV_COPY_ADD )

void
n_wav_copy( n_wav *f, n_wav *t, u32 fx, u32 sx, u32 tx, n_type_real ratio_l, n_type_real ratio_r, int mode )
{

	if ( n_wav_error_format( f ) ) { return; }
	if ( n_wav_error_format( t ) ) { return; }

	if ( n_posix_false == n_wav_sample_is_accessible( f, fx ) ) { return; }
	if ( n_posix_false == n_wav_sample_is_accessible( t, tx ) ) { return; }


	u32 count_f = N_WAV_COUNT( f );
	u32 count_t = N_WAV_COUNT( t );

	if ( sx == 0 ) { sx = count_f; }

	if ( fx >= count_f ) { return; }
	if ( fx <        0 ) { sx += fx; tx += ( fx * -1 ); fx = 0; }

	if ( tx >= count_t ) { return; }
	if ( tx <        0 ) { sx += tx; fx += ( fx * -1 ); tx = 0; }

	if ( sx <=       0 ) { return; }


	n_type_real l, r;


	u32 x = 0;
	n_posix_loop
	{//break;

		if ( ( fx + x ) >= count_f ) { break; }
		if ( ( tx + x ) >= count_t ) { break; }


		n_wav_sample_get( f, fx + x, &l, &r );

		if ( mode == N_WAV_COPY_SET )
		{
			n_wav_sample_set( t, tx + x, l * ratio_l, r * ratio_r );
		} else
		if ( mode == N_WAV_COPY_MIX )
		{
			n_wav_sample_mix( t, tx + x, l, r, ratio_l, ratio_r );
		} else
		if ( mode == N_WAV_COPY_ADD )
		{
			n_wav_sample_add( t, tx + x, l * ratio_l, r * ratio_r );
		}// else


		x++;
		if ( x >= sx ) { break; }
	}


	return;
}




#endif // _H_NONNON_NEUTRAL_WAV_FILTER

