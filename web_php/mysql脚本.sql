-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- 服务器版本:                        5.5.47 - MySQL Community Server (GPL)
-- 服务器操作系统:                      Win32
-- HeidiSQL 版本:                  9.3.0.4984
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- 导出 wlwdb 的数据库结构
CREATE DATABASE IF NOT EXISTS `wlwdb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `wlwdb`;


-- 导出  表 wlwdb.creat_dev_id_tb 结构
CREATE TABLE IF NOT EXISTS `creat_dev_id_tb` (
  `dev_id` varchar(73) NOT NULL,
  `bindflag` varchar(1) NOT NULL COMMENT '1绑定 0未绑定',
  `create_user` varchar(50) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='生成设备ID表';

-- 正在导出表  wlwdb.creat_dev_id_tb 的数据：20 rows
/*!40000 ALTER TABLE `creat_dev_id_tb` DISABLE KEYS */;
INSERT INTO `creat_dev_id_tb` (`dev_id`, `bindflag`, `create_user`, `timestamp`) VALUES
	('bcg57ef6bf1b6f62', '1', 'bcg', '2016-10-02 09:47:01'),
	('bcg57ef6bf37c25e', '1', 'bcg', '2016-10-02 09:50:43'),
	('bcg57ef6bf9579bd', '1', 'bcg', '2016-10-02 09:53:17'),
	('bcg57ef6bfb4708c', '1', 'bcg', '2016-10-02 13:34:40'),
	('bcg57ef6bfcd7196', '1', 'bcg', '2016-10-02 09:54:59'),
	('bcg57ef6bfe6365e', '1', 'bcg', '2016-10-06 09:17:33'),
	('bcg57efbba420706', '1', 'bcg', '2016-10-06 09:17:34'),
	('bcg57efbc9fb2c65', '1', 'bcg', '2016-10-06 15:14:04'),
	('bcg57efbceeaec03', '1', 'bcg', '2016-10-06 15:14:02'),
	('bcg57efbe51add93', '1', 'bcg', '2016-10-06 15:11:45'),
	('bcg57efbe584e7b5', '0', 'bcg', '2016-10-01 21:47:04'),
	('牛逼57f5d7c9dc7de', '1', '牛逼', '2016-10-06 12:51:10'),
	('牛逼57f5d834b380d', '1', '牛逼', '2016-10-06 13:50:37'),
	('牛逼57f5d94a15445', '1', '牛逼', '2016-10-06 12:55:40'),
	('牛逼57f5e04df0ae4', '1', '牛逼', '2016-10-06 13:56:58'),
	('牛逼57f5e11eeb0dd', '1', '牛逼', '2016-10-06 13:57:05'),
	('牛逼57f5e13411934', '1', '牛逼', '2016-10-06 14:04:31'),
	('牛逼57f5e14e34da4', '1', '牛逼', '2016-10-06 14:05:16'),
	('牛逼57f5e18113a09', '1', '牛逼', '2016-10-06 14:05:24'),
	('牛逼57f5e811947d1', '1', '牛逼', '2016-10-06 14:05:27');
/*!40000 ALTER TABLE `creat_dev_id_tb` ENABLE KEYS */;


-- 导出  表 wlwdb.test 结构
CREATE TABLE IF NOT EXISTS `test` (
  `c1` varchar(50) DEFAULT NULL,
  `c2` varchar(50) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=gbk;

-- 正在导出表  wlwdb.test 的数据：0 rows
/*!40000 ALTER TABLE `test` DISABLE KEYS */;
/*!40000 ALTER TABLE `test` ENABLE KEYS */;


-- 导出  表 wlwdb.user_dev_tb 结构
CREATE TABLE IF NOT EXISTS `user_dev_tb` (
  `username` varchar(50) NOT NULL,
  `deviceid` varchar(50) NOT NULL,
  `dev_name` varchar(50) DEFAULT NULL,
  `enable` varchar(1) NOT NULL COMMENT '1可以使用0不能使用',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '绑定时间',
  `dev_type` varchar(2) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='用户设备表';

-- 正在导出表  wlwdb.user_dev_tb 的数据：5 rows
/*!40000 ALTER TABLE `user_dev_tb` DISABLE KEYS */;
INSERT INTO `user_dev_tb` (`username`, `deviceid`, `dev_name`, `enable`, `timestamp`, `dev_type`) VALUES
	('bcg', 'bcg57ef6bfe6365e', '电蚊香', '1', '2016-10-08 13:37:34', '1'),
	('bcg', 'bcg57ef6bf37c25e', '电视2', '1', '2016-10-08 13:37:34', '2'),
	('bcg', 'bcg57ef6bf9579bd', '电饭煲d', '1', '2016-10-08 13:37:34', '3'),
	('牛逼', '牛逼57f5d7c9dc7de', '呵呵', '1', '2016-10-06 12:56:01', NULL),
	('牛逼', '牛逼57f5d94a15445', '我的', '1', '2016-10-06 12:55:58', NULL);
/*!40000 ALTER TABLE `user_dev_tb` ENABLE KEYS */;


-- 导出  表 wlwdb.user_reg_tb 结构
CREATE TABLE IF NOT EXISTS `user_reg_tb` (
  `username` varchar(50) NOT NULL,
  `pwd` varchar(50) NOT NULL,
  `qqnum` varchar(25) DEFAULT NULL,
  `telephone` varchar(15) DEFAULT NULL,
  `email` varchar(60) DEFAULT NULL,
  `enable` varchar(1) NOT NULL DEFAULT '1',
  `role` varchar(5) NOT NULL DEFAULT '1',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '当前时间'
) ENGINE=MyISAM DEFAULT CHARSET=gbk COMMENT='用户注册表';

-- 正在导出表  wlwdb.user_reg_tb 的数据：8 rows
/*!40000 ALTER TABLE `user_reg_tb` DISABLE KEYS */;
INSERT INTO `user_reg_tb` (`username`, `pwd`, `qqnum`, `telephone`, `email`, `enable`, `role`, `timestamp`) VALUES
	('牛逼', '123456', '448251593', '13127969758', '1@qq.com', '1', '1', '2016-10-06 12:45:13'),
	('bcg3', '123456', '222222', '1111111111', '2@qq.com', '1', '1', '2016-09-25 16:17:07'),
	('b', '123456', '333333', '3333333', '1@qq.com', '1', '1', '2016-09-25 15:31:42'),
	('123456', '123456', '123456', '123456', '123456@qq.com', '1', '1', '2016-09-16 13:31:01'),
	('1234568', '123456', '123456', '123456', '123456@qq.com', '1', '1', '2016-09-16 16:07:19'),
	('123456qqq', '123456', '123456', '123456', '123456@qq.com', '1', '1', '2016-09-16 16:09:35'),
	('123456qqq5', '123456', '123456', '123456', '123456@qq.com', '1', '1', '2016-09-16 16:09:55'),
	('bcg', '123456', '123456', '123456', '123456@qq.com', '1', '1', '2016-09-16 16:10:17');
/*!40000 ALTER TABLE `user_reg_tb` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
