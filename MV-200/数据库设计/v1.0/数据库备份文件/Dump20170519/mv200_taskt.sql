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
) ENGINE=InnoDB AUTO_INCREMENT=2004 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `taskt`
--

LOCK TABLES `taskt` WRITE;
/*!40000 ALTER TABLE `taskt` DISABLE KEYS */;
INSERT INTO `taskt` VALUES (2003,0000000000,'20170518162000001','2017-05-18 08:20:41.000000',NULL,1,1,1,0,3,'2-1','80',19,0,NULL);
/*!40000 ALTER TABLE `taskt` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-05-19  9:26:32
