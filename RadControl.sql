CREATE DATABASE IF NOT EXISTS rad_control;
USE rad_control;

CREATE TABLE IF NOT EXISTS log(
  message VARCHAR(64) NOT NULL,
  time DATETIME NOT NULL
)ENGINE=ARCHIVE;
CREATE TABLE IF NOT EXISTS readings(
  modbus_id INT UNSIGNED NOT NULL,
  EDR FLOAT NOT NULL,
  count INT UNSIGNED NOT NULL,
  exposition INT UNSIGNED NOT NULL,
  exposition_by_count INT UNSIGNED NOT NULL,
  time DATETIME NOT NULL
)ENGINE=ARCHIVE;
#CREATE USER IF NOT EXISTS 'rad_control_bot' IDENTIFIED BY 'hflrjynhjkm'; --not supported until mysql 5.7.6
GRANT SELECT,INSERT on `rad_control`.* TO 'rad_control_bot'@'localhost' IDENTIFIED BY 'hflrjynhjkm';#--will create user if not exists
