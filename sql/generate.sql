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
  position_counter INT not null,
  heading DOUBLE not null,
  depth DOUBLE not null,
  temperature DOUBLE not null,
  drive_mode INT not null,
  gps_satellites INT not null,
  gps_hdop DOUBLE not null,
  gps_haccuracy DOUBLE not null,
  dvl_distance DOUBLE not null,
  dvl_fom DOUBLE not null,
  dvl_velocity_valid BOOLEAN not null,
  track_name varchar(500) default 'unknown',
  created_at DATETIME(1) not null default CURRENT_TIMESTAMP,
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
INSERT INTO status (latitude, longitude, position_counter, heading, depth, temperature, drive_mode, gps_satellites, gps_hdop, gps_haccuracy, dvl_distance, dvl_fom, dvl_velocity_valid, track_name) 
VALUES (5, 6, 0, 90, 1000, 13, 1, 5, 0.9, 1, 0.8, 0.5, true, 'unknown');

/*----------------------------
* ADD settings
------------------------------*/
INSERT INTO settings (field_name, field_value) VALUES ('LOGGER_STATE', 'STOPPED');
INSERT INTO settings (field_name, field_value) VALUES ('RATE','500');
INSERT INTO settings (field_name, field_value) VALUES ('CURRENT_TRACK', 'unknown');
INSERT INTO settings (field_name, field_value) VALUES ('TIME_ZONE', '+0');