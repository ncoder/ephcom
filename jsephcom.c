#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h> /* needed because we call sqrt() */
#include <time.h>

#include "ephcom.h"

// allocate this data staically as it is returned as a pointer to javascript.
struct ephcom_Coords coords;

// expect ephemeris file at jplbin/2000.405.eph
//
// returns two doubles for the calculated julian date.
// followed by contents of the pv pointer of the coords struct.
// size will be:  (EPHCOM_NUMOBJECTS * 6 + 2) doubles.
double* calcposvel(double unixtime) {

	char* ephfile = "jplbin/2000.405.eph";

	time_t t = (time_t)unixtime;
	double second_fraction = unixtime - t;


	struct tm* time_tm = gmtime(&t);

	struct ephcom_Header header1;
	double *datablock;  /* Will hold coefficients from a data block */
	int datablocknum;
	int i;
	double dist, sqrt(double); /* To get Geocentric distance */
							   /*
							   deltat = 32.184 000 sec + (TAI-UTC) - (UT1-UTC)

							   From IERS Bulletin A, vol. XVII no. 019, 13 May 2004:

							   32.184 000 sec
							   TAI-UTC = 32.000 000 sec
							   -(UT1-UTC) =  0.469 74  sec
							   --------------
							   deltat  = 64.653 74  sec
							   */
	double deltat = 64.65374; /* value on 8 June 2004 */
							  /*
							  Test parameter variables.
							  */
	double testjd; /* Test Julian Day */
	int testntarg, testnctr; /* Target body and center body */
	int testncoord; /* Coordinate number in testr to compare */
	double testr[6]; /* To hold x, xdot, y, ydot, z, zdot for all bodies */
	int idate[6];    /* To hold year, month, day, hour, minute, second */
					 /*
					 Output file parameters.
					 */
	FILE *infp, *outfp = stdout;

	if ((infp = fopen(ephfile, "rb")) == NULL) {
		fprintf(stderr, "\nERROR: Can't open ephemeris file %s for input.\n\n", ephfile);
		exit(1);
	}

	ephcom_readbinary_header(infp, &header1);
	// ephcom_writeascii_header(outfp, &header1);
	/*
	Done with header.  Now we'll read and write data blocks.
	*/
	datablock = (double *)malloc(header1.ncoeff * sizeof(double));
	datablocknum = 0;
	coords.km = 0;        /* Use AU, not kilometers */
	coords.seconds = 0;   /* Timescale is days, not seconds */
	coords.bary = 1;      /* Center is Solar System Barycenter */
	coords.coordtype = 0;   /* No correction for light travel time or
							relativistic effects from Sun */
							/*
							2004.June.08 00:00:00
							*/
	idate[0] = time_tm->tm_year + 1900;
	idate[1] = time_tm->tm_mon + 1; //6 june
	idate[2] = time_tm->tm_mday;
	idate[3] = time_tm->tm_hour;
	idate[4] = time_tm->tm_min;
	idate[5] = time_tm->tm_sec;

	testjd = ephcom_cal2jd(idate, 0);

	/*
	Make sure time is in range that ephemeris covers.
	*/
	if (testjd >= header1.ss[0] && testjd <= header1.ss[1]) {

		testjd = ephcom_cal2jd(idate, 0);
		coords.et2[0] = testjd;
		coords.et2[1] = second_fraction;
		if (ephcom_get_coords(infp, &header1, &coords, datablock) == 0) {
			return coords.et2;
		}
		else {
			fprintf(outfp, "Julian Day %15.2f not found.\n", testjd);
			return 0;
		}
	}
	else {
		fprintf(outfp,
			"\nERROR: Ephemeris file doesn't cover Julian Day %15.2f\n",
			testjd);
		return 0;
	}
}
