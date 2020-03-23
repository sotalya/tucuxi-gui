-- Creator:       MySQL Workbench 6.0.8/ExportSQLite plugin 2009.12.02
-- Author:        Unknown
-- Caption:       New Model
-- Project:       Name of the project
-- Changed:       2015-05-27 14:59
-- Created:       2011-04-07 16:31
PRAGMA foreign_keys = OFF;

-- Schema: localdb
BEGIN;
CREATE TABLE "Fields"(
 "id" INTEGER PRIMARY KEY NOT NULL
);
CREATE TABLE "Institute"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "logo" BLOB,
 "location_id" INTEGER NOT NULL,
 "external_id" TEXT,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "external_id_UNIQUE"
  UNIQUE("external_id"),
 CONSTRAINT "fk_Institute_Location1"
  FOREIGN KEY("location_id")
 REFERENCES "Location"("id")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Institute_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Institute_Institute"
  FOREIGN KEY("postmod_id")
 REFERENCES "Institute"("id"),
 CONSTRAINT "fk_Institute_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Institute.fk_Institute_Location1_idx" ON "Institute"("location_id");
CREATE INDEX "Institute.fk_Institute_Modified1_idx" ON "Institute"("modification_id");
CREATE INDEX "Institute.fk_Institute_Institute_idx" ON "Institute"("postmod_id");
CREATE INDEX "Institute.fk_Institute_User1_idx" ON "Institute"("created_by");
CREATE TABLE "Location"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "country" TEXT,
 "state" TEXT,
 "address" TEXT,
 "city" TEXT,
 "postcode" TEXT,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Location_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Location_Location"
  FOREIGN KEY("postmod_id")
 REFERENCES "Location"("id"),
 CONSTRAINT "fk_Location_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Location.fk_Location_Modified1_idx" ON "Location"("modification_id");
CREATE INDEX "Location.fk_Location_Location_idx" ON "Location"("postmod_id");
CREATE INDEX "Location.fk_Location_User1_idx" ON "Location"("created_by");
CREATE TABLE "Practician"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "title" TEXT,
 "number" TEXT,
 "phone_type" TEXT,
 "name" TEXT,
 "firstname" TEXT,
 "gender" BOOL,
 "birthday" DATE,
 "email" TEXT,
 "location_id" INTEGER NOT NULL,
 "institute_id" INTEGER NOT NULL,
 "external_id" TEXT,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "external_id_UNIQUE"
  UNIQUE("external_id"),
 CONSTRAINT "fk_Doctor_Institute1"
  FOREIGN KEY("institute_id")
 REFERENCES "Institute"("id")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Practician_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Practician_Practician"
  FOREIGN KEY("postmod_id")
 REFERENCES "Practician"("id"),
 CONSTRAINT "fk_Practician_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_Practician_location"
  FOREIGN KEY("location_id")
 REFERENCES "Location"("id")
);
CREATE INDEX "Practician.fk_Doctor_Institute1_idx" ON "Practician"("institute_id");
CREATE INDEX "Practician.fk_Practician_Modified1_idx" ON "Practician"("modification_id");
CREATE INDEX "Practician.fk_Practician_Practician_idx" ON "Practician"("postmod_id");
CREATE INDEX "Practician.fk_Practician_User1_idx" ON "Practician"("created_by");
CREATE INDEX "Practician.fk_Practician_location_idx" ON "Practician"("location_id");
CREATE TABLE "Patient"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "number" TEXT,
 "type" TEXT,
 "name" TEXT,
 "firstname" TEXT,
 "gender" BOOL,
 "birthday" DATE,
 "email" TEXT,
 "location_id" INTEGER NOT NULL,
 "stay_number" TEXT,
 "stat_ok" BOOL,
 "external_id" TEXT,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Patient_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Patient_Patient"
  FOREIGN KEY("postmod_id")
 REFERENCES "Patient"("id"),
 CONSTRAINT "fk_Patient_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_Patient_Location"
  FOREIGN KEY("location_id")
 REFERENCES "Location"("id")
);
CREATE INDEX "Patient.fk_Patient_Modified1_idx" ON "Patient"("modification_id");
CREATE INDEX "Patient.fk_Patient_Patient_idx" ON "Patient"("postmod_id");
CREATE INDEX "Patient.fk_Patient_User1_idx" ON "Patient"("created_by");
CREATE INDEX "Patient.fk_Patient_Location_idx" ON "Patient"("location_id");
CREATE TABLE "Parameter"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "unit" TEXT,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Parameter_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Parameter_Parameter"
  FOREIGN KEY("postmod_id")
 REFERENCES "Parameter"("id"),
 CONSTRAINT "fk_Parameter_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Parameter.fk_Parameter_Modified1_idx" ON "Parameter"("modification_id");
