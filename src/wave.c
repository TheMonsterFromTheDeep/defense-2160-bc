#include "common.h"

#define create_wave(spq, spr, bq, br, snq, snr)\
	(wave){ spq, spr, snq, snr, bq, br }

wave waves[WAVE_COUNT] = {
	create_wave(
		10, 100,
		3, 100,
		0, 0
	),
	create_wave(
		15, 95,
		3, 100,
		0, 0
	),
	create_wave(
		20, 95,
		10, 100,
		3, 115
	),
	create_wave(
		20, 95,
		15, 100,
		3, 115
	),
	create_wave(
		20, 85,
		15, 100,
		3, 110
	)
};