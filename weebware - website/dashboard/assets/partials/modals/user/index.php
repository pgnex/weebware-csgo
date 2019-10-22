<?php
    if(!isset($_SESSION['loggedin']) && !isset($_SESSION['sessionid']) || !isset($_SESSION['loggedin'])) {
        header('Location: /login');
        die;
    } else if(!isset($user)) {
        header('Location: /login');
    }

    if(!$user->admin) {
        header('Location: /login');
    }

    if (isset($_GET['user'])) {
        $data = HardwareID::fetchRequest($_GET['user']);
    }
?>
<form method="post" target="">
    <div id="hwid_reset" class="modal normal">
        <div class="modal-content bounceInDown fast animated" style="max-width: 450px;">
            <div class="modal-header">
                <h2>User infomation</h2>
            </div>
            <div class="tab-header">
                <div class="tab-link active-tab">
                    Info
                </div>
                <div class="tab-link">
                    Actions
                </div>
                <div class="tab-link">
                    Orders
                </div>
                <div class="tab-link">
                    Sessions
                </div>
            </div>
            <div class="tab-body" id="info">
                <div clasds="content">
                    <p><b>Username</b><span>Elian</span></p>
                </div>
            </div>
        </div>
    </div>
</form>