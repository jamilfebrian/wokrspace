import 'package:flutter/material.dart';
import 'package:home_automation/widgets/devices_widget.dart';

class RoomPages extends StatelessWidget {
  const RoomPages({super.key});

  static const routeName = "/room_pages";

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Room"),
      ),
      body: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 10.0),
        child: GridView.count(
          primary: false,
          padding: const EdgeInsets.all(5),
          crossAxisSpacing: 5,
          mainAxisSpacing: 5,
          crossAxisCount: 2,
          children: const [
            DevicesWidget(
              deviceTittle: "Kipas",
            ),
            DevicesWidget(
              deviceTittle: "Lampu",
            ),
            DevicesWidget(
              deviceTittle: "AC",
            ),
            DevicesWidget(
              deviceTittle: "Cahrger",
            ),
          ],
        ),
      ),
    );
  }
}
