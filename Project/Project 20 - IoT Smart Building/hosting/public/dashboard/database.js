var Lampu1;
var Lampu2;
var Lampu3;
var Lampu4;
var Lampu5;
var Lampu6;

var url = "https://script.google.com/macros/s/AKfycbz9vR2A3Ka_2HK6LyXO2q_vgT7AExz2VQkh6Fsn_wIzG_fb-zsZn8BqeCuFk-5YvEKO/exec";

window.toggleCheckbox = function(element){
    if (document.getElementById("tg1").checked) { 
        Lampu1="ON";
    } else {
        Lampu1="OFF";
    }
    
    if (document.getElementById("tg2").checked){ 
        Lampu2="ON";
    } else {
        Lampu2="OFF";
    }

    if (document.getElementById("tg3").checked){ 
        Lampu3="ON";
    } else {
        Lampu3="OFF";
    }

    if (document.getElementById("tg4").checked){ 
        Lampu4="ON";
    } else {
        Lampu4="OFF";
    }

    if (document.getElementById("tg5").checked){ 
        Lampu5="ON";
    } else {
        Lampu5="OFF";
    }

    if (document.getElementById("tg6").checked){ 
        Lampu6="ON";
    } else {
        Lampu6="OFF";
    }

    let fullURL = `${url}?sts=writeRelay&b1=${Lampu1}&b2=${Lampu2}&b3=${Lampu3}&b4=${Lampu4}&b5=${Lampu5}&b6=${Lampu6}`;

    fetch(fullURL)
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok ' + response.statusText);
        }
        return response.text(); // Menguraikan respons menjadi JSON
    })
    .then(data => {
        console.log(data); // Menampilkan data yang diterima dari server
    })
    .catch(error => {
        console.error('There was a problem with the fetch operation:', error);
    });
}

function readData(){

    let fullURL = `${url}?sts=read`;
    
    fetch(fullURL)
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok ' + response.statusText);
        }
        return response.text(); // Menguraikan respons menjadi JSON
    })
    .then(data => {
        const tegangan = document.getElementById("volt");
        const arus = document.getElementById("amp");
        const daya = document.getElementById("watt");
        const frekuensi = document.getElementById("freq");

        const validJsonString = '[' + data.split(',').map(item => '"' + item + '"').join(',') + ']';
        const dataObject = JSON.parse(validJsonString);

        tegangan.innerHTML = dataObject[1];
        arus.innerHTML = dataObject[2];
        daya.innerHTML = dataObject[3];
        frekuensi.innerHTML = dataObject[4];


        if (dataObject[6] == "ON"){
            document.getElementById("tg1").checked = true;
            document.getElementById("lampu1").innerHTML = "ON";
        } else {
            document.getElementById("tg1").checked = false;
            document.getElementById("lampu1").innerHTML = "OFF"; 
        }

        if (dataObject[7] == "ON"){
            document.getElementById("tg2").checked = true;
            document.getElementById("lampu2").innerHTML = "ON"; 
        } else {
            document.getElementById("tg2").checked = false;
            document.getElementById("lampu2").innerHTML = "OFF";        
        }

        if (dataObject[8] == "ON"){
            document.getElementById("tg3").checked = true;
            document.getElementById("lampu3").innerHTML = "ON"; 
        } else {
            document.getElementById("tg3").checked = false;
            document.getElementById("lampu3").innerHTML = "OFF"; 
        }

        if (dataObject[9] == "ON"){
            document.getElementById("tg4").checked = true;
            document.getElementById("lampu4").innerHTML = "ON"; 
        } else {
            document.getElementById("tg4").checked = false;
            document.getElementById("lampu1").innerHTML = "OFF"; 
        }

        if (dataObject[10] == "ON"){
            document.getElementById("tg5").checked = true;
            document.getElementById("lampu5").innerHTML = "ON"; 
        } else {
            document.getElementById("tg5").checked = false;
            document.getElementById("lampu5").innerHTML = "OFF"; 
        }

        if (dataObject[11] == "ON"){
            document.getElementById("tg6").checked = true;
            document.getElementById("lampu6").innerHTML = "ON"; 
        } else {
            document.getElementById("tg6").checked = false;
            document.getElementById("lampu6").innerHTML = "OFF"; 
        }

        console.log(dataObject); // Menampilkan data yang diterima dari server
    })
    .catch(error => {
        console.error('There was a problem with the fetch operation:', error);
        });
}

setInterval(readData, 3000);