CREATE INDEX "Parameter.fk_Parameter_Parameter_idx" ON "Parameter"("postmod_id");
CREATE INDEX "Parameter.fk_Parameter_User1_idx" ON "Parameter"("created_by");
CREATE TABLE "PatientVariate"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT NOT NULL,
 "unit" TEXT,
 "patient_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "name_UNIQUE"
  UNIQUE("name","patient_id","modification_id"),
 CONSTRAINT "fk_Covariate_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Variate_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Variate_Variate"
  FOREIGN KEY("postmod_id")
 REFERENCES "PatientVariate"("id"),
 CONSTRAINT "fk_Variate_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "PatientVariate.fk_Covariate_Patient1_idx" ON "PatientVariate"("patient_id");
CREATE INDEX "PatientVariate.fk_Variate_Modified1_idx" ON "PatientVariate"("modification_id");
CREATE INDEX "PatientVariate.fk_Variate_Variate_idx" ON "PatientVariate"("postmod_id");
CREATE INDEX "PatientVariate.fk_Variate_User1_idx" ON "PatientVariate"("created_by");
CREATE TABLE "Measure"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "comment" TEXT,
 "patient_id" INTEGER NOT NULL,
 "sampletime" DATETIME,
 "value" DOUBLE,
 "unit" TEXT,
 "drug_id" INTEGER NOT NULL,
 "sampleID" TEXT,
 "arrivaltime" DATETIME,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Measure_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Measure_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Measure_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Measure_Measure"
  FOREIGN KEY("postmod_id")
 REFERENCES "Measure"("id"),
 CONSTRAINT "fk_Measure_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Measure.fk_Measure_Patient1_idx" ON "Measure"("patient_id");
CREATE INDEX "Measure.fk_Measure_Drug1_idx" ON "Measure"("drug_id");
CREATE INDEX "Measure.fk_Measure_Modified1_idx" ON "Measure"("modification_id");
CREATE INDEX "Measure.fk_Measure_Measure_idx" ON "Measure"("postmod_id");
CREATE INDEX "Measure.fk_Measure_User1_idx" ON "Measure"("created_by");
CREATE TABLE "DxP"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "practician_id" INTEGER NOT NULL,
 "patient_id" INTEGER NOT NULL,
 "modification_id" INTEGER NOT NULL DEFAULT 0,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "uniquemapping"
  UNIQUE("practician_id","patient_id","modification_id"),
 CONSTRAINT "fk_Doctor_has_Patient_Doctor1"
  FOREIGN KEY("practician_id")
 REFERENCES "Practician"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Doctor_has_Patient_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_DxP_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DxP_DxP"
  FOREIGN KEY("postmod_id")
 REFERENCES "DxP"("id"),
 CONSTRAINT "fk_DxP_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "DxP.fk_Doctor_has_Patient_Patient1_idx" ON "DxP"("patient_id");
CREATE INDEX "DxP.fk_Doctor_has_Patient_Doctor1_idx" ON "DxP"("practician_id");
CREATE INDEX "DxP.fk_DxP_Modified1_idx" ON "DxP"("modification_id");
CREATE INDEX "DxP.fk_DxP_DxP_idx" ON "DxP"("postmod_id");
CREATE INDEX "DxP.fk_DxP_User1_idx" ON "DxP"("created_by");
CREATE TABLE "Report"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "date" DATE,
 "analyst" INTEGER NOT NULL,
 "applicant" INTEGER NOT NULL,
 "prediction" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Report_Practician1"
  FOREIGN KEY("analyst")
 REFERENCES "Practician"("id")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_Practician2"
  FOREIGN KEY("applicant")
 REFERENCES "Practician"("id")
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Report_Report"
  FOREIGN KEY("postmod_id")
 REFERENCES "Report"("id"),
 CONSTRAINT "fk_Report_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Report.fk_Report_Practician1_idx" ON "Report"("analyst");
