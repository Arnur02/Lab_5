
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

// #define Call_ARRIVALRATE 3   /* calls/minute */
// #define MEAN_CALL_DURATION 3 /* minutes */
// #define NUMBER_OF_CHANNELS 11

// Our variables
#define Queue_Size 100
#define Queue_Sizes 1, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50
#define Token_Queue_Sizes 1, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50
#define Bucket_Output_Rate 100

#define Bucket_Output_Rates 5, 10, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000


#define Token_Queue_Size 100
#define Token_Generation_rate 10//   tokens/second
#define Mean_Host_Output_Rate 20

#define Service_Rate (1.0/(Clock_tick_duration)) // cycle/second
#define Clock_tick_duration 0.001 // second
#define COUNTER_INTIAL 100
#define RUNLENGTH 1000 /* number of successful calls */
#define BLIPRATE 1e3

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400310269, 400373330

/*******************************************************************************/

#endif /* simparameters.h */




