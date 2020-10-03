/*
Navicat SQLite Data Transfer

Source Server         : SF307
Source Server Version : 30808
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30808
File Encoding         : 65001

Date: 2020-03-17 11:18:23
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for data_record
-- ----------------------------
DROP TABLE IF EXISTS "main"."data_record";
CREATE TABLE "data_record" (
"mac_addr"  TEXT NOT NULL,				-- MAC地址
"ip_addr"  TEXT NOT NULL,				-- IP地址
"hardware"  TEXT,						-- 硬件固件的版本号
"date"  REAL NOT NULL,					-- 记录的时间日期 时间戳
"day_age"  INTEGER,						-- 日龄
"aim_t"  REAL,							-- 目标温度
"in_t"  REAL,							-- 室内温度
"in_h"  REAL,							-- 室内湿度
"in_pa"  REAL,							-- 室内压力
"out_t"  REAL,							-- 室外温度
"duct_t"  REAL,							-- 风管温度
"t6"  REAL,								-- 废气温度T6						
"fp"  INTEGER,							-- 旁通阀
"hot"  INTEGER,							-- 热水阀
"recovery_q"  INTEGER,					-- 回收Q
"recovery_e"  REAL,						-- 回收E
"recovery_n"  INTEGER,					-- 回收N
"back_up"  TEXT,						-- 备用	(预热温度T7)
"level"  INTEGER,						-- 级别
PRIMARY KEY ("mac_addr", "ip_addr", "date")
);
