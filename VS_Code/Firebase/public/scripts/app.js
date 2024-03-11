const suhu = document.getElementById("temp");
const humy = document.getElementById("hum");


firebase.database().ref("float").on("value", (snapshot) => {
    console.log(snapshot.value);
    suhu.innerHTML = 22;
});

    