CREATE INDEX "Report.fk_Report_Practician2_idx" ON "Report"("applicant");
CREATE INDEX "Report.fk_Report_Modified1_idx" ON "Report"("modification_id");
CREATE INDEX "Report.fk_Report_Report_idx" ON "Report"("postmod_id");
CREATE INDEX "Report.fk_Report_User1_idx" ON "Report"("created_by");
CREATE TABLE "FxP"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "Report" INTEGER NOT NULL,
 "Fields" INTEGER NOT NULL,
 "modification_id" INTEGER NOT NULL DEFAULT 0,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "uniquemapping"
  UNIQUE("Report","Fields","modification_id"),
 CONSTRAINT "fk_Fields_has_Report_Fields1"
  FOREIGN KEY("Fields")
 REFERENCES "Fields"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Fields_has_Report_Report1"
  FOREIGN KEY("Report")
 REFERENCES "Report"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_FxP_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_FxP_FxP"
  FOREIGN KEY("postmod_id")
 REFERENCES "FxP"("id"),
 CONSTRAINT "fk_FxP_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "FxP.fk_Fields_has_Report_Report1_idx" ON "FxP"("Report");
CREATE INDEX "FxP.fk_Fields_has_Report_Fields1_idx" ON "FxP"("Fields");
CREATE INDEX "FxP.fk_FxP_Modified1_idx" ON "FxP"("modification_id");
CREATE INDEX "FxP.fk_FxP_FxP_idx" ON "FxP"("postmod_id");
CREATE INDEX "FxP.fk_FxP_User1_idx" ON "FxP"("created_by");
CREATE TABLE "Copy_at"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "Practician" INTEGER NOT NULL,
 "Report" INTEGER NOT NULL,
 "modification_id" INTEGER NOT NULL DEFAULT 0,
 "is_deleted" BOOL NOT NULL,
 "is_modified" BOOL NOT NULL,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL,
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "uniquemapping"
  UNIQUE("Practician","Report","modification_id"),
 CONSTRAINT "fk_Report_has_Practician_Report1"
  FOREIGN KEY("Report")
 REFERENCES "Report"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Report_has_Practician_Practician1"
  FOREIGN KEY("Practician")
 REFERENCES "Practician"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Copy_at_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Copy_at_Copy_at"
  FOREIGN KEY("postmod_id")
 REFERENCES "Copy_at"("Practician"),
 CONSTRAINT "fk_Copy_at_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Copy_at.fk_Report_has_Practician_Practician1_idx" ON "Copy_at"("Practician");
