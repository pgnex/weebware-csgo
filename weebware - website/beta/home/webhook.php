<?php
    
    require '../inc/utils.php';

    if (!isset($_GET['secret'])) die();

    db_connect();

    $order_data = get_order_data($_GET['secret']);
    $user_data = get_user_data($order_data['username']);

    $date = new DateTime();
    switch ($order_data['length']) {
        case '1month':
            $expire = $date->getTimestamp() + 2592000;
            break;
        case '3months':
            $expire = $date->getTimestamp() + 7776000;
            break;
        case '6months':
            $expire = $date->getTimestamp() + 15552000;
            break;
        case 'lifetime':
            $expire = 2000000000;
            break;
    }

    $post_body = file_get_contents('php://input');
    $data = json_decode($post_body, true);

    global $db;
    $update_order = $db->prepare("UPDATE orders SET status=:status WHERE secret=:secret");
    $update_order->execute([
        "status" => $data['status'],
        "secret" => $_GET['secret']
    ]);

    switch ($data['status']) {
        case 100:
            $update_sub = $db->prepare("UPDATE users SET expire=:expire WHERE username=:username");
            $update_sub->execute([
                "expire" => $expire,
                "username" => $order_data['username'],
            ]);
            
            if (isset($order_data['referrer'])) {
                $code_data = get_referral_code_info($order_data['referrer']);
                $update_code = $db->prepare("UPDATE referrals SET uses=:uses, balance=:balance WHERE username=:username");
                $update_code->execute([
                    "uses" => $code_data['uses'] + 1,
                    "balance" => $code_data['balance'] + ($order_data['price'] * ($code_data['discount'] / 100)),
                    "username" => $code_data['username'],
                ]);
            }
            
        break;
        case 56:
        case 51:
            $hwid = NULL;

            if ($user_data['hwid'] != "0") {
                $hwid = $user_data['hwid'];
            }

            add_ban($order_data['username'], $hwid, NULL, "Chargeback");
        break;
    }

?>