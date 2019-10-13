<?php
	require '../../inc/utils.php';

    session_start();
    
    if (isset($_SESSION['username'])) {
    	header('location: ../home');
    	die();
    }

	if (isset($_POST['email']) && isset($_POST['username']) && isset($_POST['password']) && isset($_POST['password2'])) {

		// check that values are set
		
		$_POST['email'] = htmlentities($_POST['email'], ENT_QUOTES, "UTF-8");
		$_POST['username'] = htmlentities($_POST['username'], ENT_QUOTES, "UTF-8");

		if (empty($_POST['email'])) {
			$issue = "Please fill out all fields";
			goto failed_register;
		}
		
		if (!filter_var($_POST['email'], FILTER_VALIDATE_EMAIL)) {
			$issue = "Invalid Email";
			goto failed_register;
		}

		if (empty($_POST['username'])) {
			$issue = "Please fill out all fields";
			goto failed_register;
		}
		
		if (!preg_match('/^\w{0,16}$/', $_POST['username'])) {
		    $issue = "Username must be less than 16 characters and alphanumeric";
		    goto failed_register;
		}

		if (empty($_POST['password'])) {
			$issue = "Please fill out all fields";
			goto failed_register;
		}

		if (empty($_POST['password2'])) {
			$issue = "Please fill out all fields";
			goto failed_register;
		}

		// make sure passwords match
		if (!($_POST['password'] == $_POST['password2'])) {
			$issue = "Passwords do not match";
			goto failed_register;
		}

		// initialize db connection
		db_connect();

		if (check_user_exists($_POST['username'])) {
			$issue = "Username already exists";
			goto failed_register;
		}

		if (check_email_exists($_POST['email'])) {
			$issue = "Email already exists";
			goto failed_register;
		}
		
		$date = new DateTime();

		// ok everything checks out, lets insert the new user now.
		create_user($_POST['username'], $_POST['password'], $_POST['email']);
		insert_registration($_POST['username'], $date->getTimestamp(), get_client_ip());
		session_start();
		$_SESSION['username'] = $_POST['username'];
		header('location: ../home');

	}

	// meh we failed what can we do
	failed_register:

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
    <div class="container">
        <img src="https://weebware.net/images/weebware_logo.png" alt="">
        <form method="post" target="">
            <? if (isset($issue)) { ?>
            <div style="margin: auto; color: red; padding-bottom: 10px;"><?=$issue?></div>
            <? } ?>	
            <input type="text" name="username" placeholder="Username">
            <input type="email" name="email" placeholder="E-Mail">

            <input type="password" name="password" placeholder="Password">
            <input type="password" name="password2" placeholder="Repeat password">
            <button>Register</button>
            <a href="../login">Already have an account?</a>
        </form>
    </div>
</body>
</html>