-- MySQL dump 10.13  Distrib 5.7.12, for Win64 (x86_64)
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
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-15 16:39:13
