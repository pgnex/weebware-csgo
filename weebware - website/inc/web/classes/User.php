<?php

class User {
    public $id = 1;
    public $username = "";
    public $password = "";
    public $email = "";
    public $admin = false;
    public $sub = "";

    public function __construct($username, $password, $email) {
        $this->username = $username;
        $this->password =  password_hash($password, PASSWORD_BCRYPT);
        $this->email = $email;
    }

    public function login() {
        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM users WHERE username=:username");
        $stmt->execute([
            "username" => $this->username
        ]);

        $data = $stmt->fetch();

        $this->admin = $data['rank'] == 2 ? true : false;
        $this->sub = $data['expire'];
        $this->id = $data['id'];
    }

    public function add() {
        $db = Database::connect();

        $stmt = $db->prepare("INSERT INTO users (username, password, email, expire, hwid, rank, ignore_auth) VALUES (:username, :password, :email, 0, 0, 0, 0)");
        $stmt->execute([
            "username" => $this->username,
            "password" => $this->password,
            "email" => $this->email
        ]);

        // Set session as class construct;
        return true;
    }

    public function subTime() {
        if ($this->sub >= 2000000000) {
            return 'Never';
        }  else if ($this->sub == 0) {
            return 'Inactive';
        }
        else {
            return date("F d, Y h:i A", $this->sub);
        }
    }

    public function subActive() {
        return $this->sub > time();
    }

    static function usernameExist($username) {
        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM users WHERE username=:username");
        $stmt->execute(["username" => $username]);

        return $stmt->rowCount() > 0 ? true : false;
    }

    static function emailExist($email) {
        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM users WHERE email=:email");
        $stmt->execute(["email" => $email]);

        return $stmt->rowCount() > 0 ? true : false;
    }

    public function changePassword($current, $new) {
        $db = Database::connect();

        if(password_verify($current, self::returnPassword($this->username)['password'])) {
            $stmt = $db->prepare('UPDATE users SET password=:password WHERE username=:username');

            $stmt->execute([
                "username" => $this->username,
                "password" => password_hash($new, PASSWORD_BCRYPT)
            ]);

            return true;
        } else {
            // Current password is wrong
            return false;
        }
    }


    //This is going to be used for logging in
    static function returnPassword($username) {

        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM users WHERE username=:username");
        $stmt->execute(["username" => $username]);

        $data = $stmt->fetch();

        return $data;
    }

    static function isBanned($username) {
        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM bans WHERE username=:username");
        $stmt->execute([
            "username" => $username
        ]);

        return $stmt->rowCount() > 0 ? true : false;
    }

    static function isHWIDBanned($hwid) {
        $db = Database::connect();

        $check_hwid_ban = $db->prepare('SELECT * FROM bans WHERE hwid=:hwid');
        $check_hwid_ban->execute([
            "hwid" => $hwid
        ]);

        return $check_hwid_ban->rowCount() > 0 ? true : false;
    }

    static function isIPBanned($ip) {
        $db = Database::connect();

        if ($ip == NULL) {
            return false;
        }

        $check_ip_ban = $db->prepare('SELECT * FROM bans WHERE ip=:ip');
        $check_ip_ban->execute([
            "ip" => $ip
        ]);

        return $check_ip_ban->rowCount() > 0 ? true : false;
    }

    static function getLastClient($username) {
        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM logins_cheat WHERE user=:username ORDER BY id DESC");
        $stmt->execute([
            "username" => $username
        ]);

        return $stmt->fetch();
    }

    public function updateSession() {
        $this->login();
    }

    static function getData($username) {
        $db = Database::connect();

        $stmt = $db->prepare("SELECT * FROM users WHERE username=:username");
        $stmt->execute([
            "username" => $username
        ]);

        return $stmt->fetch();
    }
}