-- CREATE DATABASE `main_site`;
use rtype;

CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;


-- ce schema est utilisé pour la création d'une db je fps online
-- on doit donc avoir toutes les tables necessaire contenant les donnees suffisantes pour le jeu

CREATE TABLE `binds` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `forward` int(11) NOT NULL DEFAULT '87',
  `backward` int(11) NOT NULL DEFAULT '83',
  `left` int(11) NOT NULL DEFAULT '65',
  `right` int(11) NOT NULL DEFAULT '68',
  `shoot` int(11) NOT NULL DEFAULT '0',
  `scope` int(11) NOT NULL DEFAULT '1',
  `reload` int(11) NOT NULL DEFAULT '82',
  `run` int(11) NOT NULL DEFAULT '340',
  `swap` int(11) NOT NULL DEFAULT '341',
  `jump` int(11) NOT NULL DEFAULT '32',
  `pause` int(11) NOT NULL DEFAULT '80',
  `chat` int(11) NOT NULL DEFAULT '257',
  `score` int(11) NOT NULL DEFAULT '258',
  `map` int(11) NOT NULL DEFAULT '59',
  `mute` int(11) NOT NULL DEFAULT '71',
  `quit` int(11) NOT NULL DEFAULT '256',
  PRIMARY KEY (`id`),
  FOREIGN KEY (`user_id`) REFERENCES `users`(`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `mouse_settings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `sensitivity` int(11) NOT NULL DEFAULT '100',
  `invert_y` tinyint(1) NOT NULL DEFAULT '0',
  `invert_x` tinyint(1) NOT NULL DEFAULT '0',
  `mouse_acceleration` tinyint(1) NOT NULL DEFAULT '0',
  `mouse_smoothing` tinyint(1) NOT NULL DEFAULT '0',
  `mouse_filtering` tinyint(1) NOT NULL DEFAULT '0',
  `mouse_acceleration_x` int(11) NOT NULL DEFAULT '100',
  `mouse_acceleration_y` int(11) NOT NULL DEFAULT '100',
  `mouse_smoothing_x` int(11) NOT NULL DEFAULT '100',
  `mouse_smoothing_y` int(11) NOT NULL DEFAULT '100',
  `mouse_filtering_x` int(11) NOT NULL DEFAULT '100',
  `mouse_filtering_y` int(11) NOT NULL DEFAULT '100',
  PRIMARY KEY (`id`),
  FOREIGN KEY (`user_id`) REFERENCES `users`(`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `display_settings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `resolution` enum('1280x720', '1920x1080', 'fullscreen') NOT NULL DEFAULT '1920x1080',
  `framerate` enum('30', '60', '90', '120', '150', '180', '210', '240', 'unlimited') NOT NULL DEFAULT '120',
  `vsync` tinyint(1) NOT NULL DEFAULT '1',
  `fov` int(11) NOT NULL DEFAULT '90',
  `brightness` int(11) NOT NULL DEFAULT '100',
  `gamma` int(11) NOT NULL DEFAULT '100',
  `contrast` int(11) NOT NULL DEFAULT '100',
  `saturation` int(11) NOT NULL DEFAULT '100',
  `sharpness` int(11) NOT NULL DEFAULT '100',
  `colorblind` enum('none', 'protanopia', 'deuteranopia') NOT NULL DEFAULT 'none',
  PRIMARY KEY (`id`),
  FOREIGN KEY (`user_id`) REFERENCES `users`(`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `audio_settings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `master` int(11) NOT NULL DEFAULT '100',
  `music` int(11) NOT NULL DEFAULT '100',
  `effects` int(11) NOT NULL DEFAULT '100',
  `voice` int(11) NOT NULL DEFAULT '100',
  `voice_chat` int(11) NOT NULL DEFAULT '100',
  `voice_chat_push_to_talk` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  FOREIGN KEY (`user_id`) REFERENCES `users`(`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `profile` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `level` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  FOREIGN KEY (`user_id`) REFERENCES `users`(`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `game_history` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  -- duration time of the game in minutes
  `duration` int(11) NOT NULL DEFAULT '0',
  `start_time` timestamp NOT NULL DEFAULT current_timestamp(),
  `end_time` timestamp NOT NULL DEFAULT current_timestamp(),
  -- game mode
  `game_mode` enum('deathmatch', 'team deathmatch', 'capture the flag', 'king of the hill', 'free for all') NOT NULL DEFAULT 'deathmatch',
  -- game map
  `map` enum('map1', 'map2', 'map3', 'map4', 'map5') NOT NULL DEFAULT 'map1',
  `winner` int(11) NOT NULL DEFAULT '0',
  `player1_id` int(11) DEFAULT NULL,
  `player2_id` int(11) DEFAULT NULL,
  `player3_id` int(11) DEFAULT NULL,
  `player4_id` int(11) DEFAULT NULL,
  `player5_id` int(11) DEFAULT NULL,
  `player6_id` int(11) DEFAULT NULL,
  `player7_id` int(11) DEFAULT NULL,
  `player8_id` int(11) DEFAULT NULL,
  `player9_id` int(11) DEFAULT NULL,
  `player10_id` int(11) DEFAULT NULL,
  FOREIGN KEY (`player1_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player2_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player3_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player4_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player5_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player6_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player7_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player8_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player9_id`) REFERENCES `users`(`id`),
  FOREIGN KEY (`player10_id`) REFERENCES `users`(`id`),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `game_history_player` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `game_history_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `team` int(11) NOT NULL DEFAULT '0',
  `kills` int(11) NOT NULL DEFAULT '0',
  `deaths` int(11) NOT NULL DEFAULT '0',
  `assists` int(11) NOT NULL DEFAULT '0',
  `damage` int(11) NOT NULL DEFAULT '0',
  `shots` int(11) NOT NULL DEFAULT '0',
  `hits` int(11) NOT NULL DEFAULT '0',
  `headshots` int(11) NOT NULL DEFAULT '0',
  `accuracy` int(11) NOT NULL DEFAULT '0',
  `score` int(11) NOT NULL DEFAULT '0',
  FOREIGN KEY (`game_history_id`) REFERENCES `game_history`(`id`),
  FOREIGN KEY (`user_id`) REFERENCES `users`(`id`),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TABLE `friendships` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id_1` int(11) NOT NULL,
  `user_id_2` int(11) NOT NULL,
  `status` enum('pending', 'accepted', 'declined') NOT NULL DEFAULT 'pending',
  FOREIGN KEY (`user_id_1`) REFERENCES `users`(`id`),
  FOREIGN KEY (`user_id_2`) REFERENCES `users`(`id`),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;