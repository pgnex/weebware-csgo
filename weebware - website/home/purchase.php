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
                $price = '14';
                break;
            case '3months':
                $title = 'weebware.net 3 months';
                $price = '26';
                break;
            case '6months':
                $title = 'weebware.net 6 months';
                $price = '38';
            case 'lifetime':
                $title = 'weebware lifetime';
                $price = '46';
        }
        
        $referrer = NULL;
        
        if (isset($_POST['referrer'])) {
            $code_data = get_referral_info($_POST['referrer']);
            
            if ($code_data) {
                $price = $price * ( 1 - ($code_data['discount'] / 100));
                $referrer = $_POST['referrer'];
            }

        }

        $data = get_user_data($_SESSION['username']);
        if (isset($_POST['length']) && isset($_POST['paymentmethod'])) {
            $email       = $data['email'];
            $secret      = generate_random_string(16);
            $webhook_url = 'https://weebware.net/home/webhook.php?secret=' . $secret;
            $request     = selly_pay($title, 'PayPal', $email, $price, 'USD', 'https://weebware.net/home', $webhook_url, get_client_ip());
            $createOrder = $db->prepare("INSERT INTO orders (id, secret, order_id, price, username, referrer, length, timestamp, ip, status) VALUES ('', :secret, :order_id, :price, :username, :referrer, :length, :timestamp, :ip, :status);");
            $createOrder->bindValue(':secret', $secret);
            $createOrder->bindValue(':order_id', $request['id']);
            $createOrder->bindValue(':price', $price);
            $createOrder->bindValue(':username', $data['username']);
            $createOrder->bindValue(':referrer', $referrer);
            $createOrder->bindValue(':length', $length);
            $createOrder->bindValue(':timestamp', date("Y-m-d H:i:s", $date->getTimestamp()));
            $createOrder->bindValue(':ip', get_client_ip());
            $createOrder->bindValue(':status', $request['status']);
            $createOrder->execute();
            $redirect = $request['url'];
            header('location:' . $redirect);
        }
    }
       

?>