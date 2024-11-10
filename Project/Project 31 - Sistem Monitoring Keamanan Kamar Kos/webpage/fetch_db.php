<?php 
session_start();

if (isset($_SESSION['id']) && isset($_SESSION['user_name'])) {
include "db_conn.php";

$rows = mysqli_query($conn, "SELECT * FROM tb_display");

?>

<style>
    th.caption {
        background-color: darkblue;
        font-family: Arial, Helvetica, sans-serif;
        font-size: 20px;
        color: aliceblue;
        text-align: left;
    }

    tr#head{
        background-color: darkblue;
        color: aliceblue;
        font-weight: 200;
    }

    #customers td, #customers th {
        border: 1px solid #ddd;
        padding: 8px;
    } 

    #customers tr:nth-child(even){background-color: darkcyan;}
    #customers tr:hover {background-color: #ddd;}

    .highlight {
        background-color: darkred !important;
        color: #ddd;
    }
</style>


<table border="1" cellpadding="10" width="100%" id="customers">
    <tr>
        <th colspan="11" class="caption">Data Nama Pengguna Kos</th>
    </tr>
    <tr id="head">
        <th>No</th>
        <th>Nama</th>
        <th>NIK</th>
        <th>No Kamar</th>
        <th>Aksi</th>
        <th>Status</th>
        <th>Hari</th>
        <th>Waktu</th>
        <th>Sewa Awal</th>
        <th>Sewa Akhir</th>
        <th>No HP</th>
        <!-- <th></th> -->
    </tr>
    <?php $i = 1; ?>
    <?php foreach($rows as $row) : ?>
        <?php $myData = ($row["nama"] === 'Unknown') ? 'highlight' : ''; ?>
        <tr class="<?= $myData; ?>">
            <td><?= $i++; ?></td>
            <td><?= $row["nama"] ?></td>
            <td><?= $row["NIK"] ?></td>
            <td><?= $row["no_kamar"] ?></td>
            <td><?= $row["aksi"] ?></td>
            <td><?= $row["status"] ?></td>
            <td><?= $row["hari"] ?></td>
            <td><?= $row["waktu"] ?></td>
            <td><?= $row["sewa_awal"] ?></td>
            <td><?= $row["sewa_akhir"] ?></td>
            <td><?= $row["no_hp"] ?></td>
            <!-- <td><a href="">X</a></td> -->
        </tr>
    <?php endforeach; ?>
</table>



<?php 
}else{
     header("Location: index.php");
     exit();
}
?>