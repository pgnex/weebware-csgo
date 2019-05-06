<?php 
    session_start(); 
    ini_set('display_errors', 0);


    if (!isset($_SESSION['username'])) {
    	header('location: ..\login');
    	die();
    }
    
    if (isset($_GET['logout'])) {
    	session_destroy();
    	unset($_SESSION['username']);
    	header("location: ..\login");
    }
  

    // connect to database..
	include secret_directory('config.php'); // SQL Server stuff
	$server_server = $config['server'];
	$server_username = $config['username'];
	$server_password = $config['password'];
	$server_dbname = $config['dbname'];
	$server_status = $config['status'];
	$conn = new PDO('mysql:host=' . $server_server . ';dbname=' . $server_dbname, $server_username, $server_password,[PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);
	
	$download_url = 'https://weebware.net/download/10UZpA5rfOKgpMNkF.exe';
	$purchase_url = 'https://weebware.net/buy';
	
	$checkUser = $conn->prepare('SELECT * FROM users WHERE username=:username');
	$checkUser->bindValue(':username', $_SESSION['username']);
	$checkUser->execute();
	$checkUserResult = $checkUser->fetch();
	$currentExpireValue = $checkUserResult['expire'];
	$userID = $checkUserResult['id'];
	
	
	// get total registered users
    $checkSlots = $conn->prepare("SELECT * FROM users WHERE 1");
	$checkSlots->execute();
	$registered = $checkSlots->rowCount();
	
	// get active subscriptions
	date_default_timezone_set('UTC');
	$date = new DateTime();
	$expireValue = $date->getTimestamp();
	$checkActive = $conn->prepare("SELECT * FROM users WHERE expire > $expireValue");
	$checkActive->execute();
	$active = $checkActive->rowCount();
	
	// get expiration date
	if ($currentExpireValue > $expireValue) {
	if ($currentExpireValue == 2000000000) $expireDate = "never";
	else $expireDate = date('m/d/y', $currentExpireValue);
	}
	
	
	function secret_directory($fileName) {
	return 'RmNy5KApCY/' . $fileName;
	}
	
	
    function generateRandomString($length) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
    }
    
    $filename = generateRandomString(16);

?>
<!DOCTYPE html>
<html>
<head>
	<title>&#65279;</title>
	<link rel="icon" href="../favicon.ico">
	<link rel="stylesheet" href="style.css">
</head>
<body>
	<p><font face="verdana" size="1">welcome back, <span style="color:#f199ff;"><?=$_SESSION['username']?></span><span> [uid=<?=$userID?>]</span></font></p>
	<p><font face="verdana" size="1">&#8226; <?php  if ($currentExpireValue > $expireValue)	{ echo 'your sub expires ' . $expireDate; } else { echo "<a href='".$purchase_url."'>please purchase a sub here</a>"; } ?> </font></p>
	<p><font face="verdana" size="1"> <?php if ($currentExpireValue > $expireValue) {  ?> &#8226; <a href="https://weebware.net/download/loader.exe" download="<?=$filename?>.exe">download</a> <? } ?> </font></p> 
	<p><font face="verdana" size="1">&#8226; <a href="https://weebware.net/discord">discord</a></font></p>
	<p><font face="verdana" size="1">- <a href="index.php?logout=1" style="color:#ff2323;">sign out</a></font></p>
	 <!-- <p><font face="verdana" size="1">registered: <?=$registered?></font></p> -->
	<p><font face="verdana" size="1">copyright &copy; <b>weebware.net</b> all rights reserved</font></p>
	<img src="1.png" class='bottomright' alt="zz">
	<audio id="mp3" loop autoplay src="mp3.mp3"></audio>
</body>
</html>