<?php 

    define('ONE_MONTH', 86400); // one day in seconds
    session_set_cookie_params(ONE_MONTH); // cookie lifetime = 1 day 

	$started = @session_start();
	if(!$started) {
		session_regenerate_id(true); // replace the Session ID
		session_start(); 
    }
    
    if (!isset($_SESSION['username'])) {
        session_destroy();
        header("location: ../login/");
        die();
    }

	// handle logout
	if (isset($_GET['logout'])) {
        session_destroy();
        header("location: ../login/");
        die();
    }


    // function to require a login
	function require_login() {
		if (!is_logged_in()) {
            header("location: ../login/");
            die();
       	}
	}

	// function to check if a user is logged in
	function is_logged_in() {
		return isset($_SESSION['username']);
	}

?>