CREATE EXTERNAL TABLE Zipcode2 (
    RecordNumber STRING,
    Zipcode STRING,
    ZipCodeType STRING,
    City STRING,
    State STRING,
    LocationType STRING,
    Lat STRING,
    Long STRING,
    Xaxis STRING,
    Yaxis STRING,
    Zaxis STRING,
    WorldRegion STRING,
    Country STRING,
    LocationText STRING,
    Location STRING,
    Decommisioned STRING,
    TaxReturnsFiled STRING,
    EstimatedPopulation STRING,
    TotalWages STRING,
    Notes STRING
)
ROW FORMAT DELIMITED
FIELDS TERMINATED BY ','
LOCATION '/user/test/zipcodes'
TBLPROPERTIES("skip.header.line.count"="1");

CREATE TABLE ZipcodePARQUET (
    RecordNumber STRING,
    Zipcode STRING,
    ZipCodeType STRING,
    City STRING,
    State STRING,
    LocationType STRING,
    Lat STRING,
    Long STRING,
    Xaxis STRING,
    Yaxis STRING,
    Zaxis STRING,
    WorldRegion STRING,
    Country STRING,
    LocationText STRING,
    Location STRING,
    Decommisioned STRING,
    TaxReturnsFiled STRING,
    EstimatedPopulation STRING,
    TotalWages STRING,
    Notes STRING
)
STORED AS PARQUET;

INSERT INTO ZipcodePARQUET
SELECT * FROM Zipcode2;

LOAD DATA INPATH '/user/test/free-zipcode-database.csv' OVERWRITE
INTO TABLE Zipcode;

CREATE TABLE ZipcodeFL AS
SELECT *
FROM Zipcode
WHERE state = '"FL"';