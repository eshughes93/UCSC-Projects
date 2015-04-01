DROP SEQUENCE mg_customers_seq;

INSERT INTO mg_customers (customer_id, first_name, last_name, email, address_id, active)
SELECT *
FROM dv_customers;

CREATE SEQUENCE mg_customers_seq START 600;

ALTER TABLE mg_customers
ALTER COLUMN customer_id
SET DEFAULT NEXTVAL('mg_customers_seq');

INSERT INTO mg_customers (first_name,last_name)
SELECT cb_customers.first_name, cb_customers.last_name 
FROM cb_customers
EXCEPT
SELECT d.first_name, d.last_name
FROM dv_customers d, cb_customers c
WHERE d.first_name = c.first_name AND d.last_name = c.last_name;
