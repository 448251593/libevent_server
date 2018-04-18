-- --------------------------------------------------------
-- 主机:                           103.229.28.233
-- 服务器版本:                        5.6.20 - Source distribution
-- 服务器操作系统:                      Linux
-- HeidiSQL 版本:                  9.2.0.4947
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- 导出 wenzhangdb 的数据库结构
CREATE DATABASE IF NOT EXISTS `wenzhangdb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `wenzhangdb`;


-- 导出  表 wenzhangdb.articaltb 结构
CREATE TABLE IF NOT EXISTS `articaltb` (
  `id` int(5) NOT NULL AUTO_INCREMENT,
  `title` varchar(80) NOT NULL,
  `context` longtext NOT NULL,
  `type` varchar(5) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='文章表';

-- 数据导出被取消选择。
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
