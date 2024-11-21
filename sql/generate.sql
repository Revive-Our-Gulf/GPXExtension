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
* Create the database
------------------------------*/
USE BlueROV;

/*----------------------------
* Create a status table
------------------------------*/

create table status 
(
  id INT auto_increment,
  latitude DOUBLE not null,
  longitude DOUBLE not null,
  heading DOUBLE not null,
  rov_depth DOUBLE not null,
  dvl_altitude DOUBLE not null,
  temperature DOUBLE not null,
  drive_mode varchar(255) not null,
  gps_sat_count INT not null,
  gps_pos_certainty DOUBLE not null,
  dvl_velocity_valid BOOLEAN not null,
  dvl_fom DOUBLE not null,
  created_at DATETIME not null default CURRENT_TIMESTAMP,
  primary key(id),
  INDEX create_idx (created_at)
);

/*----------------------------
* Create a settings table
------------------------------*/
create table settings 
(
  id INT auto_increment,
  field_name VARCHAR(255),
  field_value VARCHAR(255),
  PRIMARY KEY(id)
);

/*----------------------------
* Update Example
------------------------------*/
INSERT INTO status (latitude, longitude, heading, rov_depth, dvl_altitude, temperature, drive_mode, gps_sat_count, gps_pos_certainty, dvl_velocity_valid, dvl_fom) 
VALUES (5,6,90, 1000, 1, 13, 'DEPTH HOLD', 5, 0.9, 1, 0.8); 

/*----------------------------
* ADD settings
------------------------------*/
INSERT INTO settings (field_name, field_value) VALUES ('LOGGER_STATE', 'STOPPED');
INSERT INTO settings (field_name, field_value) VALUES ('RATE','1000');
