-- Creator:       MySQL Workbench 6.0.8/ExportSQLite plugin 2009.12.02
-- Author:        Unknown
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2015-07-27 21:44
-- Created:       2011-04-07 16:31
PRAGMA foreign_keys = OFF;

-- Schema: localdb
BEGIN;
CREATE TABLE "Encryption"(
 "key" TEXT PRIMARY KEY NOT NULL,
 "value" BOOL NOT NULL DEFAULT 0
);
CREATE TABLE "Option"(
 "sql_id" INTEGER PRIMARY KEY NOT NULL,
 "id" TEXT,
 "value" BLOB
);
CREATE TABLE "Version"(
 "id" TEXT PRIMARY KEY NOT NULL
);
CREATE TABLE "User"(
 "username" TEXT PRIMARY KEY NOT NULL,
 "key" TEXT,
 "authorization" INTEGER NOT NULL DEFAULT 0
);
CREATE TABLE "Modification"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL DEFAULT 1
);
CREATE TABLE "DrugModel"(
 "pkid_drugmodel" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_DrugModel_DrugModel1"
  FOREIGN KEY("postmod_id")
 REFERENCES "DrugModel"("pkid_drugmodel"),
 CONSTRAINT "fk_DrugModel_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "DrugModel.fk_DrugModel_DrugModel1_idx" ON "DrugModel"("postmod_id");
CREATE INDEX "DrugModel.fk_DrugModel_Modification1_idx" ON "DrugModel"("modification_id");
CREATE TABLE "PredictionSpec"(
 "pkid_predictionspec" INTEGER PRIMARY KEY NOT NULL,
 "startDate" DATETIME,
 "endDate" DATETIME,
 "steadyState" BOOLEAN,
 "nbPoints" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_PredictionSpec_PredictionSpec1"
  FOREIGN KEY("postmod_id")
 REFERENCES "PredictionSpec"("pkid_predictionspec"),
 CONSTRAINT "fk_PredictionSpec_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "PredictionSpec.fk_PredictionSpec_PredictionSpec1_idx" ON "PredictionSpec"("postmod_id");
CREATE INDEX "PredictionSpec.fk_PredictionSpec_Modification1_idx" ON "PredictionSpec"("modification_id");
CREATE TABLE "Location"(
 "pkid_location" INTEGER PRIMARY KEY NOT NULL,
 "country" TEXT,
 "state" TEXT,
 "address" TEXT,
 "city" TEXT,
 "postcode" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Location_Location1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Location"("pkid_location"),
 CONSTRAINT "fk_Location_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Location.fk_Location_Location1_idx" ON "Location"("postmod_id");
CREATE INDEX "Location.fk_Location_Modification1_idx" ON "Location"("modification_id");
CREATE TABLE "Person"(
 "pkid_person" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "firstname" TEXT,
 "gender" BOOL,
 "birthday" DATE,
 "location_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Person_Location1"
  FOREIGN KEY("location_id")
 REFERENCES "Location"("pkid_location")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Person_Person1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Person"("pkid_person"),
 CONSTRAINT "fk_Person_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Person.fk_Person_Location1_idx" ON "Person"("location_id");
CREATE INDEX "Person.fk_Person_Person1_idx" ON "Person"("postmod_id");
CREATE INDEX "Person.fk_Person_Modification1_idx" ON "Person"("modification_id");
CREATE TABLE "Phone"(
 "pkid_phone" INTEGER PRIMARY KEY NOT NULL,
 "number" TEXT,
 "Phone_Type" TEXT,
 "person_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Phone_Person1"
  FOREIGN KEY("person_id")
 REFERENCES "Person"("pkid_person")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Phone_Phone1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Phone"("pkid_phone"),
 CONSTRAINT "fk_Phone_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Phone.fk_Phone_Person1_idx" ON "Phone"("person_id");
CREATE INDEX "Phone.fk_Phone_Phone1_idx" ON "Phone"("postmod_id");
CREATE INDEX "Phone.fk_Phone_Modification1_idx" ON "Phone"("modification_id");
CREATE TABLE "Patient"(
 "pkid_patient" INTEGER PRIMARY KEY NOT NULL,
 "stat_ok" BOOL,
 "person_id" INTEGER NOT NULL,
 "external_id" TEXT,
 "stay_number" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Patient_Person1"
  FOREIGN KEY("person_id")
 REFERENCES "Person"("pkid_person")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Patient_Patient1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Patient"("pkid_patient"),
 CONSTRAINT "fk_Patient_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Patient.fk_Patient_Person1_idx" ON "Patient"("person_id");
CREATE INDEX "Patient.fk_Patient_Patient1_idx" ON "Patient"("postmod_id");
CREATE INDEX "Patient.fk_Patient_Modification1_idx" ON "Patient"("modification_id");
CREATE TABLE "PatientVariate"(
 "pkid_patientvariate" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "patient_id" INTEGER NOT NULL,
 "date" DATETIME,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Covariate_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("pkid_patient")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Covariate_Covariate1"
  FOREIGN KEY("postmod_id")
 REFERENCES "PatientVariate"("pkid_patientvariate"),
 CONSTRAINT "fk_Covariate_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "PatientVariate.fk_Covariate_Patient1_idx" ON "PatientVariate"("patient_id");
CREATE INDEX "PatientVariate.fk_Covariate_Covariate1_idx" ON "PatientVariate"("postmod_id");
CREATE INDEX "PatientVariate.fk_Covariate_Modification1_idx" ON "PatientVariate"("modification_id");
CREATE TABLE "PostEngine"(
 "pkid_postengine" INTEGER PRIMARY KEY NOT NULL,
 "version" FLOAT,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Post_engine_Post_engine1"
  FOREIGN KEY("postmod_id")
 REFERENCES "PostEngine"("pkid_postengine"),
 CONSTRAINT "fk_Post_engine_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "PostEngine.fk_Post_engine_Post_engine1_idx" ON "PostEngine"("postmod_id");
CREATE INDEX "PostEngine.fk_Post_engine_Modification1_idx" ON "PostEngine"("modification_id");
CREATE TABLE "ModelEngine"(
 "pkid_modelengine" INTEGER PRIMARY KEY NOT NULL,
 "version" FLOAT,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Model_Model1"
  FOREIGN KEY("postmod_id")
 REFERENCES "ModelEngine"("pkid_modelengine"),
 CONSTRAINT "fk_Model_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "ModelEngine.fk_Model_Model1_idx" ON "ModelEngine"("postmod_id");
CREATE INDEX "ModelEngine.fk_Model_Modification1_idx" ON "ModelEngine"("modification_id");
CREATE TABLE "Drug"(
 "pkid_drug" INTEGER PRIMARY KEY NOT NULL,
 "version" FLOAT,
 "name" TEXT,
 "atc" VARCHAR(45),
 "studyname" TEXT,
 "domainname" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Drug_Drug1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Drug"("pkid_drug"),
 CONSTRAINT "fk_Drug_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Drug.fk_Drug_Drug1_idx" ON "Drug"("postmod_id");
CREATE INDEX "Drug.fk_Drug_Modification1_idx" ON "Drug"("modification_id");
CREATE TABLE "PercentileEngine"(
 "pkid_percentileengine" INTEGER PRIMARY KEY NOT NULL,
 "version" FLOAT,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Percentile_engine_Percentile_engine1"
  FOREIGN KEY("postmod_id")
 REFERENCES "PercentileEngine"("pkid_percentileengine"),
 CONSTRAINT "fk_Percentile_engine_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "PercentileEngine.fk_Percentile_engine_Percentile_engine1_idx" ON "PercentileEngine"("postmod_id");
CREATE INDEX "PercentileEngine.fk_Percentile_engine_Modification1_idx" ON "PercentileEngine"("modification_id");
CREATE TABLE "ReverseEngine"(
 "pkid_reverseengine" INTEGER PRIMARY KEY NOT NULL,
 "version" FLOAT,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Reverse_engine_Reverse_engine1"
  FOREIGN KEY("postmod_id")
 REFERENCES "ReverseEngine"("pkid_reverseengine"),
 CONSTRAINT "fk_Reverse_engine_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "ReverseEngine.fk_Reverse_engine_Reverse_engine1_idx" ON "ReverseEngine"("postmod_id");
CREATE INDEX "ReverseEngine.fk_Reverse_engine_Modification1_idx" ON "ReverseEngine"("modification_id");
CREATE TABLE "Clinical"(
 "pkid_clinical" INTEGER PRIMARY KEY NOT NULL,
 "patient_id" INTEGER NOT NULL,
 "date" DATETIME,
 "value" TEXT,
 "name" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Clinicals_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("pkid_patient")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Clinicals_Clinicals1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Clinical"("pkid_clinical"),
 CONSTRAINT "fk_Clinicals_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Clinical.fk_Clinicals_Patient1_idx" ON "Clinical"("patient_id");
CREATE INDEX "Clinical.fk_Clinicals_Clinicals1_idx" ON "Clinical"("postmod_id");
CREATE INDEX "Clinical.fk_Clinicals_Modification1_idx" ON "Clinical"("modification_id");
CREATE TABLE "DrugVariate"(
 "pkid_drugvariate" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "date" DATETIME,
 "name" TEXT,
 "drug_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Covariate_Covariate10"
  FOREIGN KEY("postmod_id")
 REFERENCES "DrugVariate"("pkid_drugvariate"),
 CONSTRAINT "fk_Covariate_Modification10"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DrugVariate_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("pkid_drug")
);
CREATE INDEX "DrugVariate.fk_Covariate_Covariate1_idx" ON "DrugVariate"("postmod_id");
CREATE INDEX "DrugVariate.fk_Covariate_Modification1_idx" ON "DrugVariate"("modification_id");
CREATE INDEX "DrugVariate.fk_DrugVariate_Drug1_idx" ON "DrugVariate"("drug_id");
CREATE TABLE "ValidDose"(
 "pkid_validdose" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "unit" TEXT,
 "drug_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Covariate_Covariate100"
  FOREIGN KEY("postmod_id")
 REFERENCES "ValidDose"("pkid_validdose"),
 CONSTRAINT "fk_Covariate_Modification100"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DrugVariate_Drug10"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("pkid_drug")
);
CREATE INDEX "ValidDose.fk_Covariate_Covariate1_idx" ON "ValidDose"("postmod_id");
CREATE INDEX "ValidDose.fk_Covariate_Modification1_idx" ON "ValidDose"("modification_id");
CREATE INDEX "ValidDose.fk_DrugVariate_Drug1_idx" ON "ValidDose"("drug_id");
CREATE TABLE "ValidInterval"(
 "pkid_validinterval" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "unit" TEXT,
 "drug_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Covariate_Covariate1000"
  FOREIGN KEY("postmod_id")
 REFERENCES "ValidInterval"("pkid_validinterval"),
 CONSTRAINT "fk_Covariate_Modification1000"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DrugVariate_Drug100"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("pkid_drug")
);
CREATE INDEX "ValidInterval.fk_Covariate_Covariate1_idx" ON "ValidInterval"("postmod_id");
CREATE INDEX "ValidInterval.fk_Covariate_Modification1_idx" ON "ValidInterval"("modification_id");
CREATE INDEX "ValidInterval.fk_DrugVariate_Drug1_idx" ON "ValidInterval"("drug_id");
CREATE TABLE "ValidInfusion"(
 "pkid_validinfusion" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "unit" TEXT,
 "drug_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Covariate_Covariate1001"
  FOREIGN KEY("postmod_id")
 REFERENCES "ValidInfusion"("pkid_validinfusion"),
 CONSTRAINT "fk_Covariate_Modification1001"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DrugVariate_Drug101"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("pkid_drug")
);
CREATE INDEX "ValidInfusion.fk_Covariate_Covariate1_idx" ON "ValidInfusion"("postmod_id");
CREATE INDEX "ValidInfusion.fk_Covariate_Modification1_idx" ON "ValidInfusion"("modification_id");
CREATE INDEX "ValidInfusion.fk_DrugVariate_Drug1_idx" ON "ValidInfusion"("drug_id");
CREATE TABLE "DrugTreatment"(
 "pkid_drugtreatment" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "drug_id" INTEGER NOT NULL,
 "patient_id" INTEGER NOT NULL,
 "drugModel_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_DrugTreatment_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("pkid_drug"),
 CONSTRAINT "fk_DrugTreatment_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("pkid_patient"),
 CONSTRAINT "fk_DrugTreatment_DrugTreatment1"
  FOREIGN KEY("postmod_id")
 REFERENCES "DrugTreatment"("pkid_drugtreatment"),
 CONSTRAINT "fk_DrugTreatment_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DrugTreatment_DrugModel1"
  FOREIGN KEY("drugModel_id")
 REFERENCES "DrugModel"("pkid_drugmodel")
);
CREATE INDEX "DrugTreatment.fk_DrugTreatment_Drug1_idx" ON "DrugTreatment"("drug_id");
CREATE INDEX "DrugTreatment.fk_DrugTreatment_Patient1_idx" ON "DrugTreatment"("patient_id");
CREATE INDEX "DrugTreatment.fk_DrugTreatment_DrugTreatment1_idx" ON "DrugTreatment"("postmod_id");
CREATE INDEX "DrugTreatment.fk_DrugTreatment_Modification1_idx" ON "DrugTreatment"("modification_id");
CREATE INDEX "DrugTreatment.fk_DrugTreatment_DrugModel1_idx" ON "DrugTreatment"("drugModel_id");
CREATE TABLE "DrugResponseAnalysis"(
 "pkid_drugresponseanalysis" INTEGER PRIMARY KEY NOT NULL,
 "reverseEngine_id" INTEGER NOT NULL,
 "percentileEngine_id" INTEGER NOT NULL,
 "postEngine_id" INTEGER NOT NULL,
 "drugTreatment_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_DrugResponseAnalysis_ReverseEngine1"
  FOREIGN KEY("reverseEngine_id")
 REFERENCES "ReverseEngine"("pkid_reverseengine"),
 CONSTRAINT "fk_DrugResponseAnalysis_PercentileEngine1"
  FOREIGN KEY("percentileEngine_id")
 REFERENCES "PercentileEngine"("pkid_percentileengine"),
 CONSTRAINT "fk_DrugResponseAnalysis_PostEngine1"
  FOREIGN KEY("postEngine_id")
 REFERENCES "PostEngine"("pkid_postengine"),
 CONSTRAINT "fk_DrugResponseAnalysis_DrugTreatment1"
  FOREIGN KEY("drugTreatment_id")
 REFERENCES "DrugTreatment"("pkid_drugtreatment"),
 CONSTRAINT "fk_DrugResponseAnalysis_DrugResponseAnalysis1"
  FOREIGN KEY("postmod_id")
 REFERENCES "DrugResponseAnalysis"("pkid_drugresponseanalysis"),
 CONSTRAINT "fk_DrugResponseAnalysis_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "DrugResponseAnalysis.fk_DrugResponseAnalysis_ReverseEngine1_idx" ON "DrugResponseAnalysis"("reverseEngine_id");
CREATE INDEX "DrugResponseAnalysis.fk_DrugResponseAnalysis_PercentileEngine1_idx" ON "DrugResponseAnalysis"("percentileEngine_id");
CREATE INDEX "DrugResponseAnalysis.fk_DrugResponseAnalysis_PostEngine1_idx" ON "DrugResponseAnalysis"("postEngine_id");
CREATE INDEX "DrugResponseAnalysis.fk_DrugResponseAnalysis_DrugTreatment1_idx" ON "DrugResponseAnalysis"("drugTreatment_id");
CREATE INDEX "DrugResponseAnalysis.fk_DrugResponseAnalysis_DrugResponseAnalysis1_idx" ON "DrugResponseAnalysis"("postmod_id");
CREATE INDEX "DrugResponseAnalysis.fk_DrugResponseAnalysis_Modification1_idx" ON "DrugResponseAnalysis"("modification_id");
CREATE TABLE "Institute"(
 "pkid_institute" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "logo" BLOB,
 "externalId" TEXT,
 "location_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Institute_Location1"
  FOREIGN KEY("location_id")
 REFERENCES "Location"("pkid_location")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Institute_Institute1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Institute"("pkid_institute"),
 CONSTRAINT "fk_Institute_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Institute.fk_Institute_Location1_idx" ON "Institute"("location_id");
CREATE INDEX "Institute.fk_Institute_Institute1_idx" ON "Institute"("postmod_id");
CREATE INDEX "Institute.fk_Institute_Modification1_idx" ON "Institute"("modification_id");
CREATE TABLE "Email"(
 "pkid_email" INTEGER PRIMARY KEY NOT NULL,
 "address" TEXT,
 "person_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_email_Person1"
  FOREIGN KEY("person_id")
 REFERENCES "Person"("pkid_person")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Email_Email1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Email"("pkid_email"),
 CONSTRAINT "fk_Email_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Email.fk_email_Person1_idx" ON "Email"("person_id");
CREATE INDEX "Email.fk_Email_Email1_idx" ON "Email"("postmod_id");
CREATE INDEX "Email.fk_Email_Modification1_idx" ON "Email"("modification_id");
CREATE TABLE "Practician"(
 "pkid_practician" INTEGER PRIMARY KEY NOT NULL,
 "title" TEXT,
 "person_id" INTEGER NOT NULL,
 "institute_id" INTEGER NOT NULL,
 "external_id" TEXT,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER DEFAULT null,
 CONSTRAINT "fk_Doctor_Person1"
  FOREIGN KEY("person_id")
 REFERENCES "Person"("pkid_person")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Doctor_Institute1"
  FOREIGN KEY("institute_id")
 REFERENCES "Institute"("pkid_institute")
 ON DELETE SET NULL
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Practician_Practician1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Practician"("pkid_practician"),
 CONSTRAINT "fk_Practician_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Practician.fk_Doctor_Person1_idx" ON "Practician"("person_id");
CREATE INDEX "Practician.fk_Doctor_Institute1_idx" ON "Practician"("institute_id");
CREATE INDEX "Practician.fk_Practician_Practician1_idx" ON "Practician"("postmod_id");
CREATE INDEX "Practician.fk_Practician_Modification1_idx" ON "Practician"("modification_id");
CREATE TABLE "Measure"(
 "pkid_measure" INTEGER PRIMARY KEY NOT NULL,
 "comment" TEXT,
 "patient_id" INTEGER NOT NULL,
 "moment" DATETIME,
 "value" DOUBLE,
 "unit" TEXT,
 "drug_id" INTEGER NOT NULL,
 "sampleID" TEXT,
 "arrivalDate" DATETIME,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Measure_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("pkid_patient")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Measure_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("pkid_drug")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Measure_Measure1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Measure"("pkid_measure"),
 CONSTRAINT "fk_Measure_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Measure.fk_Measure_Patient1_idx" ON "Measure"("patient_id");
CREATE INDEX "Measure.fk_Measure_Drug1_idx" ON "Measure"("drug_id");
CREATE INDEX "Measure.fk_Measure_Measure1_idx" ON "Measure"("postmod_id");
CREATE INDEX "Measure.fk_Measure_Modification1_idx" ON "Measure"("modification_id");
CREATE TABLE "DxP"(
 "Doctor" INTEGER NOT NULL,
 "Patient" INTEGER NOT NULL,
 PRIMARY KEY("Doctor","Patient"),
 CONSTRAINT "fk_Doctor_has_Patient_Doctor1"
  FOREIGN KEY("Doctor")
 REFERENCES "Practician"("pkid_practician")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Doctor_has_Patient_Patient1"
  FOREIGN KEY("Patient")
 REFERENCES "Patient"("pkid_patient")
 ON DELETE CASCADE
 ON UPDATE CASCADE
);
CREATE INDEX "DxP.fk_Doctor_has_Patient_Patient1_idx" ON "DxP"("Patient");
CREATE INDEX "DxP.fk_Doctor_has_Patient_Doctor1_idx" ON "DxP"("Doctor");
CREATE TABLE "Prediction"(
 "pkid_prediction" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "comment" TEXT,
 "drugResponseAnalysis_id" INTEGER NOT NULL,
 "paramsType" INTEGER,
 "first_take" DATETIME,
 "curveType" INTEGER,
 "predictionSpec_id" INTEGER NOT NULL,
 "drugTreatment_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Curve_Curve1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Prediction"("pkid_prediction"),
 CONSTRAINT "fk_Curve_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Prediction_DrugResponseAnalysis1"
  FOREIGN KEY("drugResponseAnalysis_id")
 REFERENCES "DrugResponseAnalysis"("pkid_drugresponseanalysis"),
 CONSTRAINT "fk_Prediction_PredictionSpec1"
  FOREIGN KEY("predictionSpec_id")
 REFERENCES "PredictionSpec"("pkid_predictionspec"),
 CONSTRAINT "fk_Prediction_DrugTreatment1"
  FOREIGN KEY("drugTreatment_id")
 REFERENCES "DrugTreatment"("pkid_drugtreatment")
);
CREATE INDEX "Prediction.fk_Curve_Curve1_idx" ON "Prediction"("postmod_id");
CREATE INDEX "Prediction.fk_Curve_Modification1_idx" ON "Prediction"("modification_id");
CREATE INDEX "Prediction.fk_Prediction_DrugResponseAnalysis1_idx" ON "Prediction"("drugResponseAnalysis_id");
CREATE INDEX "Prediction.fk_Prediction_PredictionSpec1_idx" ON "Prediction"("predictionSpec_id");
CREATE INDEX "Prediction.fk_Prediction_DrugTreatment1_idx" ON "Prediction"("drugTreatment_id");
CREATE TABLE "MxO"(
 "Model" INTEGER NOT NULL,
 "Option" INTEGER NOT NULL,
 "Curve" INTEGER NOT NULL,
 PRIMARY KEY("Model","Option"),
 CONSTRAINT "fk_Model_has_Option_Model1"
  FOREIGN KEY("Model")
 REFERENCES "ModelEngine"("pkid_modelengine")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Model_has_Option_Option1"
  FOREIGN KEY("Option")
 REFERENCES "Option"("sql_id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_MxO_Curve1"
  FOREIGN KEY("Curve")
 REFERENCES "Prediction"("pkid_prediction")
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
 REFERENCES "PostEngine"("pkid_postengine")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Post_Engine_has_Option_Option1"
  FOREIGN KEY("Option")
 REFERENCES "Option"("sql_id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_PxO_Curve1"
  FOREIGN KEY("Curve")
 REFERENCES "Prediction"("pkid_prediction")
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
 REFERENCES "PercentileEngine"("pkid_percentileengine")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Prediction_Engine_has_Option_Option1"
  FOREIGN KEY("Option")
 REFERENCES "Option"("sql_id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_ExO_Curve1"
  FOREIGN KEY("Curve")
 REFERENCES "Prediction"("pkid_prediction")
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
 REFERENCES "ReverseEngine"("pkid_reverseengine")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Reverse_Engine_has_Option_Option1"
  FOREIGN KEY("Option")
 REFERENCES "Option"("sql_id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_RxO_Curve1"
  FOREIGN KEY("Curve")
 REFERENCES "Prediction"("pkid_prediction")
 ON DELETE CASCADE
 ON UPDATE CASCADE
);
CREATE INDEX "RxO.fk_Reverse_Engine_has_Option_Option1_idx" ON "RxO"("Option");
CREATE INDEX "RxO.fk_Reverse_Engine_has_Option_Reverse_Engine1_idx" ON "RxO"("Reverse_Engine");
CREATE INDEX "RxO.fk_RxO_Curve1_idx" ON "RxO"("Curve");
CREATE TABLE "Dosage"(
 "pkid_dosage" INTEGER PRIMARY KEY NOT NULL,
 "applied" DATE,
 "value" DOUBLE,
 "prediction_id" INTEGER NOT NULL,
 "tinf" DOUBLE,
 "unit" TEXT,
 "interval" DOUBLE,
 "endtime" DATETIME,
 "route" INTEGER NOT NULL DEFAULT 0,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Dosage_Dosage1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Dosage"("pkid_dosage"),
 CONSTRAINT "fk_Dosage_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Dosage_Prediction1"
  FOREIGN KEY("prediction_id")
 REFERENCES "Prediction"("pkid_prediction")
);
CREATE INDEX "Dosage.fk_Dosage_Dosage1_idx" ON "Dosage"("postmod_id");
CREATE INDEX "Dosage.fk_Dosage_Modification1_idx" ON "Dosage"("modification_id");
CREATE INDEX "Dosage.fk_Dosage_Prediction1_idx" ON "Dosage"("prediction_id");
CREATE TABLE "Report"(
 "pkid_report" INTEGER PRIMARY KEY NOT NULL,
 "template" TEXT NOT NULL,
 "title" TEXT,
 "reference" TEXT,
 "creation" DATETIME,
 "patientId" INTEGER,
 "analystId" INTEGER,
 "recipientId" INTEGER,
 "instituteId" INTEGER,
 "treatmentId" INTEGER,
 "publication" DATETIME,
 CONSTRAINT "fk_Report_Patient"
  FOREIGN KEY("patientId")
 REFERENCES "Patient"("pkid_patient")
 ON DELETE SET NULL
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_Analyst"
  FOREIGN KEY("analystId")
 REFERENCES "Practician"("pkid_practician")
 ON DELETE SET NULL
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_Recipient"
  FOREIGN KEY("recipientId")
 REFERENCES "Practician"("pkid_practician")
 ON DELETE SET NULL
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_Institute"
  FOREIGN KEY("instituteId")
 REFERENCES "Institute"("pkid_institute")
 ON DELETE SET NULL
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_Treatment"
  FOREIGN KEY("treatmentId")
 REFERENCES "Prediction"("pkid_prediction")
 ON DELETE SET NULL
 ON UPDATE CASCADE
);
CREATE INDEX "Report.fk_Report_Patient_idx" ON "Report"("patientId");
CREATE INDEX "Report.fk_Report_Analyst_idx" ON "Report"("analystId");
CREATE INDEX "Report.fk_Report_Recipient_idx" ON "Report"("recipientId");
CREATE INDEX "Report.fk_Report_Institute_idx" ON "Report"("instituteId");
CREATE INDEX "Report.fk_Report_Treatment_idx" ON "Report"("treatmentId");
CREATE TABLE "ReportValue"(
 "reportId" INTEGER NOT NULL,
 "id" TEXT NOT NULL,
 "value" TEXT,
 PRIMARY KEY("reportId","id"),
 CONSTRAINT "fk_ReportValue_Report"
  FOREIGN KEY("reportId")
 REFERENCES "Report"("pkid_report")
 ON DELETE CASCADE
 ON UPDATE CASCADE
);
CREATE TABLE "ReportImage"(
 "reportId" INTEGER NOT NULL,
 "id" TEXT NOT NULL,
 "image" BLOB,
 PRIMARY KEY("reportId","id"),
 CONSTRAINT "fk_ReportImage_Report"
  FOREIGN KEY("reportId")
 REFERENCES "Report"("pkid_report")
 ON DELETE CASCADE
 ON UPDATE CASCADE
);
CREATE TABLE "ReportHiddenSection"(
 "reportId" INTEGER NOT NULL,
 "section" TEXT NOT NULL,
 PRIMARY KEY("reportId","section"),
 CONSTRAINT "fk_ReportHiddenSection_Report"
  FOREIGN KEY("reportId")
 REFERENCES "Report"("pkid_report")
 ON DELETE CASCADE
 ON UPDATE CASCADE
);
CREATE TABLE "Parameter"(
 "sql_id" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "pid" TEXT,
 "curve_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "created_by" INTEGER NOT NULL DEFAULT 1,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "modification_id" INTEGER,
 CONSTRAINT "fk_Parameter_Curve1"
  FOREIGN KEY("curve_id")
 REFERENCES "Prediction"("pkid_prediction")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Parameter_Parameter1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Parameter"("sql_id"),
 CONSTRAINT "fk_Parameter_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id")
);
CREATE INDEX "Parameter.fk_Parameter_Curve1_idx" ON "Parameter"("curve_id");
CREATE INDEX "Parameter.fk_Parameter_Parameter1_idx" ON "Parameter"("postmod_id");
CREATE INDEX "Parameter.fk_Parameter_Modification1_idx" ON "Parameter"("modification_id");
COMMIT;
