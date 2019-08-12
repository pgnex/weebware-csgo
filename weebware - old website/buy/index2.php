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
    <center><p>subscription to weebware.net</p></center>
  <form method="post" action="index.php">
  	<div class="input-group">
  		<label>Email:</label>
  		<input type="text" name="email" style="width: 100%; padding: 5px 0px;">
  	</div>
  	
  <label for="Length">Duration:</label>
  <br>
  <select name="Length" class="minimal">
    <option value="1">1 Month ($14)</option>
    <option value="3">3 Months ($26)</option>
    <option value="69">Lifetime ($46)</option>
  </select>
  <br>
  	<div class="input-group"; style=float:right>
  		<button type="submit" class="btn" name="submit_purchase">Continue</button>
  	</div>
  </form>
  </div>
  </div>
</body>
</html>