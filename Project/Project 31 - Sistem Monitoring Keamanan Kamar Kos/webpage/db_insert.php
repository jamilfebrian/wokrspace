<?php 

include "db_conn.php";

    if(isset($_POST["UID"]) && isset($_POST["ruang"]) && isset($_POST["aksi"])){
      
        $UID = $_POST["UID"];
        $ruang = $_POST["ruang"];
        $aksi = $_POST["aksi"];

        date_default_timezone_set('Asia/Jakarta'); 
        $dayNames = ["Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum\'at", "Sabtu"];
        $day = $dayNames[date('w')]; 
        $date = date('d/m/Y');
        $time = date('H:i:s'); 

        $sql_getUser = "SELECT * FROM tb_user WHERE uuid = '$UID'";
        $query_getDatabase = mysqli_query($conn, $sql_getUser);
        $getDatabase = mysqli_fetch_array($query_getDatabase);

        $nama = $getDatabase['nama'];
        $uuid = $getDatabase['uuid'];
        $nik = $getDatabase['NIK'];
        $no_kamar = $getDatabase['no_kamar'];
        $awal_sewa = $getDatabase['awal_sewa'];
        $akhir_sewa = $getDatabase['akhir_sewa'];
        $no_hp = $getDatabase['no_hp'];

        $nama = (empty($uuid)) ? "Unknown" : $nama;
        $status = ($ruang == $no_kamar || $no_kamar == 10 ) ?  "Diterima" : "Ditolak";
        
        $sql_dbInsert = "INSERT INTO tb_display (nama, uuid, NIK, no_kamar, aksi, status, hari, sewa_awal, sewa_akhir, no_hp) 
                        VALUES ('$nama', '$UID', '$nik', '$ruang', '$aksi', '$status', '$day', '$awal_sewa', '$akhir_sewa', '$no_hp')";

        if (mysqli_query($conn, $sql_dbInsert)) {
            echo "New record created successfully";
          } else {
            echo "Error: " . $sql_dbInsert . "<br>" . mysqli_error($conn);
          }
          
        mysqli_close($conn);
    }

 