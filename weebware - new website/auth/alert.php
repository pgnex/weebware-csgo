<?php
    require '../inc/utils.php';

	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') {
		die('incorrect user-agent');
	}
	
    // establish connection to db
    if (!db_connect()) {
        die('failed to connect to db');
    }
    
	add_ban(NULL, $_POST['hwid'], get_client_ip(), 'Crack Attempt');

	$log_message = $_SERVER['REMOTE_ADDR'] . ' attempted to debug or dump ' . $_POST['reason'];
	$headers = 'From: '.'admin@weebware.net'."\r\n".
				'Reply-To: '. 'ifrickinghatelife@gmail.com' ."\r\n" .
				'X-Mailer: PHP/' . phpversion();
	@mail('ifrickinghatelife@gmail.com', 'weebware crack attempt', $log_message, $headers);  
	die();
?>