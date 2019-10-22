<?php
    date_default_timezone_set('US/Eastern');
    require 'server.php';
    
    function check_user_exists($username) {
        global $db;

        $get_users = $db->prepare('SELECT * FROM users WHERE username=:username');
        $get_users->execute([
            "username" => $username
        ]);

        return $get_users->rowCount() > 0 ? true : false;
	}

    function check_email_exists($email) {
        global $db;

        $get_email = $db->prepare('SELECT * FROM users WHERE email=:email');
        $get_email->execute([
            "email" => $email
        ]);

        return $get_email->rowCount() > 0 ? true : false;
    }

    function create_user($username, $password, $email) {
        global $db;

        $create_account = $db->prepare("INSERT INTO users (username, password, email, hwid) VALUES (:username, :password, :email, 0);");
        $create_account->execute([
            "username" => $username,
            "email" => $email,
            "password" => password_hash($password, PASSWORD_BCRYPT)
        ]);
    }

    function login($username, $password) {
        global $db;

        $check_login = $db->prepare('SELECT * FROM users WHERE username=:username');
        $check_login->execute([
            "username" => $username
        ]);

        $user = $check_login->fetch();

        return $user && password_verify($password, $user['password']);
    }

    function successful_login($username, $website = false) {
        global $db;

        $table = 'logins_cheat';
        if ($website)
            $table = 'logins_website';

        $log_login = $db->prepare("INSERT INTO `$table` (user, ip, timestamp, status) VALUES (:username, :ip, :timestamp, 'WAITING');");

        return $log_login->execute([
            "username" => $username,
            "ip" => get_client_ip(),
            "timestamp" => time(),
        ]);
    }

    function update_login($status, $id) {
        global $db;

        $update_login = $db->prepare('UPDATE logins_cheat SET status=:status WHERE id=:id');

        return $update_login->execute([
            "id" => $id,
            "status" => $status
        ]);
    }

    function get_user_data($username) {
        global $db;

        $get_data = $db->prepare('SELECT * FROM users WHERE username=:username');
        $get_data->execute([
            "username" => $username
        ]);

        return $get_data->fetch();
    }

	function getLastClient($username) {
        global $db;

        $get_data = $db->prepare("SELECT * FROM logins_cheat WHERE user=:username ORDER BY id DESC");
        $get_data->execute([
            "username" => $username
        ]);

        return $get_data->fetch();
    }
    
    function update_hwid($username, $hwid) {
        global $db;

        $update_hwid = $db->prepare('UPDATE users SET hwid=:hwid WHERE username=:username');
        $update_hwid->execute([
            "hwid" => $hwid,
            "username" => $username
        ]);
	}

	function get_order_data($secret) {
		global $db;

		$get_order = $db->prepare('SELECT * FROM orders WHERE secret=:secret');
		$get_order->execute([
		    "secret" => $secret
        ]);

		return $get_order->fetch();
	}

    function is_sub_active($expire) {
        return $expire > time();
    }

    function sub_expire_date($timestamp) {
        if ($timestamp >= 2000000000)
            return 'Never';
        else if ($timestamp == 0)
            return 'Inactive';
        else
            return date("F d, Y h:i A", $timestamp);
    }
    
    function get_referral_code_info($code) {
        global $db;

        $check_code = $db->prepare('SELECT * FROM referrals WHERE code=:code');
        $check_code->execute([
            "code" => $code
        ]);
        
        return $check_code->fetch();        
    }
    
    function get_referrer_info($username) {
        global $db;

        $get_referrer = $db->prepare('SELECT * FROM referrals WHERE username=:username');
        $get_referrer->execute([
            "username" => $username
        ]);
        
        return $get_referrer->fetch();        
    }
    
    function is_sponsor($username) {
        global $db;

        $get_sponsor = $db->prepare('SELECT * FROM referrals WHERE username=:username');
        $get_sponsor->execute([
            "username" => $username
        ]);
        
        return $get_sponsor->rowCount() > 0;
    }

    function request_hwid_reset($username, $reason) {
        $reset_flag = get_hwid_reset_status($username);
        if ($reset_flag === -1 || $reset_flag === 0)
            return false;

        global $db;

        $request_reset = $db->prepare("INSERT INTO hwid_resets (username, reason, ip, timestamp) VALUES (:username, :reason, :ip, :timestamp);");

        return $request_reset->execute([
            "username" => $username,
            "reason" => htmlspecialchars(substr($reason, 0, 256)),
            "ip" => get_client_ip(),
            "timestamp" => time()
        ]);
    }
    
    function get_hwid_reset_status($username) {
        global $db;

        $pending_check = $db->prepare("SELECT * from hwid_resets WHERE username=:username ORDER BY id DESC LIMIT 1");
        $pending_check->execute(["username" => $username]);

        $result = $pending_check->fetch();
        
        switch ($result['status']) {
            case "PENDING": // pending
                return -1;
                break;
            case "DENIED": // denied
                return 0;
                break;
            case "APPROVED": // accepted
                return 1;
                break;
            default: // default :3
                return 1;
                break;
        }
    }

    function get_hwid($id) {
        global $db;

        $get_user_info = $db->prepare("SELECT * FROM hwid_resets WHERE id=:id AND status='PENDING'");
        $get_user_info->execute([
            "id" => $id
        ]);

        $data = $get_user_info->fetch();

        return [
            "id" => $data['id'],
            "username" => $data['username'],
            "reason" => $data['reason'],
            "ip" => $data['ip'],
            "cheat_ip" => getLastClient($data['username'])['ip']
        ];
    }

    function update_hwid_reset($id, $username, $status) {
        global $db;

        // It got accepted, so reset HWID
        if($status === 'ACCEPTED') {
            update_hwid($username, 0);
        }

        $update_request = $db->prepare('UPDATE hwid_resets SET status=:status WHERE id=:id');
        $update_request->execute([
            "id" => $id,
            "status" => $status
        ]);
    }

	function countHWID() {
        global $db;
        $countRequests = $db->prepare("SELECT COUNT(*) as amount FROM hwid_resets WHERE status='PENDING'");
        $countRequests->execute();

        return $countRequests->fetch()['amount'];
    }

    function getPlayerList($search) {
        global $db;

        if(empty($search)) {
            $player_list = $db->prepare('SELECT * FROM users LIMIT 10');
            $player_list->execute();
            $data = $player_list->fetchAll();
        } else {
            $player_list = $db->prepare('SELECT * FROM users WHERE username LIKE :username LIMIT 10');
            $player_list->execute([
                "username" => '%'.$search.'%'
            ]);

            $data = $player_list->fetchAll();
        }

        return $data;
    }
	
	function update_email($username, $email) {
	    global $db;

	    $update_email = $db->prepare('UPDATE users SET email=:email WHERE username=:username');

	    return $update_email->execute([
	        "username" => $username,
            "email" => $email
        ]);
	}
	
	function update_password($username, $password, $current_pass) {
	    global $db;
	    
        $currPass = get_user_data($username)['password'];

        //Check if current password is correct
        if(password_verify($current_pass, $currPass)) {
            $reset_password = $db->prepare('UPDATE users SET password=:password WHERE username=:username');

            return $reset_password->execute([
                "username" => $username,
                "password" => password_hash($password, PASSWORD_BCRYPT)
            ]);
        } else {
            return false;
        }
	}
	
	function is_admin($username) {
	    global $db;

	    $check_admin = $db->prepare('SELECT * FROM users WHERE username=:username');
	    $check_admin->execute([
	        "username" => $username
        ]);
	    
	    return $check_admin->fetch()['rank'] == 2;
	}
	
	function get_pending_hwid_resets() {
	    global $db;

	    $get_resets = $db->prepare("SELECT * FROM hwid_resets WHERE status='PENDING'");
	    $get_resets->execute();
	    
	    return $get_resets->fetchAll();
	}

	function user_banned($username) {
	    global $db;

	    $check_user_ban = $db->prepare('SELECT * FROM bans WHERE username=:username');
	    $check_user_ban->execute([
	        "username" => $username
        ]);

        return $check_user_ban->rowCount() > 0 ? true : false;
	}
	
	function ip_banned($ip) {
	    
	    if ($ip == NULL) return false;
	    
	    global $db;

	    $check_ip_ban = $db->prepare('SELECT * FROM bans WHERE ip=:ip');
	    $check_ip_ban->execute([
	        "ip" => $ip
        ]);

        return $check_ip_ban->rowCount() > 0 ? true : false;
	}
	
	function hwid_banned($hwid) {
	    global $db;

	    $check_hwid_ban = $db->prepare('SELECT * FROM bans WHERE hwid=:hwid');
	    $check_hwid_ban->execute([
	        "hwid" => $hwid
        ]);

	    return $check_hwid_ban->rowCount() > 0 ? true : false;
	}
	
	function set_sub($username, $sub) {
	    global $db;

	    $update_sub = $db->prepare('UPDATE users SET expire=:expire WHERE username=:username');

	    return $update_sub->execute([
	        "username" => $username,
            "expire" => $sub
        ]);
	}
	
	function add_ban($username, $hwid, $ip, $reason) {
	    global $db;
	    
	    // check if already banned for any reason
	    if (user_banned($username) || hwid_banned($hwid) || ip_banned($ip)) {
            return;
        }
	    
	    $add_ban = $db->prepare("INSERT INTO bans (username, hwid, ip, reason) VALUES (:username, :hwid, :ip, :reason);");

	    $add_ban->execute([
	        "username" => $username,
            "hwid" => $hwid,
            "ip" => $ip,
            "reason" => $reason
        ]);
	}
	
	function insert_registration($username, $timestamp, $ip) {
	    global $db;
	    
        $add_reg = $db->prepare("INSERT INTO registrations (id, username, timestamp, ip) VALUES ('', :username, :timestamp, :ip);");
	    
	    $add_reg->execute([
	        "username" => $username,
            "timestamp" => $timestamp,
            "ip" => $ip
        ]);
	}

	define("SELLY_EMAIL", "ifrickinghatelife@gmail.com");
	define("SELLY_API_KEY", "4zXzzy2csz9PeMqZafQUWykCNzoxKYzzujDs-MV5_j9rbJTG4w");
    // thank justin for help ;3
    function selly_pay($title, $gateway, $email, $value, $currency, $return_url, $webhook_url, $ip) {
        $auth = base64_encode(SELLY_EMAIL.':'.SELLY_API_KEY);

        $context = stream_context_create([
            'http'=> [
                'method'=> "POST",
                'header'=>
                    "User-agent: {$email} - {$_SERVER['SERVER_NAME']}\r\n" .
                    "Authorization: Basic {$auth}\r\n" .
                    "Content-Type: application/x-www-form-urlencoded",
                'content' => http_build_query ([
                    'title' => $title,
                    'gateway' => $gateway,
                    'email' => $email,
                    'value' => $value,
                    'currency' => $currency,
                    'return_url' => $return_url,
                    'webhook_url' => $webhook_url,
                    'white_label' => true,
                    'ip_address' => $ip
                ])
            ]
        ]);

        $result = file_get_contents('https://selly.gg/api/v2/pay', false, $context);

        return json_decode($result, true);
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

        $ipaddress = "";

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