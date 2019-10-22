<?php
	date_default_timezone_set('US/Eastern'); // Set time zone for logging.
	ini_set('log_errors', 1);
	ini_set('display_errors', 0);
    include 'safe_request.php';
    require '../inc/utils.php';
    define('ENCRYPTION_KEY', 'kBjQEvQqf3zFBTD2');
    $sr = new SafeRequest(ENCRYPTION_KEY);
    
	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') {
		$response = [
		    'status' => 'failed',
            'detail' => 'connection error'
        ];

		$sr->output(false, 'test', $response);
	}
    
    // make sure we have server online
	if ($config['status'] != 'online') {
		$response = [
            'status' => 'failed',
            'detail' => 'server offline',
            'reason' => $config['reason']
        ];
		$sr->output(false, 'test', $response);
	}
	
	// made this so i dont have to change directories and set shit to offline for every loader update
	if ($config['version_key'] != $_POST['version_key']) {
	    $response = [
	        'status' => 'failed',
            'detail' => 'update'
        ];

	    $sr->output(false, 'test', $response);
	}
    
    // establish connection to db
    if (!db_connect()) {
        $response = [
            'status' => 'failed',
            'detail' => 'connection error'
        ];
	    $sr->output(false, 'test', $response);
    }
    
    // make sure this user exists
    if (!check_user_exists($_POST['username'])) {
		$response = array('status' => 'failed', 'detail' => 'no account');
		$sr->output(false, 'test', $response);
    }
    
    $data = get_user_data($_POST['username']);
    
    // check if banned
    if (user_banned($_POST['username'])) {
	    $response = array('status' => 'failed', 'detail' => 'banned');
		$sr->output(false, 'test', $response);
    }
    
    if (ip_banned(get_client_ip())) {
	    $response = array('status' => 'failed', 'detail' => 'banned');
		$sr->output(false, 'test', $response);
    }
    
    if (hwid_banned($_POST['hwid'])) {
	    $response = array('status' => 'failed', 'detail' => 'banned');
		$sr->output(false, 'test', $response);
    }
    
    
    // hwid is set but incorrect
    if (($_POST['hwid'] != $data['hwid']) && ($data['hwid'] != "0")) {
	    $response = array('status' => 'failed', 'detail' => 'hwid mismatch');
		$sr->output(false, 'test', $response);
    }
        
    // hwid is not set, lets set it now.
    if ($data['hwid'] == 0) {
        update_hwid($_POST['username'], $_POST['hwid']);
    }
    
    if (!login($_POST['username'], $_POST['password'])) {
		$response = array('status' => 'failed', 'detail' => 'Incorrect password');
		$sr->output(false, 'test', $response);
    }
    
    $expire_time = intval($data['expire']) - time();
    if ($expire_time < 1) {
        $response =[
            'status' => 'failed',
            'detail' => 'sub invalid'
        ];

		$sr->output(false, 'test', $response);
    }

	if (intval($data["expire"]) >= 2000000000) {
		$expire_time = 2000000000;
	}
	
	successful_login($_POST['username']);
	
	// Use the informtion from cheat files to send data to the client
	include 'cheat.php';
	$response = [
        'username' => $_POST['username'],
        'time_left' => $expire_time,
        'rank' => $data['rank'],
        'notify' => 'Please join the new discord via https://weebware.net/discord',
        'name' => $cheat['name'],
        'download' => $cheat['download'],
        'key' => $cheat['key'],
        'detected' => $cheat['status'],
        'email' => empty($data['email']),
        'version' => $cheat['version'],
    ];
	$sr->output(true, 'test', $response);

?>