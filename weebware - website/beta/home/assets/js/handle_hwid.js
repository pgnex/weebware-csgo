let modals = document.querySelectorAll('.modal');
let modalcontent = document.querySelectorAll('.modal-content');

modals.forEach(modal => {
    modal.addEventListener('click', () => {
      location.href = location.href.split('?')[0];
    });
});

modalcontent.forEach(content => {
    content.addEventListener('click', (e) => {
        e.stopPropagation();
    });
})
