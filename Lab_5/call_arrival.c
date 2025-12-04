
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "call_duration.h"
#include "simparameters.h"
#include "call_departure.h"
#include "call_arrival.h"

/*******************************************************************************/

/*
 * Function to schedule a call arrival event. 
 */

long int
schedule_packet_arrival_event(Simulation_Run_Ptr simulation_run, 
			    double event_time)
{
  Event new_event;

  new_event.description = "Packet Arrival";
  new_event.function = packet_arrival_event;
  new_event.attachment = NULL;

  return simulation_run_schedule_event(simulation_run, new_event, event_time);
}

/*******************************************************************************/

/*
 * Call arrival event function.
 */

void
packet_arrival_event(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Packet_Ptr new_packet;
  
  Channel_Ptr my_channel;
  my_channel = get_free_channel(simulation_run);
  Simulation_Run_Data_Ptr sim_data;
  double now;

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
  new_packet->arrive_time = now;
  new_packet->size = Packet_Size;
  int residual_packet_size = my_channel->n - new_packet->size;

  now = simulation_run_get_time(simulation_run);

  sim_data = simulation_run_data(simulation_run);
  sim_data->packet_arrival_count++;
  Fifoqueue_Ptr bucket_queue = sim_data->bucket_queue;

  /* See if there is a free channel.*/
  // if((free_channel = get_free_channel(simulation_run)) != NULL)
  if( residual_packet_size >= 0)
   {
    // We have enough space in my channel to send the whole packet
        // server_put(my_channel, (void*) new_packet);
        new_packet->channel = my_channel;
        schedule_end_packet_on_channel_event(simulation_run,
                   now + (Bit_Service_Time) * new_packet->size,
                   (void *) my_channel);
        my_channel->n -= new_packet->size;

   } else if( residual_packet_size < 0 && my_channel->n > 0) {
     // I have space in my channel but not enough for the whole packet, so put residual on the queue
    // server_put(my_channel, (void*) new_packet);
    new_packet->channel = my_channel;
    schedule_end_packet_on_channel_event(simulation_run,
                now + (Bit_Service_Time) * residual_packet_size,
                (void *) my_channel);
    my_channel->n -= residual_packet_size;
    // Create a new packet for the residual and put it on the queue
    Packet_Ptr residual_packet;
    residual_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
    residual_packet->arrive_time = now;
    residual_packet->size = new_packet->size - residual_packet_size;
    fifoqueue_put(bucket_queue, (void *) residual_packet);

    } else if (residual_packet_size < 0 && my_channel->n == 0) {
    // No space in my channel put the whole packet on the queue
    fifoqueue_put(bucket_queue, (void *) new_packet);

  } else {
    // No space in my channel and no space on the queue, so block the packet
    sim_data->blocked_packet_count++;
  }



    /* Yes, we found one. Allocate some memory and start the call. */

    // new_packet->packet_duration = get_packet_duration();

    /* Place the call in the free channel and schedule its
       departure. */
    server_put(free_channel, (void*) new_packet);
    new_packet->channel = free_channel;

  } else if(bucket_queue->size < sim_data->queue_size) {
    new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));
    new_packet->arrive_time = now;
    fifoqueue_put(bucket_queue, (void *) new_packet);
  } else {
    /* No free channel was found. The call is blocked. */
    sim_data->blocked_packet_count++;
  }

  /* Schedule the next call arrival. */
  schedule_packet_arrival_event(simulation_run,
	      now + exponential_generator((double) 1/(Mean_Host_Output_Rate)));
}

/*******************************************************************************/

/*
 * Scan through the channels and return a free one, if possible. Otherwise
 * return NULL.
 */

Channel_Ptr get_free_channel(Simulation_Run_Ptr simulation_run)
{
  Channel_Ptr *channels;
  int i;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = simulation_run_data(simulation_run);
  channels = sim_data->channels;

  // for (i=0; i<1; i++) {
    // if (server_state(*(channels+i)) == FREE)
  return *(channels+0);
  // }
  // for (i=0; i<1; i++) {
  //   if (server_state(*(channels+i)) == FREE)
  //     return *(channels+i);
  // }
  // return (Channel_Ptr) NULL;
}


