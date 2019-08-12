<?php

    session_start();
    
    if (!isset($_SESSION['username'])) {
    	header('location: ..\login');
    	die();
    }
    else {
    	header('location: ..\home');
    	die();
    }

?>