<?php
session_start();
ini_set('display_errors', 0);

    if (!isset($_SESSION['username'])) {
    	header('location: ..\login');
    	die();
    }
    
    if (isset($_POST['Length'])) {
    
        $length = $_POST['Length'];
        if ($length != 1 && $length != 3 && $length != 69) die('Stop snooping around');
        
        switch ($length) {
            case '1':
                $title = 'weebware.net 1 month';
                $price = '14';
                break;
            case '3':
                $title = 'weebware.net 3 months';
                $price = '26';
                break;
            case '69':
                $title = 'weebware lifetime';
                $price = '46';
        }
        
        define("SELLY_EMAIL", "ifrickinghatelife@gmail.com");
        define("SELLY_API_KEY", "4zXzzy2csz9PeMqZafQUWykCNzoxKYzzujDs-MV5_j9rbJTG4w");

        // connect to the database
        include secret_directory('config.php'); // SQL Server stuff
        $server_server = $config['server'];
        $server_username = $config['username'];
        $server_password = $config['password'];
        $server_dbname = $config['dbname'];
        $server_status = $config['status'];
        $conn = new PDO('mysql:host=' . $server_server . ';dbname=' . $server_dbname, $server_username, $server_password,[PDO::ATTR_DEFAULT_FETCH_MODE=>PDO::FETCH_ASSOC]);

        $username = $_SESSION['username'];  
        $ip = $_SERVER['REMOTE_ADDR'];
        
        // get active subscriptions
        date_default_timezone_set('UTC');
        $date = new DateTime();
        $expireValue = $date->getTimestamp();
        $checkActive = $conn->prepare("SELECT * FROM users WHERE expire > $expireValue");
        $checkActive->execute();
        $active = $checkActive->rowCount();

        if (isset($_POST['submit_purchase'])) {
            if (!empty($_POST['email'])) {
                $email       = $_POST['email'];
                $secret      = generateRandomString(16);
                $webhook_url = 'https://weebware.net/buy/webhook.php?secret=' . $secret;
                $request     = selly_pay($title, 'PayPal', $email, $price, 'USD', 'https://weebware.net/home', $webhook_url, $ip);
                $createOrder = $conn->prepare("INSERT INTO orders (id, secret, order_id, user, length, ip, status) VALUES ('', :secret, :order_id, :user, :length, :ip, :status);");
                $createOrder->bindValue(':secret', $secret);
                $createOrder->bindValue(':order_id', $request['id']);
                $createOrder->bindValue(':user', $username);
                $createOrder->bindValue(':length', $length);
                $createOrder->bindValue(':ip', $ip);
                $createOrder->bindValue(':status', $request['status']);
                $createOrder->execute();
                $redirect = $request['url'];
                header('location:' . $redirect);
            }  
            else echo "<script type='text/javascript'>alert('Please enter an email');</script>";
        //    echo "<script type='text/javascript'>alert('Currently unavailable for purchase. Please wait until release is announced.');</script>";
        }
    }

 
    // thank justin for help ;3
    function selly_pay($title, $gateway, $email, $value, $currency, $return_url, $webhook_url, $ip) {
        $params = array ('title' => $title, 'gateway' => $gateway, 'email' => $email, 'value' => $value, 'currency' => $currency, 'return_url' => $return_url, 'webhook_url' => $webhook_url, 'white_label' => true, 'ip_address' => $ip);
        $query = http_build_query ($params);
        $auth = base64_encode(SELLY_EMAIL.':'.SELLY_API_KEY);
        $opts = array(
            'http'=>array(
                'method'=>"POST",
                'header'=>
                "User-agent: {$email} - {$_SERVER['SERVER_NAME']}\r\n" .
                "Authorization: Basic {$auth}\r\n" .
                "Content-Type: application/x-www-form-urlencoded",
                'content' => $query
            )
        );
        $context = stream_context_create($opts);
        $result = file_get_contents('https://selly.gg/api/v2/pay', false, $context);
        $data = json_decode($result, true);
        return $data;
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

	function secret_directory($fileName) {
		return 'RmNy5KApCY/' . $fileName;
	}

?>


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
    <center><h3>Subscription to weebware.net</h3></center>
  <form method="post" action="">
  	<div class="input-group">
  		<label>Email:</label>
  		<input type="text" name="email" style="width: 100%; padding-left: 10px; height: 40px; background-color: #2b2b31; border-radius: 5px;">
  	</div>
  	
  <label>Duration:</label>
  <br>
  <select name="Length" class="minimal" style="border-radius: 5px;">
    <option value="1">1 Month - $14</option>
    <option value="3">3 Months - $26</option>
    <option value="69">Lifetime - $46</option>
  </select>
  <br>
  	<div class="input-group"; style=float:right>
  		<button type="submit" class="btn" name="submit_purchase" style="background-color: #2b2b31;">Checkout</button>
  	</div>
  </form>
  </div>
  </div>
</body>
</html>