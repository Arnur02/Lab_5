
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
  Simulation_Run_Data_Ptr sim_data;
  static int header_printed = 0;

  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  double sim_time = simulation_run_get_time(this_simulation_run);
  double loss_rate = (sim_data->packet_arrival_count == 0) ? 0.0 :
    (double)sim_data->blocked_packet_count/(double)sim_data->packet_arrival_count;
  double throughput_bps = (sim_time == 0) ? 0.0 :
    ((double) sim_data->total_bits_transmitted / sim_time);

  printf("\n");
  printf("tokens_per_tick=%d, tick=%.4f s, line_rate=%.0f bps, queue=%d packets\n",
    sim_data->tokens_per_tick,
    sim_data->tick_period,
    sim_data->line_rate_bps,
    sim_data->queue_capacity_packets);
  printf("packet loss is %f \n", loss_rate);
  printf("mean output data rate is %.3f bps \n", throughput_bps);

  printf("random seed = %d \n", sim_data->random_seed);
  printf("number of packets processed = %ld \n",
   sim_data->number_of_packets_processed);
  printf("packets arrived = %ld (blocked %ld)\n",
    sim_data->packet_arrival_count, sim_data->blocked_packet_count);

  if(!header_printed) {
    printf("csv_header,seed,line_rate_bps,tokens_per_tick,tick_period,queue_capacity,loss_rate,throughput_bps,sim_time\n");
    header_printed = 1;
  }
  printf("csv,%d,%.0f,%d,%.6f,%d,%.6f,%.3f,%.6f\n",
    sim_data->random_seed,
    sim_data->line_rate_bps,
    sim_data->tokens_per_tick,
    sim_data->tick_period,
    sim_data->queue_capacity_packets,
    loss_rate,
    throughput_bps,
    sim_time);
  printf("\n");
}
