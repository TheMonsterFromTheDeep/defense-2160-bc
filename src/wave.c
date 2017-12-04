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
		7, 100,
		3, 115
	),
	create_wave(
		20, 95,
		8, 100,
		3, 115
	),
	create_wave(
		20, 85,
		9, 100,
		3, 110
	),
	create_wave(
		20, 85,
		10, 100,
		6, 100
	),
	create_wave(
		25, 85,
		11, 100,
		7, 100
	),
	create_wave(
		25, 85,
		11, 100,
		7, 100
	),
	create_wave(
		25, 85,
		12, 100,
		8, 100
	),
	create_wave(
		25, 85,
		13, 90,
		8, 90
	),
	create_wave(
		25, 80,
		13, 90,
		10, 90
	),
	create_wave(
		27, 80,
		15, 90,
		10, 90
	),
	create_wave(
		30, 80,
		15, 90,
		13, 90
	),
	create_wave(
		30, 80,
		15, 90,
		15, 90
	),
	create_wave(
		30, 75,
		16, 85,
		16, 85
	),
	create_wave(
		35, 75,
		17, 83,
		17, 83
	),
	create_wave(
		35, 74,
		18, 82,
		18, 82
	),
	create_wave(
		35, 72,
		18, 82,
		19, 82
	),
	create_wave(
		37, 70,
		18, 81,
		19, 81
	),
	create_wave(
		40, 65,
		19, 80,
		19, 80
	),
	create_wave(
		41, 65,
		19, 80,
		20, 80
	),
	create_wave(
		41, 65,
		20, 80,
		22, 78
	),
	create_wave(
		42, 65,
		20, 80,
		24, 76
	),
	create_wave(
		44, 65,
		20, 75,
		24, 75
	),
	create_wave(
		44, 64,
		20, 75,
		24, 75
	),
	create_wave(
		44, 64,
		21, 74,
		25, 74
	),
	create_wave(
		44, 63,
		21, 73,
		26, 72
	),
	create_wave(
		45, 63,
		22, 73,
		26, 72
	),
	create_wave(
		46, 62,
		23, 72,
		28, 72
	),
	create_wave(
		47, 62,
		23, 71,
		28, 71
	),
	create_wave(
		48, 61,
		23, 71,
		28, 71
	),
	create_wave(
		48, 60,
		23, 70,
		28, 70
	),
	create_wave(
		49, 60,
		24, 70,
		29, 70
	),
	create_wave(
		50, 60,
		25, 70,
		30, 70
	)
};