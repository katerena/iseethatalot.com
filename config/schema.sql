CREATE TABLE `alot` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `word` varchar(100) NOT NULL,
  `image` text,
  `added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `verified` timestamp NULL DEFAULT NULL,
  `curator_rating` int(11) NOT NULL DEFAULT '0',
  `up_votes` int(11) NOT NULL DEFAULT '0',
  `down_votes` int(11) NOT NULL DEFAULT '0',
  `composed_url` text,
  `composed_path` text,
  `status` varchar(100) DEFAULT NULL,
  `processed` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `word` (`word`)
) DEFAULT CHARSET=utf8;
