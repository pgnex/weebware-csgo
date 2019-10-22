document.querySelector('#search').addEventListener('keydown', e => {
    if(e.key === 'Enter') {
        const value = document.querySelector('#search').value;

        window.location.href = `/home/admin.php?search=${value}`;
    }
})

const link = window.location.href;

if (link.includes('search')) {
    window.scrollTo(0,document.body.scrollHeight);
}