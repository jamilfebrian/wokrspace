import { initializeApp } from "https://www.gstatic.com/firebasejs/10.11.0/firebase-app.js";
import { getAuth, signInWithEmailAndPassword } from "https://www.gstatic.com/firebasejs/10.11.0/firebase-auth.js";

const firebaseConfig = {
  apiKey: "AIzaSyAT6fK0djMUblHDICdb6XpxkFDiGDkm4R4",
  authDomain: "login-page-77ed3.firebaseapp.com",
  databaseURL: "https://login-page-77ed3-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "login-page-77ed3",
  storageBucket: "login-page-77ed3.appspot.com",
  messagingSenderId: "194997614192",
  appId: "1:194997614192:web:682cd6611f8d772343e6b5"
};

const app = initializeApp(firebaseConfig);

const submit = document.getElementById('submit');
const auth = getAuth(app);

submit.addEventListener("click", function(event){
    event.preventDefault();
    const email = document.getElementById('email').value;
    const password = document.getElementById('password').value;
    
    signInWithEmailAndPassword(auth, email, password)
        .then((userCredential) => {
            // Signed in 
            const user = userCredential.user;
            window.location.href = "./dashboard/dashboard.html";
        })
        .catch((error) => {
            const errorCode = error.code;
            const errorMessage = error.message;
            alert("Login not Found");
        });
})