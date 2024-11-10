-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Nov 05, 2024 at 10:58 AM
-- Server version: 10.4.28-MariaDB
-- PHP Version: 8.0.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `esp8266_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `tb_display`
--

CREATE TABLE `tb_display` (
  `id` int(11) NOT NULL,
  `nama` varchar(50) NOT NULL,
  `uuid` varchar(20) NOT NULL,
  `NIK` varchar(50) NOT NULL,
  `no_kamar` varchar(20) NOT NULL,
  `aksi` varchar(20) NOT NULL,
  `status` varchar(20) NOT NULL,
  `hari` varchar(20) NOT NULL,
  `waktu` timestamp NOT NULL DEFAULT current_timestamp(),
  `sewa_awal` varchar(20) NOT NULL,
  `sewa_akhir` varchar(20) NOT NULL,
  `no_hp` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `tb_user`
--

CREATE TABLE `tb_user` (
  `id` int(11) NOT NULL,
  `nama` varchar(50) NOT NULL,
  `uuid` varchar(20) NOT NULL,
  `NIK` varchar(50) NOT NULL,
  `no_kamar` int(11) NOT NULL,
  `awal_sewa` date NOT NULL,
  `akhir_sewa` date NOT NULL,
  `no_hp` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `tb_user`
--

INSERT INTO `tb_user` (`id`, `nama`, `uuid`, `NIK`, `no_kamar`, `awal_sewa`, `akhir_sewa`, `no_hp`) VALUES
(1, 'Deby Aulia Ramadini', '63 9B 24 28', '1206074811020002', 1, '2024-10-08', '2024-11-06', '081234567890'),
(2, 'Lailatul Rahmi', '03 E4 E4 26', '1305063422010001', 2, '2024-08-01', '2024-11-30', '089612343121'),
(3, 'Fitri Kumala Sari', '43 91 23 28', '1302031387642182', 3, '2024-11-01', '2024-12-31', '081234567890'),
(5, 'Ibuk Kos', 'CC C3 66 3B', '03 2F 69 14', 10, '2024-09-01', '2034-12-31', '080808080808');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `user_name` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `user_name`, `password`, `name`) VALUES
(1, 'admin', '123', 'Admin'),
(2, 'ibukos', 'abc', 'Ibuk Kos');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `tb_display`
--
ALTER TABLE `tb_display`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `tb_user`
--
ALTER TABLE `tb_user`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `tb_display`
--
ALTER TABLE `tb_display`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=308;

--
-- AUTO_INCREMENT for table `tb_user`
--
ALTER TABLE `tb_user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
