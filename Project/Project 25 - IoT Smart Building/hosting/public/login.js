import { initializeApp } from "https://www.gstatic.com/firebasejs/10.11.0/firebase-app.js";
import { getAuth, signInWithEmailAndPassword } from "https://www.gstatic.com/firebasejs/10.11.0/firebase-auth.js";

const firebaseConfig = {
    apiKey: "AIzaSyB3wTsiccb_dLysMbbu1SMt5Wdg484PLy0",
    authDomain: "smart-build-monitoring.firebaseapp.com",
    projectId: "smart-build-monitoring",
    storageBucket: "smart-build-monitoring.appspot.com",
    messagingSenderId: "1045606601201",
    appId: "1:1045606601201:web:c7cd74f93b24e853b8f450"
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
            window.location.href = "./dashboard/index.html";
        })
        .catch((error) => {
            const errorCode = error.code;
            const errorMessage = error.message;
            alert("Login not Found");
        });
})