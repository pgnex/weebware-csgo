<?php
    include 'safe_request.php';
    require '../inc/utils.php';
    
    define('ENCRYPTION_KEY', 'kBjQEvQqf3zFBTD2');
    $sr = new SafeRequest(ENCRYPTION_KEY);

	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') {
		$response = array('status' => 'failed', 'detail' => 'connection error');
		$sr->output(false, 'test', $response);
	}

    // establish connection to db
    if (!db_connect()) {
        $response = array('status' => 'failed', 'detail' => 'connection error');
	    $sr->output(false, 'test', $response);
    }
    
	if ($config['status'] != 'online') {
		$response = array('status' => 'failed', 'detail' => 'server offline', 'reason' => $config['reason']);
		$sr->output(false, 'test', $response);
	}

	
	if (isset($_POST['email']) && isset($_POST['username'])) {
	    if (update_email($_POST['username'], $_POST['email'])) {
	        $response = array('added' => true);
		    $sr->output(true, 'test', $response);
	    }
	}
	
?>