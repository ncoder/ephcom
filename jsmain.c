#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> /* needed because we call sqrt() */
#include <time.h>

#include "ephcom.h"

double* calcposvel(double unixtime);

// simple test main method to get jsmain.html to do something.
// expect ephemeris file at jplbin/2000.405.eph
int main(int argc, char *argv[]) {
	time_t t = time(NULL);
	double* result = calcposvel(t);
	int index = 0;
	int planeti;

	if (result) {
		fprintf(stdout,
			"julian date %15.8lf, %15.8lf\n", result[index], result[index + 1]);
		index += 2;

		for (planeti = 0; planeti < EPHCOM_NUMOBJECTS; ++planeti)
		{
			fprintf(stdout,
				"object %d: %15.8lf, %15.8lf, %15.8lf, %15.8lf, %15.8lf, %15.8lf\n", planeti,
				result[index + 0],
				result[index + 1],
				result[index + 2],
				result[index + 3],
				result[index + 4],
				result[index + 5]
				);

			index += 6;
		}
		return 0;
	}
	else
	{
		return -1;
	}

}
