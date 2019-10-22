<?php

    class Database {
        static function connect() {

            $config = [
                "server" => "45.77.76.146",
                "dbname" => "weebware_cheat",
                "username" => "root",
                "password" => "9CZc3e0%fjBq",
            ];

            try {
                return new PDO('mysql:host=' . $config['server'] . ';dbname=' . $config['dbname'], $config['username'], $config['password'], [PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);
            } catch (PDOException $e) {
                die('hi');
            }
        }
    }