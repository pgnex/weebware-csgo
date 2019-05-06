<?php
session_start();
ini_set('display_errors', 0);

// connect to the database
include secret_directory('config.php'); // SQL Server stuff
$server_server   = $config['server'];
$server_username = $config['username'];
$server_password = $config['password'];
$server_dbname   = $config['dbname'];
$server_status   = $config['status'];

try {
    $conn = new PDO('mysql:host=' . $server_server . ';dbname=' . $server_dbname, $server_username, $server_password,[PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);
}
catch (PDOException $e) {
    die();
}

// check user info and stuff
if (isset($_POST['login_user'])) {
    
    if (empty($_POST['username'])) {
        echo "<script type='text/javascript'>alert('Please enter your username');</script>";
        die();
    }
    if (empty($_POST['password'])) {
        echo "<script type='text/javascript'>alert('Please enter your password');</script>";
        die();
    }
    
    $checkLogin = $conn->prepare("SELECT * FROM users WHERE username=:username");
    $checkLogin->bindvalue(':username', $_POST['username']);
    $checkLogin->execute();
    $result = $checkLogin->fetch();
    if (password_verify($_POST['password'], $result["password"])) {
        // successfully logged in..
        if ($checkLogin->rowCount() == 1) {
            $_SESSION['username'] = $_POST['username'];
            $_SESSION['success']  = "You are now logged in";
            header('location: ..\home');
        }
    } else {
        // ur fucking stupid and something is incorrect
        echo "<script type='text/javascript'>alert('Incorrect username or password');</script>";
    }
}

function secret_directory($fileName)
{
    return 'RmNy5KApCY/' . $fileName;
}

?>