-- Creator:       MySQL Workbench 6.0.8/ExportSQLite plugin 2009.12.02
-- Author:        Unknown
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2014-10-28 15:48
-- Created:       2011-04-07 16:31
PRAGMA foreign_keys = OFF;

-- Schema: localdb
BEGIN;
CREATE TABLE "Location"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "country" TEXT,
  "state" TEXT,
  "address" TEXT,
  "city" TEXT,
  "postcode" TEXT
);
CREATE TABLE "Person"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" TEXT,
  "firstname" TEXT,
  "gender" BOOL,
  "birthday" DATE,
  "Location" INTEGER NOT NULL,
  CONSTRAINT "fk_Person_Location1"
    FOREIGN KEY("Location")
    REFERENCES "Location"("id")
    ON UPDATE CASCADE
);
CREATE INDEX "Person.fk_Person_Location1_idx" ON "Person"("Location");
CREATE TABLE "Phone"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "number" TEXT,
  "Phone_Type" TEXT,
  "Person" INTEGER NOT NULL,
  CONSTRAINT "fk_Phone_Person1"
    FOREIGN KEY("Person")
    REFERENCES "Person"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Phone.fk_Phone_Person1_idx" ON "Phone"("Person");
CREATE TABLE "Patient"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "stat_ok" BOOL,
  "Person" INTEGER NOT NULL,
  "external_id" TEXT,
  "stay_number" TEXT,
  CONSTRAINT "external_id_UNIQUE"
    UNIQUE("external_id"),
  CONSTRAINT "fk_Patient_Person1"
    FOREIGN KEY("Person")
    REFERENCES "Person"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Patient.fk_Patient_Person1_idx" ON "Patient"("Person");
CREATE TABLE "Covariate"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "value" DOUBLE,
  "Patient" INTEGER NOT NULL,
  "date" DATETIME,
  "mid" TEXT,
  CONSTRAINT "fk_Covariate_Patient1"
    FOREIGN KEY("Patient")
    REFERENCES "Patient"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Covariate.fk_Covariate_Patient1_idx" ON "Covariate"("Patient");
CREATE TABLE "Fields"(
  "id" INTEGER PRIMARY KEY NOT NULL
);
CREATE TABLE "Post_Engine"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "version" FLOAT,
  "id" TEXT
);
CREATE TABLE "Model"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "version" FLOAT,
  "id" TEXT
);
CREATE TABLE "Drug"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "version" FLOAT,
  "id" TEXT
);
CREATE TABLE "Encryption"(
  "key" TEXT PRIMARY KEY NOT NULL,
  "value" BOOL NOT NULL DEFAULT 0
);
CREATE TABLE "Percentile_Engine"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "version" FLOAT,
  "id" TEXT
);
CREATE TABLE "Option"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "id" TEXT,
  "value" BLOB
);
CREATE TABLE "Reverse_Engine"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "version" FLOAT,
  "id" TEXT
);
CREATE TABLE "Clinicals"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "Patient" INTEGER NOT NULL,
  "date" DATETIME,
  "value" TEXT,
  "type" TEXT,
  CONSTRAINT "fk_Clinicals_Patient1"
    FOREIGN KEY("Patient")
    REFERENCES "Patient"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Clinicals.fk_Clinicals_Patient1_idx" ON "Clinicals"("Patient");
CREATE TABLE "Version"(
  "id" TEXT PRIMARY KEY NOT NULL
);
CREATE TABLE "User1"(
  "username" TEXT PRIMARY KEY NOT NULL,
  "key" TEXT,
  "authorization" INTEGER NOT NULL DEFAULT 0
);
CREATE TABLE "Institute"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" TEXT,
  "logo" BLOB,
  "Location" INTEGER NOT NULL,
  "external_id" TEXT,
  CONSTRAINT "external_id_UNIQUE"
    UNIQUE("external_id"),
  CONSTRAINT "fk_Institute_Location1"
    FOREIGN KEY("Location")
    REFERENCES "Location"("id")
    ON UPDATE CASCADE
);
CREATE INDEX "Institute.fk_Institute_Location1_idx" ON "Institute"("Location");
CREATE TABLE "Email"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "address" TEXT,
  "Person" INTEGER NOT NULL,
  CONSTRAINT "fk_email_Person1"
    FOREIGN KEY("Person")
    REFERENCES "Person"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Email.fk_email_Person1_idx" ON "Email"("Person");
