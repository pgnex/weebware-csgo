<?php
    if (!isset($_SESSION)) session_start();

    // require classes
    require_once ('classes/Database.php');
    require_once ('classes/General.php');
    require_once ('classes/User.php');
    require_once ('classes/HardwareID.php');
    require_once ('classes/Admin.php');
    require_once ('classes/Shop.php');

    if (isset($_POST['login'])) {

        $data = User::returnPassword($_POST['username']);

        if ($data) {
            if (!User::isBanned($_POST['username'])) {
                if (password_verify($_POST['password'], $data['password'])) {
                    $user = new User($data['username'], $data['password'], $data['email']);
                    $user->login();

                    $_SESSION['loggedin'] = serialize($user);
                    $_SESSION['sessionid'] = session_id();
                } else {
                    $_SESSION['loggin_error'] = "Password is incorrect.";
                }
            } else {
                $_SESSION['loggin_error'] = "You're banned. 🧨";
            }
        } else {
            $_SESSION['loggin_error'] = 'Invalid username';
        }
    }

    if (isset($_POST['register'])) {
        $_POST['email'] = htmlentities($_POST['email'], ENT_QUOTES, "UTF-8");
        $_POST['username'] = htmlentities($_POST['username'], ENT_QUOTES, "UTF-8");

        if (empty($_POST['username']) || empty($_POST['email']) || empty($_POST['password']) || empty($_POST['password2'])) {
            $_SESSION['register_error'] = "Please fill out all fields 🧨";
        } else if (empty($_POST['g-recaptcha-response'])) {
            $_SESSION['register_error'] = "Invalid Recaptcha!";
        } else if (!filter_var($_POST['email'], FILTER_VALIDATE_EMAIL)) {
            $_SESSION['register_error'] = "Invalid Email!";
        } else if (!preg_match('/^\w{0,16}$/', $_POST['username'])) {
            $_SESSION['register_error'] = "Username must be less than 16 characters and alphanumeric 🧨";
        } else if (!($_POST['password'] == $_POST['password2'])) {
            $_SESSION['register_error'] = "Passwords do not match! 🧨";
        } else if (User::usernameExist($_POST['username'])) {
            $_SESSION['register_error'] = "Username already in use! 🧨";
        } else if (User::emailExist($_POST['email'])) {
            $_SESSION['register_error'] = "E-mail already in use! 🧨";
        }

        $secret = "6Ldo174UAAAAAMkV1ZSBnPw5HNG0rAYETjhL7_ZR";
        $verify_recaptcha = file_get_contents('https://www.google.com/recaptcha/api/siteverify?secret='.$secret.'&response='.$_POST['g-recaptcha-response']);
        $response = json_decode($verify_recaptcha);

        if (!$response->success)
            $_SESSION['register_error'] = "Invalid Recaptcha!";

        //Check if error is set, if true go back and display error;
        if (isset($_SESSION['register_error'])) {
            header('Location: ' . $_SERVER['HTTP_REFERER']);
            exit;
        }

        $user = new User($_POST['username'], $_POST['password'], $_POST['email']);

        $user->add();
        $user->login();

        $_SESSION['loggedin'] = serialize($user);
        $_SESSION['sessionid'] = session_id();

        header('Location: index.php');
    }



    if (isset($_POST['hwid_request'])) {

        if (isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {

            $user = unserialize($_SESSION['loggedin']);

            if($user->subActive()) {

                $return = HardwareID::addRequest($user->username, $_POST['reason']);

                if ($return) {

                    $_SESSION['popup'] = [

                        "message" => "HWID Request has been submitted!",

                        "class" => "succes-alert"

                    ];

                } else {

                    $_SESSION['popup'] = [

                        "message" => "HWID Request failed",

                        "class" => "danger-alert"

                    ];

                }

            } else {

                $_SESSION['popup'] = [

                    "message" => "You don't own weebware",

                    "class" => "danger-alert"

                ];

            }

        }



        header('Location: index.php');

    }



    if (isset($_POST['changePassword'])) {

        if (isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {

            $user = unserialize($_SESSION['loggedin']);



            if ($_POST['new_password1'] !== $_POST['new_password2']) {

                $_SESSION['popup'] = [

                    "message" => "New passwords are not the same.",

                    "class" => "danger-alert"

                ];

            } else if ($user->changePassword($_POST['current_password'], $_POST['new_password1'])) {

                $_SESSION['popup'] = [

                    "message" => "Password has been changed!",

                    "class" => "succes-alert"

                ];

            } else {

                $_SESSION['popup'] = [

                    "message" => "Current password is incorrect.",

                    "class" => "danger-alert"

                ];

            }

        }



        header('Location: index.php');

    }



    if (isset($_POST['hwid_reset'])) {

        if (isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {

            $user = unserialize($_SESSION['loggedin']);



            if( $user->admin) {

                if ($_POST['hwid_reset'] == "accept") {

                    // Set status to accepted.

                    HardwareID::updateStatusRequest($_POST['id'], $_POST['username'], 'ACCEPTED');



                    $_SESSION['popup'] = [

                        "message" => "HWID of {$_POST['username']} has been reset!",

                        "class" => "succes-alert"

                    ];

                }  else if ($_POST['hwid_reset'] == "decline") {

                    // Set status to denied.

                    HardwareID::updateStatusRequest($_POST['id'], $_POST['username'], 'DENIED');



                    // Show confirm pop-up.

                    $_SESSION['popup'] = [

                        "message" => "HWID Request of {$_POST['username']} has been denied!",

                        "class" => "danger-alert"

                    ];

                }



            }



        }



        header('Location: admin.php');

    }



    if (isset($_POST['purchase'])) {

        if (isset($_SESSION['loggedin']) && $_SESSION['sessionid'] === session_id()) {

            $user = unserialize($_SESSION['loggedin']);



            if (!in_array($_POST['length'], ['1month', '3months', 'lifetime'])) {

                die('Stop snooping around');

            }



            $purchaseSelection = [

                "1month" => [

                    "title" => 'weebware.net 1 month',

                    "price" => '11.99',

                ],

                "3months" => [

                    "title" => 'weebware.net 3 months',

                    "price" => '29.99'

                ],

                'lifetime' => [

                    "title" => 'weebware.net lifetime',

                    "price" => '59.99'

                ]

            ];



            $price = $purchaseSelection[$_POST['length']]['price'];

            $title = $purchaseSelection[$_POST['length']]['title'];

            $referrer = null;



            if (!empty($_POST['discount'])) {

                $code_data = Shop::getReferralCodeInfo($_POST['discount']);



                if ($code_data != false) {

                    $price = $price * ( 1 - ($code_data['discount'] / 100));

                    $referrer = $_POST['discount'];

                }

            }



            $redirect = Shop::createSelly($title, $price, $user->email, General::randomString(16), $_POST['length'], $referrer);

            header("location: $redirect");

        } else {

            header('Location: /login');

        }

    }



    if (isset($_GET['secret']) && !empty($_GET['secret'])) {

        $date = new DateTime();



        $order = Shop::getOrder($_GET['secret']);

        $user = User::getData($order['username']);



        $subLength = [

            "1month" => [

                "length" => $date->getTimestamp() + 2592000,

            ],

            "3months" => [

                "length" => $date->getTimestamp() + 2592000,

            ],

            "lifetime" => [

                "length" => 2000000000,

            ]

        ];



        $expire = $subLength[$order['length']]['length'];



        $data = json_decode( file_get_contents('php://input'), true);



        //Update order status

        Shop::updateOrder($data['status'], $_GET['secret']);



        if ($data['status'] === 100) {

            //Update the sub of the user

            Shop::updateSub($expire, $order['username']);



            if (isset($data['referrer'])) {

                // Update referrer info

                Shop::updateReferrer($order['referrer'], $order['price']);

            }

        } else if($data['status'] === 56 || $data['status'] === 51) {

            // If refunded, or on refund status, ban the bitch

            $hwid = NULL;



            if ($user['hwid'] != "0") {

                $hwid = $user['hwid'];

            }



            Admin::ban($order['username'], $hwid, NULL, "Chargeback");

        }

    }



    if(isset($_POST['getDiscount'])) {

        $discountCode = $_POST['getDiscount'];

        $return = "0";



        $code_data = Shop::ReferrerCodeInfo($discountCode);



        if (isset($code_data['discount'])) {

            $return = $code_data['discount'];

        }



        echo json_encode(["return" => $return]);

        die;

    }



    if (isset($_GET['logout'])) {

        unset ($_SESSION['loggedin']);

        unset ($_SESSION['sessionid']);



        header('location: /login');

    }