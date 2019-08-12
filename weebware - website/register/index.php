<?php
	require '../inc/utils.php';

    session_start();
    
    if (isset($_SESSION['username'])) {
    	header('location: ..\home');
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
	<title>weebware.net - register</title>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
<!--===============================================================================================-->	
	<link rel="icon" type="image/png" href="../images/favicon.png"/>
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/bootstrap/css/bootstrap.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="fonts/font-awesome-4.7.0/css/font-awesome.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="fonts/iconic/css/material-design-iconic-font.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/animate/animate.css">
<!--===============================================================================================-->	
	<link rel="stylesheet" type="text/css" href="vendor/css-hamburgers/hamburgers.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/animsition/css/animsition.min.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="vendor/select2/select2.min.css">
<!--===============================================================================================-->	
	<link rel="stylesheet" type="text/css" href="vendor/daterangepicker/daterangepicker.css">
<!--===============================================================================================-->
	<link rel="stylesheet" type="text/css" href="css/util.css">
	<link rel="stylesheet" type="text/css" href="css/main.css">
<!--===============================================================================================-->
</head>
<body>
	
	<div class="limiter">
		<div class="container-login100" style="background: #2b333c">
			<div class="wrap-login100" style="background: #1b2026">
				<form class="login100-form" method="post" target="">

					<? if (isset($issue)) { ?>

					<span class="login100-form-title p-b-48">
          				<div class="txt1" style="margin: auto; color: red"><?=$issue?></div>
					</span>

					<? } ?>	


					<span class="login100-form-title p-b-48">
          				<div class="site-logo" style="margin: auto"><img src="../images/weebware_logo.png" alt="Image" style="height: auto;"></div>
					</span>

					<div class="wrap-input100">
						<input class="input100" type="text" name="email">
						<span class="focus-input100" data-placeholder="Email"></span>
					</div>

					<div class="wrap-input100">
						<input class="input100" type="text" name="username">
						<span class="focus-input100" data-placeholder="Username"></span>
					</div>

					<div class="wrap-input100">
						<input class="input100" type="password" name="password">
						<span class="focus-input100" data-placeholder="Password"></span>
					</div>


					<div class="wrap-input100">
						<input class="input100" type="password" name="password2">
						<span class="focus-input100" data-placeholder="Confirm Password"></span>
					</div>


					<div class="container-login100-form-btn">
						<div class="wrap-login100-form-btn">
							<div class="login100-form-bgbtn"></div>
							<button class="login100-form-btn">
								Register
							</button>
						</div>
					</div>

					<div class="text-center p-t-115">
						<span class="txt1">
							Already have an account?
						</span>

						<a class="txt2" href="../login" style="color: white">
							Sign in
						</a>
					</div>
				</form>
			</div>
		</div>
	</div>
	
<!--===============================================================================================-->
	<script src="vendor/jquery/jquery-3.2.1.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/animsition/js/animsition.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/bootstrap/js/popper.js"></script>
	<script src="vendor/bootstrap/js/bootstrap.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/select2/select2.min.js"></script>
<!--===============================================================================================-->
	<script src="vendor/daterangepicker/moment.min.js"></script>
	<script src="vendor/daterangepicker/daterangepicker.js"></script>
<!--===============================================================================================-->
	<script src="vendor/countdowntime/countdowntime.js"></script>
<!--===============================================================================================-->
	<script src="js/main.js"></script>

</body>
</html>