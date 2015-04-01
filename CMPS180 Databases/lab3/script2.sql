/*
 * Evan Hughes
 * CMPS180 Lab 3
 * script2.sql
 */

/* 3.2.1 Sets foreign key for address id from mg_customers referencing dv_address */
ALTER TABLE mg_customers
ADD FOREIGN KEY (address_id)
REFERENCES dv_address (address_id);

/* 3.2.2 Sets foreign key for author_id from cb_books referencing cb_authors */
ALTER TABLE cb_books
ADD FOREIGN KEY (author_id)
REFERENCES cb_authors (author_id);


