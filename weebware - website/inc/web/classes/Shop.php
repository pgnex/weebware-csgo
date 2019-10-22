<?php

    class Shop {
        static function getReferralCodeInfo($code) {
            $db = Database::connect();

            $stmt = $db->prepare('SELECT * FROM referrals WHERE code=:code');
            $stmt->execute([
                "code" => $code
            ]);

            return $stmt->fetch();
        }

        static function create($request, $secret, $duration, $referrer, $price) {
            $db = Database::connect();
            $user = unserialize($_SESSION['loggedin']);

            $stmt = $db->prepare("INSERT INTO orders (secret, order_id, price, username, referrer, length, timestamp, ip, status) VALUES (:secret, :order_id, :price, :username, :referrer, :length, :timestamp, :ip, :status);");
            $stmt->execute([
                "secret" => $secret,
                "order_id" => $request['id'],
                "price" => $price,
                "username" => $user->username,
                "referrer" => $referrer,
                "length" => $duration,
                "timestamp" => date("Y-m-d H:i:s", (new DateTime())->getTimestamp()),
                "ip" => General::getClientIp(),
                "status" => $request['status']
            ]);
        }

        static function createSelly($title, $price, $email, $secret, $duration, $referrer) {
            $auth = base64_encode("ifrickinghatelife@gmail.com".':'."4zXzzy2csz9PeMqZafQUWykCNzoxKYzzujDs-MV5_j9rbJTG4w");

            $context = stream_context_create([
                'http'=> [
                    'method'=> "POST",
                    'header'=>
                        "User-agent: {$email} - {$_SERVER['SERVER_NAME']}\r\n" .
                        "Authorization: Basic {$auth}\r\n" .
                        "Content-Type: application/x-www-form-urlencoded",
                    'content' => http_build_query ([
                        'title' => $title,
                        'gateway' => 'PayPal',
                        'email' => $email,
                        'value' => $price,
                        'currency' => 'USD',
                        'return_url' => 'https://weebware.net/home',
                        'webhook_url' => 'https://weebware.net/inc/web/index.php?secret=' . $secret,
                        'white_label' => true,
                        'ip_address' => General::getClientIp(),
                    ])
                ]
            ]);

            $result = file_get_contents('https://selly.gg/api/v2/pay', false, $context);
            $request = json_decode($result, true);

            self::create($request, $secret, $duration, $referrer, $price);

            return $request['url'];
        }

        static function updateOrder($status, $secret) {
            $db = Database::connect();

            $stmt = $db->prepare("UPDATE orders SET status=:status WHERE secret=:secret");
            $stmt->execute([
                "status" => $status,
                "secret" => $secret
            ]);
        }

        static function updateSub($expire, $username) {
            $db = Database::connect();

            $stmt = $db->prepare("UPDATE users SET expire=:expire WHERE username=:username");
            $stmt->execute([
                "expire" => $expire,
                "username" => $username
            ]);
        }

        static function updateReferrer($code, $price) {
            $db = Database::connect();
            $codeInfo = self::ReferrerodeInfo($code);

            $update_code = $db->prepare("UPDATE referrals SET uses=:uses, balance=:balance WHERE username=:username");
            $update_code->execute([
                "uses" => $codeInfo['uses'] + 1,
                "balance" => $codeInfo['balance'] + ($price * ($codeInfo['discount'] / 100)),
                "username" => $codeInfo['username'],
            ]);
        }

        static function ReferrerCodeInfo($code) {
            $db = Database::connect();

            $stmt = $db->prepare('SELECT * FROM referrals WHERE code=:code');
            $stmt->execute([
                "code" => $code
            ]);

            return $stmt->fetch();
        }

        static function getOrder($secret) {
            $db = Database::connect();

            $get_order = $db->prepare('SELECT * FROM orders WHERE secret=:secret');
            $get_order->execute([
                "secret" => $secret
            ]);

            return $get_order->fetch();
        }
    }