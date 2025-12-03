
/*
 * 
 * Call Blocking in Circuit Switched Networks
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/*******************************************************************************/
#define MEAN_HOST_OUTPUT_RATE 100 /* packets/s Poisson Process*/
//#define Call_ARRIVALRATE 3   /* calls/minute */
//#define MEAN_CALL_DURATION 3 /* minutes */
#define RUNLENGTH 5e6 /* number of successful calls */
#define BLIPRATE 1e3
#define NUMBER_OF_CHANNELS 1

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400301269
//#define QUEUE_SIZE_LIST 1, 2, 3, 4, 5, 6, 7, 8
#define QUEUE_SIZE_LIST 3
#define BUCKET_OUTPUT_RATE_LIST 5, 10, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000
//#define BUCKET_OUTPUT_RATE_LIST 1000
/*******************************************************************************/

#endif /* simparameters.h */



