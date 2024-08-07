/*--------------------------------------------------------------
 * The SQL for generating the table that we want to track
 *
 * @author: Wild Boar
 *--------------------------------------------------------------*/

/*----------------------------
* Create the database
------------------------------*/
CREATE DATABASE BlueROV;

/*----------------------------
* Create a status table
------------------------------*/

create table status (
  id INT auto_increment,
  latitude FLOAT not null,
  longitude FLOAT not null,
  heading FLOAT not null,
  depth FLOAT not null,
  altitude FLOAT not null,
  temperature FLOAT not null,
  mode varchar(255) not null,
  sat_count INT not null,
  pos_certainty FLOAT not null,
  velocity_valid BOOLEAN not null,
  fom FLOAT not null,
  created_at datetime not null default CURRENT_TIMESTAMP,
  primary key(id),
  INDEX create_idx (created_at)
);

/*----------------------------
* Update Example
------------------------------*/
INSERT INTO status (latitude, longitude, heading, depth, altitude, temperature, mode, sat_count, pos_certainty, velocity_valid, fom) 
VALUES (5,6,90, 1000, 1, 13, 'DEPTH HOLD', 5, 0.9, 1, 0.8); 