CREATE TABLE "Practician"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "title" TEXT,
  "Person" INTEGER NOT NULL,
  "Institute" INTEGER NOT NULL,
  "external_id" TEXT,
  CONSTRAINT "external_id_UNIQUE"
    UNIQUE("external_id"),
  CONSTRAINT "fk_Doctor_Person1"
    FOREIGN KEY("Person")
    REFERENCES "Person"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Doctor_Institute1"
    FOREIGN KEY("Institute")
    REFERENCES "Institute"("id")
    ON UPDATE CASCADE
);
CREATE INDEX "Practician.fk_Doctor_Person1_idx" ON "Practician"("Person");
CREATE INDEX "Practician.fk_Doctor_Institute1_idx" ON "Practician"("Institute");
CREATE TABLE "Measure"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "comment" TEXT,
  "Patient" INTEGER NOT NULL,
  "date" DATETIME,
  "value" DOUBLE,
  "unit" TEXT,
  "Drug" INTEGER NOT NULL,
  "sampleID" TEXT,
  "arrivalDate" DATETIME,
  CONSTRAINT "fk_Measure_Patient1"
    FOREIGN KEY("Patient")
    REFERENCES "Patient"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Measure_Drug1"
    FOREIGN KEY("Drug")
    REFERENCES "Drug"("sql_id")
    ON UPDATE CASCADE
);
CREATE INDEX "Measure.fk_Measure_Patient1_idx" ON "Measure"("Patient");
CREATE INDEX "Measure.fk_Measure_Drug1_idx" ON "Measure"("Drug");
CREATE TABLE "DxP"(
  "Doctor" INTEGER NOT NULL,
  "Patient" INTEGER NOT NULL,
  PRIMARY KEY("Doctor","Patient"),
  CONSTRAINT "fk_Doctor_has_Patient_Doctor1"
    FOREIGN KEY("Doctor")
    REFERENCES "Practician"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Doctor_has_Patient_Patient1"
    FOREIGN KEY("Patient")
    REFERENCES "Patient"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "DxP.fk_Doctor_has_Patient_Patient1_idx" ON "DxP"("Patient");
CREATE INDEX "DxP.fk_Doctor_has_Patient_Doctor1_idx" ON "DxP"("Doctor");
CREATE TABLE "Report"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "date" DATE,
  "analyst" INTEGER NOT NULL,
  "applicant" INTEGER NOT NULL,
  "Prediction" INTEGER NOT NULL,
  CONSTRAINT "fk_Report_Practician1"
    FOREIGN KEY("analyst")
    REFERENCES "Practician"("id")
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Report_Practician2"
    FOREIGN KEY("applicant")
    REFERENCES "Practician"("id")
    ON UPDATE CASCADE
);
CREATE INDEX "Report.fk_Report_Practician1_idx" ON "Report"("analyst");
CREATE INDEX "Report.fk_Report_Practician2_idx" ON "Report"("applicant");
CREATE TABLE "FxP"(
  "Report" INTEGER NOT NULL,
  "Fields" INTEGER NOT NULL,
  PRIMARY KEY("Report","Fields"),
  CONSTRAINT "fk_Fields_has_Report_Fields1"
    FOREIGN KEY("Fields")
    REFERENCES "Fields"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Fields_has_Report_Report1"
    FOREIGN KEY("Report")
    REFERENCES "Report"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "FxP.fk_Fields_has_Report_Report1_idx" ON "FxP"("Report");
CREATE INDEX "FxP.fk_Fields_has_Report_Fields1_idx" ON "FxP"("Fields");
CREATE TABLE "Copy_at"(
  "Practician" INTEGER NOT NULL,
  "Report" INTEGER NOT NULL,
  PRIMARY KEY("Practician","Report"),
  CONSTRAINT "fk_Report_has_Practician_Report1"
    FOREIGN KEY("Report")
    REFERENCES "Report"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Report_has_Practician_Practician1"
    FOREIGN KEY("Practician")
    REFERENCES "Practician"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Copy_at.fk_Report_has_Practician_Practician1_idx" ON "Copy_at"("Practician");
CREATE INDEX "Copy_at.fk_Report_has_Practician_Report1_idx" ON "Copy_at"("Report");
CREATE TABLE "Curve"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "name" TEXT,
  "Drug" INTEGER NOT NULL,
  "comment" TEXT,
  "Patient" INTEGER NOT NULL,
  "Model" INTEGER NOT NULL,
  "Post_Engine" INTEGER NOT NULL,
  "Percentile_Engine" INTEGER NOT NULL,
  "Reverse_Engine" INTEGER NOT NULL,
  "ptype" INTEGER,
  "first_take" DATETIME,
  "ctype" INTEGER,
  CONSTRAINT "fk_Curve_Drug1"
    FOREIGN KEY("Drug")
    REFERENCES "Drug"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Curve_Patient1"
    FOREIGN KEY("Patient")
    REFERENCES "Patient"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Curve_Model1"
    FOREIGN KEY("Model")
    REFERENCES "Model"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Curve_Post_Engine1"
    FOREIGN KEY("Post_Engine")
    REFERENCES "Post_Engine"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Curve_Prediction_Engine1"
    FOREIGN KEY("Percentile_Engine")
    REFERENCES "Percentile_Engine"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Curve_Reverse_Engine1"
    FOREIGN KEY("Reverse_Engine")
    REFERENCES "Reverse_Engine"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE
);
CREATE INDEX "Curve.fk_Curve_Drug1_idx" ON "Curve"("Drug");
CREATE INDEX "Curve.fk_Curve_Patient1_idx" ON "Curve"("Patient");
CREATE INDEX "Curve.fk_Curve_Model1_idx" ON "Curve"("Model");
CREATE INDEX "Curve.fk_Curve_Post_Engine1_idx" ON "Curve"("Post_Engine");
CREATE INDEX "Curve.fk_Curve_Prediction_Engine1_idx" ON "Curve"("Percentile_Engine");
CREATE INDEX "Curve.fk_Curve_Reverse_Engine1_idx" ON "Curve"("Reverse_Engine");
CREATE TABLE "MxO"(
  "Model" INTEGER NOT NULL,
  "Option" INTEGER NOT NULL,
  "Curve" INTEGER NOT NULL,
  PRIMARY KEY("Model","Option"),
  CONSTRAINT "fk_Model_has_Option_Model1"
    FOREIGN KEY("Model")
    REFERENCES "Model"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Model_has_Option_Option1"
    FOREIGN KEY("Option")
    REFERENCES "Option"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_MxO_Curve1"
    FOREIGN KEY("Curve")
    REFERENCES "Curve"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "MxO.fk_Model_has_Option_Option1_idx" ON "MxO"("Option");
CREATE INDEX "MxO.fk_Model_has_Option_Model1_idx" ON "MxO"("Model");
CREATE INDEX "MxO.fk_MxO_Curve1_idx" ON "MxO"("Curve");
CREATE TABLE "PxO"(
  "Post_Engine" INTEGER NOT NULL,
  "Option" INTEGER NOT NULL,
  "Curve" INTEGER NOT NULL,
  PRIMARY KEY("Post_Engine","Option"),
  CONSTRAINT "fk_Post_Engine_has_Option_Post_Engine1"
    FOREIGN KEY("Post_Engine")
    REFERENCES "Post_Engine"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Post_Engine_has_Option_Option1"
    FOREIGN KEY("Option")
    REFERENCES "Option"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_PxO_Curve1"
    FOREIGN KEY("Curve")
    REFERENCES "Curve"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "PxO.fk_Post_Engine_has_Option_Option1_idx" ON "PxO"("Option");
CREATE INDEX "PxO.fk_Post_Engine_has_Option_Post_Engine1_idx" ON "PxO"("Post_Engine");
CREATE INDEX "PxO.fk_PxO_Curve1_idx" ON "PxO"("Curve");
CREATE TABLE "ExO"(
  "Prediction_Engine" INTEGER NOT NULL,
  "Option" INTEGER NOT NULL,
  "Curve" INTEGER NOT NULL,
  PRIMARY KEY("Prediction_Engine","Option"),
  CONSTRAINT "fk_Prediction_Engine_has_Option_Prediction_Engine1"
    FOREIGN KEY("Prediction_Engine")
    REFERENCES "Percentile_Engine"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Prediction_Engine_has_Option_Option1"
    FOREIGN KEY("Option")
    REFERENCES "Option"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_ExO_Curve1"
    FOREIGN KEY("Curve")
    REFERENCES "Curve"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "ExO.fk_Prediction_Engine_has_Option_Option1_idx" ON "ExO"("Option");
CREATE INDEX "ExO.fk_Prediction_Engine_has_Option_Prediction_Engine1_idx" ON "ExO"("Prediction_Engine");
CREATE INDEX "ExO.fk_ExO_Curve1_idx" ON "ExO"("Curve");
CREATE TABLE "RxO"(
  "Reverse_Engine" INTEGER NOT NULL,
  "Option" INTEGER NOT NULL,
  "Curve" INTEGER NOT NULL,
  PRIMARY KEY("Reverse_Engine","Option"),
  CONSTRAINT "fk_Reverse_Engine_has_Option_Reverse_Engine1"
    FOREIGN KEY("Reverse_Engine")
    REFERENCES "Reverse_Engine"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_Reverse_Engine_has_Option_Option1"
    FOREIGN KEY("Option")
    REFERENCES "Option"("sql_id")
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT "fk_RxO_Curve1"
    FOREIGN KEY("Curve")
    REFERENCES "Curve"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "RxO.fk_Reverse_Engine_has_Option_Option1_idx" ON "RxO"("Option");
CREATE INDEX "RxO.fk_Reverse_Engine_has_Option_Reverse_Engine1_idx" ON "RxO"("Reverse_Engine");
CREATE INDEX "RxO.fk_RxO_Curve1_idx" ON "RxO"("Curve");
CREATE TABLE "Dosage"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "comment" TEXT,
  "date" DATE,
  "Curve" INTEGER NOT NULL,
  "value" DOUBLE,
  "unit" TEXT,
  "tinf" DOUBLE,
  "interval" DOUBLE,
  CONSTRAINT "fk_Dosage_Curve1"
    FOREIGN KEY("Curve")
    REFERENCES "Curve"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Dosage.fk_Dosage_Curve1_idx" ON "Dosage"("Curve");
CREATE TABLE "View"(
  "id" INTEGER PRIMARY KEY NOT NULL,
  "comment" TEXT,
  "name" TEXT,
  "image" BLOB,
  "Report" INTEGER NOT NULL,
  CONSTRAINT "fk_View_Report1"
    FOREIGN KEY("Report")
    REFERENCES "Report"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "View.fk_View_Report1_idx" ON "View"("Report");
CREATE TABLE "Parameter"(
  "sql_id" INTEGER PRIMARY KEY NOT NULL,
  "value" DOUBLE,
  "pid" TEXT,
  "Curve" INTEGER NOT NULL,
  CONSTRAINT "fk_Parameter_Curve1"
    FOREIGN KEY("Curve")
    REFERENCES "Curve"("id")
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
CREATE INDEX "Parameter.fk_Parameter_Curve1_idx" ON "Parameter"("Curve");
COMMIT;
