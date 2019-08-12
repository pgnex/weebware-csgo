<?php
	date_default_timezone_set('US/Eastern'); // Set time zone for logging.
	ini_set('log_errors', 1);
	ini_set('display_errors', 0);

	if ($_SERVER['HTTP_USER_AGENT'] != 'kawaiipriv') 
	{
		$response = 'failed';
		die(json_encode($response));
	}

	include secret_directory('config.php'); // SQL Server stuff
	$server_server = $config['server'];
	$server_username = $config['username'];
	$server_password = $config['password'];
	$server_dbname = $config['dbname'];
	$server_status = $config['status'];

	if ($server_status != 'online') {
		$response = 'server offline';
		die(json_encode($response));
	}
	
	try 
	{
		$conn = new PDO('mysql:host=' . $server_server . ';dbname=' . $server_dbname, $server_username, $server_password,[PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);
	} catch (PDOException $e) {
    	$response = 'connection error';
		die(json_encode($response));
	}

	// Successfully connected to the database.

		$checkKey = $conn->prepare('SELECT * FROM regkeys WHERE regkey=:regkey');
	$checkKey->bindValue(':regkey', $_POST['regkey']);
	$checkKey->execute();
	$checkKeyResult = $checkKey->fetch();
	if ((int)$checkKeyResult['used'] == 1) { 
		// Key has already been used
		$response = 'key used';
		die(json_encode($response));
	}
	if ($checkKey->rowCount() == 0) {
		// Key doesn't exist
		$response = 'key invalid';
		die(json_encode($response));
	}
	
	$checkUsername = $conn->prepare("SELECT * FROM users WHERE username=:username");
	$checkUsername->bindValue(':username', $_POST['username']);
	$checkUsername->execute();
	$checkUsernameResult = $checkUsername->fetch();
	if ($checkUsername->rowCount() == 0) { 
		// Username doesn't exist.
		$response = 'user not found';
		die(json_encode($response));
	}

	$currentExpireValue = (int) $checkUsernameResult['expire'];
	if ($currentExpireValue >= 2000000000) {
		// User already has lifetime.
		$response = 'already lifetime';
		die(json_encode($response));
	}

	// Key hasn't been used yet, username is valid. Establish expiration date.
	$expireValue = 0;
	if ((int)$checkKeyResult['lifetime'] == 1) { 
		// Value >= 2 billion is considered lifetime
		$expireValue = 2000000000;
	} else {
		// Add days/months/years to current time (add weeks * 7 to days, since DateInterval doesn't accept weeks.)
		$daysToAdd = (int)$checkKeyResult['days'];
		$weeksToAdd = (int)$checkKeyResult['weeks'];
		$monthsToAdd = (int)$checkKeyResult['months'];
		$yearsToAdd = (int)$checkKeyResult['years'];
		$daysToAdd += $weeksToAdd * 7;
		$date = new DateTime();
		$date->setTimestamp($currentExpireValue);
		$date->add(new DateInterval("P{$yearsToAdd}Y{$monthsToAdd}M{$daysToAdd}D"));
		$expireValue = $date->getTimestamp();
		if ($currentExpireValue < time()) { $expireValue = $expireValue + (time() - $currentExpireValue); }
	}

	// Update users expiration time.
	$updateAccount = $conn->prepare("UPDATE users SET expire=:expire WHERE username=:username");
	$updateAccount->bindValue(':username', $_POST['username']);
	$updateAccount->bindValue(':expire', $expireValue);

	// Mark the key as used, let it know who the user was.
	$usedKey = $conn->prepare("UPDATE regkeys SET used='1', user=:user WHERE regkey=:regkey");
	$usedKey->bindValue(':regkey', $_POST['regkey']);
	$usedKey->bindValue(':user', $_POST['username']);

	// Execute statement to update expiration date and key info.
	if($updateAccount->execute() && $usedKey->execute()) {
		$log_message = $_POST['username'] . ' used the registration key ' .  $_POST['regkey'] . ' from ' . $_SERVER['REMOTE_ADDR'];
		$headers = 'From: '.'request@justin.ooo'."\r\n".
					'Reply-To: '. 'me@justin.ooo' ."\r\n" .
					'X-Mailer: PHP/' . phpversion();
		@mail('justin@garofolo.net', 'AntarticaCheats Activated', $log_message, $headers);  
		prepend('[' . date('Y-m-d H:i:s') . '] ' . $log_message, secret_directory('logs/redeems.data'));
		$response = 'successful';
		die(json_encode($response));
	} else {
		$response = 'connection error';
		die(json_encode($response));
	}	

	function secret_directory($fileName) {
		return 'G85270wGe7/' . $fileName;
	}

	function prepend($string, $orig_filename) {
  		$string .= ("<br>" . PHP_EOL);
  		$context = stream_context_create();
  		$orig_file = fopen($orig_filename, 'r', 1, $context);

  		$temp_filename = tempnam(sys_get_temp_dir(), 'php_prepend_');
  		file_put_contents($temp_filename, $string);
  		file_put_contents($temp_filename, $orig_file, FILE_APPEND);

  		fclose($orig_file);
  		unlink($orig_filename);
  		rename($temp_filename, $orig_filename);
  		chmod($orig_filename, 0777);
	}
?>