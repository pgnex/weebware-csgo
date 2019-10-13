<?php
	require '../../inc/utils.php';

    session_start();
    
    if (isset($_SESSION['username'])) {
    	header('location: ..\home');
    	die();
    }

	// make sure the username and password is set
	if (isset($_POST['username']) && isset($_POST['password'])) {

		// initialize db connection
		db_connect();


		// check if valid login, if it is, store the username in session
		if (!login($_POST['username'], $_POST['password'])) {
			$issue = "Invalid username or password";
		}
		else if (user_banned($_POST['username'])) {
            $issue = "You have been banned.";
        }
		else {
			session_start();
			$_SESSION['username'] = $_POST['username'];
			successful_login($_POST['username'], true);
			header('location: ../home');
		}

	}
?>


<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <link rel="icon" type="image/png" href="https://weebware.net/images/favicon.png">
    <link rel="stylesheet" href="assets/css/circular-std.css">
    <link rel="stylesheet" href="assets/css/main.css">
    <title>weebware</title>
</head>
<body>
    <div class="container login">
        <img src="https://weebware.net/images/weebware_logo.png" alt="">
        <form method="post" target="">
            <? if (isset($issue)) { ?>
            <div style="margin: auto; color: red; padding-bottom: 10px;"><?=$issue?></div>
            <? } ?>	
            <input type="text" name="username" placeholder="Username">
            <input type="password" name="password" placeholder="Password">
            <button>Login</button>
            <a href="../register">Create an account</a>
        </form>
    </div>
</body>
</html>