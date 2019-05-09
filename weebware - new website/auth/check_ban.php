<?php

    include 'safe_request.php';
    require '../inc/utils.php';
    define('ENCRYPTION_KEY', 'kBjQEvQqf3zFBTD2');
    $sr = new SafeRequest(ENCRYPTION_KEY);

	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') {
		$response = array('status' => 'failed', 'detail' => 'connection error');
		$sr->output(false, 'fk u', $response);
	}
    
    // establish connection to db
    if (!db_connect()) {
        $response = array('status' => 'failed', 'detail' => 'connection error');
		$sr->output(false, 'fk u', $response);
    }
    
    if (ip_banned(get_client_ip())) {
	    $response = array('status' => 'failed', 'detail' => 'banned');
		$sr->output(false, 'fk u', $response);
    }
    
    if (hwid_banned($_POST['hwid'])) {
	    $response = array('status' => 'failed', 'detail' => 'banned');
		$sr->output(false, 'fk u', $response);
    }
    
    // not banned
    $sr->output(true, 'pp', '');
?>