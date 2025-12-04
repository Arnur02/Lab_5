
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

#ifndef _MAIN_H_
#define _MAIN_H_

/*******************************************************************************/

#include "simlib.h"

/*******************************************************************************/

typedef Server Channel;
typedef Server_Ptr Channel_Ptr;

typedef enum {XMTTING, WAITING} Packet_Status;

typedef struct _packet_
{
  double arrive_time;
  int size;
  // double call_duration;
  Channel_Ptr channel;
} Packet, * Packet_Ptr;

typedef struct _simulation_run_data_
{
  Channel_Ptr * channels;
  Fifoqueue_Ptr bucket_queue;
  long int blip_counter;
  long int packet_arrival_count;
  long int packets_processed;
  long int blocked_packet_count;
  long int number_of_packets_processed;
  int queue_size;
  // double accumulated_call_time;
  unsigned random_seed;
} Simulation_Run_Data, * Simulation_Run_Data_Ptr;

/*******************************************************************************/

/*
 * Function prototypes
 */

extern int main(void);

/*******************************************************************************/

#endif /* main.h */






