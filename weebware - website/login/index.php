<?php 
ini_set('log_errors', 1);
ini_set('display_errors', 0);
include('server.php') ?>

<!DOCTYPE html>
<html>
<head>
    
        <style>
      .pScaling
      {
       margin-left: auto;
       margin-right: auto;
       width: 60%;
      }
    </style>
    
<title>&lrm;</title>
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="pScaling">
  <div style = "margin-top: 25%;">
  <div class="header">

  </div>
	 
  <form method="post" action="index.php">
  	<div class="input-group">
  		<label>Username</label>
  		<input type="text" name="username" >
  	</div>
  	<div class="input-group">
  		<label>Password</label>
  		<input type="password" name="password">
  	</div>
  	<div class="input-group"; style=float:right>
  		<button type="submit" class="btn" name="login_user">Enter</button>
  	</div>
  	<p>
  	    </br>
  		<a href="..\register">Not yet a member?</a>
  	</p>
  </form>
  </div>
  </div>
</body>
</html>