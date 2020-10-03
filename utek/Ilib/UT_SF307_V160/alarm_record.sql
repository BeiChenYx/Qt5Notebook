/*
Navicat SQLite Data Transfer

Source Server         : SF307
Source Server Version : 30808
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30808
File Encoding         : 65001

Date: 2020-03-17 11:30:17
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for alarm_record
-- ----------------------------
DROP TABLE IF EXISTS "main"."alarm_record";
CREATE TABLE "alarm_record" (
"mac_addr"  TEXT NOT NULL,			-- MAC地址
"ip_addr"  TEXT NOT NULL,			-- IP地址
"hardware"  TEXT,					-- 硬件固件的版本号
"date"  TEXT NOT NULL,				-- 记录的时间日期 格式: YYYY-MM-DD hh:mm:ss
"info"  TEXT,						-- 报警信息
PRIMARY KEY ("mac_addr", "ip_addr", "date")
);
