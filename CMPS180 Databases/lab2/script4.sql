/*
SELECT d.first_name, d.last_name
FROM dv_customers d, cb_customers c
WHERE d.first_name = c.first_name AND d.last_name = c.last_name;
*/
SELECT phone
FROM dv_address
WHERE address_id IN (SELECT d.address_id 
					 FROM dv_customers d, cb_customers c
					 WHERE d.first_name = c.first_name AND d.last_name = c.last_name);
/*					 
SELECT first_name, last_name
FROM dv_customers
WHERE address_id IN (SELECT address_id 
					 FROM dv_address
					 WHERE district IN (SELECT district
					 					FROM dv_address
					 					ORDER BY count(address_id) DESC
					 					LIMIT 1));
					 					
SELECT rating, COUNT(*)
FROM dv_film
WHERE rating IN (SELECT rating
				 FROM dv_film
				 ORDER BY count(rating) ASC
				 LIMIT 1);
*/