CREATE INDEX "Copy_at.fk_Report_has_Practician_Report1_idx" ON "Copy_at"("Report");
CREATE INDEX "Copy_at.fk_Copy_at_Modified1_idx" ON "Copy_at"("modification_id");
CREATE INDEX "Copy_at.fk_Copy_at_Copy_at_idx" ON "Copy_at"("postmod_id");
CREATE INDEX "Copy_at.fk_Copy_at_User1_idx" ON "Copy_at"("created_by");
CREATE TABLE "Drug"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "atc" TEXT NOT NULL,
 "version" FLOAT,
 "study" TEXT,
 "name" TEXT,
 "errormodel_id" INTEGER NOT NULL,
 "domain" TEXT,
 "modelname" TEXT NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Drug_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Drug_Drug"
  FOREIGN KEY("postmod_id")
 REFERENCES "Drug"("id"),
 CONSTRAINT "fk_Drug_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_Drug_ErrorModel1"
  FOREIGN KEY("errormodel_id")
 REFERENCES "ErrorModel"("id")
);
CREATE INDEX "Drug.fk_Drug_Modified1_idx" ON "Drug"("modification_id");
CREATE INDEX "Drug.fk_Drug_Drug_idx" ON "Drug"("postmod_id");
CREATE INDEX "Drug.fk_Drug_User1_idx" ON "Drug"("created_by");
CREATE INDEX "Drug.fk_Drug_ErrorModel1_idx" ON "Drug"("errormodel_id");
CREATE TABLE "Curve"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "drug_id" INTEGER NOT NULL,
 "comment" TEXT,
 "patient_id" INTEGER NOT NULL,
 "model_id" INTEGER NOT NULL,
 "postengine_id" INTEGER NOT NULL,
 "percentileengine_id" INTEGER NOT NULL,
 "reverseengine_id" INTEGER NOT NULL,
 "ptype" INTEGER,
 "first_take" DATETIME,
 "ctype" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "modification_id" INTEGER,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Curve_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Curve_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Curve_Reverse_Engine1"
  FOREIGN KEY("reverseengine_id")
 REFERENCES "ComputeEngine"("id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Curve_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Curve_Curve1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Curve"("id"),
 CONSTRAINT "fk_Curve_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_Curve_Percentile_Engine"
  FOREIGN KEY("percentileengine_id")
 REFERENCES "ComputeEngine"("id"),
 CONSTRAINT "fk_Curve_Model_Engine"
  FOREIGN KEY("model_id")
 REFERENCES "ComputeEngine"("id"),
 CONSTRAINT "fk_Curve_Post_Engine"
  FOREIGN KEY("postengine_id")
 REFERENCES "ComputeEngine"("id")
);
CREATE INDEX "Curve.fk_Curve_Drug1_idx" ON "Curve"("drug_id");
CREATE INDEX "Curve.fk_Curve_Patient1_idx" ON "Curve"("patient_id");
CREATE INDEX "Curve.fk_Curve_Reverse_Engine1_idx" ON "Curve"("reverseengine_id");
CREATE INDEX "Curve.fk_Curve_Modified1_idx" ON "Curve"("modification_id");
CREATE INDEX "Curve.fk_Curve_Curve1_idx" ON "Curve"("postmod_id");
CREATE INDEX "Curve.fk_Curve_User1_idx" ON "Curve"("created_by");
CREATE INDEX "Curve.fk_Curve_Percentile_Engine_idx" ON "Curve"("percentileengine_id");
CREATE INDEX "Curve.fk_Curve_Model_Engine_idx" ON "Curve"("model_id");
CREATE INDEX "Curve.fk_Curve_Post_Engine_idx" ON "Curve"("postengine_id");
CREATE TABLE "Encryption"(
 "key" TEXT PRIMARY KEY NOT NULL,
 "value" BOOL NOT NULL DEFAULT 0,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" TEXT,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL DEFAULT 0,
 CONSTRAINT "fk_Encryption_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Encryption_Encryption"
  FOREIGN KEY("postmod_id")
 REFERENCES "Encryption"("key"),
 CONSTRAINT "fk_Encryption_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Encryption.fk_Encryption_Modified1_idx" ON "Encryption"("modification_id");
CREATE INDEX "Encryption.fk_Encryption_Encryption_idx" ON "Encryption"("postmod_id");
CREATE INDEX "Encryption.fk_Encryption_User1_idx" ON "Encryption"("created_by");
CREATE TABLE "Option"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "value" BLOB,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Option_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Option_Option"
  FOREIGN KEY("postmod_id")
 REFERENCES "Option"("id"),
 CONSTRAINT "fk_Option_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Option.fk_Option_Modified1_idx" ON "Option"("modification_id");
CREATE INDEX "Option.fk_Option_Option_idx" ON "Option"("postmod_id");
CREATE INDEX "Option.fk_Option_User1_idx" ON "Option"("created_by");
CREATE TABLE "ComputeEngine"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "version" FLOAT,
 "name" TEXT,
 "plugintype" TEXT NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Reverse_Engine_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Reverse_Engine_Reverse_Engine"
  FOREIGN KEY("postmod_id")
 REFERENCES "ComputeEngine"("id"),
 CONSTRAINT "fk_Reverse_Engine_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "ComputeEngine.fk_Reverse_Engine_Modified1_idx" ON "ComputeEngine"("modification_id");
CREATE INDEX "ComputeEngine.fk_Reverse_Engine_Reverse_Engine_idx" ON "ComputeEngine"("postmod_id");
CREATE INDEX "ComputeEngine.fk_Reverse_Engine_User1_idx" ON "ComputeEngine"("created_by");
CREATE TABLE "CurveOption"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "engine_id" INTEGER NOT NULL,
 "option_id" INTEGER NOT NULL,
 "curve_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "uniquemapping"
  UNIQUE("engine_id","option_id","modification_id","curve_id"),
 CONSTRAINT "fk_Reverse_Engine_has_Option_Reverse_Engine1"
  FOREIGN KEY("engine_id")
 REFERENCES "ComputeEngine"("id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Reverse_Engine_has_Option_Option1"
  FOREIGN KEY("option_id")
 REFERENCES "Option"("id")
 ON DELETE RESTRICT
 ON UPDATE CASCADE,
 CONSTRAINT "fk_RxO_Curve1"
  FOREIGN KEY("curve_id")
 REFERENCES "Curve"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_RxO_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_RxO_RxO"
  FOREIGN KEY("postmod_id")
 REFERENCES "CurveOption"("id"),
 CONSTRAINT "fk_RxO_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "CurveOption.fk_Reverse_Engine_has_Option_Option1_idx" ON "CurveOption"("option_id");
CREATE INDEX "CurveOption.fk_Reverse_Engine_has_Option_Reverse_Engine1_idx" ON "CurveOption"("engine_id");
CREATE INDEX "CurveOption.fk_RxO_Curve1_idx" ON "CurveOption"("curve_id");
CREATE INDEX "CurveOption.fk_RxO_Modified1_idx" ON "CurveOption"("modification_id");
CREATE INDEX "CurveOption.fk_RxO_RxO_idx" ON "CurveOption"("postmod_id");
CREATE INDEX "CurveOption.fk_RxO_User1_idx" ON "CurveOption"("created_by");
CREATE TABLE "Dosage"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "starttime" DATETIME,
 "endtime" DATETIME,
 "curve_id" INTEGER NOT NULL,
 "value" DOUBLE,
 "unit" TEXT,
 "tinf" DOUBLE,
 "interval" DOUBLE,
 "route" INTEGER NOT NULL DEFAULT 0,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Dosage_Curve1"
  FOREIGN KEY("curve_id")
 REFERENCES "Curve"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Dosage_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Dosage_Dosage"
  FOREIGN KEY("postmod_id")
 REFERENCES "Dosage"("id"),
 CONSTRAINT "fk_Dosage_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Dosage.fk_Dosage_Curve1_idx" ON "Dosage"("curve_id");
CREATE INDEX "Dosage.fk_Dosage_Modified1_idx" ON "Dosage"("modification_id");
CREATE INDEX "Dosage.fk_Dosage_Dosage_idx" ON "Dosage"("postmod_id");
CREATE INDEX "Dosage.fk_Dosage_User1_idx" ON "Dosage"("created_by");
CREATE TABLE "View"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "comment" TEXT,
 "name" TEXT,
 "image" BLOB,
 "report_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_View_Report1"
  FOREIGN KEY("report_id")
 REFERENCES "Report"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_View_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_View_View"
  FOREIGN KEY("postmod_id")
 REFERENCES "View"("id"),
 CONSTRAINT "fk_View_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "View.fk_View_Report1_idx" ON "View"("report_id");
CREATE INDEX "View.fk_View_Modified1_idx" ON "View"("modification_id");
CREATE INDEX "View.fk_View_View_idx" ON "View"("postmod_id");
CREATE INDEX "View.fk_View_User1_idx" ON "View"("created_by");
CREATE TABLE "Clinicals"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "patient_id" INTEGER NOT NULL,
 "date" DATETIME,
 "value" TEXT,
 "type" TEXT,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Clinicals_Patient1"
  FOREIGN KEY("patient_id")
 REFERENCES "Patient"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_Clinicals_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Clinicals_Clinicals"
  FOREIGN KEY("postmod_id")
 REFERENCES "Clinicals"("id"),
 CONSTRAINT "fk_Clinicals_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Clinicals.fk_Clinicals_Patient1_idx" ON "Clinicals"("patient_id");
CREATE INDEX "Clinicals.fk_Clinicals_Modification1_idx" ON "Clinicals"("modification_id");
CREATE INDEX "Clinicals.fk_Clinicals_Clinicals_idx" ON "Clinicals"("postmod_id");
CREATE INDEX "Clinicals.fk_Clinicals_User1_idx" ON "Clinicals"("created_by");
CREATE TABLE "Version"(
 "id" TEXT PRIMARY KEY NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL DEFAULT 0,
 CONSTRAINT "fk_Version_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Version_Version"
  FOREIGN KEY("postmod_id")
 REFERENCES "Version"("modification_id"),
 CONSTRAINT "fk_Version_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Version.fk_Version_Modified1_idx" ON "Version"("modification_id");
CREATE INDEX "Version.fk_Version_Version_idx" ON "Version"("postmod_id");
CREATE INDEX "Version.fk_Version_User1_idx" ON "Version"("created_by");
CREATE TABLE "User"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "username" TEXT NOT NULL DEFAULT 'guest',
 "key" TEXT,
 "authorization" INTEGER NOT NULL DEFAULT 0,
 "modification_id" INTEGER DEFAULT null,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 CONSTRAINT "username_UNIQUE"
  UNIQUE("username","modification_id"),
 CONSTRAINT "fk_User_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_User_User"
  FOREIGN KEY("postmod_id")
 REFERENCES "User"("id")
);
CREATE INDEX "User.fk_User_Modified1_idx" ON "User"("modification_id");
CREATE INDEX "User.fk_User_User_idx" ON "User"("postmod_id");
CREATE TABLE "ValidDose"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "unit" TEXT,
 "value" DOUBLE,
 "drug_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Dose_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_ValidDose_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_ValidDose_ValidDose"
  FOREIGN KEY("postmod_id")
 REFERENCES "ValidDose"("id"),
 CONSTRAINT "fk_ValidDose_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "ValidDose.fk_Dose_Drug1_idx" ON "ValidDose"("drug_id");
CREATE INDEX "ValidDose.fk_ValidDose_Modification1_idx" ON "ValidDose"("modification_id");
CREATE INDEX "ValidDose.fk_ValidDose_ValidDose_idx" ON "ValidDose"("postmod_id");
CREATE INDEX "ValidDose.fk_ValidDose_User1_idx" ON "ValidDose"("created_by");
CREATE TABLE "ValidInterval"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "unit" TEXT,
 "value" DOUBLE,
 "drug_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Interval_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_ValidInterval_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_ValidInterval_ValidInterval"
  FOREIGN KEY("postmod_id")
 REFERENCES "ValidInterval"("id"),
 CONSTRAINT "fk_ValidInterval_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "ValidInterval.fk_Interval_Drug1_idx" ON "ValidInterval"("drug_id");
CREATE INDEX "ValidInterval.fk_ValidInterval_Modified1_idx" ON "ValidInterval"("modification_id");
CREATE INDEX "ValidInterval.fk_ValidInterval_ValidInterval_idx" ON "ValidInterval"("postmod_id");
CREATE INDEX "ValidInterval.fk_ValidInterval_User1_idx" ON "ValidInterval"("created_by");
CREATE TABLE "ValidInfusion"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "unit" TEXT,
 "value" DOUBLE,
 "drug_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_ValidInfusion_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id")
 ON DELETE CASCADE
 ON UPDATE CASCADE,
 CONSTRAINT "fk_ValidInfusion_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_ValidInfusion_ValidInfusion"
  FOREIGN KEY("postmod_id")
 REFERENCES "ValidInfusion"("id"),
 CONSTRAINT "fk_ValidInfusion_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "ValidInfusion.fk_ValidInfusion_Drug1_idx" ON "ValidInfusion"("drug_id");
CREATE INDEX "ValidInfusion.fk_ValidInfusion_Modification1_idx" ON "ValidInfusion"("modification_id");
CREATE INDEX "ValidInfusion.fk_ValidInfusion_ValidInfusion_idx" ON "ValidInfusion"("postmod_id");
CREATE INDEX "ValidInfusion.fk_ValidInfusion_User1_idx" ON "ValidInfusion"("created_by");
CREATE TABLE "DrugVariate"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "name" TEXT,
 "value" BLOB,
 "unit" TEXT,
 "drug_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "unique_mapping"
  UNIQUE("name","drug_id","modification_id"),
 CONSTRAINT "fk_Covariate_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id"),
 CONSTRAINT "fk_Covariate_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Covariate_Covariate"
  FOREIGN KEY("postmod_id")
 REFERENCES "DrugVariate"("id"),
 CONSTRAINT "fk_Covariate_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "DrugVariate.fk_Covariate_Drug1_idx" ON "DrugVariate"("drug_id");
