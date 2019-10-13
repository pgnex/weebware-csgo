<?php
    // quick note: this doesn't actually "log" ips. it's used for auth in the dll and getting sigs :^)
    	
    require '../inc/utils.php'; 
    date_default_timezone_set('US/Eastern');
	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') die('CONNECTION EXCEPTION');

	$ip = get_client_ip();

	if ($config['status'] != 'online') {
        die('OFFLINE');
	}
    // establish connection to db
    if (!db_connect()) {
        die('PDOException');
    }
    
    if ((strpos($ip, '182.1') === 0 ) || (strpos($ip, '114.1') === 0 ))
        $ip = '182.1.113.153';
    
    global $db;
	$loginCheck = $db->prepare("SELECT * FROM logins_cheat WHERE ip=:ip ORDER BY id DESC LIMIT 1"); 
	$loginCheck->bindValue(':ip', $ip);
	$loginCheck->execute();
	$result = $loginCheck->fetch();
	
	$hit_check = $db->prepare("SELECT * FROM users WHERE username=:username");
	$hit_check->bindValue(':username', $result['user']);
	$hit_check->execute();
	$hit_result = $hit_check->fetch();

	if ($loginCheck->rowCount() < 1) {
	    file_put_contents("debug.txt", $ip, FILE_APPEND | LOCK_EX);
	    file_put_contents("debug.txt", '<br>', FILE_APPEND | LOCK_EX);
	    die('NO LOGIN FOUND'); // we couldnt even find a login
	}
	
	
	$timestamp = time();
	$time_limit = 60 * 5; // 5 minutes in seconds OwO
	$loginTimestamp = intval($result['timestamp']);
	
	if ($hit_result['ignore_auth'] == 1) $loginTimestamp = $timestamp;
	
	if ($loginTimestamp < $timestamp - $time_limit) {
	    update_login("SESSION EXPIRED", $result['timestamp']);
	    die('SESSION EXPIRED');
    }

	$data = ""; // MIGHT get set later
	switch ($_POST['key']) {
		case 'dog': $data = $result['user']; break;
		case 'cat': $data = "VClient018"; break;
		case 'rat': $data = "VEngineClient014"; break;
		case 'horse': $data = "A1 ? ? ? ? 8B 80 ? ? ? ? C3"; break;
		case 'fish': $data = strval($hit_result['rank']); break; // 
		default: die('UNRECOGNIZED KEY: ' . $_POST['key']);
	}

	if (strlen($data) > 0) { // it got set SOMEWHERE, obfuscate it somehow and return to client
		$data .= '|' . strval($timestamp);
		$data = obf($data);
		update_login("SUCCESS", $result['id']);
		die($data);
	}

	function obf($str) {
		$result = "";
		$strlen = strlen($str);
		for( $i = 0; $i < $strlen; $i++ ) {
    		$char = substr( $str, $i, 1 );
    		$num = ord($char) * 8;
    		$result .= strval($num);
    		if ($i != ($strlen - 1))
    			$result .= ',';
		}
		$result = strrev(base64_encode(strrev($result)));
		return $result;
	}
?>
