
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

/*
 * Simulation parameters for the bit-counting leaky bucket.
 */

#define QUEUE_CAPACITY_PACKETS 100
#define MEAN_HOST_OUTPUT_RATE 100              /* packets/second */
#define RUNLENGTH 5e4                          /* number of successfully sent packets */
#define BLIPRATE 1e3

/* Token bucket options. */
#define TOKENS_PER_TICK_LIST 500, 1000, 1500, 2000, 2500
#define TICK_PERIOD_LIST 0.0001, 0.0005, 0.001, 0.005 /* seconds */
#define LINE_RATE_LIST 5e5, 1e6, 2e6                  /* bits/second */

#define MIN_PACKET_SIZE 500
#define MAX_PACKET_SIZE 2500

/* Comma separated list of random seeds to run. */
//#define RANDOM_SEED_LIST 400310269, 400373330
#define RANDOM_SEED_LIST 400310269
/*******************************************************************************/

#endif /* simparameters.h */



