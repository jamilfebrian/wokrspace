import 'package:flutter/material.dart';
import 'package:home_automation/widgets/room_widget.dart';

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 10.0),
      child: GridView.count(
        primary: false,
        padding: const EdgeInsets.all(5),
        crossAxisSpacing: 5,
        mainAxisSpacing: 5,
        crossAxisCount: 2,
        children: const [
          RoomWidget(
            cartTittle: "Living Room",
            imageURL: "assets/images/clock.png",
            numberOfDevices: 10,
          ),
          RoomWidget(
            cartTittle: "Kitchen",
            imageURL: "assets/images/water.png",
            numberOfDevices: 5,
          ),
          RoomWidget(
            cartTittle: "BedRoom 1",
            imageURL: "assets/images/fan-1.png",
            numberOfDevices: 12,
          ),
          RoomWidget(
            cartTittle: "BedRoom 2",
            imageURL: "assets/images/fan-2.png",
            numberOfDevices: 3,
          ),
        ],
      ),
    );
  }
}
