
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "call_arrival.h"
#include "main.h"

/*******************************************************************************/

int main(void)
{
  int i;
  int j=0;
  int k=0;

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data; /* Simulation_Run_Data is defined in main.h. */

  /* 
   * Get the list of random number generator seeds defined in simparameters.h.
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned Q_Sizes[] = {Queue_Sizes, 0};
  unsigned random_seed;
  unsigned Q_Size;
  
  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */
  while (((random_seed = RANDOM_SEEDS[j++]) != 0)){
    k = 0;
    while ((Q_Size = Q_Sizes[k++]) != 0) {
      printf("\n\nStarting simulation run with random seed %u and queue size %u...\n",
       random_seed, Q_Size);
      /* Create a new simulation_run. This gives a clock and eventlist. */
      simulation_run = simulation_run_new();

      /* Add our data definitions to the simulation_run. */
      simulation_run_set_data(simulation_run, (void *) & data);

      /* Initialize our simulation_run data variables. */
      data.blip_counter = 0;
      data.packet_arrival_count = 0;
      data.packets_processed = 0;
      data.blocked_packet_count = 0;
      data.number_of_packets_processed = 0;
      // data.accumulated_call_time = 0.0;
      data.random_seed = random_seed;
      data.queue_size = Q_Size;

      /* Create the channels. */
      data.channels = (Channel_Ptr *) xcalloc((int) 1,
                sizeof(Channel_Ptr));
      
      data.bucket_queue = fifoqueue_new();
      /* Initialize the channels. */
      for (i=0; i<1; i++) {
        *(data.channels+i) = server_new(); 
      }

      /* Set the random number generator seed. */
      random_generator_initialize((unsigned) random_seed);

      /* Schedule the initial call arrival. */
      schedule_packet_arrival_event(simulation_run,
        simulation_run_get_time(simulation_run) +
        exponential_generator((double) 1/(Mean_Host_Output_Rate)));
      
      /* Execute events until we are finished. */
      while(data.number_of_packets_processed < RUNLENGTH) {
        simulation_run_execute_event(simulation_run);
      }
      
      /* Print out some results. */
      output_results(simulation_run);

      /* Clean up memory. */
      cleanup(simulation_run);
    }
  }
  /* Pause before finishing. */
  getchar();
  return 0;
}