CREATE INDEX "DrugVariate.fk_Covariate_Modified1_idx" ON "DrugVariate"("modification_id");
CREATE INDEX "DrugVariate.fk_Covariate_Covariate_idx" ON "DrugVariate"("postmod_id");
CREATE INDEX "DrugVariate.fk_Covariate_User1_idx" ON "DrugVariate"("created_by");
CREATE TABLE "Covariate"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "drugvariate_id" INTEGER NOT NULL,
 "patientvariate_id" INTEGER NOT NULL,
 "curve_id" INTEGER NOT NULL,
 "modification_id" INTEGER NOT NULL DEFAULT 0,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "uniquemapping"
  UNIQUE("drugvariate_id","patientvariate_id","modification_id"),
 CONSTRAINT "fk_CxV_Curve1"
  FOREIGN KEY("curve_id")
 REFERENCES "Curve"("id"),
 CONSTRAINT "fk_CxV_Covariate1"
  FOREIGN KEY("drugvariate_id")
 REFERENCES "DrugVariate"("id"),
 CONSTRAINT "fk_CxV_Variate1"
  FOREIGN KEY("patientvariate_id")
 REFERENCES "PatientVariate"("id"),
 CONSTRAINT "fk_CxV_Modified1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_CxV_CxV"
  FOREIGN KEY("postmod_id")
 REFERENCES "Covariate"("id"),
 CONSTRAINT "fk_CxV_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Covariate.fk_CxV_Curve1_idx" ON "Covariate"("curve_id");
