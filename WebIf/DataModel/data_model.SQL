SET FOREIGN_KEY_CHECKS=0;



CREATE TABLE Log
(
	id INTEGER NOT NULL,
	log_date DATETIME,
	log_entry TEXT,
	state INTEGER,
	PRIMARY KEY (id),
	UNIQUE UQ_Log_id(id)
) 
;


CREATE TABLE Motor
(
	id INTEGER NOT NULL,
	duration INTEGER NOT NULL,
	channel INTEGER,
	PRIMARY KEY (id),
	UNIQUE UQ_Motor_id(id)
) 
;


CREATE TABLE Plant
(
	id INTEGER NOT NULL,
	name VARCHAR(30),
	description TEXT,
	watering_interval INTEGER,
	PRIMARY KEY (id),
	UNIQUE UQ_Plant_id(id)
) 
;


CREATE TABLE Sensor
(
	id INTEGER NOT NULL,
	channel INTEGER,
	frequency INTEGER,
	PRIMARY KEY (id),
	UNIQUE UQ_Sensor_id(id)
) 
;


CREATE TABLE User
(
	id INTEGER NOT NULL,
	name VARCHAR(20),
	pssword VARCHAR(20),
	PRIMARY KEY (id),
	UNIQUE UQ_User_id(id)
) 
;


CREATE TABLE Watering
(
	id INTEGER NOT NULL,
	watering_date DATETIME,
	duration INTEGER,
	state INTEGER,
	PRIMARY KEY (id),
	UNIQUE UQ_Watering_id(id)
) 
;



SET FOREIGN_KEY_CHECKS=1;