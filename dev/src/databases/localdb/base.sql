--Current versions
INSERT INTO Version (id) VALUES ('0.2.1');
INSERT INTO Version (id) VALUES ('0.2.2');
INSERT INTO Version (id) VALUES ('0.2.3');
INSERT INTO Version (id) VALUES ('0.2.4');
INSERT INTO Version (id) VALUES ('0.2.5');
INSERT INTO Version (id) VALUES ('0.2.6');
INSERT INTO Version (id) VALUES ('0.2.7');

--Encryption config
INSERT INTO Encryption (key) VALUES ('encrypted');

--Temporary institute
INSERT INTO Location (country, state, address, city, postcode) VALUES ('Suisse', 'VD', 'Division de pharmacologie clinique\nRue du Bugnon 17', 'Lausanne', '1005');
INSERT INTO Institute (name, location) VALUES ('Centre hospitalier universitaire vaudois', 1);
