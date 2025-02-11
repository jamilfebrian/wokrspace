import 'package:flutter/material.dart';

class DevicesWidget extends StatefulWidget {
  final String? deviceTittle;

  const DevicesWidget({super.key, this.deviceTittle});

  @override
  State<DevicesWidget> createState() => _DevicesWidgetState();
}

class _DevicesWidgetState extends State<DevicesWidget> {
  @override
  Widget build(BuildContext context) {
    bool isSwitch = false;
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
            onTap: () {},
            child: FittedBox(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.start,
                  mainAxisSize: MainAxisSize.max,
                  children: [
                    Image.asset(
                      "assets/images/clock.png",
                      height: 100,
                      width: 100,
                    ),
                    const SizedBox(height: 8),
                    Text(
                      widget.deviceTittle!,
                      style: const TextStyle(
                        fontSize: 18,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    const SizedBox(height: 4),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        const Text("OFF"),
                        Switch(
                          value: isSwitch,
                          onChanged: (bool value) {
                            setState(() {
                              isSwitch = value;
                            });
                          },
                        ),
                      ],
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