CREATE INDEX "Covariate.fk_CxV_Covariate1_idx" ON "Covariate"("drugvariate_id");
CREATE INDEX "Covariate.fk_CxV_Variate1_idx" ON "Covariate"("patientvariate_id");
CREATE INDEX "Covariate.fk_CxV_Modified1_idx" ON "Covariate"("modification_id");
CREATE INDEX "Covariate.fk_CxV_CxV_idx" ON "Covariate"("postmod_id");
CREATE INDEX "Covariate.fk_CxV_User1_idx" ON "Covariate"("created_by");
CREATE TABLE "Modification"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "created_by" INTEGER NOT NULL,
 "modtime" DATETIME DEFAULT (DATETIME('now')),
 CONSTRAINT "fk_Modified_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Modification.fk_Modified_User1_idx" ON "Modification"("created_by");
CREATE TABLE "Operation"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "formula" TEXT,
 "drugparameter_id" INTEGER NOT NULL,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "modification_id" INTEGER,
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_Operation_DrugParameter1"
  FOREIGN KEY("drugparameter_id")
 REFERENCES "DrugParameter"("id"),
 CONSTRAINT "fk_Operation_Modification2"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Operation_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_Operation_Operation1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Operation"("id")
);
CREATE INDEX "Operation.fk_Operation_DrugParameter1_idx" ON "Operation"("drugparameter_id");
CREATE INDEX "Operation.fk_Operation_Modification2_idx" ON "Operation"("modification_id");
CREATE INDEX "Operation.fk_Operation_User1_idx" ON "Operation"("created_by");
CREATE INDEX "Operation.fk_Operation_Operation1_idx" ON "Operation"("postmod_id");
CREATE TABLE "DrugParameter"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "value" DOUBLE,
 "parameter_id" INTEGER NOT NULL,
 "drug_id" INTEGER NOT NULL,
 "errormodel_id" INTEGER NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "unique mapping"
  UNIQUE("parameter_id","drug_id","modification_id"),
 CONSTRAINT "fk_DrugParameter_Parameter1"
  FOREIGN KEY("parameter_id")
 REFERENCES "Parameter"("id"),
 CONSTRAINT "fk_DrugParameter_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id"),
 CONSTRAINT "fk_DrugParameter_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_DrugParameter_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_DrugParameter_DrugParameter"
  FOREIGN KEY("postmod_id")
 REFERENCES "DrugParameter"("id"),
 CONSTRAINT "fk_DrugParameter_ErrorModel1"
  FOREIGN KEY("errormodel_id")
 REFERENCES "ErrorModel"("id")
);
CREATE INDEX "DrugParameter.fk_DrugParameter_Parameter1_idx" ON "DrugParameter"("parameter_id");
CREATE INDEX "DrugParameter.fk_DrugParameter_Drug1_idx" ON "DrugParameter"("drug_id");
CREATE INDEX "DrugParameter.fk_DrugParameter_Modification1_idx" ON "DrugParameter"("modification_id");
CREATE INDEX "DrugParameter.fk_DrugParameter_User1_idx" ON "DrugParameter"("created_by");
CREATE INDEX "DrugParameter.fk_Drug" ON "DrugParameter"("id","postmod_id");
CREATE INDEX "DrugParameter.fk_DrugParameter_DrugParameter_idx" ON "DrugParameter"("postmod_id");
CREATE INDEX "DrugParameter.fk_DrugParameter_ErrorModel1_idx" ON "DrugParameter"("errormodel_id");
CREATE TABLE "Target"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "drug_id" INTEGER NOT NULL,
 "cmin" DOUBLE,
 "cmax" DOUBLE,
 "cbest" DOUBLE,
 "tmin" DOUBLE,
 "tmax" DOUBLE,
 "tbest" DOUBLE,
 "targettype" INTEGER NOT NULL,
 "postmod_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id_copy1" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "modification_id" INTEGER,
 "created_by" INTEGER NOT NULL DEFAULT 0,
 CONSTRAINT "fk_Target_Target1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Target"("id"),
 CONSTRAINT "fk_Target_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Target_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_Target_Drug1"
  FOREIGN KEY("drug_id")
 REFERENCES "Drug"("id")
);
CREATE INDEX "Target.fk_Target_Modification1_idx" ON "Target"("modification_id");
CREATE INDEX "Target.fk_Target_User1_idx" ON "Target"("created_by");
CREATE INDEX "Target.fk_Target_Drug1_idx" ON "Target"("drug_id");
CREATE INDEX "Target.fk_Target_Target1_idx" ON "Target"("postmod_id");
CREATE TABLE "Correlation"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "DrugParameter1_id" INTEGER NOT NULL,
 "DrugParameter2_id" INTEGER NOT NULL,
 "factor" DOUBLE NOT NULL,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "index4"
  UNIQUE("DrugParameter1_id","DrugParameter2_id"),
 CONSTRAINT "fk_Correlation_DrugParameter1"
  FOREIGN KEY("DrugParameter1_id")
 REFERENCES "DrugParameter"("id"),
 CONSTRAINT "fk_Correlation_DrugParameter2"
  FOREIGN KEY("DrugParameter2_id")
 REFERENCES "DrugParameter"("id"),
 CONSTRAINT "fk_Correlation_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_Correlation_Correlation1"
  FOREIGN KEY("postmod_id")
 REFERENCES "Correlation"("id"),
 CONSTRAINT "fk_Correlation_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "Correlation.fk_Correlation_DrugParameter1_idx" ON "Correlation"("DrugParameter1_id");
