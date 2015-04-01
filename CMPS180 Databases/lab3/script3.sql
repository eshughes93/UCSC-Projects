/*
 * Evan Hughes
 * CMPS180 Lab 3
 * script3.sql
 */

/* 3.3.1 Film length must be positive  */
ALTER TABLE dv_film
ADD CONSTRAINT pos_length CHECK (length>0);

/* 3.3.2 Customer records must not have empty foreign key
   to address table. First updates null fields then sets constraint */
UPDATE mg_customers
SET address_id = 1
WHERE address_id IS NULL;

ALTER TABLE mg_customers
ALTER COLUMN address_id SET NOT NULL;


/* 3.3.3 Address records can't be empty ( NULL or '' ???) */
ALTER TABLE dv_address
ALTER COLUMN address SET NOT NULL;
/*
ALTER TABLE dv_address
ADD CONSTRAINT not_empty CHECK (address <> '');
*/
