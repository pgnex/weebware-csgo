let open = false;

document.querySelector('#openNav').addEventListener('click', () => {
    document.querySelector('nav').style.display = 'block';
    setTimeout(() => {
        open = true;
    }, 50);
})

document.body.addEventListener('click', e => {
    if(open) {
        console.log(e.path)
        if(e.path[0].tagName !== 'NAV' && e.path[1].tagName !== 'NAV') {
            document.querySelector('nav').style.display = 'none';    
            open = false;
        }
    }
}) 