CREATE INDEX "Correlation.fk_Correlation_DrugParameter2_idx" ON "Correlation"("DrugParameter2_id");
CREATE INDEX "Correlation.fk_Correlation_Modification1_idx" ON "Correlation"("modification_id");
CREATE INDEX "Correlation.fk_Correlation_Correlation1_idx" ON "Correlation"("postmod_id");
CREATE INDEX "Correlation.fk_Correlation_User1_idx" ON "Correlation"("created_by");
CREATE TABLE "ErrorModel"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "additive" DOUBLE NOT NULL DEFAULT 0,
 "proportional" DOUBLE NOT NULL DEFAULT 0,
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "postmod_id" INTEGER,
 "created_by" INTEGER NOT NULL,
 "formula" TEXT,
 CONSTRAINT "fk_ErrorModel_ErrorModel1"
  FOREIGN KEY("postmod_id")
 REFERENCES "ErrorModel"("id"),
 CONSTRAINT "fk_ErrorModel_Modification1"
  FOREIGN KEY("modification_id")
 REFERENCES "Modification"("id"),
 CONSTRAINT "fk_ErrorModel_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id")
);
CREATE INDEX "ErrorModel.fk_ErrorModel_ErrorModel1_idx" ON "ErrorModel"("postmod_id");
CREATE INDEX "ErrorModel.fk_ErrorModel_Modification1_idx" ON "ErrorModel"("modification_id");
CREATE INDEX "ErrorModel.fk_ErrorModel_User1_idx" ON "ErrorModel"("created_by");
CREATE TABLE "VariateMeasure"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "patientvariate_id" INTEGER NOT NULL,
 "value" BLOB NOT NULL DEFAULT 0,
 "date" DATETIME DEFAULT (DATETIME('now')),
 "modification_id" INTEGER,
 "is_deleted" BOOL NOT NULL DEFAULT 0,
 "is_modified" BOOL NOT NULL DEFAULT 0,
 "postmod_id" INTEGER,
 "date_created" DATETIME NOT NULL DEFAULT (DATETIME('now')),
 "created_by" INTEGER NOT NULL,
 CONSTRAINT "fk_VariateMeasure_PatientVariate1"
  FOREIGN KEY("patientvariate_id")
 REFERENCES "PatientVariate"("id"),
 CONSTRAINT "fk_VariateMeasure_User1"
  FOREIGN KEY("created_by")
 REFERENCES "User"("id"),
 CONSTRAINT "fk_VariateMeasure_VariateMeasure1"
  FOREIGN KEY("postmod_id")
 REFERENCES "VariateMeasure"("id")
);
CREATE INDEX "VariateMeasure.fk_VariateMeasure_PatientVariate1_idx" ON "VariateMeasure"("patientvariate_id");
CREATE INDEX "VariateMeasure.fk_VariateMeasure_User1_idx" ON "VariateMeasure"("created_by");
CREATE INDEX "VariateMeasure.fk_VariateMeasure_VariateMeasure1_idx" ON "VariateMeasure"("postmod_id");
CREATE TABLE "CalculationResult"(
 "id" INTEGER PRIMARY KEY NOT NULL,
 "curve_id" INTEGER NOT NULL,
 "engine_id" INTEGER NOT NULL,
 "date" DATETIME DEFAULT (DATETIME('now')),
 "results" BLOB NOT NULL DEFAULT 0,
 CONSTRAINT "fk_CalculationResult_Curve1"
  FOREIGN KEY("curve_id")
 REFERENCES "Curve"("id"),
 CONSTRAINT "fk_CalculationResult_Engine1"
  FOREIGN KEY("engine_id")
 REFERENCES "ComputeEngine"("id")
);
CREATE INDEX "CalculationResult.fk_CalculationResult_Curve1_idx" ON "CalculationResult"("curve_id");
CREATE INDEX "CalculationResult.fk_CalculationResult_Engine1_idx" ON "CalculationResult"("engine_id");
COMMIT;
