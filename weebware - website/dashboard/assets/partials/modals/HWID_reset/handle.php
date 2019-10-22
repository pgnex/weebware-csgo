<?php
    if (isset($_GET['request'])) {
        $data = HardwareID::fetchRequest($_GET['request']);
    }
?>
<form method="post" target="">
    <input type="hidden" name="action" value="hwid_reset">
    <input type="hidden" name="username" value="<?= $data['username'] ?>">
    <input type="hidden" name="id" value="<?= $data['id'] ?>">
    <div id="hwid_reset" class="modal normal">
        <div class="modal-content bounceInDown fast animated" style="max-width: 450px;">
            <div class="modal-header">
                <h2>HWID Reset</h2>
            </div>
    
            <div class="group-label">
                <label for="">Username</label>
                <p><?= $data['username'] ?></p>
            </div>
    
            <div class="group-label">
                <label for="">Reason</label>
                <p class="hwid_reason"><?= $data['reason'] ?></p>
            </div>

            <details>
                <summary>Check IP</summary>
                <table class="ip-table">
                    <thead>
                    <th>Request IP</th>
                    <th>Cheat IP</th>
                    </thead>
                    <tbody>
                    <tr>
                        <td><span><img src="<?= Admin::fetchGeo($data['ip']); ?>">  <?= $data['ip'] ?></span></td>
                        <td><span><img src="<?= Admin::fetchGeo($data['cheat_ip']); ?>"> <?= $data['cheat_ip'] ?></span></td>
                    </tr>
                    </tbody>
                </table>
            </details>
    
            <div class="btn-row-hwid">
                <button class="btn-main accept" type="submit" name="hwid_reset" value="accept"><img src="assets/images/check.svg" alt=""></button>
                <button class="btn-main decline" type="submit" name="hwid_reset" value="decline"><img src="assets/images/close.svg" alt=""></button>
            </div>
        </div>
    </div>
</form>