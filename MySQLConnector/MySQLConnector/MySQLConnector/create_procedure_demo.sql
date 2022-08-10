CREATE PROCEDURE add_country (IN country_code CHAR(3),
                              IN country_name CHAR(52),
                              IN continent_name CHAR(30))
BEGIN
  INSERT INTO Country(Code, Name, Continent)
  VALUES (country_code, country_name, continent_name)
    
END;