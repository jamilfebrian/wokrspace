<?php 

include "db_conn.php";


if(isset($_POST["UID"]) && isset($_POST["ruang"])){

    $UID = $_POST["UID"];
    // $UID = "63 9B 24 28";

    date_default_timezone_set('Asia/Jakarta'); 
    $dayNames = ["Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum\'at", "Sabtu"];
    $day = $dayNames[date('w')]; 
    $tanggal_sekarang = date('Y-m-d');
    $waktu_sekarang = date('H:i:s'); 

    $sql_getUser = "SELECT * FROM tb_user WHERE uuid = '$UID'";
    $query_getDatabase = mysqli_query($conn, $sql_getUser);
    $getDatabase = mysqli_fetch_array($query_getDatabase);

    $tanggal_db = $getDatabase['akhir_sewa'];

    $tanggal_database = new DateTime($tanggal_db);
    $tanggal_hari_ini = new DateTime($tanggal_sekarang);

    $selisih_hari = $tanggal_hari_ini->diff($tanggal_database)->days;
    $waktu_habis = $tanggal_hari_ini > $tanggal_database;

    if ($waktu_habis || $selisih_hari == 0) {
        echo "0";
    } elseif ($selisih_hari == 1) {
        echo "1";
    } else {
        echo $selisih_hari;
    }

    mysqli_close($conn);
}


