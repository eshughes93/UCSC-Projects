/*
 * Evan Hughes
 * CMPS180 Lab 3
 * script5.sql
 */


/* 3.5.1 Create view cusomter city with fn, ln, and cityid of all people
   who are customers of Downtown Video and City Books */
CREATE VIEW customer_city AS
   SELECT d.first_name, d.last_name, da.city_id
   FROM dv_customer d, dv_address da, cb_customers c
   WHERE d.first_name = c.first_name AND
         d.last_name = c.last_name AND
         d.address_id = da.address_id;

/* 3.5.2 Create view district stats with district and number of 
   Downtown Video customers in that district*/
CREATE VIEW district_stats AS
   SELECT da.district, COUNT(da.address_id)
   FROM dv_address da, dv_customer dc
   WHERE da.address_id = dc.address_id
   GROUP BY district
   ORDER BY COUNT(da.address_id) ASC;

/* 3.5.3 First five districts with least customers from
   district stats view */
SELECT district
FROM district_stats
LIMIT 5;

/* Rename customer_city view to ccity*/
ALTER VIEW customer_city RENAME TO ccity;
