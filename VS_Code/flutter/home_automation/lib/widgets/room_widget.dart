import 'package:flutter/material.dart';

import '../pages/room_pages.dart';

class RoomWidget extends StatelessWidget {
  final String? cartTittle;
  final String? imageURL;
  final int? numberOfDevices;

  const RoomWidget({
    super.key,
    this.cartTittle = "",
    this.imageURL = "",
    this.numberOfDevices = 0,
  });

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Card(
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(16), // Sudut melengkung
        ),
        clipBehavior: Clip.hardEdge,
        child: SizedBox(
          width: double.infinity,
          child: InkWell(
            splashColor: Colors.blue.withAlpha(30),
            onTap: () {
              debugPrint('$cartTittle Card tapped.');
              Navigator.of(context).pushNamed(
                RoomPages.routeName,
              );
            },
            child: FittedBox(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    Image.asset(
                      imageURL!,
                      height: 100,
                      width: 100,
                    ),
                    const SizedBox(height: 8),
                    Text(
                      cartTittle!,
                      style: const TextStyle(
                        fontSize: 18,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    const SizedBox(height: 4),
                    Text(
                      '$numberOfDevices Devices',
                      style: const TextStyle(fontSize: 14, color: Colors.grey),
                    ),
                  ],
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
