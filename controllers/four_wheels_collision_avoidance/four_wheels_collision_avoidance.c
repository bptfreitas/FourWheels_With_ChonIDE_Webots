/*
 * Copyright 1996-2023 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <webots/robot.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <javino.h>

// time in [ms] of a simulation step
#define TIME_STEP 100

#define TTY_EXOGENOUS_PORT "/dev/ttyExogenous0"

#define LOG "/tmp/robot.log"


// entree point of the controller
int main(int argc, char **argv) {

  static int reasoning_cycle = 1;
  char percepts_msg[262];  

 
  // initialise the Webots API
  wb_robot_init();

  // internal variables
  int i;

  // initialise distance sensors
  WbDeviceTag ds[2];
  char ds_names[2][10] = {"ds_left", "ds_right"};
  for (i = 0; i < 2; i++) {
    ds[i] = wb_robot_get_device(ds_names[i]);
    wb_distance_sensor_enable(ds[i], TIME_STEP);
  }
  
  // initialise motors
  WbDeviceTag wheels[4];
  
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  for (i = 0; i < 4; i++) {
    wheels[i] = wb_robot_get_device(wheels_names[i]);
    wb_motor_set_position(wheels[i], INFINITY);
  }
  
  
  char *javino_received_msg;

  // feedback loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // init speeds
    double left_speed = 0.0;
    double right_speed = 0.0;
    
    javino_received_msg = javino_get_msg("/dev/ttyExogenous0");   
    
    if ( ! strcmp( javino_received_msg , "getPercepts" )  ){
        
      fprintf(stderr,
        "\nReceived: getPercepts (%d)\n",
        reasoning_cycle++ );
      
      // Distance sensor value
      float distance = wb_distance_sensor_get_value( (ds[0] + ds[1])/2 );
      
      // Composing percepts message to send to Javino
      int nbytes_written = sprintf(percepts_msg, 
        "distance(%.1f);",
        (1000 - distance) );
        
       if ( nbytes_written == -1 ){
       
         fprintf(stderr, "\nERROR: Couldn't compose perception strings!");
         
       } else {
       
         fprintf(stderr, "\n%s\n", percepts_msg);
         
       }
       
       nbytes_written = javino_send_msg("/dev/ttyExogenous0",
         percepts_msg);
         
       if ( (strlen(percepts_msg) + JAVINO_HEADER_LEN) != nbytes_written ){
         
         fprintf(stderr, 
           "\nError! Message length plus header (%lu) different from sent by Javino!(%u)!",
           (strlen(percepts_msg) + JAVINO_HEADER_LEN),
           nbytes_written );
              
       }
                                                      
    } else if ( ! strcmp( javino_received_msg , "goAhead" ) ){
    
      fprintf(stderr, 
        "\nReceived: goAhead (%d)\n",
        reasoning_cycle++ );
    
      left_speed = 1.0;
      right_speed = 1.0;          
    
    } else if ( ! strcmp( javino_received_msg , "goBack" ) ){
    
      fprintf(stderr, 
        "\nReceived: goBack (%d)\n",
        reasoning_cycle++ );
    
      left_speed = -1.0;
      right_speed = -1.0;             
    
    } else {
    
      fprintf(stderr, 
        "\nWARNING: unknown received reasoning (%d): (%s) \n",
        reasoning_cycle++,
        javino_received_msg ); 
    
    }
    
    if ( javino_received_msg ){
      free( javino_received_msg );
    }
     
    
/*    
    if (avoid_obstacle_counter > 0) {
    
      avoid_obstacle_counter--;
      left_speed = 1.0;
      right_speed = -1.0;
      
    } else {
      // read sensors outputs
      
      double ds_values[2];
      for (i = 0; i < 2; i++)
        ds_values[i] = wb_distance_sensor_get_value(ds[i]);

      // increase counter in case of obstacle
      if (ds_values[0] < 950.0 || ds_values[1] < 950.0)
        avoid_obstacle_counter = 100;
    }
*/    

    // write actuators inputs
    wb_motor_set_velocity(wheels[0], left_speed);
    wb_motor_set_velocity(wheels[1], right_speed);
    wb_motor_set_velocity(wheels[2], left_speed);
    wb_motor_set_velocity(wheels[3], right_speed);
  }

  // cleanup the Webots API
  wb_robot_cleanup();
  return 0;  // EXIT_SUCCESS
}
