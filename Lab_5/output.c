
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
  double loss_rate;
  double throughput;
  double sim_time;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  sim_time = simulation_run_get_time(this_simulation_run);
  loss_rate = (double)sim_data->blocked_packet_count/(double)sim_data->packet_arrival_count;
  throughput = (double) sim_data->number_of_packets_processed / sim_time;

  printf("\n");
  printf("config: data_queue=%d, token_queue=%d\n", sim_data->queue_size, sim_data->token_queue_size);
  printf("packet loss is %f \n", loss_rate);
  /* mean output data rate = number of packets processed divided by sim time */

  printf("mean output data rate is %f packets/second \n", throughput);
  printf("sim time = %.4f seconds\n", sim_time);
  printf("random seed = %d \n", sim_data->random_seed);

  /* Single line format for easier parsing later. */
  printf("RESULT,data_queue=%d,token_queue=%d,loss=%.6f,throughput=%.6f,time=%.6f,arrivals=%ld,blocked=%ld\n",
    sim_data->queue_size,
    sim_data->token_queue_size,
    loss_rate,
    throughput,
    sim_time,
    sim_data->packet_arrival_count,
    sim_data->blocked_packet_count);

  printf("\n");
}
