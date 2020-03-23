SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';


-- -----------------------------------------------------
-- Table `localdb`.`Location`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Location` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Location` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `country` TEXT NULL,
  `state` TEXT NULL,
  `address` TEXT NULL,
  `city` TEXT NULL,
  `postcode` TEXT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Institute`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Institute` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Institute` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  `logo` BLOB NULL,
  `Location` INT NOT NULL,
  `external_id` VARCHAR(25) NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Institute_Location1_idx` (`Location` ASC),
  UNIQUE INDEX `external_id_UNIQUE` (`external_id` ASC),
  CONSTRAINT `fk_Institute_Location1`
    FOREIGN KEY (`Location`)
    REFERENCES `localdb`.`Location` (`id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Person`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Person` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Person` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  `firstname` TEXT NULL,
  `gender` TINYINT(1) NULL,
  `birthday` DATE NULL,
  `Location` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Person_Location1_idx` (`Location` ASC),
  CONSTRAINT `fk_Person_Location1`
    FOREIGN KEY (`Location`)
    REFERENCES `localdb`.`Location` (`id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Email`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Email` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Email` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `address` TEXT NULL,
  `Person` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_email_Person1_idx` (`Person` ASC),
  CONSTRAINT `fk_email_Person1`
    FOREIGN KEY (`Person`)
    REFERENCES `localdb`.`Person` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Phone`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Phone` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Phone` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `number` TEXT NULL,
  `Phone_Type` TEXT NULL,
  `Person` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Phone_Person1_idx` (`Person` ASC),
  CONSTRAINT `fk_Phone_Person1`
    FOREIGN KEY (`Person`)
    REFERENCES `localdb`.`Person` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Practician`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Practician` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Practician` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `title` TEXT NULL,
  `Person` INT NOT NULL,
  `Institute` INT NOT NULL,
  `external_id` VARCHAR(25) NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Doctor_Person1_idx` (`Person` ASC),
  INDEX `fk_Doctor_Institute1_idx` (`Institute` ASC),
  UNIQUE INDEX `external_id_UNIQUE` (`external_id` ASC),
  CONSTRAINT `fk_Doctor_Person1`
    FOREIGN KEY (`Person`)
    REFERENCES `localdb`.`Person` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Doctor_Institute1`
    FOREIGN KEY (`Institute`)
    REFERENCES `localdb`.`Institute` (`id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Patient`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Patient` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Patient` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `stat_ok` TINYINT(1) NULL,
  `Person` INT NOT NULL,
  `external_id` VARCHAR(25) NULL,
  `stay_number` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Patient_Person1_idx` (`Person` ASC),
  UNIQUE INDEX `external_id_UNIQUE` (`external_id` ASC),
  CONSTRAINT `fk_Patient_Person1`
    FOREIGN KEY (`Person`)
    REFERENCES `localdb`.`Person` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Drug`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Drug` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Drug` (
  `sql_id` INT NOT NULL AUTO_INCREMENT,
  `version` FLOAT NULL,
  `id` TEXT NULL,
  PRIMARY KEY (`sql_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Model`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Model` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Model` (
  `sql_id` INT NOT NULL AUTO_INCREMENT,
  `version` FLOAT NULL,
  `id` TEXT NULL,
  PRIMARY KEY (`sql_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Post_Engine`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Post_Engine` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Post_Engine` (
  `sql_id` INT NOT NULL AUTO_INCREMENT,
  `version` FLOAT NULL,
  `id` TEXT NULL,
  PRIMARY KEY (`sql_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Percentile_Engine`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Percentile_Engine` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Percentile_Engine` (
  `sql_id` INT NOT NULL AUTO_INCREMENT,
  `version` FLOAT NULL,
  `id` TEXT NULL,
  PRIMARY KEY (`sql_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Reverse_Engine`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Reverse_Engine` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Reverse_Engine` (
  `sql_id` INT NOT NULL AUTO_INCREMENT,
  `version` FLOAT NULL,
  `id` TEXT NULL,
  PRIMARY KEY (`sql_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Curve`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Curve` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Curve` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NULL,
  `Drug` INT NOT NULL,
  `comment` TEXT NULL,
  `Patient` INT NOT NULL,
  `Model` INT NOT NULL,
  `Post_Engine` INT NOT NULL,
  `Percentile_Engine` INT NOT NULL,
  `Reverse_Engine` INT NOT NULL,
  `ptype` INT NULL,
  `first_take` DATETIME NULL,
  `ctype` INT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Curve_Drug1_idx` (`Drug` ASC),
  INDEX `fk_Curve_Patient1_idx` (`Patient` ASC),
  INDEX `fk_Curve_Model1_idx` (`Model` ASC),
  INDEX `fk_Curve_Post_Engine1_idx` (`Post_Engine` ASC),
  INDEX `fk_Curve_Prediction_Engine1_idx` (`Percentile_Engine` ASC),
  INDEX `fk_Curve_Reverse_Engine1_idx` (`Reverse_Engine` ASC),
  CONSTRAINT `fk_Curve_Drug1`
    FOREIGN KEY (`Drug`)
    REFERENCES `localdb`.`Drug` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Curve_Patient1`
    FOREIGN KEY (`Patient`)
    REFERENCES `localdb`.`Patient` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Curve_Model1`
    FOREIGN KEY (`Model`)
    REFERENCES `localdb`.`Model` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Curve_Post_Engine1`
    FOREIGN KEY (`Post_Engine`)
    REFERENCES `localdb`.`Post_Engine` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Curve_Prediction_Engine1`
    FOREIGN KEY (`Percentile_Engine`)
    REFERENCES `localdb`.`Percentile_Engine` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Curve_Reverse_Engine1`
    FOREIGN KEY (`Reverse_Engine`)
    REFERENCES `localdb`.`Reverse_Engine` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Parameter`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Parameter` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Parameter` (
  `sql_id` INT NOT NULL,
  `value` DOUBLE NULL COMMENT '	',
  `pid` TEXT NULL,
  `Curve` INT NOT NULL,
  PRIMARY KEY (`sql_id`),
  INDEX `fk_Parameter_Curve1_idx` (`Curve` ASC),
  CONSTRAINT `fk_Parameter_Curve1`
    FOREIGN KEY (`Curve`)
    REFERENCES `localdb`.`Curve` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Covariate`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Covariate` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Covariate` (
  `sql_id` INT NOT NULL,
  `value` DOUBLE NULL,
  `Patient` INT NOT NULL,
  `date` DATETIME NULL,
  `mid` TEXT NULL,
  PRIMARY KEY (`sql_id`),
  INDEX `fk_Covariate_Patient1_idx` (`Patient` ASC),
  CONSTRAINT `fk_Covariate_Patient1`
    FOREIGN KEY (`Patient`)
    REFERENCES `localdb`.`Patient` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Measure`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Measure` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Measure` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `comment` TEXT NULL,
  `Patient` INT NOT NULL,
  `date` DATETIME NULL,
  `value` DOUBLE NULL,
  `unit` TEXT NULL,
  `Drug` INT NOT NULL,
  `sampleID` TEXT NULL,
  `arrivalDate` DATETIME NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Measure_Patient1_idx` (`Patient` ASC),
  INDEX `fk_Measure_Drug1_idx` (`Drug` ASC),
  CONSTRAINT `fk_Measure_Patient1`
    FOREIGN KEY (`Patient`)
    REFERENCES `localdb`.`Patient` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Measure_Drug1`
    FOREIGN KEY (`Drug`)
    REFERENCES `localdb`.`Drug` (`sql_id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`DxP`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`DxP` ;

CREATE TABLE IF NOT EXISTS `localdb`.`DxP` (
  `Doctor` INT NOT NULL,
  `Patient` INT NOT NULL,
  PRIMARY KEY (`Doctor`, `Patient`),
  INDEX `fk_Doctor_has_Patient_Patient1_idx` (`Patient` ASC),
  INDEX `fk_Doctor_has_Patient_Doctor1_idx` (`Doctor` ASC),
  CONSTRAINT `fk_Doctor_has_Patient_Doctor1`
    FOREIGN KEY (`Doctor`)
    REFERENCES `localdb`.`Practician` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Doctor_has_Patient_Patient1`
    FOREIGN KEY (`Patient`)
    REFERENCES `localdb`.`Patient` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Report`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Report` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Report` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `date` DATE NULL,
  `analyst` INT NOT NULL,
  `applicant` INT NOT NULL,
  `Prediction` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Report_Practician1_idx` (`analyst` ASC),
  INDEX `fk_Report_Practician2_idx` (`applicant` ASC),
  CONSTRAINT `fk_Report_Practician1`
    FOREIGN KEY (`analyst`)
    REFERENCES `localdb`.`Practician` (`id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Report_Practician2`
    FOREIGN KEY (`applicant`)
    REFERENCES `localdb`.`Practician` (`id`)
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Fields`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Fields` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Fields` (
  `id` INT NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`FxP`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`FxP` ;

CREATE TABLE IF NOT EXISTS `localdb`.`FxP` (
  `Report` INT NOT NULL,
  `Fields` INT NOT NULL,
  PRIMARY KEY (`Report`, `Fields`),
  INDEX `fk_Fields_has_Report_Report1_idx` (`Report` ASC),
  INDEX `fk_Fields_has_Report_Fields1_idx` (`Fields` ASC),
  CONSTRAINT `fk_Fields_has_Report_Fields1`
    FOREIGN KEY (`Fields`)
    REFERENCES `localdb`.`Fields` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Fields_has_Report_Report1`
    FOREIGN KEY (`Report`)
    REFERENCES `localdb`.`Report` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Copy_at`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Copy_at` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Copy_at` (
  `Practician` INT NOT NULL,
  `Report` INT NOT NULL,
  PRIMARY KEY (`Practician`, `Report`),
  INDEX `fk_Report_has_Practician_Practician1_idx` (`Practician` ASC),
  INDEX `fk_Report_has_Practician_Report1_idx` (`Report` ASC),
  CONSTRAINT `fk_Report_has_Practician_Report1`
    FOREIGN KEY (`Report`)
    REFERENCES `localdb`.`Report` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Report_has_Practician_Practician1`
    FOREIGN KEY (`Practician`)
    REFERENCES `localdb`.`Practician` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Encryption`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Encryption` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Encryption` (
  `enckey` VARCHAR(50) NOT NULL,
  `value` TINYINT(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`enckey`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Option`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`OptionTable` ;

CREATE TABLE IF NOT EXISTS `localdb`.`OptionTable` (
  `sql_id` INT NOT NULL AUTO_INCREMENT,
  `id` TEXT NULL,
  `value` BLOB NULL,
  PRIMARY KEY (`sql_id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`MxO`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`MxO` ;

CREATE TABLE IF NOT EXISTS `localdb`.`MxO` (
  `Model` INT NOT NULL,
  `Option_id` INT NOT NULL,
  `Curve` INT NOT NULL,
  PRIMARY KEY (`Model`, `Option_id`),
  INDEX `fk_Model_has_Option_Option1_idx` (`Option_id` ASC),
  INDEX `fk_Model_has_Option_Model1_idx` (`Model` ASC),
  INDEX `fk_MxO_Curve1_idx` (`Curve` ASC),
  CONSTRAINT `fk_Model_has_Option_Model1`
    FOREIGN KEY (`Model`)
    REFERENCES `localdb`.`Model` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Model_has_Option_Option1`
    FOREIGN KEY (`Option_id`)
    REFERENCES `localdb`.`OptionTable` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_MxO_Curve1`
    FOREIGN KEY (`Curve`)
    REFERENCES `localdb`.`Curve` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`PxO`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`PxO` ;

CREATE TABLE IF NOT EXISTS `localdb`.`PxO` (
  `Post_Engine` INT NOT NULL,
  `Option_id` INT NOT NULL,
  `Curve` INT NOT NULL,
  PRIMARY KEY (`Post_Engine`, `Option_id`),
  INDEX `fk_Post_Engine_has_Option_Option1_idx` (`Option_id` ASC),
  INDEX `fk_Post_Engine_has_Option_Post_Engine1_idx` (`Post_Engine` ASC),
  INDEX `fk_PxO_Curve1_idx` (`Curve` ASC),
  CONSTRAINT `fk_Post_Engine_has_Option_Post_Engine1`
    FOREIGN KEY (`Post_Engine`)
    REFERENCES `localdb`.`Post_Engine` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Post_Engine_has_Option_Option1`
    FOREIGN KEY (`Option_id`)
    REFERENCES `localdb`.`OptionTable` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_PxO_Curve1`
    FOREIGN KEY (`Curve`)
    REFERENCES `localdb`.`Curve` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`ExO`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`ExO` ;

CREATE TABLE IF NOT EXISTS `localdb`.`ExO` (
  `Prediction_Engine` INT NOT NULL,
  `Option_id` INT NOT NULL,
  `Curve` INT NOT NULL,
  PRIMARY KEY (`Prediction_Engine`, `Option_id`),
  INDEX `fk_Prediction_Engine_has_Option_Option1_idx` (`Option_id` ASC),
  INDEX `fk_Prediction_Engine_has_Option_Prediction_Engine1_idx` (`Prediction_Engine` ASC),
  INDEX `fk_ExO_Curve1_idx` (`Curve` ASC),
  CONSTRAINT `fk_Prediction_Engine_has_Option_Prediction_Engine1`
    FOREIGN KEY (`Prediction_Engine`)
    REFERENCES `localdb`.`Percentile_Engine` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Prediction_Engine_has_Option_Option1`
    FOREIGN KEY (`Option_id`)
    REFERENCES `localdb`.`OptionTable` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_ExO_Curve1`
    FOREIGN KEY (`Curve`)
    REFERENCES `localdb`.`Curve` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`RxO`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`RxO` ;

CREATE TABLE IF NOT EXISTS `localdb`.`RxO` (
  `Reverse_Engine` INT NOT NULL,
  `Option_id` INT NOT NULL,
  `Curve` INT NOT NULL,
  PRIMARY KEY (`Reverse_Engine`, `Option_id`),
  INDEX `fk_Reverse_Engine_has_Option_Option1_idx` (`Option_id` ASC),
  INDEX `fk_Reverse_Engine_has_Option_Reverse_Engine1_idx` (`Reverse_Engine` ASC),
  INDEX `fk_RxO_Curve1_idx` (`Curve` ASC),
  CONSTRAINT `fk_Reverse_Engine_has_Option_Reverse_Engine1`
    FOREIGN KEY (`Reverse_Engine`)
    REFERENCES `localdb`.`Reverse_Engine` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Reverse_Engine_has_Option_Option1`
    FOREIGN KEY (`Option_id`)
    REFERENCES `localdb`.`OptionTable` (`sql_id`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE,
  CONSTRAINT `fk_RxO_Curve1`
    FOREIGN KEY (`Curve`)
    REFERENCES `localdb`.`Curve` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Dosage`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Dosage` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Dosage` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `comment` TEXT NULL,
  `date` DATE NULL,
  `Curve` INT NOT NULL,
  `value` DOUBLE NULL,
  `unit` TEXT NULL,
  `tinf` DOUBLE NULL,
  `dinterval` DOUBLE NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Dosage_Curve1_idx` (`Curve` ASC),
  CONSTRAINT `fk_Dosage_Curve1`
    FOREIGN KEY (`Curve`)
    REFERENCES `localdb`.`Curve` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`View`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`View` ;

CREATE TABLE IF NOT EXISTS `localdb`.`View` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `comment` TEXT NULL,
  `name` TEXT NULL,
  `image` BLOB NULL,
  `Report` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_View_Report1_idx` (`Report` ASC),
  CONSTRAINT `fk_View_Report1`
    FOREIGN KEY (`Report`)
    REFERENCES `localdb`.`Report` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Clinicals`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Clinicals` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Clinicals` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `Patient` INT NOT NULL,
  `date` DATETIME NULL,
  `value` TEXT NULL,
  `type` TEXT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Clinicals_Patient1_idx` (`Patient` ASC),
  CONSTRAINT `fk_Clinicals_Patient1`
    FOREIGN KEY (`Patient`)
    REFERENCES `localdb`.`Patient` (`id`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`Version`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`Version` ;

CREATE TABLE IF NOT EXISTS `localdb`.`Version` (
  `id` VARCHAR(50) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `localdb`.`UserTable`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `localdb`.`UserTable` ;

CREATE TABLE IF NOT EXISTS `localdb`.`UserTable` (
  `username` VARCHAR(50) NOT NULL,
  `enckey` TEXT NULL,
  `authorization` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`username`))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
