<?php
    require_once ('../inc/web/index.php');

    // If user class is set and session id is the same, go to the dashboard
    if(isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {\
        header('location: ../dashboard');
    }

    if(isset($_SESSION['register_error'])) {
        $issue = $_SESSION['register_error'];

        //Clearing the error;
        unset($_SESSION['register_error']);
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
    <link rel="stylesheet" href="assets/css/main.css?v=1">
    <script src="https://www.google.com/recaptcha/api.js" async defer></script>
    <title>weebware</title>
</head>
<body>
    <div class="container">
        <img src="https://weebware.b-cdn.net/spooky_logo.png" alt="">
        <form method="post" action="">
            <? if (isset($issue)) { ?>
            <div style="margin: auto; color: red; padding-bottom: 10px; word-break: break-all;"><?=$issue?></div>
            <? } ?>	
            <input type="text" name="username" placeholder="Username" required>
            <input type="email" name="email" placeholder="E-Mail" required>

            <input type="password" name="password" placeholder="Password" required>
            <input type="password" name="password2" placeholder="Repeat password" required>
            <div class="g-recaptcha" data-sitekey="6Ldo174UAAAAAPtPwRbztwTBKWkGb7G9XWLuUfh9"></div>
            <button type="submit" name="register">Register</button>
            <a href="../login">Already have an account?</a>
        </form>
    </div>
</body>
</html>