<?php

	// server.php
	// Initialize database connection and acquire utility functions.
	
    $config = array(
        "server" => "localhost", // Usually local host.
        "dbname" => "weebware_cheat",
        "username" => "weebware_admin",
        "password" => "HIuQC&Z*42hz",
        "status" => "online", // Not really needed, just implemented to disable client
        "reason" => "Maintainence", // Returned to client if server isn't marked as online
        "version_key" => "BGBrrQ3te7c7SCHvHnYcDwFbKpRB9aa7"
    );
    
    function db_connect() {
        global $config;
        
        try {
            $GLOBALS['db'] = new PDO('mysql:host=' . $config['server'] . ';dbname=' . $config['dbname'], $config['username'], $config['password'], [PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);
            return true;
        } catch (PDOException $e) {
            return false;
        }        
    }
	
?>