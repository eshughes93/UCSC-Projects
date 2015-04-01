/*
 * Evan Hughes
 * CMPS180 Lab 3
 * script4.sql
 */

/* Foreign key contraints unit tests */
DELETE FROM dv_address
WHERE address_id IS NOT NULL;

DELETE FROM cb_authors
WHERE author_id IS NOT NULL;

/* General constraints unit tests */
/* (1) Film length is positive*/
INSERT INTO dv_film (film_id, length)
VALUES (9999,120);

INSERT INTO dv_film (film_id, length)
VALUES (9999,-120);


/* (2) Customer records must not have empty foreign
   key to address table */
INSERT INTO mg_customers (address_id)
VALUES (1);

INSERT INTO mg_customers (address_id)
VALUES (NULL);


/* (3) Address records are not empty */
INSERT INTO dv_address (address, address_id)
VALUES (9999, 9999);

INSERT INTO dv_address(address, address_id)
VALUES (NULL, 9998);

