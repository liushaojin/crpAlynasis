-- MySQL dump 10.13  Distrib 5.7.17, for Win64 (x86_64)
--
-- Host: localhost    Database: mv200
-- ------------------------------------------------------
-- Server version	5.7.17-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `calibratemethodt`
--

DROP TABLE IF EXISTS `calibratemethodt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `calibratemethodt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(20) DEFAULT NULL,
  `remarks` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `calibratemethodt`
--

LOCK TABLES `calibratemethodt` WRITE;
/*!40000 ALTER TABLE `calibratemethodt` DISABLE KEYS */;
INSERT INTO `calibratemethodt` VALUES (1,'Logic5P',NULL);
/*!40000 ALTER TABLE `calibratemethodt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `errort`
--

DROP TABLE IF EXISTS `errort`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `errort` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `dateTime` timestamp(6) NULL DEFAULT NULL,
  `errID` int(11) DEFAULT NULL,
  `errNo` int(11) DEFAULT NULL,
  `Details` varchar(45) DEFAULT NULL,
  `remarks` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `errort`
--

LOCK TABLES `errort` WRITE;
/*!40000 ALTER TABLE `errort` DISABLE KEYS */;
/*!40000 ALTER TABLE `errort` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `historyt`
--

DROP TABLE IF EXISTS `historyt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `historyt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `taskNo` varchar(20) DEFAULT NULL,
  `stateID` int(11) DEFAULT NULL,
  `applyTime` datetime(6) DEFAULT NULL,
  `inspectionDate` datetime(6) DEFAULT NULL,
  `testDate` datetime(6) DEFAULT NULL,
  `reprotDate` datetime(6) DEFAULT NULL,
  `userID` int(11) DEFAULT NULL,
  `testTypeID` int(11) DEFAULT NULL,
  `projectID` int(11) DEFAULT NULL,
  `sampleCode` varchar(20) DEFAULT NULL,
  `samplePos` varchar(15) DEFAULT NULL,
  `Cup1` varchar(5) DEFAULT NULL,
  `Cup2` varchar(5) DEFAULT NULL,
  `sampleTypeID` int(11) DEFAULT NULL,
  `isPrint` int(11) DEFAULT '0',
  `isEmergency` int(11) DEFAULT '0',
  `result` double DEFAULT NULL,
  `reactionRange` double DEFAULT NULL,
  `isRetest` int(11) DEFAULT NULL,
  `isModify` int(11) DEFAULT NULL,
  `modifyDate` datetime(6) DEFAULT NULL,
  `modifierID` int(11) DEFAULT NULL,
  `modifyResult` double DEFAULT NULL,
  `lightPos1` int(11) DEFAULT NULL,
  `lightPos2` int(11) DEFAULT NULL,
  `AD1` double DEFAULT NULL,
  `AD2` double DEFAULT NULL,
  `AD3` double DEFAULT NULL,
  `AD4` double DEFAULT NULL,
  `AD5` double DEFAULT NULL,
  `AD6` double DEFAULT NULL,
  `AD7` double DEFAULT NULL,
  `AD8` double DEFAULT NULL,
  `AD9` double DEFAULT NULL,
  `AD10` double DEFAULT NULL,
  `AD11` double DEFAULT NULL,
  `AD12` double DEFAULT NULL,
  `AD13` double DEFAULT NULL,
  `AD14` double DEFAULT NULL,
  `AD15` double DEFAULT NULL,
  `AD16` double DEFAULT NULL,
  `AD17` double DEFAULT NULL,
  `AD18` double DEFAULT NULL,
  `AD19` double DEFAULT NULL,
  `AD20` double DEFAULT NULL,
  `AD21` double DEFAULT NULL,
  `AD22` double DEFAULT NULL,
  `AD23` double DEFAULT NULL,
  `AD24` double DEFAULT NULL,
  `AD25` double DEFAULT NULL,
  `AD26` double DEFAULT NULL,
  `AD27` double DEFAULT NULL,
  `AD28` double DEFAULT NULL,
  `AD29` double DEFAULT NULL,
  `AD30` double DEFAULT NULL,
  `AD31` double DEFAULT NULL,
  `AD32` double DEFAULT NULL,
  `AD33` double DEFAULT NULL,
  `AD34` double DEFAULT NULL,
  `AD35` double DEFAULT NULL,
  `AD36` double DEFAULT NULL,
  `AD37` double DEFAULT NULL,
  `AD38` double DEFAULT NULL,
  `AD39` double DEFAULT NULL,
  `AD40` double DEFAULT NULL,
  `T1` datetime(6) DEFAULT NULL,
  `T2` datetime(6) DEFAULT NULL,
  `T3` datetime(6) DEFAULT NULL,
  `T4` datetime(6) DEFAULT NULL,
  `T5` datetime(6) DEFAULT NULL,
  `T6` datetime(6) DEFAULT NULL,
  `T7` datetime(6) DEFAULT NULL,
  `T8` datetime(6) DEFAULT NULL,
  `T9` datetime(6) DEFAULT NULL,
  `T10` datetime(6) DEFAULT NULL,
  `T11` datetime(6) DEFAULT NULL,
  `T12` datetime(6) DEFAULT NULL,
  `T13` datetime(6) DEFAULT NULL,
  `T14` datetime(6) DEFAULT NULL,
  `T15` datetime(6) DEFAULT NULL,
  `T16` datetime(6) DEFAULT NULL,
  `T17` datetime(6) DEFAULT NULL,
  `T18` datetime(6) DEFAULT NULL,
  `T19` datetime(6) DEFAULT NULL,
  `T20` datetime(6) DEFAULT NULL,
  `T21` datetime(6) DEFAULT NULL,
  `T22` datetime(6) DEFAULT NULL,
  `T23` datetime(6) DEFAULT NULL,
  `T24` datetime(6) DEFAULT NULL,
  `T25` datetime(6) DEFAULT NULL,
  `T26` datetime(6) DEFAULT NULL,
  `T27` datetime(6) DEFAULT NULL,
  `T28` datetime(6) DEFAULT NULL,
  `T29` datetime(6) DEFAULT NULL,
  `T30` datetime(6) DEFAULT NULL,
  `T31` datetime(6) DEFAULT NULL,
  `T32` datetime(6) DEFAULT NULL,
  `T33` datetime(6) DEFAULT NULL,
  `T34` datetime(6) DEFAULT NULL,
  `T35` datetime(6) DEFAULT NULL,
  `T36` datetime(6) DEFAULT NULL,
  `T37` datetime(6) DEFAULT NULL,
  `T38` datetime(6) DEFAULT NULL,
  `T39` datetime(6) DEFAULT NULL,
  `T40` datetime(6) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `historyt`
--

LOCK TABLES `historyt` WRITE;
/*!40000 ALTER TABLE `historyt` DISABLE KEYS */;
/*!40000 ALTER TABLE `historyt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `historytempt`
--

DROP TABLE IF EXISTS `historytempt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `historytempt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `T_id` int(11) DEFAULT '0',
  `taskNo` varchar(20) DEFAULT NULL,
  `stateID` int(11) DEFAULT NULL,
  `applyTime` datetime(6) DEFAULT NULL,
  `inspectionDate` datetime(6) DEFAULT NULL,
  `testDate` datetime(6) DEFAULT NULL,
  `reprotDate` datetime(6) DEFAULT NULL,
  `userID` int(11) DEFAULT NULL,
  `testTypeID` int(11) DEFAULT NULL,
  `projectID` int(11) DEFAULT NULL,
  `sampleCode` varchar(20) DEFAULT NULL,
  `samplePos` varchar(15) DEFAULT NULL,
  `Cup1` varchar(5) DEFAULT NULL,
  `Cup2` varchar(5) DEFAULT NULL,
  `sampleTypeID` int(11) DEFAULT NULL,
  `isPrint` int(11) DEFAULT '0',
  `isEmergency` int(11) DEFAULT '0',
  `result` double DEFAULT NULL,
  `reactionRange` double DEFAULT NULL,
  `isRetest` int(11) DEFAULT NULL,
  `isModify` int(11) DEFAULT NULL,
  `modifyDate` datetime(6) DEFAULT NULL,
  `modifierID` int(11) DEFAULT NULL,
  `modifyResult` double DEFAULT NULL,
  `lightPos1` int(11) DEFAULT NULL,
  `lightPos2` int(11) DEFAULT NULL,
  `AD1` double DEFAULT NULL,
  `AD2` double DEFAULT NULL,
  `AD3` double DEFAULT NULL,
  `AD4` double DEFAULT NULL,
  `AD5` double DEFAULT NULL,
  `AD6` double DEFAULT NULL,
  `AD7` double DEFAULT NULL,
  `AD8` double DEFAULT NULL,
  `AD9` double DEFAULT NULL,
  `AD10` double DEFAULT NULL,
  `AD11` double DEFAULT NULL,
  `AD12` double DEFAULT NULL,
  `AD13` double DEFAULT NULL,
  `AD14` double DEFAULT NULL,
  `AD15` double DEFAULT NULL,
  `AD16` double DEFAULT NULL,
  `AD17` double DEFAULT NULL,
  `AD18` double DEFAULT NULL,
  `AD19` double DEFAULT NULL,
  `AD20` double DEFAULT NULL,
  `AD21` double DEFAULT NULL,
  `AD22` double DEFAULT NULL,
  `AD23` double DEFAULT NULL,
  `AD24` double DEFAULT NULL,
  `AD25` double DEFAULT NULL,
  `AD26` double DEFAULT NULL,
  `AD27` double DEFAULT NULL,
  `AD28` double DEFAULT NULL,
  `AD29` double DEFAULT NULL,
  `AD30` double DEFAULT NULL,
  `AD31` double DEFAULT NULL,
  `AD32` double DEFAULT NULL,
  `AD33` double DEFAULT NULL,
  `AD34` double DEFAULT NULL,
  `AD35` double DEFAULT NULL,
  `AD36` double DEFAULT NULL,
  `AD37` double DEFAULT NULL,
  `AD38` double DEFAULT NULL,
  `AD39` double DEFAULT NULL,
  `AD40` double DEFAULT NULL,
  `T1` datetime(6) DEFAULT NULL,
  `T2` datetime(6) DEFAULT NULL,
  `T3` datetime(6) DEFAULT NULL,
  `T4` datetime(6) DEFAULT NULL,
  `T5` datetime(6) DEFAULT NULL,
  `T6` datetime(6) DEFAULT NULL,
  `T7` datetime(6) DEFAULT NULL,
  `T8` datetime(6) DEFAULT NULL,
  `T9` datetime(6) DEFAULT NULL,
  `T10` datetime(6) DEFAULT NULL,
  `T11` datetime(6) DEFAULT NULL,
  `T12` datetime(6) DEFAULT NULL,
  `T13` datetime(6) DEFAULT NULL,
  `T14` datetime(6) DEFAULT NULL,
  `T15` datetime(6) DEFAULT NULL,
  `T16` datetime(6) DEFAULT NULL,
  `T17` datetime(6) DEFAULT NULL,
  `T18` datetime(6) DEFAULT NULL,
  `T19` datetime(6) DEFAULT NULL,
  `T20` datetime(6) DEFAULT NULL,
  `T21` datetime(6) DEFAULT NULL,
  `T22` datetime(6) DEFAULT NULL,
  `T23` datetime(6) DEFAULT NULL,
  `T24` datetime(6) DEFAULT NULL,
  `T25` datetime(6) DEFAULT NULL,
  `T26` datetime(6) DEFAULT NULL,
  `T27` datetime(6) DEFAULT NULL,
  `T28` datetime(6) DEFAULT NULL,
  `T29` datetime(6) DEFAULT NULL,
  `T30` datetime(6) DEFAULT NULL,
  `T31` datetime(6) DEFAULT NULL,
  `T32` datetime(6) DEFAULT NULL,
  `T33` datetime(6) DEFAULT NULL,
  `T34` datetime(6) DEFAULT NULL,
  `T35` datetime(6) DEFAULT NULL,
  `T36` datetime(6) DEFAULT NULL,
  `T37` datetime(6) DEFAULT NULL,
  `T38` datetime(6) DEFAULT NULL,
  `T39` datetime(6) DEFAULT NULL,
  `T40` datetime(6) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `historytempt`
--

LOCK TABLES `historytempt` WRITE;
/*!40000 ALTER TABLE `historytempt` DISABLE KEYS */;
/*!40000 ALTER TABLE `historytempt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `projectt`
--

DROP TABLE IF EXISTS `projectt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `projectt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(15) NOT NULL,
  `batchNum` varchar(20) DEFAULT NULL,
  `isDiluent` int(2) NOT NULL DEFAULT '0',
  `unit` varchar(10) NOT NULL DEFAULT 'mg/L',
  `accuracy` int(2) NOT NULL DEFAULT '2',
  `lowLinear` double unsigned zerofill DEFAULT NULL,
  `K` double NOT NULL DEFAULT '1',
  `bloodFactor` double NOT NULL DEFAULT '1',
  `lightPos1` int(2) NOT NULL DEFAULT '3',
  `lightPos2` int(2) NOT NULL DEFAULT '10',
  `r0ID` int(2) unsigned zerofill DEFAULT '00',
  `r1ID` int(2) unsigned zerofill DEFAULT '00',
  `r2ID` int(2) unsigned zerofill DEFAULT '00',
  `sampleVol` int(3) unsigned zerofill DEFAULT NULL,
  `r0Vol` int(3) unsigned zerofill DEFAULT NULL,
  `r1Vol` int(3) unsigned zerofill DEFAULT NULL,
  `rsVol` int(3) unsigned zerofill DEFAULT NULL,
  `r2Vol` int(3) unsigned zerofill DEFAULT NULL,
  `testMethodID` int(2) DEFAULT '1',
  `reactionDirID` int(2) DEFAULT '1',
  `calibrateID` int(2) DEFAULT '1',
  `calibrate1_R0` double DEFAULT NULL,
  `calibrate1_K` double DEFAULT NULL,
  `calibrate1_a` double DEFAULT NULL,
  `calibrate1_b` double DEFAULT NULL,
  `calibrate1_c` double DEFAULT NULL,
  `calibrate1_RR` double DEFAULT NULL,
  `calibrate1_CC` double DEFAULT NULL,
  `isCalibrate2` int(2) NOT NULL DEFAULT '0',
  `calibrate2_R0` double unsigned zerofill DEFAULT NULL,
  `calibrate2_K` double unsigned zerofill DEFAULT NULL,
  `calibrate2_a` double unsigned zerofill DEFAULT NULL,
  `calibrate2_b` double unsigned zerofill DEFAULT NULL,
  `calibrate2_c` double unsigned zerofill DEFAULT NULL,
  `calibrate2_RR` double unsigned zerofill DEFAULT NULL,
  `calibrate2_CC` double unsigned zerofill DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `projectt`
--

LOCK TABLES `projectt` WRITE;
/*!40000 ALTER TABLE `projectt` DISABLE KEYS */;
INSERT INTO `projectt` VALUES (19,'crp','20170424',0,'mg/L',1,00000000000000000.5666,1.01,0.5555,5,12,00,00,00,010,400,300,040,100,1,1,1,15,142.7,13.3698,-12.3693,11.3695,10258,320,1,00000000000000018888.4,0000000000000007.36926,0000000000000006.25897,000000000000000005.258,000000000000000004.369,0000000000000000003123,0000000000000000002147),(21,'crpddd','20170424',0,'mg/L',1,00000000000000000.5666,1.01,0.5555,5,12,00,00,00,010,400,300,040,100,1,1,1,15,142.7,13.3698,-12.3693,11.3695,10258,320,1,00000000000000018888.4,0000000000000007.36926,0000000000000006.25897,000000000000000005.258,000000000000000004.369,0000000000000000003123,0000000000000000002147);
/*!40000 ALTER TABLE `projectt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `reagentt`
--

DROP TABLE IF EXISTS `reagentt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `reagentt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(5) DEFAULT NULL,
  `vol` int(11) DEFAULT NULL,
  `usedVol` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `reagentt`
--

LOCK TABLES `reagentt` WRITE;
/*!40000 ALTER TABLE `reagentt` DISABLE KEYS */;
INSERT INTO `reagentt` VALUES (1,NULL,NULL,NULL),(2,NULL,NULL,NULL),(3,NULL,NULL,NULL),(4,NULL,NULL,NULL),(5,NULL,NULL,NULL),(6,NULL,NULL,NULL),(7,NULL,NULL,NULL),(8,NULL,NULL,NULL),(9,NULL,NULL,NULL),(10,NULL,NULL,NULL),(11,NULL,NULL,NULL),(12,NULL,NULL,NULL),(13,NULL,NULL,NULL),(14,NULL,NULL,NULL),(15,NULL,NULL,NULL),(16,NULL,NULL,NULL),(17,NULL,NULL,NULL),(19,NULL,NULL,NULL),(20,NULL,NULL,NULL),(21,NULL,NULL,NULL),(22,NULL,NULL,NULL),(23,NULL,NULL,NULL),(24,NULL,NULL,NULL);
/*!40000 ALTER TABLE `reagentt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sampletypet`
--

DROP TABLE IF EXISTS `sampletypet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sampletypet` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `nameEN` varchar(20) DEFAULT NULL,
  `nameCN` varchar(20) DEFAULT NULL,
  `isMix` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idSampleTypeT_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sampletypet`
--

LOCK TABLES `sampletypet` WRITE;
/*!40000 ALTER TABLE `sampletypet` DISABLE KEYS */;
INSERT INTO `sampletypet` VALUES (1,'Serum','血清',0),(2,'Blood','全血',1),(3,'Plasma','血浆',0),(4,'Urine','尿液',0);
/*!40000 ALTER TABLE `sampletypet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `statet`
--

DROP TABLE IF EXISTS `statet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `statet` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(15) DEFAULT NULL,
  `nameCN` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `statet`
--

LOCK TABLES `statet` WRITE;
/*!40000 ALTER TABLE `statet` DISABLE KEYS */;
INSERT INTO `statet` VALUES (1,'IDLE','空闲'),(2,'Applicated','已申请'),(3,'Wait','等待'),(4,'Testing','测试中'),(5,'Diluent','加稀释液'),(6,'Sample','加样本'),(7,'R1','加R1'),(8,'R_S','加稀释样本'),(9,'R2','加Review'),(10,'Dirty','脏杯'),(11,'Finished','完成'),(12,'Clean','清洗'),(13,'Error','出错'),(14,'Blank','空跑'),(15,'unFinished','未完成'),(16,'noSample','缺样本'),(17,'End1','加样完成'),(18,'End2','采光完成'),(19,'BlankOut','取消/作废');
/*!40000 ALTER TABLE `statet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sysparamt`
--

DROP TABLE IF EXISTS `sysparamt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sysparamt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `CupDirtyAD` int(11) DEFAULT '8000',
  `isLiquidAlarm` int(11) DEFAULT '1',
  `lessWaterTimes` int(11) DEFAULT '10',
  `lessCleanLiquidTimes` int(11) DEFAULT '10',
  `fullWasteTimes` int(11) DEFAULT '10',
  `hospitalName` varchar(60) DEFAULT NULL,
  `isAutoTransfer` int(11) DEFAULT '0',
  `SampleNO` int(11) DEFAULT '1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sysparamt`
--

LOCK TABLES `sysparamt` WRITE;
/*!40000 ALTER TABLE `sysparamt` DISABLE KEYS */;
INSERT INTO `sysparamt` VALUES (1,8000,1,10,10,10,'中山迈盛医疗科技有限公司',0,58);
/*!40000 ALTER TABLE `sysparamt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `taskt`
--

DROP TABLE IF EXISTS `taskt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `taskt` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `History_id` int(10) unsigned zerofill DEFAULT '0000000000',
  `taskNo` varchar(20) DEFAULT NULL,
  `dateTime` timestamp(6) NULL DEFAULT NULL,
  `testTime` datetime(6) DEFAULT NULL,
  `userID` int(11) DEFAULT NULL,
  `sampleTypeID` int(11) DEFAULT NULL,
  `typeID` int(11) DEFAULT NULL,
  `isEmergency` int(11) DEFAULT NULL,
  `stateID` int(11) DEFAULT NULL,
  `samplePos` varchar(10) DEFAULT NULL,
  `sampleCode` varchar(15) DEFAULT NULL,
  `projectID` int(11) DEFAULT NULL,
  `isRetest` int(11) DEFAULT NULL,
  `solutionID` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=15319 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taskt`
--

LOCK TABLES `taskt` WRITE;
/*!40000 ALTER TABLE `taskt` DISABLE KEYS */;
INSERT INTO `taskt` VALUES (15310,0000000000,'20170512154000010','2017-05-12 07:40:19.000000',NULL,1,1,1,0,3,'4-3','59',18,0,NULL),(15311,0000000000,'20170512154200001','2017-05-12 07:42:53.000000',NULL,1,1,1,0,3,'2-2','58',8,0,NULL),(15312,0000000000,'20170512154300001','2017-05-12 07:43:18.000000',NULL,1,1,1,0,3,'2-1','58',20,0,NULL),(15313,0000000000,'20170512154600001','2017-05-12 07:46:32.000000',NULL,1,1,1,0,3,'3-2','58',19,0,NULL),(15314,0000000000,'20170512154600001','2017-05-12 07:46:32.000000',NULL,1,1,1,0,3,'3-2','58',19,0,NULL),(15315,0000000000,'20170512154800001','2017-05-12 07:48:15.000000',NULL,1,1,1,0,3,'2-1','58',19,0,NULL),(15316,0000000000,'20170512154900001','2017-05-12 07:49:37.000000',NULL,1,1,1,0,3,'2-2','58',19,0,NULL),(15317,0000000000,'20170512155000001','2017-05-12 07:50:54.000000',NULL,1,1,1,0,3,'2-2','58',21,0,NULL),(15318,0000000000,'20170512155000002','2017-05-12 07:51:13.000000',NULL,1,1,1,0,3,'3-3','59',19,0,NULL);
/*!40000 ALTER TABLE `taskt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `testtypet`
--

DROP TABLE IF EXISTS `testtypet`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `testtypet` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(20) DEFAULT NULL,
  `remarks` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idTestTypeT_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `testtypet`
--

LOCK TABLES `testtypet` WRITE;
/*!40000 ALTER TABLE `testtypet` DISABLE KEYS */;
INSERT INTO `testtypet` VALUES (1,'Normal','常规测试'),(2,'QCTest','质控测试'),(3,'CalibrateTest','定标测试');
/*!40000 ALTER TABLE `testtypet` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userloginrecordt`
--

DROP TABLE IF EXISTS `userloginrecordt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `userloginrecordt` (
  `id` int(10) unsigned NOT NULL,
  `dateTime` datetime(6) DEFAULT NULL,
  `userID` int(11) DEFAULT NULL,
  `event` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userloginrecordt`
--

LOCK TABLES `userloginrecordt` WRITE;
/*!40000 ALTER TABLE `userloginrecordt` DISABLE KEYS */;
/*!40000 ALTER TABLE `userloginrecordt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usert`
--

DROP TABLE IF EXISTS `usert`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `usert` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL,
  `pwd` varchar(20) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idUserT_UNIQUE` (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `usert`
--

LOCK TABLES `usert` WRITE;
/*!40000 ALTER TABLE `usert` DISABLE KEYS */;
INSERT INTO `usert` VALUES (1,'root','root');
/*!40000 ALTER TABLE `usert` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-12 15:55:38
