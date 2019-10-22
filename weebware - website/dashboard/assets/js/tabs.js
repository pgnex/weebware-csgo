const tabLinks = document.querySelectorAll('.tab-header .tab-link');
let curTab = 'info';

tabLinks.forEach(tab => {
    tab.addEventListener('click', () => {
        tab.classList = 'tab-link active-tav';
        document.querySelector(`[data-id="${curTab}"]`).classList = 'tab-link';
        document.querySelector(`#${curTab}`).style.display = 'none';
        curTab = tab.getAttribute('data-id');
        document.querySelector(`#${curTab}`).style.display = 'block';
    })
})


