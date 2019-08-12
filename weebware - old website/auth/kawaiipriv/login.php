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
		$response = 'failed';
		die(json_encode($response));
	}

	// Successfully connected to the database.

	$checkLogin = $conn->prepare("SELECT * FROM users WHERE username=:username"); 
	$checkLogin->bindValue(':username', $_POST['username']);
	$checkLogin->execute();
	$result = $checkLogin->fetch();
	if ($checkLogin->rowCount() < 1) {
		// Account doesn't exist.
		$response = 'no account';
		die(json_encode($response));
	}

	if (password_verify($_POST['password'], $result["password"])) {
		// Password is correct.

		// Check verification time.
		$seconds_TimeLeft = intval($result["expire"]) - time();
		if ($seconds_TimeLeft < 1) {
			$response = 'sub invalid';
			die(json_encode($response));
		}
		if (intval($result["expire"]) >= 2000000000) {
			$seconds_TimeLeft = 2000000000;
		}

		// Make sure HWID matches.
		if (($_POST['hwid'] != $result["hwid"]) && ($result["hwid"] != "0")) {
			$response = 'hwid mismatch';
			die(json_encode($response));
		}  

		// Update HWID if needed.
		if ($result["hwid"] == "0") {
			$updateAccount = $conn->prepare("UPDATE users SET hwid=:hwid WHERE username=:username");
			$updateAccount->bindValue(':username', $_POST['username']);
			$updateAccount->bindValue(':hwid', $_POST["hwid"]);
			$updateAccount->execute();
		}

		prepend('[' . date('Y-m-d H:i:s') . '] ' . $_POST['username'] . ' logged in from ' . $_SERVER['REMOTE_ADDR'], secret_directory('logs/logins.data'));

		$response = 'successful';
		die(json_encode($response));
	
	} else {
		// Password is incorrect.
		$response = 'wrong password';
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

	function generateRandomString($length = 10) {
    	$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    	$charactersLength = strlen($characters);
    	$randomString = '';
    	for ($i = 0; $i < $length; $i++) {
        	$randomString .= $characters[rand(0, $charactersLength - 1)];
    	}
    	return $randomString;
	}
?>