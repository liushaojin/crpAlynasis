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
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `projectt`
--

LOCK TABLES `projectt` WRITE;
/*!40000 ALTER TABLE `projectt` DISABLE KEYS */;
INSERT INTO `projectt` VALUES (19,'crp','20170424',0,'mg/L',1,00000000000000000.5666,1.01,0.5555,5,12,00,00,00,010,400,300,040,100,1,1,1,15,142.7,13.3698,-12.3693,11.3695,2000,25,0,00000000000000018888.4,0000000000000007.36926,0000000000000006.25897,000000000000000005.258,000000000000000004.369,0000000000000000008569,0000000000000000000320),(24,'hs-CRP','20170420',1,'mg/L',1,00000000000000000000.5,1.01,0.55,5,12,00,00,00,010,400,300,040,100,1,1,1,15,142.7,13.3698,-12.3693,11.3695,3125,20,1,00000000000000018888.4,0000000000000007.36926,0000000000000006.25897,000000000000000005.258,000000000000000004.369,0000000000000000010258,0000000000000000000320);
/*!40000 ALTER TABLE `projectt` ENABLE KEYS */;
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
