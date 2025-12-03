
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

#include <stdio.h>
#include "simparameters.h"
#include "main.h"
#include "output.h"

/*******************************************************************************/

void output_progress_msg_to_screen(Simulation_Run_Ptr this_simulation_run)
{
  double percentagedone;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  sim_data->blip_counter++;

  if((sim_data->blip_counter >= BLIPRATE)
     ||
     (sim_data->number_of_packets_processed >= RUNLENGTH))
    {
      sim_data->blip_counter = 0;

      percentagedone =
	100 * (double) sim_data->number_of_packets_processed/RUNLENGTH;

      printf("%3.0f%% ", percentagedone);

      printf("Call Count = %ld \r", sim_data->number_of_packets_processed);

      fflush(stdout);
    }
}

/*******************************************************************************/

void output_results(Simulation_Run_Ptr this_simulation_run)
{
  double xmtted_fraction;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  printf("\n");

  printf("random seed = %d \n", sim_data->random_seed);
  printf("queue capacity (B) = %d\n", sim_data->queue_capacity);
  printf("bucket output rate (R) = %d packets/s\n", sim_data->bucket_output_rate);
  printf("call arrival count = %ld \n", sim_data->packet_arrival_count);
  printf("blocked call count = %ld \n", sim_data->blocked_packets_count);
  
  xmtted_fraction = (double) (sim_data->packet_arrival_count -
      sim_data->blocked_packets_count)/sim_data->packet_arrival_count;

  printf("Blocking probability = %.5f (Service fraction = %.5f)\n",
	 1-xmtted_fraction, xmtted_fraction);

  printf("\n");
  double sim_time;
  double packet_loss_rate;
  double mean_output_rate; /* packets per second */

  /* final simulation time */
  sim_time = simulation_run_get_time(this_simulation_run);

  /* Protect against division by zero */
  if (sim_data->packet_arrival_count > 0)
    packet_loss_rate = (double) sim_data->blocked_packets_count /
      (double) sim_data->packet_arrival_count;
  else
    packet_loss_rate = 0.0;

  /* mean output data rate = number of packets processed divided by sim time */
  if (sim_time > 0.0)
    mean_output_rate = (double) sim_data->number_of_packets_processed / sim_time;
  else
    mean_output_rate = 0.0;

  /* Print a compact CSV line for easy plotting: */
  /* Columns: B (queue capacity), R (bucket output rate), arrivals, blocked, */
  /* processed, sim_time, packet_loss_rate, mean_output_rate, seed */
  printf("# Results (CSV): B,R,arrivals,blocked,processed,sim_time,packet_loss_rate,mean_output_rate,seed\n");
  printf("%d,%d,%ld,%ld,%ld,%.6f,%.6f,%.6f,%u\n",
     sim_data->queue_capacity,
     sim_data->bucket_output_rate,
     sim_data->packet_arrival_count,
     sim_data->blocked_packets_count,
     sim_data->number_of_packets_processed,
     sim_time,
     packet_loss_rate,
     mean_output_rate,
     sim_data->random_seed);

  /* Also print a short human-readable summary */
  printf("\nrandom seed = %u\n", sim_data->random_seed);
  printf("packet arrivals = %ld\n", sim_data->packet_arrival_count);
  printf("blocked packets (due to buffer overflow) = %ld\n", sim_data->blocked_packets_count);
  printf("packet loss rate = %.6f\n", packet_loss_rate);
  printf("mean controller output rate (packets/s) = %.6f\n", mean_output_rate);
  printf("simulation time (s) = %.6f\n\n", sim_time);
}


