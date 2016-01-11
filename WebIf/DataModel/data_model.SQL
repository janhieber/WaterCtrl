
CREATE TABLE Log
(
	id integer PRIMARY KEY DEFAULT nextval('serial'),
	log_date timestamp,
	log_entry varchar(128),
	state INTEGER
) 
;


CREATE TABLE Motor
(
	id integer PRIMARY KEY DEFAULT nextval('serial'),
	duration integer,
	channel INTEGER
) 
;


CREATE TABLE Plant
(
	id integer PRIMARY KEY DEFAULT nextval('serial'),
	name VARCHAR(30),
	description varchar(128),
	watering_interval INTEGER
) 
;


CREATE TABLE Sensor
(
	id integer PRIMARY KEY DEFAULT nextval('serial'),
	channel INTEGER,
	frequency INTEGER
) 
;


CREATE TABLE User
(
	id integer PRIMARY KEY DEFAULT nextval('serial'),
	name VARCHAR(20),
	pssword VARCHAR(20)
) 
;


CREATE TABLE Watering
(
	id integer PRIMARY KEY DEFAULT nextval('serial'),
	watering_date timestamp,
	duration INTEGER,
	state INTEGER
) 
;
