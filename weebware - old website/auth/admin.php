<?php

    require '../inc/utils.php';

	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') {
		$response = array('status' => 'failed', 'detail' => 'connection error');
		die(json_encode($response));
	}

    // establish connection to db
    if (!db_connect()) {
        $response = array('status' => 'failed', 'detail' => 'connection error');
	    die(json_encode($response));
    }
    
    
    if (!check_user_exists($_POST['username'])) {
        $response = array( 'status' => 'failed', 'detail' => 'invalid');
        die(json_encode($response));
    }
    
    switch ($_POST['task']) {
        case 'hwid':
            update_hwid($_POST['username'], 0);
            $response = array('status' => 'successful');
            die(json_encode($response));
        break;
        case 'password':
            update_password($_POST['username'], $_POST['password']);
            $response = array('status' => 'successful');
            die(json_encode($response));
        break;
    }
    
?>