<?php
    require_once ('../inc/web/index.php');

    // If user class is set and session id is the same, go to the dashboard
    if(isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {
        header('location: ../dashboard');
    }

    if(isset($_SESSION['loggin_error'])) {
        $issue = $_SESSION['loggin_error'];

        //Clearing the error;
        unset($_SESSION['loggin_error']);
    }
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <link rel="icon" type="image/png" href="https://weebware.b-cdn.net/favicon.png">
    <link rel="stylesheet" href="assets/css/circular-std.css">
    <link rel="stylesheet" href="assets/css/main.css?v=3">
    <title>weebware</title>
</head>
<body>
    <div class="container login">
        <img src="https://weebware.b-cdn.net/spooky_logo.png" alt="">
        <form method="post" action="">
            <? if (isset($issue)) { ?>
            <div style="margin: auto; color: red; padding-bottom: 10px;"><?=$issue?></div>
            <? } ?>	
            <input type="text" name="username" placeholder="Username" required>
            <input type="password" name="password" placeholder="Password" required>
            <button type="submit" name="login">Login</button>
            <a href="../register">Create an account</a>
        </form>
    </div>
</body>
</html>