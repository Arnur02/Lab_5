
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

  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data; /* Simulation_Run_Data is defined in main.h. */

  const int queue_sizes[] = {QUEUE_SIZE_LIST};
  const int bucket_output_rates[] = {BUCKET_OUTPUT_RATE_LIST};
  const unsigned seeds[] = {RANDOM_SEED_LIST};
  const size_t queue_size_count = sizeof(queue_sizes)/sizeof(queue_sizes[0]);
  const size_t bucket_rate_count = sizeof(bucket_output_rates)/sizeof(bucket_output_rates[0]);
  const size_t seed_count = sizeof(seeds)/sizeof(seeds[0]);

  for (size_t qi = 0; qi < queue_size_count; qi++) {
    for (size_t ri = 0; ri < bucket_rate_count; ri++) {
      printf("\n=== B = %d, R = %d packets/s ===\n",
             queue_sizes[qi], bucket_output_rates[ri]);
      for (size_t si = 0; si < seed_count; si++) {

        unsigned random_seed = seeds[si];

        /* Create a new simulation_run. This gives a clock and eventlist. */
        simulation_run = simulation_run_new();

        /* Add our data definitions to the simulation_run. */
        simulation_run_set_data(simulation_run, (void *) & data);

        /* Initialize our simulation_run data variables. */
        data.queue_capacity = queue_sizes[qi];
        data.bucket_output_rate = bucket_output_rates[ri];
        data.service_time = 1.0 / (double) data.bucket_output_rate;
        data.blip_counter = 0;
        data.packet_arrival_count = 0;
        data.packets_processed = 0;
        data.blocked_packets_count = 0;
        data.number_of_packets_processed = 0;
        data.random_seed = random_seed;

        data.bucket_queue = fifoqueue_new();

        /* Create the channels. */
        data.channels = (Channel_Ptr *) xcalloc((int) NUMBER_OF_CHANNELS,
                                                sizeof(Channel_Ptr));

        /* Initialize the channels. */
        for (i=0; i<NUMBER_OF_CHANNELS; i++) {
          *(data.channels+i) = server_new(); 
        }

        /* Set the random number generator seed. */
        random_generator_initialize((unsigned) random_seed);

        /* Schedule the initial call arrival. */
        schedule_packet_arrival_event(simulation_run,
            simulation_run_get_time(simulation_run) +
            exponential_generator((double) 1/MEAN_HOST_OUTPUT_RATE));
        
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
  }

  return 0;
}










