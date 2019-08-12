<?php
    date_default_timezone_set('US/Eastern');
    require 'server.php';
    
    function check_user_exists($username) {
        global $db;
        $get_users = $db->prepare('SELECT * FROM users WHERE username=:username');
        $get_users->bindValue(':username', $username);
        $get_users->execute();
        
        if ($get_users->rowCount() > 0)
            return true;
        else
            return false;
	}
	
    function check_email_exists($email) {
        global $db;
        $get_email = $db->prepare('SELECT * FROM users WHERE email=:email');
        $get_email->bindValue(':email', $email);
        $get_email->execute();
        
        if ($get_email->rowCount() > 0)
            return true;
        else
            return false;
	}
	
	function create_user($username, $password, $email) {
		global $db;
		$create_account = $db->prepare("INSERT INTO users (id, username, password, email, hwid) VALUES ('', :username, :password, :email, 0);");
		$create_account->bindValue(':username', $username);
		$create_account->bindValue(':email', $email);
		$create_account->bindValue(':password', password_hash($password, PASSWORD_BCRYPT));
		$create_account->execute();
	}
    
    function get_user_data($username) {
        global $db;
        $get_data = $db->prepare('SELECT * FROM users WHERE username=:username');
        $get_data->bindValue(':username', $username);
        $get_data->execute();
        return $get_data->fetch();
    }
    
    function update_hwid($username, $hwid) {
        global $db;
        $update_hwid = $db->prepare('UPDATE users SET hwid=:hwid WHERE username=:username');
        $update_hwid->bindValue(':hwid', $hwid);
        $update_hwid->bindValue(':username', $username);
        $update_hwid->execute();
	}

	function get_order_data($secret) {
		global $db;
		$get_order = $db->prepare('SELECT * FROM orders WHERE secret=:secret');
		$get_order->bindValue(':secret', $secret);
		$get_order->execute();

		return $get_order->fetch();
	}
	
	function is_sub_active($username, $expire) {
		if ($expire > time())
			return true;
		else
			return false;
	}
    
    function login($username, $password) {
		global $db;
		$check_login = $db->prepare('SELECT * FROM users WHERE username=:username');
		$check_login->bindValue(':username', $username);
		$check_login->execute();
		$user = $check_login->fetch();
		return $user && password_verify($password, $user['password']);
    }

   	function decrypt_string($to_decrypt) {
   	    $iv = 'zrw12MoTl6idAYzETkrMxt6tqStMpDUU';
   		$dKey =	'pw92UdN8BacNdbvvolcwEFj4PYV3uqMa';
        $to_decrypt = base64_decode($to_decrypt);
        $decryptedString = mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $dKey, $to_decrypt, MCRYPT_MODE_CBC, $iv);
        $decryptedString = rtrim($decryptedString, "\0\4");
        return $decryptedString;
    }
    
	function successful_login($username) {
	   global $db;
	   $log_login = $db->prepare("INSERT INTO logins (id, user, ip, timestamp) VALUES ('', :username, :ip, :timestamp);");
	   $log_login->bindValue(':username', $username);
	   $log_login->bindValue(':ip', get_client_ip());
	   $log_login->bindValue(':timestamp', time());
	   return $log_login->execute();
	}
	
	function update_email($username, $email) {
	    global $db;
	    $update_email = $db->prepare('UPDATE users SET email=:email WHERE username=:username');
	    $update_email->bindValue(':username', $username);
	    $update_email->bindValue(':email', $email);
	    return $update_email->execute();
	}
	
	function update_password($username, $password) {
	    global $db;
	    $reset_password = $db->prepare('UPDATE users SET password=:password WHERE username=:username');
	    $reset_password->bindValue(':username', $username);
	    $reset_password->bindValue(':password', password_hash($password, PASSWORD_BCRYPT));
	    return $reset_password->execute();
	}
	
	function user_banned($username) {
	    global $db;
	    $check_user_ban = $db->prepare('SELECT * FROM bans WHERE username=:username');
	    $check_user_ban->bindValue(':username', $username);
	    $check_user_ban->execute();
	    
        if ($check_user_ban->rowCount() > 0)
            return true;
        else
            return false;
	}
	
	function ip_banned($ip) {
	    global $db;
	    $check_ip_ban = $db->prepare('SELECT * FROM bans WHERE ip=:ip');
	    $check_ip_ban->bindValue(':ip', $ip);
	    $check_ip_ban->execute();
	    
        if ($check_ip_ban->rowCount() > 0)
            return true;
        else
            return false;
	}
	
	function hwid_banned($hwid) {
	    global $db;
	    $check_hwid_ban = $db->prepare('SELECT * FROM bans WHERE hwid=:hwid');
	    $check_hwid_ban->bindValue(':hwid', $hwid);
	    $check_hwid_ban->execute();
	    
        if ($check_hwid_ban->rowCount() > 0)
            return true;
        else
            return false;
	}
	
	function set_sub($username, $sub) {
	    global $db;
	    $update_sub = $db->prepare('UPDATE users SET expire=:expire WHERE username=:username');
	    $update_sub->bindValue(':username', $username);
	    $update_sub->bindValue(':expire', $sub);
	    return $update_sub->execute();
	}
	
	function add_ban($username, $hwid, $ip, $reason) {
	    global $db;
	    
	    // check if already banned for any reason
	    if (user_banned($username)) return;
	    if (hwid_banned($hwid)) return;
	    if (ip_banned($ip)) return;
	    
	   $add_ban = $db->prepare("INSERT INTO bans (id, username, hwid, ip, reason) VALUES ('', :username, :hwid, :ip, :reason);");
	   $add_ban->bindValue(':username', $username);
	   $add_ban->bindValue(':hwid', $hwid);
	   $add_ban->bindValue(':ip', $ip);
	   $add_ban->bindValue(':reason', $reason);
	   $add_ban->execute();
	}


	define("SELLY_EMAIL", "ifrickinghatelife@gmail.com");
	define("SELLY_API_KEY", "4zXzzy2csz9PeMqZafQUWykCNzoxKYzzujDs-MV5_j9rbJTG4w");
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
	
	function generate_random_string($length) {
		$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
		$charactersLength = strlen($characters);
		$randomString = '';
		for ($i = 0; $i < $length; $i++) {
			$randomString .= $characters[rand(0, $charactersLength - 1)];
		}
		return $randomString;
	}
	
	function get_client_ip() {
        $ipaddress = '';
        if (getenv('HTTP_CLIENT_IP'))
            $ipaddress = getenv('HTTP_CLIENT_IP');
        else if(getenv('HTTP_X_FORWARDED_FOR'))
            $ipaddress = getenv('HTTP_X_FORWARDED_FOR');
        else if(getenv('HTTP_X_FORWARDED'))
            $ipaddress = getenv('HTTP_X_FORWARDED');
        else if(getenv('HTTP_FORWARDED_FOR'))
            $ipaddress = getenv('HTTP_FORWARDED_FOR');
        else if(getenv('HTTP_FORWARDED'))
           $ipaddress = getenv('HTTP_FORWARDED');
        else if(getenv('REMOTE_ADDR'))
            $ipaddress = getenv('REMOTE_ADDR');
        else
            $ipaddress = 'UNKNOWN';
        return $ipaddress;
	}


?>