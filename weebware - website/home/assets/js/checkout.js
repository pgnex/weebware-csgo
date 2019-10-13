const prices = {
    "1month": 11.99,
    "3months": 29.99,
    "lifetime": 59.99
}

let discount = 0;
let actPrice = 0;


document.querySelector('#duration').addEventListener('change', () => {
    let price = `${prices[document.querySelector('#duration').value]}`;
    if(price[2] == undefined) {
        price = prices[document.querySelector('#duration').value];
        actPrice = price;
    }
    document.querySelector('#price').textContent = `${price}`;
    applyDiscount()
})  


document.querySelector('input[name=discount]').addEventListener('change', (e) => {
    const discountCode = document.querySelector('input[name=discount]').value;

    if(discount == 0) {
		getDiscount(discountCode);
	}
})


async function getDiscount(code) {
    const form = new FormData();

    form.append('getDiscount', code);

    let getDiscount = await fetch('http://weebware.net/home/getDiscount.php', {
        "method": "post",
        "body": form
    })
    
    let returnData = JSON.parse(await getDiscount.text());

    discount = returnData.return;
    applyDiscount()
}

function applyDiscount() {
    if(discount != 0) {
        price = prices[document.querySelector('#duration').value];
        actPrice = price;
        
        percentage = 100 - discount;
        actPrice = actPrice / 100 * percentage;
    
        if(actPrice[2] == undefined) {
    
            price = `${actPrice}.00`;
        }
    
        price = actPrice;
        document.querySelector('#price').textContent = `${price}`;
    }
}
