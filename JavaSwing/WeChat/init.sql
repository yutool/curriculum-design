CREATE TABLE `user`  (
  `name` char(20) PRIMARY KEY,
  `password` char(16)  NOT NULL,
  `friends` varchar(200),
  `flocks` varchar(200)
);
INSERT INTO `user` VALUES ('John', '123456', 'Liam&Luke', NULL);
INSERT INTO `user` VALUES ('Liam', '123456', 'Luke&John', NULL);
INSERT INTO `user` VALUES ('Luke', '123456', 'Liam&John', NULL);