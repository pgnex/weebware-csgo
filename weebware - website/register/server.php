<?php
session_start();
ini_set('display_errors', 0);
// initializing variables
$username = "";
$email    = "";
$errors = 0; 


    // connect to the database
	include secret_directory('config.php'); // SQL Server stuff
	$server_server = $config['server'];
	$server_username = $config['username'];
	$server_password = $config['password'];
	$server_dbname = $config['dbname'];
	$server_status = $config['status'];
	try 
	{
		$conn = new PDO('mysql:host=' . $server_server . ';dbname=' . $server_dbname, $server_username, $server_password,[PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);
	} catch (PDOException $e) {
	    echo 'gay';
	}

// REGISTER USER
if (isset($_POST['reg_user'])) {
    // assign values to make easier
  $username =  $_POST['username'];
  $email =  $_POST['email'];
  $password_1 =  $_POST['password_1'];
  $password_2 = $_POST['password_2'];
  
  

  // check that form is filled..
  if (empty($username)) { $errors++; echo "<script type='text/javascript'>alert('Please enter a username!');</script>";; }
  if (empty($email)){ $errors++; echo "<script type='text/javascript'>alert('Please enter a email!');</script>";; }
  if (empty($password_1)) { $errors++; echo "<script type='text/javascript'>alert('Please enter a password!');</script>";; }
  if ($password_1 != $password_2) {
      echo "<script type='text/javascript'>alert('Passwords do not match');</script>";
	$errors++;
  }


    // check username, email, and invite is valid
    
	$checkUsername = $conn->prepare("SELECT * FROM users WHERE username=:username");
	$checkUsername->bindValue(':username', $username);
	$checkUsername->execute();
	if ($checkUsername->rowCount() > 0) { 
		// Username already taken
      echo "<script type='text/javascript'>alert('Username already exists');</script>";
      $errors++;
	}
  
  	$checkEmail = $conn->prepare("SELECT * FROM users WHERE email=:email");
	$checkEmail->bindValue(':email', $email);
	$checkEmail->execute();
	if ($checkEmail->rowCount() > 0) { 
		// Email already taken
      echo "<script type='text/javascript'>alert('Email already exists');</script>";
      $errors++;
	}
	
	
  // insert values if everything is valid and checks out..
  if ($errors == 0) {
      
	$createAccount = $conn->prepare("INSERT INTO users (id, username, password, email, hwid) VALUES ('', :username, :password, :email, 0);");
	$createAccount->bindValue(':username', $username);
	$createAccount->bindValue(':email', $email);
	$createAccount->bindValue(':password', password_hash($password_1, PASSWORD_BCRYPT));
	$createAccount->execute();
	
  	$_SESSION['username'] = $username;
  	header('location: ..\home');
  }
}
	function secret_directory($fileName) {
		return 'RmNy5KApCY/' . $fileName;
	}

?>