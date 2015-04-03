SET FOREIGN_KEY_CHECKS=0;



CREATE TABLE Log
(
	id INTEGER NOT NULL,
	log_date DATETIME,
	log_entry TEXT,
	PRIMARY KEY (id)
) 
;


CREATE TABLE Motor
(
	id INTEGER NOT NULL,
	duration INTEGER NOT NULL,
	channel INTEGER,
	PRIMARY KEY (id)
) 
;


CREATE TABLE Plant
(
	id INTEGER NOT NULL,
	name VARCHAR(30),
	description TEXT,
	PRIMARY KEY (id)
) 
;


CREATE TABLE Sensor
(
	id INTEGER NOT NULL,
	channel INTEGER,
	frequency INTEGER,
	PRIMARY KEY (id)
) 
;


CREATE TABLE User
(
	id INTEGER NOT NULL,
	name VARCHAR(20),
	pssword VARCHAR(20),
	PRIMARY KEY (id)
) 
;


CREATE TABLE Watering
(
	id INTEGER NOT NULL,
	watering_date DATETIME,
	duration INTEGER,
	state INTEGER,
	PRIMARY KEY (id)
) 
;



SET FOREIGN_KEY_CHECKS=1;
