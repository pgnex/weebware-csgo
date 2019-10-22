<?php
    require_once ('../inc/web/index.php');

    function navMatch($list, $output){
        foreach($list as $item){
            if($item == $_SERVER["SCRIPT_NAME"]){
                return $output;
            }
        }

        return "";
    }

    /* Checking shit */

    if(!isset($_SESSION['loggedin']) && !isset($_SESSION['sessionid']) || !isset($_SESSION['loggedin'])) {
        header('Location: /login');
        die;
    } else if(isset($_SESSION['loggedin']) && !$_SESSION['sessionid'] === session_id()) {
        $_SESSION['sessionid'] = session_id();
    }

    $user = unserialize($_SESSION['loggedin']);

    $showAlert = false;

    if(isset($_SESSION['popup'])) {
        global $showAlert;

        require_once('assets/partials/popup/index.php');

        $showAlert = true;

      if(!isset($_POST['hwid_request']) && !isset($_POST['changePassword']) && !isset($_POST['hwid_reset'])) {
          unset($_SESSION['popup']);
      }
    }

    if (!$user->subActive()) {
        $user->updateSession();
    }
    
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <meta name="theme-color" content="#EB6123">
    <link rel="icon" type="image/png" href="https://weebware.b-cdn.net/favicon.png">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.2.0/css/all.css">
    <link rel="stylesheet" href="assets/css/circular-std.css">
    <link rel="stylesheet" href="assets/css/main.css?v=<?= password_hash('test', PASSWORD_DEFAULT) ?>">
    <link rel="stylesheet" href="assets/css/style.css?v=<?= password_hash('test', PASSWORD_DEFAULT) ?>">
    <link rel="stylesheet" href="assets/css/scrollbar.css?v=1">
    <link rel="stylesheet" href="assets/css/responsive.css?v=<?= password_hash('test', PASSWORD_DEFAULT) ?>">
    <script src="assets/js/chart.min.js"></script>
    <?php if(isset($page)) if($page == 'admin') echo '<script src="assets/js/admin.js?v=1" defer="defer"></script>'; ?>
    <?= isset($_SESSION['popup']) ? '<script defer src="assets/js/alert.js"></script>' : ''; ?>
    <?php if(isset($page)) if($page == 'purchase') echo '<script src="assets/js/checkout.js?v=1" defer="defer"></script>'; ?>
    <?= isset($_GET['request']) || isset($_GET['user']) ? '<script defer src="assets/js/handle_hwid.js"></script>' : '' ?>
    <?= isset($_GET['user']) ? '<script defer src="assets/js/tabs.js?v=5"></script>' : '' ?>
    <script defer src="assets/js/index.js"></script>
    <?= navMatch(["/dashboard/index.php", "/home"], '<script defer src="assets/js/mainPage.js"></script>') ?>
    <title>weebware</title>
</head>
<body>
    <div class="mobile-bar">
        <i id="openNav" class="fas fa-bars"></i>
    </div>
    <nav>
        <div class="logo">
            <img src="https://weebware.b-cdn.net/spooky_logo.png" alt="Weebware logo">
        </div>
        <ul>
            <li <?= navMatch(["/dashboard/index.php", "/dashboard/"], 'class="active"') ?>>
                <a href="index.php">
                    <img src="assets\images\dashboard<?= navMatch(["/dashboard/index.php", "/dashboard/"], '_active') ?>.svg?v=1" alt="">
                </a>
            </li>
            <?php if ($user->sub < 2000000000) { ?>
                <li>
                    <a href="purchase.php" <?= navMatch(["/dashboard/purchase.php", "/dashboard/purchase"], 'class="active"') ?>>
                        <img src="assets\images\shopping_cart<?= navMatch(["/dashboard/purchase.php", "/dashboard/purchase"], '_active') ?>.svg?v=1" alt="">
                    </a>
                </li>
            <?php } ?>
            <!-- <li>
                <a href="/configs.php" <?= navMatch(["/configs.php", "/configs"], 'class="active"') ?>>
                    <img src="assets\images\config<?= navMatch(["/configs.php", "/configs"], '_active') ?>.svg" alt="">
                </a>
            </li> -->
             <?php if ($user->admin) { ?>
                <li>
                    <a href="admin.php" class="badge-container <?= navMatch(["/dashboard/admin.php", "/dashboard/admin"], 'active')?>" >
                        <span class="badge"><?= HardwareID::countRequests(); ?></span>
                        <img src="assets\images\admin<?= navMatch(["/dashboard/admin.php", "/dashboard/admin"], '_active') ?>.svg?v=1" alt="">
                    </a>
                </li>
            <?php } ?>
            <?php if ($user->subActive()) { ?>
                <li>
                    <a href="https://weebware.net/download/">
                        <img src="assets\images\download.svg" alt="">
                    </a>
                </li>
            <?php } ?>
        </ul>
        <ul>
            <li>
                <a href="?logout=1">
                    <img src="assets\images\logout.svg" alt="">
                </a>
            </li>
        </ul>
    </nav>
    <main>
        <div class="content">