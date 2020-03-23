--Location
INSERT INTO Location (id,country,state,address,city,postcode) VALUES ('2','CH','GE','Rue de Genève 2', 'Genève', '1200');
INSERT INTO Location (id,country,state,address,city,postcode) VALUES ('3','CH','FR','Rue de Fribourg 3', 'Fribourg', '1700');
INSERT INTO Location (id,country,state,address,city,postcode) VALUES ('4','CH','BE','Rue de Bern 4', 'Bern', '3000');
INSERT INTO Location (id,country,state,address,city,postcode) VALUES ('5','CH','VD','Rue de Lausanne 1', 'Lausanne', '1000');

--Four persons
INSERT INTO Person (id,name,firstname,gender,birthday,Location) VALUES ('1','Doe','John','true','1901-01-01','2');
INSERT INTO Person (id,name,firstname,gender,birthday,Location) VALUES ('2','Doe','Jane','false','1902-02-02','3');
INSERT INTO Person (id,name,firstname,gender,birthday,Location) VALUES ('3','Cooper','Sheldon','true','1903-03-03','4');
INSERT INTO Person (id,name,firstname,gender,birthday,Location) VALUES ('4','Banner','Bruce','false','1904-04-04','5');

--Five phones
INSERT INTO Phone (id,number,Phone_Type,Person) VALUES ('1','021/000 00 00','work','3');
INSERT INTO Phone (id,number,Phone_Type,Person) VALUES ('2','079/111 11 11','work','3');
INSERT INTO Phone (id,number,Phone_Type,Person) VALUES ('3','021/222 22 22','home','3');
INSERT INTO Phone (id,number,Phone_Type,Person) VALUES ('4','021/333 33 33','fax','3');
INSERT INTO Phone (id,number,Phone_Type,Person) VALUES ('5','021/444 44 44','work','4');

--Three emails
INSERT INTO Email (id,address,Person) VALUES ('1','sheldon.cooper@chuv.ch','3');
INSERT INTO Email (id,address,Person) VALUES ('2','bruce.banner@chuv.ch','4');
INSERT INTO Email (id,address,Person) VALUES ('3','some.service@chuv.ch','4');

--Two declared as patients
INSERT INTO Patient (id,stat_ok,Person) VALUES ('1','false','1');
INSERT INTO Patient (id,stat_ok,Person) VALUES ('2','true','2');

--Two declared as practicians
INSERT INTO Practician (id,title,Person,Institute) VALUES ('1','Dr','3','1');
INSERT INTO Practician (id,title,Person,Institute) VALUES ('2','Dr','4','1');

--Drugs for measures
INSERT INTO Drug (sql_id,id) VALUES ('1','ch.heig-vd.ezechiel.tato');
INSERT INTO Drug (sql_id,id) VALUES ('2','ch.heig-vd.ezechiel.arispine');

--Covariates for the first patient
INSERT INTO Covariate (sql_id,value,mid,Patient,date) VALUES ('1','74','weight','1','2001-01-01');
INSERT INTO Covariate (sql_id,value,mid,Patient,date) VALUES ('2','1','gist','1','2002-02-02');

--Covariates for the second patient
INSERT INTO Covariate (sql_id,value,mid,Patient,date) VALUES ('3','95','weight','2','2003-03-03');

--Measures for the first patient
INSERT INTO Measure (id,Patient,date,Drug,value,unit,arrivalDate) VALUES ('1','1','2001-01-01','1','100','[ug/l]','2001-01-01');

--Measures for the second patient
INSERT INTO Measure (id,Patient,date,Drug,value,unit,arrivalDate) VALUES ('2','2','2002-02-02','2','200','[ug/l]','2002-02-02');
INSERT INTO Measure (id,Patient,date,Drug,value,unit,arrivalDate) VALUES ('3','2','2003-03-03','1','300','[ug/l]','2003-03-03');

