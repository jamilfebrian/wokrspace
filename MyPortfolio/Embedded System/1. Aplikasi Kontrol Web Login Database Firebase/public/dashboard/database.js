import { initializeApp } from "https://www.gstatic.com/firebasejs/10.11.0/firebase-app.js";
import { getDatabase, ref, child, get, set, update, remove, onValue } from "https://www.gstatic.com/firebasejs/10.11.0/firebase-database.js";

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyAT6fK0djMUblHDICdb6XpxkFDiGDkm4R4",
  authDomain: "login-page-77ed3.firebaseapp.com",
  databaseURL: "https://login-page-77ed3-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "login-page-77ed3",
  storageBucket: "login-page-77ed3.appspot.com",
  messagingSenderId: "194997614192",
  appId: "1:194997614192:web:682cd6611f8d772343e6b5"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

const db = getDatabase();

const uuid = "3rkRbklEctY3TGaXLYZMZLHvT0z2";

window.toggleCheckbox = function(element){
    if (document.getElementById("tg1").checked) { 
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu1:1
        })
    } else {
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu1:0
        })
    }
    
    if (document.getElementById("tg2").checked){ 
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu2:1
        })
    } else {
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu2:0
        })
    }

    if (document.getElementById("tg3").checked) { 
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu3:1
        })
    } else {
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu3:0
        })
    }
    
    if (document.getElementById("tg4").checked){ 
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu4:1
        })
    } else {
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu4:0
        })
    }

    if (document.getElementById("tg5").checked) { 
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu5:1
        })
    } else {
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu5:0
        })
    }
    
    if (document.getElementById("tg6").checked){ 
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu6:1
        })
    } else {
        update(ref(db,'kontrol/'+uuid+'/'),{
            lampu6:0
        })
    }

}

const starCountRef = ref(db, 'kontrol/'+uuid+'/');
    onValue(starCountRef, (snapshot) => {
      const data = snapshot.val();
      if(data.lampu1){
        document.getElementById("tg1").checked = true;
      } else {
        document.getElementById("tg1").checked = false;
      }

      if(data.lampu2){
        document.getElementById("tg2").checked = true;
      } else {
        document.getElementById("tg2").checked = false;
      }

      if(data.lampu3){
        document.getElementById("tg3").checked = true;
      } else {
        document.getElementById("tg3").checked = false;
      }

      if(data.lampu4){
        document.getElementById("tg4").checked = true;
      } else {
        document.getElementById("tg4").checked = false;
      }

      if(data.lampu5){
        document.getElementById("tg5").checked = true;
      } else {
        document.getElementById("tg5").checked = false;
      }

      if(data.lampu6){
        document.getElementById("tg6").checked = true;
      } else {
        document.getElementById("tg6").checked = false;
      }

    });
