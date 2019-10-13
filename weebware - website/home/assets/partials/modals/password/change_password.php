<form method="post" target="">
    <input type="hidden" name="type" value="change_password">
    <div id="change_password" class="modal">
        <div class="modal-content bounceInDown fast animated" style="max-width: 450px;">
            <div class="modal-header">
                <h2>Changing your Password</h2>
            </div>
            <div class="group-label">
                 <label for="">Confirm password</label>
                 <input type="password" name="current_password" placeholder="Current password">
             </div>
            <div class="group-label">
                <label for="">Enter & confirm your new password.</label>
                <input type="password" name="new_password1" placeholder="New password">
                <input type="password" name="new_password2" placeholder="Confirm password">
            </div>
    
            <button class="btn-main" id="changedPassword"><img src="assets/images/check.svg" alt=""></button>
        </div>
    </div>
</form>