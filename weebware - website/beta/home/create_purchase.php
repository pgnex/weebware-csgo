<?
	require '../inc/utils.php';
	require '../inc/session.php';

	// make sure user is signed in
	require_login();

	// initialize database connection
    db_connect();
    
    
    $date = new DateTime();
    
    if (isset($_POST['length'])) {
    
        $length = $_POST['length'];
        if ($length != '1month' && $length != '3months' && $length != '6months' && $length != 'lifetime') die('Stop snooping around');
        
        switch ($length) {
            case '1month':
                $title = 'weebware.net 1 month';
                $price = '11.99';
                break;
            case '3months':
                $title = 'weebware.net 3 months';
                $price = '29.99';
                break;
            case 'lifetime':
                $title = 'weebware.net lifetime';
                $price = '59.99';
        }
        
        $referrer = NULL;
        
        if (isset($_POST['discount'])) {
            $code_data = get_referral_code_info($_POST['discount']);
            
            if ($code_data) {
                $price = $price * ( 1 - ($code_data['discount'] / 100));
                $referrer = $_POST['discount'];
            }

        }

        $data = get_user_data($_SESSION['username']);
        if (isset($_POST['length'])) {
            $email       = $data['email'];
            $secret      = generate_random_string(16);
            $webhook_url = 'https://weebware.net/home/webhook.php?secret=' . $secret;
            $request     = selly_pay($title, 'PayPal', $email, $price, 'USD', 'https://weebware.net/home', $webhook_url, get_client_ip());

            $createOrder = $db->prepare("INSERT INTO orders (secret, order_id, price, username, referrer, length, timestamp, ip, status) VALUES (:secret, :order_id, :price, :username, :referrer, :length, :timestamp, :ip, :status);");
            $createOrder->execute([
                "secret" => $secret,
                "order_id" => $request['id'],
                "price" => $price,
                "username" => $data['username'],
                "referrer" => $referrer,
                "length" => $length,
                "timestamp" => date("Y-m-d H:i:s", $date->getTimestamp()),
                "ip" => get_client_ip(),
                "status" => $request['status']
            ]);

            $redirect = $request['url'];

            header('location:' . $redirect);
        }
    }

?>