<?php
	date_default_timezone_set('US/Eastern'); // Set time zone for logging.
	ini_set('log_errors', 1);
	ini_set('display_errors', 0);

	if ($_SERVER['HTTP_USER_AGENT'] != 'kawaiipriv') 
	{
		$response = 'connection error';
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

	$checkUsername = $conn->prepare("SELECT * FROM users WHERE username=:username");
	$checkUsername->bindValue(':username', $_POST['username']);
	$checkUsername->execute();
	if ($checkUsername->rowCount() > 0) { 
		// Username already taken
		$response = 'username taken';
		die(json_encode($response));
	}

	// Prepare to create their account.
	$createAccount = $conn->prepare("INSERT INTO users (id, username, password, expire, hwid) VALUES ('', :username, :password, '0', '0');");
	$createAccount->bindValue(':username', $_POST['username']);
	$createAccount->bindValue(':password', password_hash($_POST['password'], PASSWORD_BCRYPT));

	// Execute statement to create account and update key info.
	if($createAccount->execute()) {
		prepend('[' . date('Y-m-d H:i:s') . '] ' . $_POST['username'] . ' has created their account from ' . $_SERVER['REMOTE_ADDR'], secret_directory('logs/registrations.data'));
		$response = 'successful';
		die(json_encode($response));
	} else {
		$response = 'connection error';
		die(json_encode($response));
	}	

	unset($conn);

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