let modals = document.querySelectorAll('.modal');
let modalcontent = document.querySelectorAll('.modal-content');

modals.forEach(modal => {
    modal.addEventListener('click', () => {
       modal.style.display = 'none';
    });
});

modalcontent.forEach(content => {
    content.addEventListener('click', (e) => {
        e.stopPropagation();
    });
})


/* Change password */

document.querySelector('#changePassword').addEventListener('click', () => {
    document.querySelector('#change_password').style.display = 'flex';
})


/* HWID Reset */

document.querySelector('#hwid_reset_open').addEventListener('click', () => {
    document.querySelector('#hwid_reset').style.display = 'flex';
})

document.querySelector('#submitReset').addEventListener('click', () => {
    document.querySelector('#hwid_reset').style.display = 'none';
})