<?php 
session_start();

if (isset($_SESSION['id']) && isset($_SESSION['user_name'])) {

?>
<!DOCTYPE html>
<html>
<head>
     <meta charset="UTF-8">
     <meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>DASHBOARD</title>
	<link rel="stylesheet" type="text/css" href="assets\css\home.css">
</head>
<body onload = "table();">
     <script type"text/javascript">
          function table(){
               const xhttp = new XMLHttpRequest();
               xhttp.onload = function(){
                    document.getElementById("table").innerHTML = this.responseText;
               }
               xhttp.open("GET", "fetch_db.php");
               xhttp.send();
          }

          setInterval(function(){
               table();
          }, 1);
     </script>

     <div class="header">
          <img src="assets\img\UNP-logo.png" alt="UNP" class="logo-unp">
          <img src="assets\img\hmte-logo.png" alt="Elektro" class="hmte-unp">
          <p class="judul">Sistem Kunci Elektronik Pintu Kos Menggunakan E-KTP Berbasis IoT</p>

          <a href="logout.php">Logout</a>
     </div>
     
     <h1>Hello, <?php echo $_SESSION['name']; ?></h1>
     
     <div id="table"> </div>

     <div id="clock"> </div>
 
     <script>
          function updateClock() {
               const clockElement = document.getElementById('clock');
               const now = new Date();

               const days = ['Minggu', 'Senin', 'Selasa', 'Rabu', 'Kamis', 'Jumat', 'Sabtu'];
               const dayName = days[now.getDay()];

               const date = String(now.getDate()).padStart(2, '0');
               const month = String(now.getMonth() + 1).padStart(2, '0');
               const year = now.getFullYear();

               const hours = String(now.getHours()).padStart(2, '0');
               const minutes = String(now.getMinutes()).padStart(2, '0');
               const seconds = String(now.getSeconds()).padStart(2, '0');

               const time = `${hours}:${minutes}:${seconds}`;
               const fullDate = `${date}/${month}/${year}`;

               clockElement.innerHTML = `${dayName}, ${fullDate}  |  ${time}`;
          }
          setInterval(updateClock, 1000);
          updateClock();
     </script>

     
</body>
</html>

<?php 
}else{
     header("Location: index.php");
     exit();
}
?>