
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

#include "trace.h"
#include "main.h"
#include "output.h"
#include "simparameters.h"
#include "call_departure.h"

/*******************************************************************************/

/* 
 * Function to schedule an end of call event. 
 */

long int
schedule_end_packet_on_channel_event(Simulation_Run_Ptr simulation_run,
				   double event_time,
				   void * channel) 
{
  Event new_event;

  new_event.description = "End of Packet";
  new_event.function = end_packet_on_channel_event;
  new_event.attachment = channel;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

/*******************************************************************************/

long int
schedule_bucket_tick_event(Simulation_Run_Ptr simulation_run, double event_time)
{
  Event new_event;

  new_event.description = "Bucket Tick";
  new_event.function = bucket_tick_event;
  new_event.attachment = NULL;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

/* 
 * Function which handles end of call events.
 */

void
end_packet_on_channel_event(Simulation_Run_Ptr simulation_run, void * c_ptr)
{
  Packet_Ptr this_packet;
  Channel_Ptr channel;
  Simulation_Run_Data_Ptr sim_data;

  channel = (Channel_Ptr) c_ptr;

  sim_data = simulation_run_data(simulation_run);

  /* Remove the call from the channel.*/
  this_packet = (Packet_Ptr) server_get(channel);

  TRACE(printf("End Of Packet.\n"););

  /* Collect statistics. */
  sim_data->number_of_packets_processed++;
  sim_data->total_bits_transmitted += this_packet->size;

  output_progress_msg_to_screen(simulation_run);

  /* This call is done. Free up its allocated memory.*/
  xfree((void*) this_packet);

  try_send_from_queue(simulation_run);
}

void
start_packet_service(Simulation_Run_Ptr simulation_run, Packet_Ptr packet, double now)
{
  Simulation_Run_Data_Ptr sim_data = simulation_run_data(simulation_run);
  Channel_Ptr channel = sim_data->channels[0];

  /* Defensive: do not overdraw tokens. */
  if(packet->size > channel->n || server_state(channel) == BUSY) {
    return;
  }

  server_put(channel, (void*) packet);
  packet->channel = channel;
  channel->n -= packet->size;

  double service_time = ((double) packet->size) / sim_data->line_rate_bps;
  schedule_end_packet_on_channel_event(simulation_run,
               now + service_time,
               (void *) channel);
}

void
try_send_from_queue(Simulation_Run_Ptr simulation_run)
{
  Simulation_Run_Data_Ptr sim_data = simulation_run_data(simulation_run);
  Channel_Ptr channel = sim_data->channels[0];

  if(server_state(channel) == BUSY) {
    return;
  }

  while(sim_data->bucket_queue->size > 0) {
    Packet_Ptr next_packet = (Packet_Ptr) fifoqueue_see_front(sim_data->bucket_queue);
    if(next_packet->size <= channel->n) {
      fifoqueue_get(sim_data->bucket_queue);
      start_packet_service(simulation_run, next_packet,
        simulation_run_get_time(simulation_run));
      return;
    } else {
      return;
    }
  }
}

void
bucket_tick_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr sim_data = simulation_run_data(simulation_run);
  Channel_Ptr channel = sim_data->channels[0];
  double now = simulation_run_get_time(simulation_run);

  channel->n = sim_data->tokens_per_tick;
  try_send_from_queue(simulation_run);

  schedule_bucket_tick_event(simulation_run, now + sim_data->tick_period);
}
