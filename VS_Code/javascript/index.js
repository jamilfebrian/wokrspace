document.getElementById("submit").onclick = function(){
    let temp;

    if(document.getElementById("Celcius").checked){
        temp = document.getElementById("textBox").value;
        temp = Number(temp)
        document.getElementById("hasil").innerHTML = temp+"°C";
    } else if(document.getElementById("Reamur").checked){
        temp = document.getElementById("textBox").value;
        temp = Number(temp)
        temp = toReamur(temp)
        document.getElementById("hasil").innerHTML = temp+"°R";
    } else if(document.getElementById("Fahrenheit").checked){
        temp = document.getElementById("textBox").value;
        temp = Number(temp)
        temp = toFahrenheit(temp)
        document.getElementById("hasil").innerHTML = temp+"°F";
    } else if(document.getElementById("Kelvin").checked){
        temp = document.getElementById("textBox").value;
        temp = Number(temp)
        temp = temp + 273;
        document.getElementById("hasil").innerHTML = temp+"°F";
    } else {
        document.getElementById("hasil").innerHTML = "Mohon pilih opsi terlebih dahulu";
    }
}

function toReamur(temp){
    return temp * 4/5;
} 
function toFahrenheit(temp){
    return (temp * 9/5) + 32;
}

let price = [5,1,3,2,4];
let total = price.sort(cekot);

console.log(`totalnya adalah ${total}`);
function cekot(x,y){
    return x-y  ;
}