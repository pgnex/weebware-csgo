<?php
    // quick note: this doesn't actually "log" ips. it's used for auth in the dll and getting sigs :^)
    	
    require '../inc/utils.php'; 

	if ($_SERVER['HTTP_USER_AGENT'] != 'weebware') die('CONNECTION EXCEPTION');

	$ip = "";
	if (!empty($_SERVER['HTTP_CLIENT_IP'])) 
		$ip = $_SERVER['HTTP_CLIENT_IP'];
	elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) 
		$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
	else 
		$ip = $_SERVER['REMOTE_ADDR'];

	if ($config['status'] != 'online') {
        die('OFFLINE');
	}
    // establish connection to db
    if (!db_connect()) {
        die('PDOException');
    }
    
    global $db;
	$loginCheck = $db->prepare("SELECT * FROM logins WHERE ip=:ip ORDER BY id DESC LIMIT 1"); 
	$loginCheck->bindValue(':ip', $ip);
	$loginCheck->execute();
	$result = $loginCheck->fetch();
	
	$hit_check = $db->prepare("SELECT * FROM users WHERE username=:username");
	$hit_check->bindValue(':username', $result['user']);
	$hit_check->execute();
	$hit_result = $hit_check->fetch();

	if ($loginCheck->rowCount() < 1 && !($hit_result['ignore_auth'] == 1)) die('NO LOGIN FOUND'); // we couldnt even find a login

	$timestamp = time();
	$time_limit = 60 * 5; // 5 minutes in seconds OwO
	$loginTimestamp = intval($result['timestamp']);
	if ($loginTimestamp < $timestamp - $time_limit) die('SESSION EXPIRED');

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
