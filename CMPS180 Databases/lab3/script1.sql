/*
 * Evan Hughes
 * CMPS180 Lab 3
 * script1.sql
 */

/* 3.1 Adds primary keys as instructed in assignment */
ALTER TABLE mg_customers
ADD PRIMARY KEY (customer_id);

ALTER TABLE dv_address
ADD PRIMARY KEY (address_id);

ALTER TABLE dv_film
ADD PRIMARY KEY (film_id);

ALTER TABLE cb_books
ADD PRIMARY KEY (title, author_id);

ALTER TABLE cb_authors
ADD PRIMARY KEY (author_id);


